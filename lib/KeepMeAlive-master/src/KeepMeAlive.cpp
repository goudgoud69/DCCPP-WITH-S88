/*
 * KeepMeAlive
 *
 * KeepMeAlive is a library to manage the watchdog timer.
 *
 * Copyright Jean-Luc Béchennec 2018
 *
 * This software is distributed under the GNU Public Licence v2 (GPLv2)
 *
 * Please read the LICENCE file
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include "KeepMeAlive.h"
#include <avr/wdt.h>

ISR(WDT_vect)
{
  WatchdogTimerClass::InterruptRoutine intFunc =
    watchdogTimer.interruptAttached();
  if (intFunc != NULL) intFunc();
}

uint8_t WatchdogTimerClass::computeActualDelay()
{
  uint16_t delay = mDelay;
  /*
   * Value put in WDP3 WDP3 WDP1 WDP0 is actually the log2 of the value minus 4
   */
  if (delay < 16) delay = 16;
  if (delay > 8192) delay = 8192;
  delay--;
  delay >>= 4;
  uint8_t wdp = 0;
  while (delay > 0) {
    delay >>= 1;
    wdp++;
  };
  return wdp;
}

/*
 * if doResetCPU or attachInterrupt have been called, enable enables the
 * watchdog and start the watchdog timer so that when it expires the
 * attached interrupt routine is called and/or a reset is done.
 * If none of these two method has been called, enable does not do anything.
 */
void WatchdogTimerClass::enable()
{
  if (! mEnabled) {
    mEnabled = true;
    uint8_t actualDelay = computeActualDelay();
    uint8_t wdtcsr_value = ((actualDelay & 0x8) << 2) | (actualDelay & 0x7);
    if (mResetCPU) wdtcsr_value |= bit(WDE);
    if (mInterruptRoutine != NULL) wdtcsr_value |= bit(WDIE);
    noInterrupts();
    MCUSR = 0;
    /* enter config mode */
    WDTCSR |= bit(WDCE) | bit(WDE);
    /* set the value of WDTCSR */
    WDTCSR = wdtcsr_value;
    interrupts();
  }
}

/*
 * disable disables the watchdog and stops the watchdog timer.
 */
void WatchdogTimerClass::disable()
{
  if (mEnabled) {
    wdt_disable();
    mEnabled = false;
  }
}

/*
 * reset resets the watchdog timer. It has to be called in the sketch with
 * a period lower than the programmed delay so that the normal execution of
 * the sketch does not lead to a reset or to a watchdog interrupt.
 */
void WatchdogTimerClass::reset()
{
  wdt_reset();
}

/*
 * alive notifies a module is alived. Each module is identified by an integer
 * ranging from 0 to 31. Each module has a bit assigned in the reset vector.
 * alive clear the bit and when all bits are cleared, the watchdog is reset
 * and the vector is set.
 */
void WatchdogTimerClass::alive(uint8_t inModuleNumber)
{
  if (inModuleNumber < mResetVectorSize) {
    mResetVector &= ~(1 << inModuleNumber);
    if (mResetVector == 0) {
      /* All modules acknowledge their liveness */
      reset();
      initResetVector();
    }
  }
}

/*
 * setModuleCount sets the number of modules, from 1 to 32
 */
void WatchdogTimerClass::setModuleCount(uint8_t inModuleCount)
{
  if (inModuleCount >= 1 && inModuleCount <= 32) {
    mResetVectorSize = inModuleCount;
    initResetVector();
  }
}


WatchdogTimerClass watchdogTimer;
