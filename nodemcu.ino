//ThatsEngineering
//Sending Data from Arduino to NodeMCU Via Serial Communication
//NodeMCU code

//Include Lib for Arduino to Nodemcu
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>

#define FIREBASE_HOST "dodi-ta-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "fymt4d2veK1bq2KtfZLs3n9NWHBcLjoqNfTvmlLN"

const char* ssid="Redmi Note 9 Pro";
const char* password = "santuy123";

//D3 = Rx & D2 = Tx
SoftwareSerial nodemcu(D3, D2);

void setup() {
  // Initialize Serial port
  Serial.begin(115200);
  nodemcu.begin(115200);
  while (!Serial) continue;

  //Konek ke WiFi
  Serial.print("Wifi connecting to ");
  Serial.println( ssid );

  WiFi.begin(ssid,password);

  Serial.println();
  Serial.print("Connecting");

  while( WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");
  }
  Serial.println("Wifi Connected Success!");
  Serial.print("NodeMCU IP Address : ");
  Serial.println(WiFi.localIP() );

  //Ke Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(nodemcu);

  if (data == JsonObject::invalid()) {
    //Serial.println("Invalid Json Object");
    jsonBuffer.clear();
    return;
  }

  Serial.println("JSON Object Recieved");
  Serial.print("Recieved NO2:  ");
  float no2 = data["no2"];
  Serial.println(no2);
  Serial.print("Recieved CO:  ");
  float co = data["co"];
  Serial.println(co);
  Serial.print("Received dB: ");
  float dB = data["dB"];
  Serial.println(dB);
  Serial.print("Received H2S: ");
  float h2s = data["h2s"];
  Serial.println(h2s);
  Serial.print("Received NH3: ");
  float nh3 = data["nh3"];
  Serial.println(nh3);
  Serial.println("-----------------------------------------");

      data["no2"] = no2;
      data["co"] = co;
      data["dB"] = dB;
      data["h2s"] = h2s;
      data["nh3"] = nh3;

      //Data ciganitri weekdays
      //String name = Firebase.push("/jlCiganitri_siang/sensor/index_udara", data);
      //String name = Firebase.push("/jlCiganitri_sore/sensor/index_udara", data);

      //Data ciganitri weekend
      //String name = Firebase.push("/jlCiganitri_siang_weekend/sensor/index_udara", data);
      //String name = Firebase.push("/jlCiganitri_sore_weekend/sensor/index_udara", data);
      
      //Data Isbi Buah Batu weekdays
      //String name = Firebase.push("/jlisbi_siang/sensor/index_udara", data);
      //String name = Firebase.push("/jlisbi_sore/sensor/index_udara", data);

      //Data Isbi Buah Batu weekend
      //String name = Firebase.push("/jlisbi_siang_weekend/sensor/index_udara", data);
      //String name = Firebase.push("/jlisbi_sore_weekend/sensor/index_udara", data);

      //Data uji coba kosan
      String name = Firebase.push("/kosan/sensor/index_udara", data);
      delay(3000);
}
