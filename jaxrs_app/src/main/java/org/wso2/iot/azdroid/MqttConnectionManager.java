package org.wso2.iot.azdroid;

import org.eclipse.paho.client.mqttv3.IMqttActionListener;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.IMqttToken;
import org.eclipse.paho.client.mqttv3.MqttAsyncClient;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;

public class MqttConnectionManager {

	public static MqttConnectionManager instance = new MqttConnectionManager();

	MqttAsyncClient mqttClient;
	MqttConnectOptions connOpt;

	static final String BROKER_URL = "tcp://localhost:1883";
	static final String COMMAND_TOPIC = "inTopic";

	private MqttConnectionManager() {
		init();
	}

	public static MqttConnectionManager getInstance() {
		return instance;
	}

	public void publish(String command) {
		
		int pubQoS = 2;
		MqttMessage message = new MqttMessage(command.getBytes());
		message.setQos(pubQoS);
		message.setRetained(false);

		// Publish the message
		System.out.println("Publishing to topic \"" + COMMAND_TOPIC + "\" qos " + pubQoS);
		// MqttDeliveryToken token = null;
		try {
			// publish message to broker

			mqttClient.publish(COMMAND_TOPIC, message, "test", new IMqttActionListener() {

				public void onSuccess(IMqttToken iMqttToken) {
					System.out.println("Publishing successful");
				}

				public void onFailure(IMqttToken iMqttToken, Throwable throwable) {
					System.err.println("Publishing failed");
				}
			}).waitForCompletion();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void init() {
		// setup MQTT Client
		// String clientID = M2MIO_THING;
		connOpt = new MqttConnectOptions();

		connOpt.setCleanSession(true);
		connOpt.setKeepAliveInterval(30);
		// connOpt.setUserName(M2MIO_USERNAME);
		// connOpt.setPassword(M2MIO_PASSWORD_MD5.toCharArray());

		// Connect to Broker
		try {
			mqttClient = new MqttAsyncClient(BROKER_URL, "azdroid-commander");
			mqttClient.setCallback(new MqttCallback() {

				public void connectionLost(Throwable throwable) {
					System.out.println("Connection lost: " + throwable.getMessage());
				}

				public void messageArrived(String topic, MqttMessage mqttMessage) throws Exception {
					System.out.println("Message received");
					System.out.println("-------------------------------------------------");
					System.out.println("| Topic:" + topic);
					System.out.println("| Message: " + new String(mqttMessage.getPayload()));
					System.out.println("-------------------------------------------------");
				}

				public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {
					System.out.println("Delivery complete");
				}
			});
			IMqttToken connect = mqttClient.connect(connOpt, new IMqttActionListener() {

				public void onSuccess(IMqttToken iMqttToken) {
					System.out.println("Connection successful: " + iMqttToken.getMessageId());
				}

				public void onFailure(IMqttToken iMqttToken, Throwable throwable) {
					System.out.println("Connection failed: " + iMqttToken.getMessageId());
				}
			});
			connect.waitForCompletion();
		} catch (MqttException e) {
			e.printStackTrace();
			System.exit(-1);
		}

		System.out.println("Connected to " + BROKER_URL);
	}
}
