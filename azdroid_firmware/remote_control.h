
namespace Azdroid
{
    class RemoteControlDriver
    {
    private:
       static const int speed = 150;
      
    public:
        /**
          * @brief abstract representation of a remote command.
          */
        struct command_t {
            enum key_t { keyNone, keyF1, keyF2, keyF3, keyF4 };
            int left;   /**< left side speed, between -255 and 255. */
            int right;  /**< right side speed, between -255 and 255. */
            key_t key;  /**< function key. */
            
            command_t() : left(0), right(0), key(keyNone) {}
            void goForward()
            {
                left = right = speed;
            }
            void goBack()
            {
                left = right = -speed;
            }
            void turnLeft()
            {
                left = -speed;
                right = speed;
            }
            void turnRight()
            {
                left = speed;
                right = -speed;
            }
            void stop()
            {
                left = right = 0;
            }
            void leftAndRightSliders(int l, int r)
            {
                left = l;
                right = r;
            }
            void forwardBackAndLeftRightSliders(int fb, int lr)
            {
                left = fb - lr;
                right = fb + lr;
                if (left < -speed)
                    left = -speed;
                else if (left > speed)
                    left = speed;
                if (right < -speed)
                    right = -speed;
                else if (right > speed)
                    right = speed;
            }
            void joystick(int x, int y)
            {
                forwardBackAndLeftRightSliders(y, x);
            }
        };
        
        /**
          * @brief Class constructor.
          */
        RemoteControlDriver() {}

        /**
         * @brief Return the next remote command, if available.
         * @param cmd a reference to a command_t struct where the command
         *   information will be stored.
         * @return true if a remote command is available, false if not.
         */
        virtual bool getRemoteCommand(command_t& cmd) = 0;
        
        virtual void initialize();
    };
};
