const int cam_open = PB3;
const int cam_close = PB4;
const int pin_ena = PB0;
const int pin_closed = PB1;
const int pin_opened = PB2;
int enabled = 0;
int closed = 0;
int opened = 0;
int stage = 0;
uint32_t tmr1;
uint32_t closetmr = 0;
uint32_t opentmr = 0;

void setup() {
  pinMode(cam_open, OUTPUT);
  pinMode(cam_close, OUTPUT);
  pinMode(pin_ena,INPUT);
  pinMode(pin_closed,INPUT);
  pinMode(pin_opened,INPUT);
}


void loop() {
  
  enabled = digitalRead(pin_ena);
  int readclosed = digitalRead(pin_closed);
  int readopened = digitalRead(pin_opened);

  if (readclosed == 0) {
    delay(1);
    closetmr = closetmr + 1;
    if (closetmr > 50) 
    {
      closed = readclosed;
    }
  }
  else
  {
    closetmr = 0;
    closed = readclosed;
  }
  
  if (readopened == 0) {
    delay(1);
    opentmr = opentmr + 1;
    if (opentmr > 50) 
    {
      opened = readopened;
    }
  }
  else
  {
    opentmr = 0;
    opened = readopened;
  }

  if (enabled == LOW & stage == 0) 
  {
    tmr1 = millis();
    stage = 1;
  }

  if (enabled == HIGH & stage == 1) 
  {
    tmr1 = millis();
    stage = 0;
  }

  if (enabled == LOW & stage == 1)// открыть
  {
    if (millis() - tmr1 >= 1500)
      {
        tmr1 = millis();
        digitalWrite(cam_open, HIGH);
        stage = 2;
      }
  }

if (stage == 2)// открыть
  {
    if (millis() - tmr1 >= 2000 || opened == LOW)
      {
        tmr1 = millis();
        digitalWrite(cam_open, LOW);
        stage = 3;
      }
  } 

  if (enabled == LOW && stage == 3) // закрыть
  {
      tmr1 = millis();
  }

  if (enabled == HIGH && stage == 3) // закрыть
  {
    if (millis() - tmr1 >= 5000)
    {
      tmr1 = millis();
      digitalWrite(cam_close, HIGH);
      stage = 4;
    }
  }

  if (stage == 4) // закрыть
  {
    if (millis() - tmr1 >= 2000 || closed == LOW)
      {
        tmr1 = millis();
        digitalWrite(cam_close, LOW);
        stage = 0;
      }
  }
}
