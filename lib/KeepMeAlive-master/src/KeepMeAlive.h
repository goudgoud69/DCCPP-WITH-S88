/*
 * KeepMeAlive
 *
 * KeepMeAlive is a library to manage the watchdog timer.
 *
 * Copyright Jean-Luc Béchennec 2018 - Locoduino
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

#ifndef __KEEPMEALIVE_H__
#define __KEEPMEALIVE_H__

#include <Arduino.h>

class WatchdogTimerClass {

public:
  typedef void (*InterruptRoutine)();

private:
  InterruptRoutine  mInterruptRoutine;
  uint16_t          mDelay;
  bool              mResetCPU:1;
  bool              mEnabled:1;
  uint8_t           mResetVectorSize:6;
  uint32_t          mResetVector;

  uint8_t computeActualDelay();

  void initResetVector()
  {
    mResetVector = 0xFFFFFFFF >> (32 - mResetVectorSize);
  }

public:
  /* Constructor, no interrupt attached, default delay set to 10000 */
  WatchdogTimerClass() :
    mInterruptRoutine(NULL),
    mDelay(10000),
    mResetCPU(false),
    mEnabled(false),
    mResetVectorSize(0),
    mResetVector(0)
  {}

  /*
   * setDelay allows you to program the watchdog timer to expire at the end
   * of delay. delay is given in milliseconds. All values are not available.
   * the minimum value is 16 ms and the maximum value 8000 ms. Possible values
   * are:
	 * - 16 ms
	 * - 32 ms
	 * - 64 ms
	 * - 125 ms
	 * - 250 ms
	 * - 500 ms
	 * - 1000 ms
	 * - 2000 ms
	 * - 4000 ms
	 * - 8000 ms
   * If delay does not correspond to an available value, setDelay chooses the
   * next higher value. For instance on an AVR ATMega 328 a delay equal to
   * 1500 ms is rounded up to 2000 ms.
   */
  void setDelay(const uint16_t delay) { mDelay = delay; }

  /*
   * By calling doResetCPU the watchdog will reset the CPU if the argument is
   * true and will do not reset the CPU if the argument is false.
   */
  void doResetCPU(const bool inDoReset) { mResetCPU = inDoReset; }

  /*
   * attachInterrupt attaches an interrupt routine to the watchdog.
   * if an interrupt routine is attached, it is called when the watchdog
   * is called before resetting the CPU.
   */
  void attachInterrupt(InterruptRoutine inInterruptRoutine) {
    mInterruptRoutine = inInterruptRoutine;
  }

  /*
   * detachInterrupt removes the interrupt routine from the watchdog.
   */
  void detachInterrupt() { mInterruptRoutine = NULL; }

  /*
   * if doResetCPU or attachInterrupt have been called, enable enables the
   * watchdog and start the watchdog timer so that when it expires the
   * attached interrupt routine is called and/or a reset is done.
   * If none of these two method has been called, enable does not do anything.
   */
  void enable();

  /*
   * disable disables the watchdog and stops the watchdog timer.
   */
  void disable();

  /*
   * reset resets the watchdog timer. It has to be called in the sketch with
   * a period lower than the programmed delay so that the normal execution of
   * the sketch does not lead to a reset or to a watchdog interrupt.
   */
  void reset();

  /*
   * alive notifies a module is alived. Each module is identified by an integer
   * ranging from 0 to 31. Each module has a bit assigned in the reset vector.
   * alive clear the bit and when all bits are cleared, the watchdog is reset
   * and the vector is set.
   */
  void alive(uint8_t inModuleNumber);

  /*
   * setModuleCount sets the number of modules, from 1 to 32
   */
  void setModuleCount(uint8_t inModuleCount);

  /*
   * interruptAttached returns the pointer to the interruption function
   */
  InterruptRoutine interruptAttached() { return mInterruptRoutine; }
};

extern WatchdogTimerClass watchdogTimer;

#endif
