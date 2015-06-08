#include "remote_control.h"

namespace Azdroid
{

    class RemoteControlCallback : public Callback
    {
    public:
        // Callback function
        virtual void callback(char* topic, byte* payload, unsigned int length) 
        {
          // In order to republish this payload, a copy must be made
          // as the orignal payload buffer will be overwritten whilst
          // constructing the PUBLISH packet.
          
          // Allocate the correct amount of memory for the payload copy
          recdMsg = payload[0];
          // Copy the payload to the new buffer
          //memcpy(recdMsg,payload,length);
          // client.publish("outTopic", p, length);
          // client.publish("outTopic", "Wuhu... got a message");
          // Free the memory
          
          // free(recdMsg);
          
          Serial.print("+++ Received message: ");
          Serial.println(recdMsg);
        }
        
        char getMessage()
        {
           return recdMsg; 
        }
        
        void clear()
        {
           recdMsg = NULL; 
        }
        
      private:
        char recdMsg;  
    };
    
    class RemoteControl : public RemoteControlDriver
    {
    public:
         
        /**
          * @brief Class constructor.
          */
        RemoteControl() : RemoteControlDriver(), lastKey(command_t::keyNone),
                           cc3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER),
                           callback(),
                           client(mqttServerHost, mqttServerPort, &callback, cc3000)
        {
   
        }
        
        virtual void initialize()
        {
            Serial.println("Initializing RemoteControl...");
            cc3000.begin();
          
            if (!cc3000.deleteProfiles()) 
            {
              while(1);
            }
          
            char *ssid = WLAN_SSID;             /* Max 32 chars */
            if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY))
            {
              while(1);
            }
             
            while (!cc3000.checkDHCP())
            {
              delay(100); // ToDo: Insert a DHCP timeout!
            }
            
              // Display the IP address DNS, Gateway, etc.
            while (! displayConnectionDetails()) 
            {
              delay(1000);
            }
          
            Serial.println("Connecting to MQTT server...");
            if (client.connect("azdroid")) 
            {
              client.publish("outTopic","hello world");
              client.subscribe("inTopic");
              Serial.println("MQTT initialization successful");
            }  
        }
        
        virtual bool getRemoteCommand(command_t& cmd)
        {
            cmd.stop();
            cmd.key = command_t::keyNone;
            client.loop();
            
            char ch = callback.getMessage();
            if(ch == lastKey)
            {
              return false;
            }
            switch (ch) {
                case '8': // up
                    cmd.goForward();
                    break;
                case '2': // down
                    cmd.goBack();
                    break;
                case '4': // left
                    cmd.turnLeft();
                    break;
                case '6': // right
                    cmd.turnRight();
                    break;
                case 'A': // function key #1
                case 'C':
                    cmd.key = command_t::keyF1;
                    break;
                case 'B': // function key #2
                case 'D':
                    cmd.key = command_t::keyF2;
                    break;
                case 'E': // function key #3
                    cmd.key = command_t::keyF3;
                    break;
                case 'F': // function key #4
                    cmd.key = command_t::keyF4;
                    break;
                default:
                    break;
            }
            // callback.clear();
            if (cmd.key != command_t::keyNone && cmd.key == lastKey) {
                // repeated key, ignore it
                return false; 
            }
            lastKey = cmd.key;
            return true;
        }
    
    private:
        command_t::key_t lastKey;
        Adafruit_CC3000 cc3000;
        RemoteControlCallback callback;
        MqttClient client;
                
        bool displayConnectionDetails(void)
        {
          uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
          
          if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
          {
            Serial.println(F("Unable to retrieve the IP Address!\r\n"));
            return false;
          }
          else
          {
            Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
            Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
            Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
            Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
            Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
            Serial.println("\n");
            return true;
          }
        }
    };
};
