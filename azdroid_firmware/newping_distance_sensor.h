
#include "distance_sensor.h"

namespace Azdroid
{
    class DistanceSensor : public DistanceSensorDriver
    {
    public:
        DistanceSensor(int triggerPin, int echoPin, int maxDistance)
            : DistanceSensorDriver(maxDistance), 
              sensor(triggerPin, echoPin, maxDistance)
        {
        }
        
        virtual unsigned int getDistance()
        {
            int distance = sensor.ping_cm();
            if (distance <= 0)
                return maxDistance;
            return distance;
        }
    private:
        NewPing sensor;
    };
};
