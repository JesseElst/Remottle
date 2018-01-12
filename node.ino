/* ************************************
 * Arduino MKR1000 buttons to MQTT
 *
 * This sample shows you how to connect 3 buttons to your mqtt server.
 * It uses some variables to save the previous state.
 * This way only state changes get send.
 *
 * The debug serial is set to 115200, so if you see a strange output, check the speed!!
 *
 * WiFi101.h needs to be installed with the library manager (search for WiFi101)
 * MQTTClient.h is found in the library manager (MQTT by Joel
 *
 **************************************/
#include <SPI.h>
#include <WiFi101.h>
#include <MQTTClient.h>
#include <Wire.h>
#include <LSM6.h>

LSM6 imu;

// constants won't change. They're used here to
// set pin numbers:

int water;
int laatsteslok;
// variables will change:
// variable for reading the pushbutton status
int lastStateRed = 0;
int lastStateOrange = 0;
int lastStateBlue = 0;

//Wifi data
char *ssid ="12connect";
WiFiClient net;
MQTTClient mqtt;


// This is the client Id that will be used, the mac will be append!
String mqttClientId = "mkr-";
// This is the start of the topic that will be used. ClientID and button will be append
String buttonTopic = "public/i363602_water/";

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  if (!imu.init())
  {
    Serial.println("Failed to detect and initialize IMU!");
    while (1);
  }
  imu.enableDefault();
  // initialize the LED pin as an output:

  // Try to start and wait for serial connection (or timeout after 5 seconds)
  // Debug serial SET to 115200!!
  while ((!Serial) && (millis() < 5000));
  Serial.begin(115200);

  // Init WiFi
  WiFi.begin(ssid);

  // Init MQTT with correct server (this is no connect!)
  mqtt.begin("mqtt.fhict.nl", net);

  // Get the mac and setup the ClientID and topic
  uint8_t mac[6];
  WiFi.macAddress(mac);
  mqttClientId += macToStr(mac);
  Serial.println("Client ID: " + mqttClientId);
  buttonTopic += mqttClientId + "/button";
  Serial.println("Publish topic: " + buttonTopic);

  connect();
}

void loop() {
  imu.read();
if(imu.a.z <= 6000)
{ delay(100);
  water = water + 3;
 
  
}
else
{  
  if(laatsteslok != water)
  {
   delay(1000);
   sendtilt(water); 
   laatsteslok = water;
   
  }
}






}



void sendtilt(int value){
 
  if(!mqtt.connected()) {
    connect();
  }

  
  mqtt.publish("public/i363602_water/tilt/", String(value) );
  Serial.print("Je hebt nu ");
Serial.print(water);
  Serial.println(" ml water gedronken vandaag" );

}



// Call to connect to the MQTT server
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!mqtt.connect((char *)mqttClientId.c_str(), "i363602_water", "Dwa7tsgZm5NV7B")) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");
}

// Convert the MAC to a string
String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
  }
  return result;
}

// This is not used, but this way you can receive messages.
void messageReceived(String topic, String payload, char * bytes, unsigned int length) {
  Serial.print("incoming: ");
  Serial.print(topic);
  Serial.print(" - ");
  Serial.print(payload);
  Serial.println();
}

