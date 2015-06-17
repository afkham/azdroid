namespace Azdroid
{
    class AzdroidFeatures
    {
      
      public:
        AzdroidFeatures() : collisionAvoidance(false), speed(150), remoteMode(true), tooCloseDistance(10)
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
        
        int getSpeed()
        {
          return speed;
        }
        
        void getSpeed(int speed)
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
        
        int getTooCloseDistance()
        {
          return tooCloseDistance;
        }
        
        void setTooCloseDistance(int tooCloseDistance)
        {
          this->tooCloseDistance = tooCloseDistance;
        }
      
      private:
        bool collisionAvoidance;
        int speed;
        bool remoteMode;
        int tooCloseDistance;
    
    };

}
