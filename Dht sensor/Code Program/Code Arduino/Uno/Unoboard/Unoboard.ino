#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space
#include <SoftwareSerial.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

SoftwareSerial sserial(10, 11); // RX | TX
#include <SimpleTimer.h>

#define DHTPIN  4        
#define DHTTYPE DHT11     
DHT dht(DHTPIN, DHTTYPE);

volatile int h;                
volatile int t;



SimpleTimer timer;
int ok =  6;
int error =  5;


void setup() {

     Serial.begin(115200);
     sserial.begin(9600);
     lcd.begin();
     pinMode(ok, OUTPUT);
     pinMode(error, OUTPUT);     
     lcd.print("Welcome To ");
     lcd.setCursor(0, 1);
     lcd.print("tem and hum");
     delay(6000);
      lcd.clear();

   
}

void loop() {
  
      int hh = dht.readHumidity();
      int tt = dht.readTemperature();
        t=tt;
        h=hh;
      tem();


    sserial.write('#');
    sserial.print(h);
    sserial.print(':');
    sserial.print(t);
    sserial.write(';');
    sserial.println();
    Serial.print(t);
    Serial.print(h);
    delay(1000);

}//loop


void tem (){

  
    if (h == 0 || t == 0) {

        lcd.print("Tem is Fall");
        lcd.setCursor(0, 1);
        lcd.print("check cable!!");
        delay(4600);
        lcd.clear();
 
      digitalWrite(error, HIGH);
      delay(200); 
      digitalWrite(error, LOW);
      delay(200); 
    }
    else{
            lcd.print("Hum is = ");
      lcd.print(h);
      lcd.print(" %");
      lcd.setCursor(0, 1);   
      lcd.print("Tem is = ");
      lcd.print(t);
      lcd.print(" *C");
      delay(4600);
      lcd.clear();
     
  digitalWrite(ok, HIGH);
  delay(200);
  digitalWrite(ok, LOW);
  delay(200); 
    }
}//oo


