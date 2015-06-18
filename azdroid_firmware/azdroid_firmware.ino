#define LOGGING

#include <avr/wdt.h>

#define DEBUG 

//#define ENABLE_WDT

// Device drivers
// Enable one driver in each category

// Motor controller:
#define ENABLE_L298N_MOTOR_DRIVER

// Distance sensor
#define ENABLE_NEWPING_DISTANCE_SENSOR_DRIVER

// Remote control:
#define ENABLE_WIFI_REMOTE_CONTROL_DRIVER

#ifdef ENABLE_L298N_MOTOR_DRIVER
  #include "l298n_motor_driver.h"
  #define LEFT_MOTOR_INIT 12,6,9
  #define RIGHT_MOTOR_INIT 7,8,11
#endif

#ifdef ENABLE_NEWPING_DISTANCE_SENSOR_DRIVER
  #include <NewPing.h>
  #include "newping_distance_sensor.h"
  #define DISTANCE_SENSOR_INIT 2,5
#endif

#ifdef ENABLE_WIFI_REMOTE_CONTROL_DRIVER
  #include <Adafruit_CC3000.h>
  #include <ccspi.h>
  #include <SPI.h>
  #include "MqttClient.h"
  
  // These are the interrupt and control pins
  #define ADAFRUIT_CC3000_IRQ   3  // MUST be an interrupt pin!
  // These can be any two pins
  #define ADAFRUIT_CC3000_VBAT  5
  #define ADAFRUIT_CC3000_CS    10
  
  #define WLAN_SSID       "chan_mobile"        // cannot be longer than 32 characters!
  #define WLAN_PASS       ""
  // Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
  #define WLAN_SECURITY   WLAN_SEC_UNSEC
  byte mqttServerHost[] = { 192, 168, 0, 100};
  int mqttServerPort = 1883;
  #include "wifi_remote_control.h"
#endif

#include "logging.h"
#include "moving_average.h"
#include "azdroid_features.h"

namespace Azdroid
{
  class Robot
  {
public:
    /*
         * @brief Class constructor.
     */
    Robot(): 
      leftMotor(LEFT_MOTOR_INIT), rightMotor(RIGHT_MOTOR_INIT),
      remoteControl(),
      features(),
      distanceSensor(DISTANCE_SENSOR_INIT, features.getTooCloseDistance() * 10),
      distanceAverage(features.getTooCloseDistance() * 10) 
      {
      }

    /*
     * @brief Initialize the robot state.
     */
    void initialize()
    {
      Serial.println("Initializing Robot...");
      if(features.isRemoteMode())
      {
         remoteControl.initialize();
      }
      else 
      {
        move();
      }
    }
    
    /*
     * @brief Update the state of the robot based on input from sensor and remote control.
     *  Must be called repeatedly while the robot is in operation.
     */
    void run()
    {
      if(features.isRemoteMode())
      {
        bool haveRemoteCmd = remoteControl.checkRemoteCommand();
        if(haveRemoteCmd)
        {
           leftMotor.setSpeed(remoteControl.getLeftSpeed());
           rightMotor.setSpeed(remoteControl.getRightSpeed());
        }
      } 
      else
      {  
        unsigned long currentTime = millis();
        int distance = distanceAverage.add(distanceSensor.getDistance());
  
        if (moving()) 
        {
          if (obstacleAhead(distance))
          {
            //stop();
            reverse(currentTime);
            turn(currentTime);
          }
        } 
        else if(reversing())
        {
          if (doneReversing(currentTime))
          {
            // stop();
            turn(currentTime);
          }
        } 
        else if (turning())
        {
          if (doneTurning(currentTime, distance))
            move();
        }
      }
    }

protected:
    void remote()
    {
      leftMotor.setSpeed(0);
      rightMotor.setSpeed(0);
      state = stateRemote;
    }

    void move()
    {
      leftMotor.setSpeed(features.getSpeed());
      rightMotor.setSpeed(features.getSpeed());
      state = stateMoving;
    }

    void stop()
    {
      leftMotor.setSpeed(0);
      rightMotor.setSpeed(0);
      delay(1000);  
      state = stateStopped;
    }

    bool obstacleAhead(unsigned int distance)
    {
      return (distance <= features.getTooCloseDistance());
    }

    bool turn(unsigned long currentTime)
    { 
      if (random(2) == 0) 
      {
        turnLeft(leftMotor, rightMotor);
      }
      else 
      {
        turnRight(leftMotor, rightMotor);
      }
      state = stateTurning;
      endStateTime = currentTime + random(500, 1000);
    }

    void reverse(unsigned long currentTime)
    {
      leftMotor.setSpeed(-features.getSpeed());
      rightMotor.setSpeed(-features.getSpeed());
      state = stateReversing;
      endStateTime = currentTime + random(500, 1000);
    }

    bool doneTurning(unsigned long currentTime, unsigned int distance)
    {
      if (currentTime >= endStateTime)
        return (distance > features.getTooCloseDistance());
      return false;
    }

    bool doneReversing(unsigned long currentTime)
    {
      return (currentTime >= endStateTime);
    }

    bool moving() 
    { 
      return (state == stateMoving); 
    }
    bool reversing() 
    { 
      return (state == stateReversing); 
    }
    bool turning()
    { 
      return (state == stateTurning); 
    }
    bool stopped()
    { 
      return (state == stateStopped); 
    }
    bool remoteControlled() 
    { 
      return (state == stateRemote); 
    }

private:
    Motor leftMotor;
    Motor rightMotor;
    DistanceSensor distanceSensor;
    MovingAverage<unsigned int, 3> distanceAverage;
    RemoteControl remoteControl;
    enum state_t {stateStopped, stateMoving, stateTurning, stateRemote, stateReversing};
    state_t state;
    unsigned long endStateTime;
    AzdroidFeatures features;
    
    void turnLeft(Motor &leftMotor, Motor &rightMotor)
    {
      turn(leftMotor, rightMotor);
    }
    
    void turnRight(Motor &leftMotor, Motor &rightMotor)
    {
      turn(rightMotor, leftMotor);
    }
    
    void turn(Motor &motor1, Motor &motor2)
    {
      motor1.setSpeed(-features.getSpeed());
      motor2.setSpeed(features.getSpeed());
    }
  };
};

Azdroid::Robot robot;

void setup()
{
  Serial.begin(115200);
  Serial.println("--------------------------");
  Serial.println("|  A  Z  D  R  O  I  D   |");
  Serial.println("--------------------------\n\n");
  #ifdef ENABLE_WDT
  wdt_enable(WDTO_8S); // enable watch dog timer (https://tushev.org/articles/arduino/5/arduino-and-watchdog-timer)
  #endif
  robot.initialize();
}

void loop()
{
  #ifdef ENABLE_WDT
  wdt_reset(); // kick the watchdog
  #endif
  robot.run();
}

