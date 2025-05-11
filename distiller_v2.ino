// Версия 2.0
// Добавлена третья фаза дистилляции
// Добавлено включение клапана загрузки
// Добавлен счетчик моточасов
#include "button.h"
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MAX31865.h>
#include <GyverEncoder.h>
#include <EEPROM.h>
#define RREF      430.0
#define RNOMINAL  100.0
int started;
int paramnum=0;
int ix=0;
int j,flag,flag2,flag3,blink=0;
uint32_t timer1,timer2,timer3,loading,stage1,stage2,stage3;
long progress1,sp,pv;
bool relayState;
Encoder enc1(15, 14);
Adafruit_MAX31865 thermo = Adafruit_MAX31865(6, 5, 4, 3); // Use software SPI: CS, DI, DO, CLK
byte simvol[8] = {
// Буква Ф
0b01110,
0b10101,
0b10101,
0b10101,
0b01110,
0b00100,
0b00100,
0b00000
};
byte simvol1[8] = {
// Буква Я
0b01111,
0b10001,
0b10001,
0b10001,
0b01111,
0b10001,
0b10001,
0b00000
};
byte simvol2[8] = {
// Буква П
0b11111,
0b10001,
0b10001,
0b10001,
0b10001,
0b10001,
0b10001,
0b00000
};
byte simvol3[8] = {
// Символ градуса
0b00100,
0b01010,
0b01010,
0b00100,
0b00000,
0b00000,
0b00000,
0b00000
};
byte simvol4[8] = {
// Буква Ш
0b10001,
0b10101,
0b10101,
0b10101,
0b10101,
0b10101,
0b11111,
0b00000
};
byte simvol5[8] = {
// Буква И
0b10001,
0b10001,
0b10001,
0b10011,
0b10101,
0b11001,
0b10001,
0b00000
};
byte simvol6[8] = {
// Буква Б
0b11111,
0b10000,
0b10000,
0b11110,
0b10001,
0b10001,
0b11110,
0b00000
};
byte simvol7[8] = {
// Буква З
0b01110,
0b10001,
0b00001,
0b00010,
0b00001,
0b10001,
0b01110,
0b00000
};

button starttrig(8);
button enterbut(16);
int fan = 12;
int heater = 11;
int valve = 10;
bool edit;
long param[9];
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  lcd.init();                    // initialize the lcd 
  lcd.backlight();
  lcd.createChar(1, simvol);
  lcd.createChar(2, simvol1);
  lcd.createChar(3, simvol2);
  lcd.createChar(4, simvol3);
  lcd.createChar(5, simvol4);
  lcd.createChar(6, simvol5);
  lcd.createChar(7, simvol6);
  lcd.createChar(8, simvol7);
  pinMode(fan, OUTPUT);
  pinMode(heater, OUTPUT);
  pinMode(valve, OUTPUT);
  EEPROM.get(0,param); // читаем из EEPROM настройки
  enc1.setType(TYPE2); // инициализируем энкодер
  thermo.begin(MAX31865_3WIRE);  // 3-х проводной Pt100
 }
