#include <ESP8266WiFi.h> //memanggil library ESP8266
#include <PubSubClient.h> //memanggil library pubsubclient

const char *ssid = "Jomblo"; // ssid wifi 
const char *pass = "sanita21102017"; //  password wifi
String flag;




// device details
char *deviceId = "device-a1115ecf7e0c39dbb4d6d6ca8a77568c";
char *deviceUsername = "a90aa23051357810786bcea2b19e2807";
char *devicePassword = "5fd869fae71782bea35b2d6047e28887";
String deviceCredential = String(deviceUsername) + ":" + String(devicePassword);

WiFiClient wclient;
PubSubClient client("geeknesia.com",1883,wclient);
void senddata(char* topic, String credential, char *var1, String nilai1 ) {

String pubString  = "{\"code\":\"";
 pubString += credential;
 pubString += "\",";
 pubString += "\"attributes\":{\"";
 pubString += String(var1) + "\":\"" + nilai1 + "\"";
 pubString += "}}";
 char message_buff[pubString.length() + 1];
 pubString.toCharArray(message_buff, pubString.length() + 1);
 client.publish("iot/data",message_buff);
 Serial.println(message_buff);
 //client.publish("iot/data",pubString);
}

void setup() {
 Serial.begin(115200);
pinMode(2, OUTPUT); // Initialize the BUILTIN_LED pin as an output
 pinMode(12, INPUT_PULLUP);
}


// the loop function runs over and over again forever
void loop() {
 if (WiFi.status() != WL_CONNECTED) {
 WiFi.begin(ssid, pass);
 Serial.println("connecting Wifi");
 if (WiFi.waitForConnectResult() != WL_CONNECTED)
 return;
 Serial.println("WiFi connected");
 }
 if (WiFi.status() == WL_CONNECTED) {
 if (!client.connected()) {
 if (client.connect(deviceId, "iot/will", 0, 0, deviceId)) {
 }
 }
 if (client.connected())
 client.loop();
 }
 int sensorValue = analogRead(A0);
 flag = sensorValue * (5.0 / 1023.0); 
 senddata(deviceId,deviceCredential,"S1",flag);
 delay(100);
}
