//Include the library
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <MQUnifiedsensor.h>
#include "MQ7.h"

int pinSensorMQ135 = A0;
float Bmq135 = 1.42, Mmq135 = -0.63, Hambatanmq135, bacamq135, VRLmq135, Rsmq135, ROmq135 = 6.00, ppm_logmq135, PPMmq135; // ROmq135 = 6 Kohm

//Gas Sensor Pins
#define F2602 A2

//Gas Sensor Load Resistance (RL)
#define RL_F2602 7.5

SoftwareSerial nodemcu(2, 3); //2=Rx, 3=Tx

/*2602 GASSES*/
float NH3_A = 0.92372844;
float NH3_B = -0.291578925;

float H2S_A = 0.38881036;
float H2S_B = -0.35010059;

//float VOC_A = 0.3220722;
//float VOC_B = -0.6007520; 


//Declare Sensor
//MQUnifiedsensor MQ135(placa, Voltage_Resolution, ADC_Bit_Resolution, pin, type);
MQ7 mq7(A1, 5.0);

float gasA = 605.18;
float gasB = -3.937;
 
void setup() {
  //Init the serial port communication - to debug the library
  Serial.begin(9600); //Init serial port

  nodemcu.begin(115200);
  delay(1000);

  Serial.println("Program started");
}
void loop()
{
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  bacamq135 = analogRead(A2);
  VRLmq135 = bacamq135*5.00/1023;
  Rsmq135 = (5.00/VRLmq135-1)*10; // dari rumus Rs = (Vc/VRL-1)*RL , RL=10 KOhm
  Hambatanmq135 = Rsmq135/ROmq135;
  ppm_logmq135 = (log10(Hambatanmq135)-Bmq135)/Mmq135;
  PPMmq135 = ((pow (10, ppm_logmq135))*200)/10000;
  Serial.print("NO2: ");
  Serial.print(PPMmq135);
  Serial.println(" PPM");
  data["no2"] = PPMmq135;
  //MQ135.serialDebug(); // Will print the table on the serial port

  float co = mq7.getPPM();
  Serial.print("CO : ");
  Serial.print(co);
  Serial.println(" PPM");
  delay(1000);
  data["co"] = co;
  // read the input on analog pin 0:
  
  int sensorValue = analogRead(A5);
  float dB = (sensorValue+83.2073) / 11.003; //Convert ADC value to dB using Regression values
  Serial.print("mic value: ");
  Serial.print(dB);
  Serial.println(" dB");
  delay(1000);
  data["dB"] = dB;
  
  //Refer to the documentation mentioned on this page to understand this better
  float VRL_F2602; 
  float Rs_F2602; 
  float Ro_F2602 = 64.88;
  float ratio_F2602;
   
  VRL_F2602 = analogRead(F2602)*(5.0/1023.0); 
  Rs_F2602 = ((5.0/VRL_F2602)-1)*(RL_F2602); 
  ratio_F2602 = Rs_F2602/Ro_F2602;

  //float ppm_VOC = VOC_A * pow(ratio_F2602, VOC_B);
  float ppm_H2S = H2S_A * pow(ratio_F2602, H2S_B);  
  float ppm_NH3 = NH3_A * pow(ratio_F2602, NH3_B);

  //Serial.print("voc: ");
  //Serial.println(ppm_VOC);
  Serial.print("h2s: ");
  Serial.print(ppm_H2S);
  Serial.println(" PPM");
  Serial.print("nh3: ");
  Serial.print(ppm_NH3);
  Serial.println(" PPM");
  delay(1000);
  data["h2s"] = ppm_H2S;
  data["nh3"] = ppm_NH3;

  data.printTo(nodemcu);
  jsonBuffer.clear();
      
}
