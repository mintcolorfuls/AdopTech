#define BLYNK_TEMPLATE_ID           "TMPLk_RRhbU5"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "H98J4iMtP3gThn6evVzPscZKPzV_G5FR"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define Peltier D0
#define PumpWater D1
#define Fans D2

#define ONE_WIRE_BUS D4

//global variable
float waterTemp = 0;

int isAuto = 0;
int pelTierState = 0;
int PumpState = 0;
int FansState = 0;


char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "mintch2001_2.4G";
char pass[] = "cnkcn0853654129";

BlynkTimer timer;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

BLYNK_WRITE(V3)
{
  pelTierState = param.asInt();

  if(isAuto == 0)
  {
    digitalWrite(Peltier, !pelTierState);
  }
  else 
  {
    Blynk.virtualWrite(V3, !pelTierState);
  }
}
BLYNK_WRITE(V0)
{
  PumpState = param.asInt();
  if(isAuto == 0)
  {
    digitalWrite(PumpWater, !PumpState);
  }
  else 
  {
    Blynk.virtualWrite(V0, !PumpState);
  }
  
}
BLYNK_WRITE(V1)
{
  FansState = param.asInt();
  if(isAuto == 0)
  {
    digitalWrite(Fans, !FansState);
  }
  else 
  {
    Blynk.virtualWrite(V1, !FansState);
  }
}
BLYNK_WRITE(V2)
{
  isAuto = param.asInt();

  if(isAuto == 0)
  {
    AutoWaterTempareture(waterTemp);
  }
  
}

BLYNK_CONNECTED()
{
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

void myTimerEvent()
{
  Blynk.virtualWrite(V8, waterTemp);
}

void setup()
{
  Serial.begin(115200);

  pinMode(Peltier, OUTPUT);
  pinMode(PumpWater, OUTPUT);
  pinMode(Fans, OUTPUT);
  
  digitalWrite(Peltier, 1); //สั่งให้ไฟดับ
  digitalWrite(PumpWater, 1); //สั่งให้ไฟดับ
  digitalWrite(Fans, 1); //สั่งให้ไฟดับ
  
  sensors.begin();
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, myTimerEvent);
}

void loop()
{  
  sensors.requestTemperatures(); //อ่านค่าอุณหภูมิ
  waterTemp = sensors.getTempCByIndex(0); //แสดงค่าอุณหภูมิ
  
  if(isAuto == 1)
  {
    AutoWaterTempareture(waterTemp);
  }
  Serial.print("Now isAuto");
  Serial.println(isAuto);
  
  Blynk.run();
  timer.run();
}
void AutoWaterTempareture(int waterTemp)
{
  if(waterTemp <= 22)
  {
    ColdOff();
  }
  else if (waterTemp >= 27)
  {
    ColdOn();
  }
}
void ColdOff()
{
    digitalWrite(Peltier, !pelTierState); // กลับด้านจาก 0 เป็น 1 เพราะ NodeMCU v3 นั้นใช้ค่า 1 ในการหยุดจ่ายไฟ
    Blynk.virtualWrite(V3, pelTierState); // ใช้ค่า 0 เพื่อปิดสวิชท์
    
    digitalWrite(PumpWater, !PumpState);
    Blynk.virtualWrite(V0, PumpState);
    
    digitalWrite(Fans, !FansState);
    Blynk.virtualWrite(V1, FansState);
}
void ColdOn()
{
    digitalWrite(Peltier, pelTierState);   // กลับด้านจาก 1 เป็น 0 เพราะ NodeMCU v3 นั้นใช้ค่า 0 ในการจ่ายไฟ 
    Blynk.virtualWrite(V3, !pelTierState); // ใช้ค่า 1 เพื่อเปิดสวิชท์
    
    digitalWrite(PumpWater, PumpState);
    Blynk.virtualWrite(V0, !PumpState);
    
    digitalWrite(Fans, FansState);
    Blynk.virtualWrite(V1, !FansState);
}