void loop() {
  
  enc1.tick(); // encoder tick

  if ((starttrig.click()) & (!edit)) {         // обработка кнопки "пуск-стоп"
       started = !started;
       flag = 0;
       flag2 = 0;
       timer1 = millis();
  }
  
  if (!started) { // обработка режима ожидания

    if ((millis() - timer1 >= 5000) & (millis() - timer1 <= 5500) & (!edit)) {  // скроллинг параметров
    // 1 phase
    lcd.setCursor(0,0);
    lcd.print(char(1));
    lcd.print("A");
    lcd.print(char(8));
    lcd.print("A 1         ");
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.print(param[1]);
    lcd.print(char(4));
    lcd.print("C     ");
    lcd.setCursor(8,1);
    lcd.print(" ");
    lcd.print(param[0] / 60);
    lcd.print(":");
    lcd.print(param[0] % 60);
    lcd.print("   ");
    }
    if ((millis() - timer1 >= 10000) & (millis() - timer1 <=10500) & (!edit) & (param[2] <= 0)) { // скипаем вторую фазу
    timer1 = timer1 - 5000;
    }
    if ((millis() - timer1 >= 10000) & (millis() - timer1 <=10500) & (!edit) & (param[2] > 0)) {
    // 2 phase             
    lcd.setCursor(0,0);
    lcd.print(char(1));
    lcd.print("A");
    lcd.print(char(8));
    lcd.print("A 2  ");
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.print(param[3]);
    lcd.print(char(4));
    lcd.print("C  ");
    lcd.setCursor(8,1);
    lcd.print(" ");
    lcd.print(param[2] / 60);
    lcd.print(":");
    lcd.print(param[2] % 60);
    lcd.print("   ");
    }
    if ((millis() - timer1 >= 15000) & (millis() - timer1 <=15500) & (!edit) & (param[4] <= 0)) { // скипаем третью фазу
      timer1 = timer1 - 5000;
    }
    if ((millis() - timer1 >= 15000) & (millis() - timer1 <= 15500) & (!edit) & (param[4] > 0)) {
    // 3 phase
    lcd.setCursor(0,0);
    lcd.print(char(1));
    lcd.print("A");
    lcd.print(char(8));
    lcd.print("A 3  ");
    lcd.setCursor(0,1);
    lcd.print(" ");
    lcd.print(param[5]);
    lcd.print(char(4));
    lcd.print("C  ");
    lcd.setCursor(8,1);
    lcd.print(" ");
    lcd.print(param[4] / 60);
    lcd.print(":");
    lcd.print(param[4] % 60);
    lcd.print("   ");
    }
    if ((millis() - timer1 >= 20000) & (!edit)) {
     // Общий пробег и предупреждение о замене масла
    lcd.setCursor(0,0);
    if (param[7] >= 2000) {
      lcd.print(" CMEH");
      lcd.print(char(6));
      lcd.print(" MAC");
      lcd.print(char(3));
      lcd.print("O!!!  ");
    }
    else
    {
      lcd.print("HAPA");
      lcd.print(char(7));
      lcd.print("OTKA:");
    }
    lcd.setCursor(0,1);    
    lcd.print(param[7]);
    lcd.print(" / ");
    lcd.print(param[8]);
    lcd.print("H          ");
    timer1 = millis();
    }
    
    if (flag==0) { // прорисовка дефолтного экрана
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("    v2.1   ");
      lcd.setCursor(0,1);
      lcd.print("   R/W EEPROM ");
      digitalWrite(fan, LOW);
      digitalWrite(valve,LOW);
      digitalWrite(heater,LOW); 
      flag=1;
    }

    if (enterbut.click()) { // вход-выход в редактирование
      if ((edit) & (j < 7)) { j++; } // увеличиваем переменную j на единицу, переходим на следующий экран
      if (((!edit) & (j==0)) | ((edit) & (j==7))) { // инверсируем признак редактирования на входе и выходе
          if (j==7) { // савим конфигу
              EEPROM.put(0,param);
              j=0;
          }
          edit=!edit; 
        }
      flag=0;
      timer1 = millis();
    }
    
  if (edit) { // редактирование параметров
      if (enc1.isLeft()) { // поворот энкодера влево
        if ((j==0) & (param[0]>0)) { // время первой фазы
            param[0]=param[0]-10;
            lcd.setCursor(8,0);
            lcd.print(" ");
            lcd.print(param[0] / 60);
            lcd.print(":");
            lcd.print(param[0] % 60);
            lcd.print("   ");
        }
        if ((j==1) & (param[1]>1)) { // температура первой фазы
            param[1]=param[1]-1;
            lcd.setCursor(8,0);
            lcd.print(" ");
            lcd.print(param[1]);
            lcd.print(char(4));
            lcd.print("C     ");  
        }
        if ((j==2) & (param[2]>0)) { // время второй фазы
            param[2]=param[2]-10;
            lcd.setCursor(8,0);
            lcd.print(" ");
            lcd.print(param[2] / 60);
            lcd.print(":");
            lcd.print(param[2] % 60);
            lcd.print("   ");
        }
        if ((j==3) & (param[3]>1)) { // температура второй фазы
            param[3]=param[3]-1;
            lcd.setCursor(8,0);
            lcd.print(" ");
            lcd.print(param[3]);
            lcd.print(char(4));
            lcd.print("C     ");  
        } 
        if ((j==4) & (param[4]>0)) { // время третьей фазы
            param[4]=param[4]-10;
            lcd.setCursor(8,0);
            lcd.print(" ");
            lcd.print(param[4] / 60);
            lcd.print(":");
            lcd.print(param[4] % 60);
            lcd.print("   ");
        }
        if ((j==5) & (param[5]>1)) { // температура третьей фазы
            param[5]=param[5]-1;
            lcd.setCursor(8,0);
            lcd.print(" ");
            lcd.print(param[5]);
            lcd.print(char(4));
            lcd.print("C     ");  
        } 
        if ((j==6) & (param[6]>1)) { // время загрузки
            param[6]=param[6]-1;
            lcd.setCursor(8,1);
            lcd.print("   ");
            lcd.print(param[6]);
            lcd.print("min");
        } 
      }
      if (enc1.isRight()) { // поворот энкодера вправо
        if ((j==0) & (param[0]<5940)) { // время первой фазы, не более 99ч
            param[0]=param[0]+10;
            lcd.setCursor(8,0);
            lcd.print(" ");
            lcd.print(param[0] / 60);
            lcd.print(":");
            lcd.print(param[0] % 60);
            lcd.print("   ");
        }
        if ((j==1) & (param[1]<200)) { // температура первой фазы
            param[1]=param[1]+1;
            lcd.setCursor(8,0);
            lcd.print(" ");
            lcd.print(param[1]);
            lcd.print(char(4));
            lcd.print("C     ");  
        }
        if ((j==2) & (param[2]<5940)) { // время второй фазы
            param[2]=param[2]+10;
            lcd.setCursor(8,0);
            lcd.print(" ");
            lcd.print(param[2] / 60);
            lcd.print(":");
            lcd.print(param[2] % 60);
            lcd.print("   ");
        }
        if ((j==3) & (param[3]<200)) { // температура второй фазы
            param[3]=param[3]+1;
            lcd.setCursor(8,0);
            lcd.print(" ");
            lcd.print(param[3]);
            lcd.print(char(4));
            lcd.print("C     ");  
        }
        if ((j==4) & (param[4]<5940)) { // время третьей фазы
            param[4]=param[4]+10;
            lcd.setCursor(8,0);
            lcd.print(" ");
            lcd.print(param[4] / 60);
            lcd.print(":");
            lcd.print(param[4] % 60);
            lcd.print("   ");
        }
        if ((j==5) & (param[5]<200)) { // температура третьей фазы
            param[5]=param[5]+1;
            lcd.setCursor(8,0);
            lcd.print(" ");
            lcd.print(param[5]);
            lcd.print(char(4));
            lcd.print("C     ");  
        }
        if ((j==6) & (param[6]<60)) { // время загрузки
            param[6]=param[6]+1;
            lcd.setCursor(8,1);
            lcd.print("   ");
            lcd.print(param[6]);
            lcd.print("min");
        }

      }
    if (j==6 & starttrig.click()) { // обнуление счетчика моточасов
      param[7]=0;
    }
    if ((j==0) & (flag == 0)) { // экран "Время Фаза 1"
          lcd.setCursor(0,0);
          lcd.print(char(1));
          lcd.print("A");
          lcd.print(char(8));
          lcd.print("A 1  ");
          lcd.setCursor(0,1);
          lcd.print(" BPEM");
          lcd.print(char(2));
          lcd.print("          ");
          lcd.setCursor(8,0);
          lcd.print(" ");
          lcd.print(param[0] / 60);
          lcd.print(":");
          lcd.print(param[0] % 60);
          lcd.print("   ");
          flag=1;
      }
    if ((j==1) & (flag == 0)) { // экран "Температура Фаза 1"
          lcd.setCursor(0,0);
          lcd.print(char(1));
          lcd.print("A");
          lcd.print(char(8));
          lcd.print("A 1  ");
          lcd.setCursor(0,1);
          lcd.print(" TEM");
          lcd.print(char(3));
          lcd.print("EPATYPA");
          lcd.setCursor(8,0);
          lcd.print(" ");
          lcd.print(param[1]);
          lcd.print(char(4));
          lcd.print("C     ");
          flag=1;
      }  
    if ((j==2) & (flag == 0)) { // экран "Время Фаза 2"
          lcd.setCursor(0,0);
          lcd.print(char(1));
          lcd.print("A");
          lcd.print(char(8));
          lcd.print("A 2  ");
          lcd.setCursor(0,1);
          lcd.print(" BPEM");
          lcd.print(char(2));
          lcd.print("          ");
          lcd.setCursor(8,0);
          lcd.print(" ");
          lcd.print(param[2] / 60);
          lcd.print(":");
          lcd.print(param[2] % 60);
          lcd.print("   ");
          flag=1;
      }  
    if ((j==3) & (flag == 0)) { // экран "Температура Фаза 2"
          lcd.setCursor(0,0);
          lcd.print(char(1));
          lcd.print("A");
          lcd.print(char(8));
          lcd.print("A 2  ");
          lcd.setCursor(0,1);
          lcd.print(" TEM");
          lcd.print(char(3));
          lcd.print("EPATYPA");
          lcd.setCursor(8,0);
          lcd.print(" ");
          lcd.print(param[3]);
          lcd.print(char(4));
          lcd.print("C     ");
          flag=1;
      }  
    if ((j==4) & (flag == 0)) { // экран "Время Фаза 3"
          lcd.setCursor(0,0);
          lcd.print(char(1));
          lcd.print("A");
          lcd.print(char(8));
          lcd.print("A 3  ");
          lcd.setCursor(0,1);
          lcd.print(" BPEM");
          lcd.print(char(2));
          lcd.print("          ");
          lcd.setCursor(8,0);
          lcd.print(" ");
          lcd.print(param[4] / 60);
          lcd.print(":");
          lcd.print(param[4] % 60);
          lcd.print("   ");
          flag=1;
      }  
    if ((j==5) & (flag == 0)) { // экран "Температура Фаза 3"
          lcd.setCursor(0,0);
          lcd.print(char(1));
          lcd.print("A");
          lcd.print(char(8));
          lcd.print("A 3  ");
          lcd.setCursor(0,1);
          lcd.print(" TEM");
          lcd.print(char(3));
          lcd.print("EPATYPA");
          lcd.setCursor(8,0);
          lcd.print(" ");
          lcd.print(param[5]);
          lcd.print(char(4));
          lcd.print("C     ");
          flag=1;
      }  
    if ((j==6) & (flag == 0)) { // экран "Время загрузки"
          lcd.setCursor(0,0);
          lcd.print(char(8));
          lcd.print("ArPY");
          lcd.print(char(8));
          lcd.print("KA        ");
          lcd.setCursor(0,1);
          lcd.print("         ");
          lcd.setCursor(8,1);
          lcd.print(param[6]);
          lcd.print("min");
          flag=1;
      }  

   } // конец обработки edit
  } // конец обработки !started
  
  if (started) { // начало обработки started

    uint16_t rtd = thermo.readRTD();
    pv = thermo.temperature(RNOMINAL, RREF);

    if (flag==0) { // инициализация экрана и таймеров
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(char(1));
      lcd.print("A");
      lcd.print(char(8));
      lcd.print("A 1          ");
      stage1=(param[0]*60000);
      stage2=(param[2]*60000);
      stage3=(param[4]*60000);
      loading=(param[6]*60000);
      digitalWrite(fan, HIGH);
      digitalWrite(valve,HIGH);
      timer3 = millis();
      flag=1; // ставим флаг
    }
    if ((millis() - timer3 >= loading) & (flag == 1)) { // Закрываем загрузочный клапан
      digitalWrite(valve,LOW);
    }
    if ((millis() - timer1 <= stage1) & (flag2 == 0)) {  // Фаза 1 активна
      if (millis() - timer2 >= 1000) { // раз в секунду перерисовываем экран и проверяем температуру
        blink = !blink;
        progress1 = (millis()-timer1) / 60000;
        lcd.setCursor(1,1);
        lcd.print(progress1 / 60);
        if (blink) { lcd.print(":"); } else { lcd.print(" "); }
        if ((progress1 % 60) < 10) { lcd.print("0"); } // "паразитный" ноль перед значениями меньше 10
        lcd.print(progress1 % 60);
        lcd.print("/");
        lcd.print(param[0] / 60);
        lcd.print(":");
        lcd.print(param[0] % 60);
        lcd.print("     ");
        lcd.setCursor(7,0);
        lcd.print(pv);
        lcd.print("/");
        lcd.print(param[1]);
        lcd.print(char(4));
        lcd.print("C     ");
        sp=param[1];
        if (pv < sp - 1) relayState = 1;
        else if (pv >= sp) relayState = 0;
        digitalWrite(heater, relayState);        
        lcd.setCursor(15,1);
        if (relayState) {lcd.print("*");} else {lcd.print(" ");}
        timer2 = millis(); // сбрасываем таймер
      }
    } 
    if ((millis() - timer1 > stage1) & (flag2==0)) {// конец 1-й фазы, переход на 2 фазу
      flag2 = 1;
      timer1 = millis();
      lcd.setCursor(5,0);
      lcd.print("2");
    }
    if ((millis() - timer1 <= stage2) & (flag2==1) & (flag3==0)){ // фаза 2 активна
      if (millis() - timer2 >= 1000) { // раз в секунду перерисовываем экран и проверяем температуру
        blink = !blink;
        progress1 = (millis()-timer1) / 60000;
        lcd.setCursor(1,1);
        lcd.print(progress1 / 60);
        if (blink) { lcd.print(":"); } else { lcd.print(" "); }
        if ((progress1 % 60) < 10) { lcd.print("0"); } // "паразитный" ноль перед значениями меньше 10
        lcd.print(progress1 % 60);
        lcd.print("/");
        lcd.print(param[2] / 60);
        lcd.print(":");
        lcd.print(param[2] % 60);
        lcd.print("     ");
        lcd.setCursor(7,0);
        lcd.print(pv);
        lcd.print("/");
        lcd.print(param[3]);
        lcd.print(char(4));
        lcd.print("C     ");
        sp=param[3];        
        if (pv < sp - 1) relayState = 1;
        else if (pv >= sp) relayState = 0;
        digitalWrite(heater, relayState);        
        lcd.setCursor(15,1);
        if (relayState) {lcd.print("*");} else {lcd.print(" ");}
        timer2 = millis(); // сбрасываем таймер
      }
    }
    if ((millis() - timer1 > stage2) & (flag2==1) & (flag3==0)) {// конец 2-й фазы, переход на 3 фазу
      flag3 = 1;
      timer1 = millis();
      lcd.setCursor(5,0);
      lcd.print("3");
    }
    if ((millis() - timer1 <= stage3) & (flag3==1)){ // фаза 3 активна
      if (millis() - timer2 >= 1000) { // раз в секунду перерисовываем экран и проверяем температуру
        blink = !blink;
        progress1 = (millis()-timer1) / 60000;
        lcd.setCursor(1,1);
        lcd.print(progress1 / 60);
        if (blink) { lcd.print(":"); } else { lcd.print(" "); }
        if ((progress1 % 60) < 10) { lcd.print("0"); } // "паразитный" ноль перед значениями меньше 10
        lcd.print(progress1 % 60);
        lcd.print("/");
        lcd.print(param[4] / 60);
        lcd.print(":");
        lcd.print(param[4] % 60);
        lcd.print("     ");
        lcd.setCursor(7,0);
        lcd.print(pv);
        lcd.print("/");
        lcd.print(param[5]);
        lcd.print(char(4));
        lcd.print("C     ");
        sp=param[5];        
        if (pv < sp - 1) relayState = 1;
        else if (pv >= sp) relayState = 0;
        digitalWrite(heater, relayState);        
        lcd.setCursor(15,1);
        if (relayState) {lcd.print("*");} else {lcd.print(" ");}
        timer2 = millis(); // сбрасываем таймер
      }
    }
    if ((millis() - timer1 >= stage3) & (flag3==1)){ // фаза 3 закончена
      flag=0;
      flag2=0;
      flag3=0;
      timer1 = millis();
      param[7] = param[7] + (param[0] + param[2] + param[4])/60;
      param[8] = param[8] + (param[0] + param[2] + param[4])/60;
      EEPROM.put(0,param);
      started=!started;
    }
  } // конец обработки started

}
