
namespace Azdroid
{
    class AzdroidFeatures
    {
      
      public:
        AzdroidFeatures() : collisionAvoidance(false), speed(100), remoteMode(true), tooCloseDistance(10)
        {
        }
        
        bool isCollisionAvoidanceEnabled()
        {
          return collisionAvoidance;
        }
        
        void setCollisionAvoidanceEnabled(bool collisionAvoidance)
        {
          this->collisionAvoidance = collisionAvoidance;
        }
        
        unsigned int getSpeed()
        {
          return speed;
        }
        
        void setSpeed(unsigned int speed)
        {
          this->speed = speed;
        }
        
        bool isRemoteMode()
        {
          return remoteMode;
        }
        
        void setRemoteMode(bool remoteMode)
        {
          this->remoteMode = remoteMode;;
        }
        
        unsigned int getTooCloseDistance()
        {
          return tooCloseDistance;
        }
        
        void setTooCloseDistance(unsigned int tooCloseDistance)
        {
          this->tooCloseDistance = tooCloseDistance;
        }
      
      private:
        bool collisionAvoidance;
        unsigned int speed;
        bool remoteMode;
        unsigned int tooCloseDistance;
    
    };

}
