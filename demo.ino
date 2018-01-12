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

// constants won't change. They're used here to
// set pin numbers:
const int buttonRed = 7;
const int buttonOrange = 8;
const int buttonBlue = 9;


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
String buttonTopic = "public/i872953_arduino/";

void setup() {
  // initialize the LED pin as an output:
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonRed, INPUT);
  pinMode(buttonOrange, INPUT);
  pinMode(buttonBlue, INPUT);

  // Try to start and wait for serial connection (or timeout after 5 seconds)
  // Debug serial SET to 115200!!
  while ((!Serial) && (millis() < 5000));
  Serial.begin(115200);

  Serial.println("Arduino buttons to MQTT");

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

  // read the states of the pushbutton value:
  int stateRed = digitalRead(buttonRed);
  // Uncomment this for more buttons
  //int stateOrange = digitalRead(buttonOrange);
  //int stateBlue = digitalRead(buttonBlue);


  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
//  if (stateRed == HIGH || stateOrange == HIGH || stateBlue == HIGH) {
  if (stateRed == HIGH){
    // turn LED on:
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    // turn LED off:
    digitalWrite(LED_BUILTIN, LOW);
  }

  if(lastStateRed != stateRed){
    sendButtonStatus("red",stateRed);

    lastStateRed = stateRed;
    Serial.println("Send red state: " + String(stateRed));
  }

  // if(lastStateOrange != stateOrange){
  //   sendButtonStatus("orange",stateOrange);
  //
  //   lastStateOrange = stateOrange;
  //   Serial.println("Send orange state: " + String(stateOrange));
  // }

  // if(lastStateBlue != stateBlue){
  //   sendButtonStatus("blue",stateBlue);
  //
  //   lastStateBlue = stateBlue;
  //   Serial.println("Send blue state: " + String(stateBlue));
  // }
}

void sendButtonStatus(String btnName,int value){
  String topic =buttonTopic + btnName;
  if(!mqtt.connected()) {
    connect();
  }

  if(value){
    mqtt.publish(topic,"on");
  } else {
    mqtt.publish(topic,"off");
  }

}



// Call to connect to the MQTT server
void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!mqtt.connect((char *)mqttClientId.c_str(), "i872953_arduino", "Xdt1Y5aptEFN67")) {
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

