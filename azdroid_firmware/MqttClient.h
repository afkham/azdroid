
#ifndef MqttClient_h
#define MqttClient_h

#include <Arduino.h>
#include "Adafruit_CC3000.h"
#include "Stream.h"

// MQTT_MAX_PACKET_SIZE : Maximum packet size
#define MQTT_MAX_PACKET_SIZE 128

// MQTT_KEEPALIVE : keepAlive interval in Seconds
#define MQTT_KEEPALIVE 15

#define MQTTPROTOCOLVERSION 3
#define MQTTCONNECT     1 << 4  // Client request to connect to Server
#define MQTTCONNACK     2 << 4  // Connect Acknowledgment
#define MQTTPUBLISH     3 << 4  // Publish message
#define MQTTPUBACK      4 << 4  // Publish Acknowledgment
#define MQTTPUBREC      5 << 4  // Publish Received (assured delivery part 1)
#define MQTTPUBREL      6 << 4  // Publish Release (assured delivery part 2)
#define MQTTPUBCOMP     7 << 4  // Publish Complete (assured delivery part 3)
#define MQTTSUBSCRIBE   8 << 4  // Client Subscribe request
#define MQTTSUBACK      9 << 4  // Subscribe Acknowledgment
#define MQTTUNSUBSCRIBE 10 << 4 // Client Unsubscribe request
#define MQTTUNSUBACK    11 << 4 // Unsubscribe Acknowledgment
#define MQTTPINGREQ     12 << 4 // PING Request
#define MQTTPINGRESP    13 << 4 // PING Response
#define MQTTDISCONNECT  14 << 4 // Client is Disconnecting
#define MQTTReserved    15 << 4 // Reserved

#define MQTTQOS0        (0 << 1)
#define MQTTQOS1        (1 << 1)
#define MQTTQOS2        (2 << 1)

namespace Azdroid {
  
  class Callback {
  
    public:
      virtual void callback(char*,uint8_t*,unsigned int);
  };
  
  class MqttClient {
  private:
     Adafruit_CC3000* _cc3000;
  //   Adafruit_CC3000_Client* _client;
     Adafruit_CC3000_Client _client;
     bool clientExists;
     uint8_t buffer[MQTT_MAX_PACKET_SIZE];
     uint16_t nextMsgId;
     unsigned long lastOutActivity;
     unsigned long lastInActivity;
     bool pingOutstanding;
     void (*callback)(char*,uint8_t*,unsigned int);
     Callback* callbackObj;
     uint16_t readPacket(uint8_t*);
     uint8_t readByte();
     boolean write(uint8_t header, uint8_t* buf, uint16_t length);
     uint16_t writeString(char* string, uint8_t* buf, uint16_t pos);
     uint8_t *ip;
     char* domain;
     uint16_t port;
     Stream* stream;
  public:
     MqttClient();
     MqttClient(uint8_t *, uint16_t, void(*)(char*,uint8_t*,unsigned int),Adafruit_CC3000& cc3000);
     MqttClient(uint8_t *, uint16_t, Callback*, Adafruit_CC3000& cc3000);
     MqttClient(uint8_t *, uint16_t, void(*)(char*,uint8_t*,unsigned int),Adafruit_CC3000& cc3000, Stream&);
     MqttClient(char*, uint16_t, void(*)(char*,uint8_t*,unsigned int),Adafruit_CC3000& cc3000);
     MqttClient(char*, uint16_t, void(*)(char*,uint8_t*,unsigned int),Adafruit_CC3000& cc3000, Stream&);
     boolean connect(char *);
     boolean connect(char *, char *, char *);
     boolean connect(char *, char *, uint8_t, uint8_t, char *);
     boolean connect(char *, char *, char *, char *, uint8_t, uint8_t, char*);
     void disconnect();
     boolean publish(char *, char *);
     boolean publish(char *, uint8_t *, unsigned int);
     boolean publish(char *, uint8_t *, unsigned int, boolean);
     boolean publish_P(char *, uint8_t PROGMEM *, unsigned int, boolean);
     boolean subscribe(char *);
     boolean subscribe(char *, uint8_t qos);
     boolean unsubscribe(char *);
     boolean loop();
     boolean connected();
  };
};


#endif