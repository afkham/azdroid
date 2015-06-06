#define LOGGING

// Device drivers
// Enable one driver in each category

// Motor controller:
#define ENABLE_L298N_MOTOR_DRIVER

// Distance sensor
#define ENABLE_NEWPING_DISTANCE_SENSOR_DRIVER

// Remote control:
//#define ENABLE_WIFI_REMOTE_CONTROL_DRIVER

// constants
#define TOO_CLOSE 10                    /**< distance to obstacle in centimeters */
#define MAX_DISTANCE (TOO_CLOSE * 20)   /**< maximum distance to track with sensor */
#define RANDOM_ANALOG_PIN 5             /**< unused analog pin to use as random seed */
#define BT_RX_PIN 10                    /**< RX pin for Bluetooth communcation */
#define BT_TX_PIN 3                    /**< TX pin for Bluetooth communcation */

#define SPEED 150

#ifdef ENABLE_L298N_MOTOR_DRIVER
#include "l298n_motor_driver.h"

#define LEFT_MOTOR_INIT 12,13,9
#define RIGHT_MOTOR_INIT 7,8,11
#endif

#ifdef ENABLE_NEWPING_DISTANCE_SENSOR_DRIVER
#include <NewPing.h>
#include "newping_distance_sensor.h"
#define DISTANCE_SENSOR_INIT 2,5,MAX_DISTANCE
#endif

#ifdef ENABLE_WIFI_REMOTE_CONTROL_DRIVER
#include "wifi_remote_control.h"
#endif

#include "logging.h"
#include "moving_average.h"

namespace Azdroid
{
  class Robot
  {
public:
    /*
         * @brief Class constructor.
     */
    Robot(): 
      leftMotor(LEFT_MOTOR_INIT), rightMotor(RIGHT_MOTOR_INIT), distanceSensor(DISTANCE_SENSOR_INIT), distanceAverage(TOO_CLOSE * 10)
      {
        initialize();
      }

    /*
         * @brief Initialize the robot state.
     */
    void initialize()
    {
      move();
    }

    /*
         * @brief Update the state of the robot based on input from sensor and remote control.
     *  Must be called repeatedly while the robot is in operation.
     */
    void run()
    {
      unsigned long currentTime = millis();
      int distance = distanceAverage.add(distanceSensor.getDistance());

      if (moving()) {
        if (obstacleAhead(distance)){
          //stop();
          reverse(currentTime);
          turn(currentTime);
        }
      } 
      else if(reversing()){
        if (doneReversing(currentTime)){
          // stop();
          turn(currentTime);
        }
      } 
      else if (turning()) {
        if (doneTurning(currentTime, distance))
          move();
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
      leftMotor.setSpeed(SPEED);
      rightMotor.setSpeed(SPEED);
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
      return (distance <= TOO_CLOSE);
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
      //            }
      state = stateTurning;
      endStateTime = currentTime + random(500, 1000);
    }

    void reverse(unsigned long currentTime){
      leftMotor.setSpeed(-SPEED);
      rightMotor.setSpeed(-SPEED);
      state = stateReversing;
      endStateTime = currentTime + random(500, 1000);
    }

    bool doneTurning(unsigned long currentTime, unsigned int distance)
    {
      if (currentTime >= endStateTime)
        return (distance > TOO_CLOSE);
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
    // RemoteControl remoteControl;
    enum state_t { 
      stateStopped, stateMoving, stateTurning, stateRemote, stateReversing     };
    state_t state;
    unsigned long endStateTime;
    
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
      motor1.setSpeed(-SPEEDz);
      motor2.setSpeed(SPEED);
    }
  };
};

Azdroid::Robot robot;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  robot.run();
}

