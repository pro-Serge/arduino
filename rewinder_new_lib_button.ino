#include "GyverStepper.h"
#include "button.h"
GStepper< STEPPER2WIRE> stepper(2000, 9, 10, 11);
int started;
int warning;
button starttrig(2);
button cntstop(3);


void setup() {
  stepper.setRunMode(KEEP_SPEED); // режим поддержания скорости
  stepper.setSpeedDeg(50);        // в градусах/сек
  pinMode(13, OUTPUT);            // индикация режима запуска светодиодом
  Serial.begin(9600);
}
void loop() {
  
  
  if (starttrig.click()) {         // обработка кнопки "пуск-стоп"
       started=!started;
       Serial.println("start/stop");
  }
  
  if (!started) {
    stepper.setSpeedDeg(1);      // сброс значения скорости после остановки. Для плавного запуска.
    digitalWrite(13, LOW);
    warning=LOW;
  }
  
  
  if (cntstop.click()) {     // обработка сигнала "стоп" со счетчика импульсов
      started=LOW;
      Serial.println("counter");
    }
    
      
  if (started == HIGH) {
  digitalWrite(13, started);
    stepper.tick();
  // сделаем таймер на 50 мс и будем опрашивать потенциометр
  // менять скорость чаще нет смысла
  static uint32_t tmr2;
  if (millis() - tmr2 > 50) {
    tmr2 = millis();
    // ставим новую скорость (0.. 1024 шага в секунду)
    if (!warning) {stepper.setSpeed(analogRead(0)+1, SMOOTH);} else {stepper.setSpeed(100, SMOOTH);} 
  }
}
}
