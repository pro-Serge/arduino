#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int ccw1, ccw2, ccw3, ccw4, ccw5, cw1, cw2, cw3, cw4, cw5;
int left_buttonState = 0;
int right_buttonState = 0;
int start_buttonState = 0;
int up_buttonState = 0;
int dn_buttonState = 0;
int save_buttonState = 0;
int pos = 0;
int dly = 0;
String stat = "                ";
const int left_buttonPin = 8;
const int right_buttonPin = 7;
const int start_buttonPin = 6;
const int up_buttonPin = 5;
const int dn_buttonPin = 4;
const int save_buttonPin = 3;
const int ccw = 12;
const int cw = 11;

void setup()
{
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Gibka v1.0");
  lcd.setCursor(2,1);
  lcd.print("sp@krmts.ru");
  ccw1 = EEPROM.read(0);
  ccw2 = EEPROM.read(1);
  ccw3 = EEPROM.read(2);
  ccw4 = EEPROM.read(3);
  ccw5 = EEPROM.read(4);
  cw1 = EEPROM.read(5);
  cw2 = EEPROM.read(6);
  cw3 = EEPROM.read(7);
  cw4 = EEPROM.read(8);
  cw5 = EEPROM.read(9);
  pinMode(left_buttonPin, INPUT);
  pinMode(right_buttonPin, INPUT);
  pinMode(start_buttonPin, INPUT);
  pinMode(up_buttonPin, INPUT);
  pinMode(dn_buttonPin, INPUT);
  pinMode(save_buttonPin, INPUT);
  pinMode(ccw, OUTPUT);
  pinMode(cw, OUTPUT);
  delay(5000);
}


