/*
 *  KeepMeAlive simple interrupt example
 *  
 *  In this example the watchdog timer is programmed to execute the dying function after 2000 ms
 *  
 *  Expected output is

STARTING
0: 6.00
0: 6.50
0: 7.00
0: 7.50
0: 8.00
1: 8.50
1: 9.00
1: 9.50
2: 10.00
2: 10.50
2: 11.00
3: 11.50
3: 12.01
3: 12.51
3: 13.01
4: 13.51
4: 14.01
4: 14.51

 * and so on
 */

#include <KeepMeAlive.h>

volatile word deathCount = 0;

void dying()
{
  for (byte i = 0; i < 10; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    for (volatile word w = 0; w < 20000; w++);
    digitalWrite(LED_BUILTIN, LOW);
    for (volatile word w = 0; w < 20000; w++);    
  }
  deathCount++;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("STARTING");

  /* 
   * Program the watchdog timer to at least 2s delay.
   * When ellapsed, dying function is executed
   */
  watchdogTimer.setDelay(2000);
  watchdogTimer.attachInterrupt(dying);
  watchdogTimer.enable();
}

void loop() {
  /*
   * First a loop that blinks 10 times the led and reset the watchdog
   */
  for (byte i = 0; i < 20; i++) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(300);
    watchdogTimer.reset();    
  }

  /*
   * Then an infinite loop
   */
  while (1) {
    Serial.print(deathCount);
    Serial.print(": ");
    Serial.println(((float)millis())/1000.0);
    delay(500);
  }
}
