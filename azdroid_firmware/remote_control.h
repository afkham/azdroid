
namespace Azdroid
{
    class RemoteControlDriver
    {
    private:
       unsigned int speed;
       int leftSpeed;
       int rightSpeed;
      
    public:
        RemoteControlDriver() : speed(150), leftSpeed(0), rightSpeed(0)
        {
        }
         
        void setSpeed(unsigned int speed)
        {
           this->speed = speed; 
        }
     
        /**
         * Check if a remote command is available
         * @return true if a remote command is available, false if not.
         */
        virtual bool checkRemoteCommand() = 0;
        
        virtual void initialize();
        
        int getLeftSpeed()
        {
          return leftSpeed;
        }
        
        int getRightSpeed()
        {
          return rightSpeed;
        }
        
      protected:
        void goForward()
        {
            leftSpeed = rightSpeed = speed;
        }
        void goBack()
        {
            leftSpeed = rightSpeed = -speed;
        }
        void turnLeft()
        {
            leftSpeed = -speed;
            rightSpeed = speed;
        }
        void turnRight()
        {
            leftSpeed = speed;
            rightSpeed = -speed;
        }
        void stop()
        {
            leftSpeed = rightSpeed = 0;
        }  
    };
};
