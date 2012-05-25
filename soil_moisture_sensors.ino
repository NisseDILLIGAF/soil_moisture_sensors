#include <Time.h>
//#include <LiquidCrystal.h>
#include <Wire.h>
#include <EEPROM.h>

//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int debug = 1;  // Debug setting

int moistureSensor_1 = 0;
int moistureSensor_2 = 1;
int moisture_val_1;
int moisture_val_2;
int redled = 9;
int greenled = 7;
int redled_val = 0;
//int pump1 = 3;
int moistlimit_1 = 750;
//int moistlimit_2 = 765;
//int lcddelay = 15;

time_t plant_1_time = now();
time_t plant_1_diff;

time_t plant_2_time = now();
time_t plant_2_diff;

time_t plant_3_time = now();
time_t plant_3_diff;

time_t plant_4_time = now();
time_t plant_4_diff;

time_t plant_5_time = now();
time_t plant_5_diff;

int plant_waitMinutes = 1;
int plantWtimes = 0;
int waitcount=0;
//int brightness = 255;
//int lcdlightloop=0;
//const int lcdPowerPin = 8;    // Pin to light up the LCD

//const int lcdButtonPin = 6;  // Switch to light up the LCD for a few loops

int addr = 0;

int logloop = 0;

int soilMoisturePin = A0; // soil moisture
int SoilValue1 = 0;

const int vattnaButtonPin = 6;  // push when watering the plant to close red light

const int soilPowerPin = 10;  // Pin to power up the soil moisture sensor


void setup()
{
  Serial.begin(9600); //open serial port
  if (debug == 1) {Serial.println("Program initialised");};
  pinMode(redled, OUTPUT);
  pinMode(greenled, OUTPUT);
  //pinMode(pump1, OUTPUT);
  //pinMode(lcdPowerPin, OUTPUT);
  //pinMode(lcdButtonPin, INPUT);
  pinMode(vattnaButtonPin, INPUT);
  pinMode(soilPowerPin, OUTPUT);
  digitalWrite(greenled,HIGH);
  //digitalWrite(lcdPowerPin, HIGH);
  //delay(500);
  //lcd.begin(16, 2);
  //lcd.clear();
  //lcd.setCursor(0, 0);
  //lcd.print("Nisses Plants!");
  //delay(2000);
  //digitalWrite(lcdPowerPin, LOW);
  
  digitalWrite(soilPowerPin, LOW);
  
  readeeprom();
  
  if (debug == 1) {Serial.println("Setup completed");};
}

void loop()
{
  if (debug == 1) {Serial.println("Starting main loop at");};
  if (debug == 1) {digitalClockDisplay(planttime());};
  
  //if (debug == 1) {
    //Serial.println("Read switch input: ");
    //Serial.println(digitalRead(lcdButtonPin));
  //}
  
  //if (digitalRead(vattnaButtonPin) == HIGH) { 
    // LCD button was pressed
    //digitalWrite(redled,LOW);
    //digitalWrite(lcdPowerPin, HIGH);
    //delay(800);
    //lcd.begin(16, 2);
    //lcdlightloop=1;
  //};

  //if (lcdlightloop > 0) {
    //lcdlightloop += 1;
  //};

  //if (lcdlightloop > lcddelay) {
    //lcdlightloop=0;
    //digitalWrite(lcdPowerPin, LOW);
  //};
  
  //lcd.clear();
  //lcd.setCursor(0, 0);
  //lcd.print("Plant1: ");
  //lcd.print( moisture_val_1 );
  //lcd.print("(");
  //lcd.print( moistlimit_1 );
  //lcd.print(")");
  //lcd.print(" - ");
  //if (debug == 1) {digitalClockDisplay(plant_1_diff);};

  //lcd.println( plantWtimes );
  
  //  Serial.print("Sensor plant 2 reads: ");
  //  Serial.print( moisture_val_2 );
  //  Serial.print(" - ");
  //  digitalClockDisplay(plant_2_diff);
  
  // Serial.println();
  
  //if (debug == 1) {digitalClockDisplay(plant_1_diff);};
  
  
  moisture_val_1 = readinput_soil(); // Read soil value

  if(moisture_val_1 < moistlimit_1 && moisture_val_1 > 100 && int(minute(planttime())) >= plant_waitMinutes)
  {
    //lcdlightloop += 1;
    //lcd.blink();
    //lcd.setCursor(0, 0);
    //lcd.print("Vattnar i 10sec!");
    //digitalClockDisplay(plant_1_diff);
    digitalWrite(redled,HIGH);
    Serial.println("Dags att Vattna!!");
    //analogWrite(pump1,255);
    //delay(10000);
    //analogWrite(pump1,0);
    //digitalWrite(redled,LOW);
    //plant_1 = now();
    //plant_1_diff = 0;
    plantWtimes++;
    //lcd.noBlink();
    
  }
//  else if(moisture_val_2 < moistlimit_2 && int(minute(plant_2_diff)) >= plant_waitMinutes)
//  {
//    Serial.print("Vattnar no2 i 5sec efter ");
//    digitalClockDisplay(plant_2_diff);
//    digitalWrite(redled,HIGH);
//    delay(10000);
//    //digitalWrite(redled,LOW);
//    plant_2 = now();
//  }
//  else
//  {
    //digitalWrite(redled,LOW);
//  }
  
  if (debug == 1) {Serial.println();};
  
  logdatatoeeprom();
  
  if (debug == 1) {Serial.println("Ending main loop at");};
  if (debug == 1) {digitalClockDisplay(planttime());};
  
  waiting(60);
  
  if (debug == 1) {Serial.println();};
}

