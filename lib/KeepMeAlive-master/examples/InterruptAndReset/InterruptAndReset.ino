/*
 *  KeepMeAlive interrupt and reset example
 *  
 *  In this example the watchdog timer is programmed to perform
 *  an interrupt and reset after 2 s. First the interrupt is
 *  triggered and after 2 more s the reset is done
 *  
 *  Expected output is

(RE)STARTING
6.86
7.36
7.86
8.36
8.86
*** 9.36
*** 9.86
*** 10.36
(RE)STARTING
6.86
7.36
7.86
8.36
8.86
*** 9.36
*** 9.86
*** 10.36

 * and so on
 */

#include <KeepMeAlive.h>

volatile bool hasDied = false;

void dying()
{
  for (byte i = 0; i < 10; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    for (volatile word w = 0; w < 20000; w++);
    digitalWrite(LED_BUILTIN, LOW);
    for (volatile word w = 0; w < 20000; w++);    
  }
  hasDied = true;
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  Serial.println("(RE)STARTING");
  
  for (byte i = 0; i < 6; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    for (volatile word w = 0; w < 60000; w++);
    digitalWrite(LED_BUILTIN, LOW);
    for (volatile word w = 0; w < 60000; w++);    
  }

  /*
   * Program the watchdog timer to at least 2s delay
   * Once delay ellapsed, dying is executed
   * After 2 more s the CPU is reset
   */
  watchdogTimer.setDelay(2000);
  watchdogTimer.attachInterrupt(dying);
  watchdogTimer.doResetCPU(true);
  /*
   * Start the watchdog timer
   */
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
    if (hasDied) Serial.print("*** ");
    Serial.println(((float)millis())/1000.0);
    delay(500);
  }
}
