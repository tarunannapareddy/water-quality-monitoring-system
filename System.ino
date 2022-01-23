#include <Wire.h>
#include "Adafruit_TCS34725.h" 
#define TCAADDR 0x70 
#include<LiquidCrystal_I2C.h>  

lcd(0x27,20,4);
Adafruit_TCS34725 tcs_1 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_60X);
Adafruit_TCS34725 tcs_2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_60X);
Adafruit_TCS34725 tcs_3 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_60X);

uint16_t red_1, green_1, blue_1, clear_1; uint16_t red_2, green_2, blue_2, clear_2; uint16_t red_3, green_3, blue_3, clear_3;
int ind=0;
float myarray[9];
float avg1=0,avg2=0;

void tcaselect(uint16_t i)
{
  if (i > 7) 
    return; 
   Wire.beginTransmission(TCAADDR); 
   Wire.write(1 << i); 
   Wire.endTransmission();
}

void setup() {
  lcd.init(); // initialize the lcd lcd.init();
  // Print a message to the LCD. lcd.backlight();
  lcd.setCursor(1,0); 
  lcd.print("Code on");
  lcd.setCursor(1,1);
  lcd.print("setting up");
  delay(2000);
  
  digitalWrite(2,HIGH);
  Serial.begin(9600); // lcd.begin(16, 2);
  Serial.println("CODE ON!!");

 tcaselect(1); 
 if (tcs_1.begin()) {
    Serial.println(" DETECTED FIRST SENSOR"); 
 }
 if (!tcs_1.begin()){
    Serial.println("sensor1 not found");
 }
 tcaselect(2);
 if (tcs_2.begin()) {
    Serial.println(" DETECTED SECOND SENSOR"); 
  }
 else {
    Serial.println("sensor2 not found");
  }
  tcaselect(3);
  if (!tcs_3.begin()) {
    Serial.println("sensor3 not found"); 
  }
  if (tcs_3.begin()) {
    Serial.println(" DETECTED THIRD SENSOR"); 
  }
  
  for (int i = 0; i < 9 ; i++) {
    myarray[i]=0; 
  }
  
}


void loop() {
  
  tcaselect(1);
  tcs_1.getRawData(&red_1, &green_1, &blue_1, &clear_1);
  tcaselect(2);
  tcs_2.getRawData(&red_2, &green_2, &blue_2, &clear_2); tcaselect(3);
  tcs_3.getRawData(&red_3, &green_3, &blue_3, &clear_3);

     
  myarray[0]+=red_1;
  myarray[1]+=green_1;
  myarray[2]+=blue_1;
  
  myarray[3]+=red_2;
  myarray[4]+=green_2;
  myarray[5]+=blue_2;
  
  myarray[6]+=red_3;
  myarray[7]+=green_3;
  myarray[8]+=blue_3;
  
 if(ind == 50) {
  delay(5000);
  avg1=(myarray[1])/(myarray[0]); 
  avg2=(myarray[1]/myarray[2]); 
  Serial.print(avg1);
  Serial.print("- ");
  Serial.println(avg2); 
  if(avg2>=4.5)
    {
      lcd.clear();
      lcd.setCursor(0, 0); lcd.print("ADD WATER!!!");
    }
   else if(avg1>4.05 || avg2>4.05) {
      lcd.clear();
      lcd.setCursor(0, 0); 
      lcd.print("Drinking water.."); 
      lcd.setCursor(0, 1); 
      lcd.print("<1NTU");
   }
   else if((avg1>=3 && avg1<=4.05)||(avg2>=3 && avg2<4)) {
      lcd.clear(); 
      lcd.setCursor(0, 0); 
      lcd.print("Tap Water.."); 
      lcd.setCursor(0, 1); 
      lcd.print("1-6NTU");
   }
   else if((avg1>=2 && avg1<3)||(avg2>=2 && avg2<3)) {
      lcd.clear(); 
      lcd.setCursor(0, 0); 
      lcd.print("Dirty Water.."); 
      lcd.setCursor(0, 1); 
      lcd.print("7-15NTU");
   } else {
      lcd.clear(); 
      lcd.setCursor(0, 0); 
      lcd.print("Waste water.."); 
      lcd.setCursor(0, 1); 
      lcd.print(">15NTU");
   }
   for(int i=0;i<9;i++)
     myarray[i]=0;
   ind=0;
// Wire.begin(); digitalWrite(2,HIGH);
  } 
}
