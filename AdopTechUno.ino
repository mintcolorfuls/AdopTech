#include <SoftwareSerial.h>

int value1 = 0;
int value2 = 100;

SoftwareSerial Com(10, 11); //10 is RX, 11 is TX
void setup() {
  Serial.begin(9600);
  Com.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print("Tx sent value1:");
  Serial.print(value1);
  Serial.print(" value2:");
  Serial.println(value2);

  //Sent 
  Com.write("#"); //ใช้บอกว่าเป็นตัวเริ่มของข้อมูล
  Com.print(value1);
  Com.write(":"); // : ถ้าอยู่ตรงนี้ ใช้เป็นตัวขั้นข้อมูล
  Com.print(value2);
  Com.write(";"); // : ถ้าอยู่ตรงนี้ ใช้บอกตัวรับว่า เป็นตัวสิ้นสุดข้อมูล

  delay(100);

  if(value1 < 100)
  {
    value1++;
  }
  else
  {
    value1 = 0;
  }

  if(value2 > 0)
  {
    value2--;
  }
  else
  {
    value2 = 100;
  }
  
}
