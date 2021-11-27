/*
 *  KeepMeAlive simple reset example
 *  
 *  In this example the watchdog timer is programmed to perform a reset after 2500 ms
 *  This lead in fact to at a 4s delay since 2,5s is not possible.
 *  
 *  Expected output is

(RE)STARTING
8.00
8.50
9.00
9.50
10.00
10.50
11.00
11.50
12.00
(RE)STARTING
8.00
8.50
9.00
9.50
10.00
10.50
11.00
11.50
12.00

 * ... and so on
 *  
 */
 
#include <KeepMeAlive.h>

void setup()
{
  Serial.begin(115200);
  Serial.println("(RE)STARTING");

  pinMode(LED_BUILTIN, OUTPUT);
  delay(2000);

  /*
   * Program the watchdog timer to at least 2,5s delay
   * Once delay ellapsed, the CPU is reset
   */
  watchdogTimer.setDelay(2500);
  watchdogTimer.doResetCPU(true);
  /*
   * Start the watchdog timer
   */
  watchdogTimer.enable();
}

void loop()
{
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
    Serial.println(((float)millis())/1000.0);
    delay(500);
  }
}
