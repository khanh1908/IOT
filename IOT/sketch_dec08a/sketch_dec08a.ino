#include <Wire.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <iostream>
#include <sstream>
#include <string>

#define FIREBASE_HOST "esp8266-30eed-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "VGbwaJejQFyMr5PAAI4x7IxaBc3UowHigKX8jfZV"
#define WIFI_SSID "Nam DN"
#define WIFI_PASSWORD "300320011"

FirebaseData firebaseData;
FirebaseData MotorData;
FirebaseData PumpData;
FirebaseData AutomaticData;
FirebaseJson json;

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define WaterSensor A0
int WaterValue = 0;
int airquality=0;
int doduc=0;

String a;

//int relay2 = 12;
//int relay3 = 14;
String test;
String test1;

void setup() {
  Serial.begin(9600);
  Wire.begin(D1, D2);
  dht.begin();
//  pinMode(relay2, OUTPUT);
//  digitalWrite(relay2, LOW);

//  pinMode(relay3, OUTPUT);
//  digitalWrite(relay3, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("."); 
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  WaterValue = analogRead(WaterSensor);
  Serial.print("Water level Sensor Value:");
  Serial.print(WaterValue);
  Serial.println();
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  Serial.print("Humidity: ");  Serial.print(h);
  String fireHumid = String(h) + String("%");
  Serial.print("%  Temperature: ");  Serial.print(t);  Serial.println("°C ");

  
  char message[100] = "";
  Wire.beginTransmission(8); /* begin with device address 8 */
  Firebase.getString(PumpData, "/FirebaseIOT/pump");
  test = PumpData.stringData();
  Serial.print(test);
  
  Firebase.getString(MotorData, "/FirebaseIOT/Motor");
  test1 = MotorData.stringData();
  Serial.print(test1);
  
//  std::string s = std::to_string(test);
  const char* pchar = test.c_str();  //use char const as target type
//  std::string s1 = std::to_string(test1);
  const char* pchar1 = test1.c_str();
  
  Wire.write(pchar);  /* sends hello slave string */
  Wire.write(pchar1);
  
  Wire.endTransmission();    /* stop transmitting */
  Wire.requestFrom(8, 50); /* request & read data of size 13 from slave */
  String dataMessage= "";
  while(Wire.available()){
    char c = Wire.read();
    if (std::isalnum(c)) dataMessage += c;
  }
  strncpy(message, dataMessage.c_str(), sizeof(message) - 1);
  message[sizeof(message) - 1] = '\0';  // Ensure null-termination

  std::stringstream ss(message);
  std::string token;
  
  std::getline(ss, token, 'a');
  doduc = std::stof(token);

  std::getline(ss, token, 'a');
  airquality = std::stof(token);
  
  Serial.println(airquality);
  Serial.println(doduc);
  if (Firebase.getString(firebaseData, "/FirebaseIOT/AirQuality"))
    {
      a= firebaseData.stringData();
      Serial.println(a);
      Serial.println();
    }
    else
    {
      Serial.println("------------------------------------");
      Serial.println();
    }
   if (doduc>=220)
    {
      Firebase.setString(firebaseData, "/FirebaseIOT/Automatic", "Đục");
    }
    else
    {
      Firebase.setString(firebaseData, "/FirebaseIOT/Automatic", "Tốt");
    }
  if (airquality>=230)
  {
    Firebase.setString(firebaseData, "/FirebaseIOT/AirQuality", "Xấu");
  }
  else
  {
    Firebase.setString(firebaseData, "/FirebaseIOT/AirQuality", "Tốt");
  }

  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/turbidity", doduc))
  {
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("------------------------------------");
    Serial.println();
  }
  
  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/temperature", t))
  {
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("------------------------------------");
    Serial.println();
  }
  if (Firebase.setDouble(firebaseData, "/FirebaseIOT/humidity", h))
  {
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("------------------------------------");
    Serial.println();
  }
  if (Firebase.setDouble(firebaseData, "/FirebaseIOT/WaterValue", WaterValue))
  {
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("------------------------------------");
    Serial.println();
  }
//  Firebase.getInt(firebaseData, "/FirebaseIOT/turbidity");
//  if (Firebase.getInt(firebaseData, "/FirebaseIOT/turbidity")>=400) {
//      Firebase.setString(AutomaticData, "/FirebaseIOT/Automatic", "OFF");
//    }
//    else{
//      Firebase.setString(AutomaticData, "/FirebaseIOT/Automatic", "On");
//    }
//
//  if (Firebase.getString(PumpData, "/FirebaseIOT/pump")) {
//    Serial.println(PumpData.stringData());
//    if (PumpData.stringData() == "1") {
//      if ((PumpData.stringData() == "1") || (h <= 70 && t > 30 && WaterValue < 50)) {
//      digitalWrite(relay3, HIGH);
//      Firebase.setString(AutomaticData, "/FirebaseIOT/pump", "1");
//    }
//    else if (PumpData.stringData() == "0") {
//      digitalWrite(relay3, LOW);
//    } 
//  }
//  if (h <= 70 && t > 30 && WaterValue < 50) {
//    digitalWrite(relay3, HIGH);
//    Firebase.setString(AutomaticData, "/FirebaseIOT/Automatic", "On");
//  }
//  else {
//    digitalWrite(relay3, LOW);
//    Firebase.setString(AutomaticData, "/FirebaseIOT/Automatic", "OFF");
//  }
//  Firebase.getString(AutomaticData, "/FirebaseIOT/Automatic");
//  Serial.println(MotorData.stringData());

  //  if (Firebase.getString(AutomaticData, "/FirebaseIOT/Automatic")) {
  //    Serial.println(AutomaticData.stringData());
  //    if (AutomaticData.stringData() == "1" && h < 100) {
  //      Firebase.setString(MotorData, "/FirebaseIOT/Motor", 0);
  //        digitalWrite(relay2, HIGH);
  //    }
  //    else if (AutomaticData.stringData() == "0") {
  //      digitalWrite(relay2, LOW);
  //    }
  //  }
  //  delay(100);
}