time_t planttime(){
  plant_1_time = now() - plant_1_time;
  plant_1_diff = plant_1_diff + plant_1_time;
  plant_1_time = now();
  
  return plant_1_diff;
}

void digitalClockDisplay(time_t plant){
  // digital clock display of the time
  //lcd.setCursor(0, 1);
  Serial.print(day(plant)-1);
  Serial.print(" dagar ");
  Serial.print(hour(plant));
  printDigits(minute(plant));
  printDigits(second(plant));
  Serial.println();
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void waiting(int tid){
  waitcount=0;
  while (waitcount<tid) {
    if (digitalRead(vattnaButtonPin) == HIGH) {
      
      // write a 0 to all 512 bytes of the EEPROM
      for (int i = 0; i < 512; i++) {
        EEPROM.write(i, 0);
      }
      Serial.println("EEPROM ERASED");
      
      resetPlant(1);
      //plant_1_diff = 0;
      //plant_1_time = now();
      //Serial.println("RESET TIME 1");
      
      digitalWrite(redled,LOW);
      if (debug == 1) {Serial.println("Reseting red light!");};
    }
    
    waitcount++;
    delay(1000);
  }

}

void resetPlant(int plant){
  
  if (plant == 1){
    plant_1_diff = 0;
    plant_1_time = now();
    Serial.println("RESET TIME 1");
  }
  
  if (plant == 2){
    plant_2_diff = 0;
    plant_2_time = now();
    Serial.println("RESET TIME 2");
  }
  
}


void readeeprom(){
  if (debug == 1) {Serial.println("reading EEPROM");};
  
  // Plant 1 eeprom 0 - eeprom 5
  plant_1_diff = (EEPROM.read(0)*86400)+(EEPROM.read(1)*3600)+(EEPROM.read(2)*60)+EEPROM.read(3);
  // soil value 1 part 1 = EEPROM.read(4)
  // soil value 1 part 2 = EEPROM.read(5)
  if (debug == 1) {Serial.print("Plant 1 time: ");};
  if (debug == 1) {Serial.println(plant_1_diff);};
  
  // Plant 2 eeprom 6 - eeprom 11
  plant_2_diff = (EEPROM.read(6)*86400)+(EEPROM.read(7)*3600)+(EEPROM.read(8)*60)+EEPROM.read(9);
  // soil value 2 part 1 = EEPROM.read(10)
  // soil value 2 part 2 = EEPROM.read(11)
  if (debug == 1) {Serial.print("Plant 2 time: ");};
  if (debug == 1) {Serial.println(plant_2_diff);};
  
  // Plant 3 eeprom 12 - eeprom 17
  plant_3_diff = (EEPROM.read(12)*86400)+(EEPROM.read(13)*3600)+(EEPROM.read(14)*60)+EEPROM.read(15);
  // soil value 2 part 1 = EEPROM.read(16)
  // soil value 2 part 2 = EEPROM.read(17)
  if (debug == 1) {Serial.print("Plant 3 time: ");};
  if (debug == 1) {Serial.println(plant_3_diff);};
  
  // Plant 4 eeprom 18 - eeprom 23
  plant_4_diff = (EEPROM.read(18)*86400)+(EEPROM.read(19)*3600)+(EEPROM.read(20)*60)+EEPROM.read(21);
  // soil value 2 part 1 = EEPROM.read(22)
  // soil value 2 part 2 = EEPROM.read(23)
  if (debug == 1) {Serial.print("Plant 4 time: ");};
  if (debug == 1) {Serial.println(plant_4_diff);};
  
  // Plant 5 eeprom 24 - eeprom 24
  plant_5_diff = (EEPROM.read(24)*86400)+(EEPROM.read(25)*3600)+(EEPROM.read(26)*60)+EEPROM.read(27);
  // soil value 2 part 1 = EEPROM.read(28)
  // soil value 2 part 2 = EEPROM.read(29)
  if (debug == 1) {Serial.print("Plant 5 time: ");};
  if (debug == 1) {Serial.println(plant_5_diff);};
  
  //for (int i = 0; i < 5; i++) {

    //byte tmp_val = EEPROM.read(i);
    //Serial.print("EEPROM ");
    //Serial.print(i);
    //Serial.print(": ");
    //Serial.println(tmp_val, DEC);   -----!!!!!!!!!!!!!-----
    
    //if (i == 4) {
      //Serial.println(int(tmp_val)*4, DEC);
    //}
    //else {
      //Serial.println(tmp_val, DEC);
    //}
    
  //}
  
  if (debug == 1) {Serial.println("Finished reading EEPROM");};
}

void logdatatoeeprom(){

  //digitalWrite(ledPin, HIGH);
  //lcd.setCursor(15,1);
  //lcd.print("E");
  
  time_t tmptime = planttime();

  if (debug == 1) {Serial.println("logdatatoeeprom started");};
  
  addr=0;
  if (debug == 1) {Serial.print("writing to eeprom ");};
  if (debug == 1) {Serial.print(addr);};
  if (debug == 1) {Serial.print(" : ");};
  if (debug == 1) {Serial.println(day(tmptime)-1);};
  EEPROM.write(addr, day(tmptime)-1);
  
  addr++;
  if (debug == 1) {Serial.print("writing to eeprom ");};
  if (debug == 1) {Serial.print(addr);};
  if (debug == 1) {Serial.print(" : ");};
  if (debug == 1) {Serial.println(hour(tmptime));};
  EEPROM.write(addr, hour(tmptime));
  
  addr++;
  if (debug == 1) {Serial.print("writing to eeprom ");};
  if (debug == 1) {Serial.print(addr);};
  if (debug == 1) {Serial.print(" : ");};
  if (debug == 1) {Serial.println(minute(tmptime));};
  EEPROM.write(addr, minute(tmptime));
  
  addr++;
  if (debug == 1) {Serial.print("writing to eeprom ");};
  if (debug == 1) {Serial.print(addr);};
  if (debug == 1) {Serial.print(" : ");};
  if (debug == 1) {Serial.println(second(tmptime));};
  EEPROM.write(addr, second(tmptime));
  
  if (moisture_val_1 > 255){
    addr++;
    if (debug == 1) {Serial.print("writing to eeprom ");};
    if (debug == 1) {Serial.print(addr);};
    if (debug == 1) {Serial.print(" : ");};
    if (debug == 1) {Serial.println(255);};
    EEPROM.write(addr, 255);
    
    addr++;
    if (debug == 1) {Serial.print("writing to eeprom ");};
    if (debug == 1) {Serial.print(addr);};
    if (debug == 1) {Serial.print(" : ");};
    if (debug == 1) {Serial.println(moisture_val_1-255);};
    EEPROM.write(addr, moisture_val_1-255);
  }
  else {  
    addr++;
    if (debug == 1) {Serial.print("writing to eeprom ");};
    if (debug == 1) {Serial.print(addr);};
    if (debug == 1) {Serial.print(" : ");};
    if (debug == 1) {Serial.println(moisture_val_1);};
    EEPROM.write(addr, moisture_val_1);
  }
  
  if (debug == 1) {Serial.println("back from writing to eeprom");};

  if (debug == 1) {Serial.println("end logdatatoeeprom");};

}


int readinput_soil() {

  if (debug == 1) {Serial.println("Readinputs (Soil) function started");};

  // Switch power pin on for the soil sensor

  digitalWrite(soilPowerPin, HIGH);
  
  waiting(5);

  //float temp1 = 0;
  //float temp2 = 0;

  int soil1 = 0;
  int soil2 = 0;
  int lpcount = 0;
  int maxlpcount = 10;

 

  if (debug == 1) {Serial.println("Read soil moisture sensor");};

 

  lpcount=0;

  while (lpcount<maxlpcount) {

    soil2 = analogRead(soilMoisturePin);
    //soil2 = analogRead(moistureSensor_1);

    //soil2 = (soil2 / 2);

    soil1 += soil2;

    if (debug == 1) {Serial.print("Soil loop :");};
    if (debug == 1) {Serial.print(lpcount);};
    if (debug == 1) {Serial.print(" : ");};
    if (debug == 1) {Serial.println(soil2);};

    lpcount=lpcount+1;
    
    waiting(2);

  };

  SoilValue1 = soil1/maxlpcount;
  
  digitalClockDisplay(planttime());
  Serial.print("Soil average: ");
  Serial.println(SoilValue1);

  digitalWrite(soilPowerPin, LOW);

  if (debug == 1) {Serial.println("Exit Readinputs (Soil)");};
  
  return SoilValue1;

}
