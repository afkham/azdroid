
namespace Azdroid
{
    class RemoteControlDriver
    {
    private:
       int leftSpeed;
       int rightSpeed;
      
    public:
        RemoteControlDriver(AzdroidFeatures* features) : _features(features), leftSpeed(0), rightSpeed(0)
        {
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
        AzdroidFeatures* _features;
        
        void goForward()
        {
            leftSpeed = rightSpeed = _features->getSpeed();
        }
        
        void goBack()
        {
            leftSpeed = rightSpeed = -_features->getSpeed();
        }
        
        void turnLeft()
        {
            leftSpeed = -_features->getSpeed();
            rightSpeed = _features->getSpeed();
        }
        
        void turnRight()
        {
            leftSpeed = _features->getSpeed();
            rightSpeed = -_features->getSpeed();
        }
        
        void stop()
        {
            leftSpeed = rightSpeed = 0;
        }  
    };
};
