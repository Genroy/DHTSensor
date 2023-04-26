#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>


#include <TridentTD_LineNotify.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>     


#define BLYNK_PRINT Serial

SoftwareSerial Node(D6,D7); // RX | TX
BlynkTimer timer;


#define swit D1
volatile int h;
volatile int t;
volatile int rel;
volatile int tv3;
volatile int hv4;
volatile int tv5;
 volatile int tv6;
 
char auth[] = "59d1c6d69b5d443a91bbab8bc6be5b13";
char linn[] = "3vXT2xs8Inh1MkITML7JRqt6iByYA7Z7Rd1sw2ehcUe";

String message1 = " Pongsakorn K ตอนนี้อุณหภูมิในห้องของคุณ อยู่ที่ ";
String message2 = "และความชื้นอยู่ที่ ";

void setup() {
  
 Node.begin(9600);
   EEPROM.begin(1200);
  Serial.begin(115200);
  pinMode(swit , OUTPUT);
  digitalWrite(swit, HIGH);   
 
  WiFiManager wifiManager;
  //wifiManager.resetSettings();   
  wifiManager.autoConnect("ESP FIX");
  Serial.println("connected...yeey :)");
  Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
  //timer.setInterval(5000L, sendSensor);
  

}

void loop() {

  Blynk.run(); 
  timer.run(); 

   bwv3();
  // bwv4();
   bwv6();
  timer.setInterval(3000, sendSensor);
  timer.setInterval(3000, ses);
  
 // Serial.println(t);
 // Serial.print(h);
 
}

void sendSensor()
{
  Blynk.virtualWrite(V10, h);
  Blynk.virtualWrite(V9, t);
  
}

BLYNK_WRITE(V1)
{
LINE.setToken(linn);
    int RelayStatus1 = param.asInt();
    if (RelayStatus1 == 0) 
  {  
    digitalWrite(swit, HIGH);
    LINE.notify("Close ปิดสวิสปั๊มสปิงเกอร");
    
  }

   else if (RelayStatus1 == 1) 
  {  
    digitalWrite(swit, LOW);
    LINE.notify("Open สวิสปั๊มสปิงเกอร์เปิดใช้งาน");
    
    
  }

}

BLYNK_WRITE(V2)
{ 
  
  LINE.setToken(linn);
 int RelayStatus = param.asInt();
  rel=RelayStatus;

   if(RelayStatus == 2)
   {

    digitalWrite(swit, HIGH);
    LINE.notify("Auto Close ปิดโหมดอัตโนมัติครับ");
  }
  
  else if(RelayStatus == 3) 
  {

        if (h == 0 || t == 0) {
        RelayStatus == 2;
        LINE.notify("ไม่สามารถเปิดแบบอัตโนมัติเนื่องจาก ไม่มีการส่งค่าของอุณหภูมิ!!");
         }//if h t
         else{
          LINE.notify("Auto Open สวิสแบบอัตโนมัติถูกเปิดใช้งาน");
         }//else
  }
  
}//v2 

BLYNK_WRITE(V3)
{
int pinValue3 = param.asInt(); 
  tv3=pinValue3;

  }//v3
BLYNK_WRITE(V4)
{
   
 int pinValue4 = param.asInt(); 
 hv4 = pinValue4;

}//v4

BLYNK_WRITE(V5)
{
  LINE.setToken(linn);
int pinValue5 = param.asInt(); 
  tv5=pinValue5;

  if(pinValue5 == 0)
   {
    LINE.notify("ปิดการแจ้งเตือนอุณหภูมิผ่าน LINE");
  }
  else if(pinValue5 == 1){

    LINE.notify("แจ้งเตือน LINE ทำงาน");
  }//ef

  }//v5

  
BLYNK_WRITE(V6)
{
 
int pinValue6 = param.asInt(); 
  tv6= (pinValue6 * 1000);


  }//v6

void bwv3(){ 

static unsigned long Timev3 = millis();

if(rel == 3)
{
    if(t == 0 || h == 0)
{
     if((millis() - Timev3)> 20000){
        Timev3 = millis();      
      // LINE.notify("โหมดอัตโนมัติไม่ทำงานเนื่องจากไม่มีการส่งค่า");
        digitalWrite(swit, HIGH);
     }

}
else
{

   if(rel == 3 && (t > tv3 || h < hv4))
     {
      digitalWrite(swit, LOW);
     
     }
    else
    {
    digitalWrite(swit, HIGH);
    } 
    
} 
}
  }//bwv3


 void bwv6(){ 
 
  LINE.setToken(linn);
 static unsigned long Time = millis();
  
    
    if(tv5 == 1)
    {
      if(tv6){
    if((millis() - Time)> tv6){
      Time = millis();      
        if (h == 0 || t == 0) {
         // Serial.println("Failed to read from DHT sensor!");
        LINE.notify("สาย DHT หลวม กรุณาตรวจสอบสายและต่ออีกครั้งครับ");
          //digitalWrite(swit, HIGH);
         }//if h t
         else{
          LINE.notify(message1 + " " + t + " องศา^^" + message2 + " " + h + " % ครับ");
         }
         Serial.print("min");
   }//if millis
      }//tv6
    }//ปุ่ม

  }//

void ses(){
  
  static int state = 0;
  static int index = 0;
  static char Buffer[3];
  static int va1,va2;

  h=va1;
  t=va2;

  if(Node.available()>0){
    char data = Node.read();
    
    switch(state){
    case 0: if(data == '#'){
        index = 0;
        state = 1;
      }//if
      break;
      
    case 1: if(data == ':'){
        Buffer[index] = '\0';
       va1 = atoi(Buffer);
       index = 0;
        state = 2;
      }//if 
      else{
        Buffer[index] = data;
        index++;
        }//else
      break;
      
    case 2: if(data == ';'){
        Buffer[index] = '\0';
        va2 = atoi(Buffer);
        state = 0;

      }//if
      else{
        Buffer[index] = data;
        index++;
        }//else
    }//switch
}
}//void