void loop()
{
  left_buttonState = digitalRead(left_buttonPin);
  right_buttonState = digitalRead(right_buttonPin);
  start_buttonState = digitalRead(start_buttonPin);
  up_buttonState = digitalRead(up_buttonPin);
  dn_buttonState = digitalRead(dn_buttonPin);
  save_buttonState = digitalRead(save_buttonPin);
  if ((left_buttonState == LOW) & (pos > -5)){
    pos = pos-1;
    switch (pos) {
  case -5:
    dly = ccw5*100;
    stat="<<<<< 5         ";
    break;
  case -4:
    dly = ccw4*100;
    stat="<<<<  4         ";
    break;
  case -3:
    dly = ccw3*100;
    stat="<<<   3         ";
    break;
  case -2:
    dly = ccw2*100;
    stat="<<    2         "; 
    break;
  case -1:
    dly = ccw1*100;
    stat="<     1         ";
    break;
  case 0:
    stat="      0         ";
  break;
  case 1:
    dly = cw1*100;
    stat="         1     >";
    break;
  case 2:
    dly = cw2*100;
    stat="         2    >>";
    break;
  case 3:
    dly = cw3*100;
    stat="         3   >>>";
    break;
  case 4:
    dly = cw4*100;
    stat="         4  >>>>";
    break;
  case 5:
    dly = cw5*100;
    stat="         5 >>>>>";
    break;
  default:
    break;
    }
    lcd.setCursor(0,0);
    lcd.print(stat);
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(6,1);
    lcd.print(dly);  
    delay(100);
  } else
  {
    
  }
  if ((right_buttonState == LOW) & (pos < 5)){
    pos = pos+1;
      switch (pos) {
  case -5:
    dly = ccw5*100;
    stat="<<<<< 5         ";
    break;
  case -4:
    dly = ccw4*100;
    stat="<<<<  4         ";
    break;
  case -3:
    dly = ccw3*100;
    stat="<<<   3         ";
    break;
  case -2:
    dly = ccw2*100;
    stat="<<    2         "; 
    break;
  case -1:
    dly = ccw1*100;
    stat="<     1         ";
    break;
  case 0:
    stat="         0      ";
  break;
  case 1:
    dly = cw1*100;
    stat="         1     >";
    break;
  case 2:
    dly = cw2*100;
    stat="         2    >>";
    break;
  case 3:
    dly = cw3*100;
    stat="         3   >>>";
    break;
  case 4:
    dly = cw4*100;
    stat="         4  >>>>";
    break;
  case 5:
    dly = cw5*100;
    stat="         5 >>>>>";
    break;
  default:
    break;
}
    lcd.setCursor(0,0);
    lcd.print(stat);
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(6,1);
    lcd.print(dly);
    delay (100);  
  }  else
  {
    
  }
if ((start_buttonState == LOW) & (pos != 0)){  

  if (pos > 0) {
    digitalWrite(cw, HIGH);
    delay(dly);
    digitalWrite(cw, LOW);
  } else {
    digitalWrite(ccw, HIGH);
    delay(dly);
    digitalWrite(ccw, LOW);
  }
}
if ((up_buttonState == LOW) & (pos != 0)) {

      switch (pos) {
  case -5:
    if (ccw5 < 255) { 
    ccw5 = ccw5+1;
    dly = ccw5*100;
    delay (50);
    }
    break;
  case -4:
    if (ccw4 < 255) { 
    ccw4 = ccw4+1;
    dly = ccw4*100;
    delay (50);
    }
    break;
  case -3:
    if (ccw3 < 255) { 
    ccw3 = ccw3+1;
    dly = ccw3*100;
    delay (50);
    }
    break;
  case -2:
    if (ccw2 < 255) { 
    ccw2 = ccw2+1;
    dly = ccw2*100;
    delay (50);
    }
    break;
  case -1:
    if (ccw1 < 255) { 
    ccw1 = ccw1+1;
    dly = ccw1*100;
    delay (50);
    }
    break;
  case 1:
    if (cw1 < 255) { 
    cw1 = cw1+1;
    dly = cw1*100;
    delay (50);
    }
    break;
  case 2:
    if (cw2 < 255) { 
    cw2 = cw2+1;
    dly = cw2*100;
    delay (50);
    }
    break;
  case 3:
    if (cw3 < 255) { 
    cw3 = cw3+1;
    dly = cw3*100;
    delay (50);
    }
    break;
  case 4:
    if (cw4 < 255) { 
    cw4 = cw4+1;
    dly = cw4*100;
    delay (50);
    }
    break;
  case 5:
    if (cw5 < 255) { 
    cw5 = cw5+1;
    dly = cw5*100;
    delay (50);
    }
    break;
  default:
    break;

      }
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(6,1);
    lcd.print(dly);
}  

if ((dn_buttonState == LOW) & (pos != 0)) {

      switch (pos) {
  case -5:
    if (ccw5 > 1) { 
    ccw5 = ccw5-1;
    dly = ccw5*100;
    delay (50);
    }
    break;
  case -4:
    if (ccw4 > 1) { 
    ccw4 = ccw4-1;
    dly = ccw4*100;
    delay (50);
    }
    break;
  case -3:
    if (ccw3 > 1) { 
    ccw3 = ccw3-1;
    dly = ccw3*100;
    delay (50);
    }
    break;
  case -2:
    if (ccw2 > 1) { 
    ccw2 = ccw2-1;
    dly = ccw2*100;
    delay (50);
    }
    break;
  case -1:
    if (ccw1 > 1) { 
    ccw1 = ccw1-1;
    dly = ccw1*100;
    delay (50);
    }
    break;
  case 1:
    if (cw1 > 1) { 
    cw1 = cw1-1;
    dly = cw1*100;
    delay (50);
    }
    break;
  case 2:
    if (cw2 > 1) { 
    cw2 = cw2-1;
    dly = cw2*100;
    delay (50);
    }
    break;
  case 3:
    if (cw3 > 1) { 
    cw3 = cw3-1;
    dly = cw3*100;
    delay (50);
    }
    break;
  case 4:
    if (cw4 > 1) { 
    cw4 = cw4-1;
    dly = cw4*100;
    delay (50);
    }
    break;
  case 5:
    if (cw5 > 1) { 
    cw5 = cw5-1;
    dly = cw5*100;
    delay (50);
    }
    break;
  default:
    break;

      }
    lcd.setCursor(0,1);
    lcd.print("                ");
    lcd.setCursor(6,1);
    lcd.print(dly);
}  

if (save_buttonState == LOW) {
  EEPROM.write(0, ccw1);
  EEPROM.write(1, ccw2);
  EEPROM.write(2, ccw3);
  EEPROM.write(3, ccw4);
  EEPROM.write(4, ccw5);
  EEPROM.write(5, cw1);
  EEPROM.write(6, cw2);
  EEPROM.write(7, cw3);
  EEPROM.write(8, cw4);
  EEPROM.write(9, cw5); 
  lcd.setCursor(0,1);
  lcd.print("    Saving...   ");
  delay(5000);
}
}
