#define BLYNK_TEMPLATE_ID "TMPLk_RRhbU5"
#define BLYNK_DEVICE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "H98J4iMtP3gThn6evVzPscZKPzV_G5FR"
#define API_KEY "AIzaSyByGf-0vRjWqaRyKUE5jxSyy1PBI5PMInA"
#define DATABASE_URL "https://adoptech-7a15d-default-rtdb.asia-southeast1.firebasedatabase.app/" 
#define WIFI_SSID "mintch2001_2.4G"
#define WIFI_PASSWORD "cnkcn0853654129"
#define SheetHost "script.google.com"
#define SheetPort 443

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Firebase_ESP_Client.h>
#include <WiFiClientSecure.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

char blynkAuth[] = BLYNK_AUTH_TOKEN;
char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASSWORD;

BlynkTimer timer;
FirebaseData fbdo;
FirebaseAuth firebaseAuth;
FirebaseConfig config;
WiFiClientSecure client;

int count = 0;
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
long now = millis();
long lastMeasure = 0;
String GAS_ID = "AKfycbx0tV3XPXSl4wkdsXUDcQKsV2O_T_8g6dcSgV4jCGPKKRX4VcikglZ1vuQhBc7cN7FX";
BLYNK_WRITE(V0)
{
  int value = param.asInt();
  Blynk.virtualWrite(V1, value);
}


BLYNK_CONNECTED()
{
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void myTimerEvent()
{
  
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(blynkAuth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  client.setInsecure();
  
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &firebaseAuth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  Firebase.begin(&config, &firebaseAuth);
  Firebase.reconnectWiFi(true);
}
int sheetSend = 0;
void loop()
{
  Blynk.run();
  timer.run();
  if(Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 3000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    FirebaseIntWrite("test/int", count);
    Blynk.virtualWrite(V4, count);
    count++;
    if(sheetSend <= 3)
    {
      sendData(count); //sendData
      sheetSend++;
    }
    
  }
}
void FirebaseIntWrite(char path[], int value)
{
  if (Firebase.RTDB.setInt(&fbdo, path, value)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
}
void sendData(float test) { //ใช้เพิ่มตัวแปรและกำหนดตัวแปรที่จะส่งข้อมูล
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(SheetHost);
  
  //----------------------------------------Connect to Google host //เชื่อมต่อกับ Host ของ Google
  if (!client.connect(SheetHost, SheetPort)) {
    Serial.println("connection failed");
    return;
  }

  //url for send
  String url = "/macros/s/" + GAS_ID + "/exec?test=" + string_test; 
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + SheetHost + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  //----------------------------------------

  //---------------------------------------
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\"")) {
    Serial.println("esp8266/Arduino CI successfull!");
  } else {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  //----------------------------------------
}
