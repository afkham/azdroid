
#include "Arduino.h"
#include "motor_driver.h"

namespace Azdroid
{
    class Motor : public MotorDriver
    {
    public:
        
        Motor(int pin1, int pin2, int pinEnable)
            : MotorDriver(), currentSpeed(0)
        {
          
          this->pin1 = pin1;
          this->pin2 = pin2;
          this->pinEnable = pinEnable;
          pinMode(pin1, OUTPUT);
          pinMode(pin2, OUTPUT);
          pinMode(pinEnable, OUTPUT);
        }

        void setSpeed(int speed)
        {
            currentSpeed = speed;
            
            if (speed == 0) {
                digitalWrite(this->pin1, LOW); 
                digitalWrite(this->pin2, LOW); 
                analogWrite(this->pinEnable, speed);
            } else if (speed > 0){
                Serial.println("Moving forward");
                digitalWrite(this->pin1, HIGH); 
                digitalWrite(this->pin2, LOW); 
                analogWrite(this->pinEnable, speed);
            } else {
                Serial.println("++++++ Reversing");
                digitalWrite(this->pin1, LOW); 
                digitalWrite(this->pin2, HIGH); 
                analogWrite(this->pinEnable, -speed);
            }
        }
        
        int getSpeed() const
        {
            return currentSpeed;
        }
        
    private:
        int pin1;
        int pin2;
        int pinEnable;
        int currentSpeed;
    };
};
