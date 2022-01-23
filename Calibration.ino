/*calibrating sensors*/
#include <Wire.h>
#include "Adafruit_TCS34725.h" 
#define TCAADDR 0x70 
#include<LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x27,16,2);
Adafruit_TCS34725 tcs_1 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_60X);
Adafruit_TCS34725 tcs_2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_60X);
Adafruit_TCS34725 tcs_3 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_101MS, TCS34725_GAIN_60X);

uint16_t red_1, green_1, blue_1, clear_1; uint16_t red_2, green_2, blue_2, clear_2; uint16_t red_3, green_3, blue_3, clear_3;
int ind=0;
int myarray[9];
void tcaselect(uint16_t i)
{
  if (i > 7) 
    return; 
   Wire.beginTransmission(TCAADDR); 
   Wire.write(1 << i); 
   Wire.endTransmission();
}


void setup() {
 Serial.begin(9600); 
 lcd.begin(16, 2); 
 lcd.clear();
 lcd.setCursor(0, 0);
 lcd.print("SETTING UP!"); // delay(5000);
 Wire.begin();
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

void loop()
{
  tcaselect(1);
  tcs_1.getRawData(&red_1, &green_1, &blue_1, &clear_1); tcaselect(2);
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
  
  ind++;
  if(ind ==50) {
    for(int i=0;i<9;i++) {
     Serial.println(myarray[i]/50);
  } 
 } 

}
