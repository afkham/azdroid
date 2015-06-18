#include "remote_control.h"

#define COMMAND_IN_TOPIC "azdroid/in"
#define DATA_OUT_TOPIC "azdroid/out"
#define CDM_IN_TOPIC "azdroid/cdm/in"
#define CDM_OUT_TOPIC "azdroid/cdm/out"

namespace Azdroid
{

    class RemoteControlCallback : public Callback
    {
    public:
        // Callback function
        virtual void callback(char* topic, byte* payload, unsigned int length) 
        {
          String topicStr = String(topic);
          
          if (topicStr.equals(COMMAND_IN_TOPIC)) 
          {
            recdMsg = payload[0];
          } 
          else if (topicStr.equals(CDM_IN_TOPIC))
          {
             //TODO: handle CDM messages
          }
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
        RemoteControl() : RemoteControlDriver(),
                           cc3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER),
                           callback(),
                           client(mqttServerHost, mqttServerPort, &callback, cc3000)
        {
   
        }
        
        virtual void initialize()
        {
            #ifdef ENABLE_WDT
            wdt_reset();
            #endif
            Serial.println("Initializing RemoteControl...");
            cc3000.begin();
          
            if (!cc3000.deleteProfiles()) 
            {
              while(1);
            }
            
            #ifdef DEBUG
            displayMACAddress();
            #endif
            
            #ifdef ENABLE_WDT
            wdt_reset();
            #endif
            char *ssid = WLAN_SSID;             /* Max 32 chars */
            if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY))
            {
              while(1);
            }
            #ifdef ENABLE_WDT
            wdt_reset(); 
            #endif
            while (!cc3000.checkDHCP())
            {
              delay(100); // ToDo: Insert a DHCP timeout!
            }
            #ifdef ENABLE_WDT
            wdt_reset();
            #endif
            #ifdef DEBUG
              // Display the IP address DNS, Gateway, etc.
            while (! displayConnectionDetails()) 
            {
              delay(1000);
            }
            #ifdef ENABLE_WDT
            wdt_reset();
            #endif
            Serial.println("Connecting to MQTT server...");
            #endif
            if (client.connect("azdroid-007")) 
            {
              client.publish(DATA_OUT_TOPIC,"Hello from azdroid-007");
              client.subscribe(COMMAND_IN_TOPIC);
              client.subscribe(CDM_IN_TOPIC);
              #ifdef DEBUG
              Serial.println("MQTT initialization successful");
              #endif
            }  
            #ifdef ENABLE_WDT
            wdt_reset();
            #endif
        }
        
        virtual bool checkRemoteCommand()
        {
            // client.publish("azdroid/out","Hearbeat from azdroid-007");
            bool connected = client.loop();
            if(!connected) 
            {
               initialize(); 
            }
            
            char ch = callback.getMessage();
            if(ch == lastCmd)
            {
              return false;
            } 
            else 
            {
               lastCmd = ch; 
            }
            
            switch (ch) {
                case '0': // stop
                    stop();
                    break;
                case '8': // up
                    goForward();
                    break;
                case '2': // down
                    goBack();
                    break;
                case '4': // left
                    turnLeft();
                    break;
                case '6': // right
                    turnRight();
                    break;
                default: 
                    break;
            }
            return true;
        }
    
    private:
        char lastCmd;
        Adafruit_CC3000 cc3000;
        RemoteControlCallback callback;
        MqttClient client;
          
        void displayMACAddress(void)
        {
          uint8_t macAddress[6];
          
          if(!cc3000.getMacAddress(macAddress))
          {
            Serial.println(F("Unable to retrieve MAC Address!\r\n"));
          }
          else
          {
            Serial.print(F("MAC Address : "));
            cc3000.printHex((byte*)&macAddress, 6);
          }
        }   
        
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
