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

int water;
int laatsteslok;

//Wifi data
char *ssid ="12connect";
WiFiClient net;
MQTTClient mqtt;


// Gegevens client
String mqttClientId = "mkr-";
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
  while ((!Serial) && (millis() < 5000));
  Serial.begin(115200);

  // maak verbinding met netwerk
  WiFi.begin(ssid);

  // maak verbinding met mqtt
  mqtt.begin("mqtt.fhict.nl", net);

  connect();
}


void loop() {
  imu.read();
    //als fles schuin staat
if(imu.a.z <= 6000)
{  //elke 0.1 seconden
    delay(100);
    //tel 3 mililiter bij
  water = water + 3;
}
    
//als de fles op de grond staat
else
{  
  //als je net hebt gedronken
  if(laatsteslok != water)
  {
   //wacht een seconden
   delay(1000);
   //stuur aantal mililiter
   sendtilt(water); 
   // maak water weer normaal
   laatsteslok = water;
   
  }
}
}



void sendtilt(int value){
 //kijk of hij verbinding heeft met mqtt
  if(!mqtt.connected()) {
    connect();
  }

//verstuur bericht naar mqtt broker  
  mqtt.publish("public/i363602_water/tilt/", String(value) );
  Serial.print("Je hebt nu ");
Serial.print(water);
  Serial.println(" ml water gedronken vandaag" );

}



// maak verbinding met wifi
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

