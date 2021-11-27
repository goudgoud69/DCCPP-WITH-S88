[![GitHub release](https://img.shields.io/github/release/Locoduino/KeepMeAlive/all.svg)](https://github.com/Locoduino/KeepMeAlive/releases/latest) [![GitHub commits](https://img.shields.io/github/commits-since/Locoduino/KeepMeAlive/1.0.svg)](https://github.com/Locoduino/KeepMeAlive/compare/1.0...master)

# KeepMeAlive
KeepMeAlive is an Arduino Library that manages the watchdog timer. The watchdog timer is a safety mechanism that reset the Arduino when it appears the software is locked because of a software or hardware fault.
## Changelog
- 1.0 - initial version, support of the ATMega 328 only (Arduino Uno, Arduino Nano and Arduino Pro Mini).

## Rationale

Most of the time, the software you develop on Arduino remains in the prototype state. You do not necessarily take great care in the quality of the software and do not necessarily think about operational reliability. However, when the software is deployed and must provide its service over long periods of time, problems occur: unexpected but rare glitches of the software and the need to restart the Arduino board.

If the board is positioned in a location that is difficult to access, a manual restart is annoying.

A watchdog timer is a device that allows you to reset the Arduino or send an interruption after a certain time interval, i. e. a certain timer value.
The sketch must, regularly, reset the watchdog timer to 0 so that it cannot reach the value where it causes the reset or interruption. If the sketch is crashed, it does not reset the watchdog timer to 0 and is restarted via the reset.

## Documentation

By including the library in your sketch:

```
#include <KeepMeAlive.h>
```

You get a ```watchdogTimer``` object. ```watchdogTimer``` has methods to program the watchdog timer which are presented below.

### setDelay(*delay*)

```setDelay``` allows you to program the watchdog timer to expire at the end of *```delay```*. *```delay```* is given in *milliseconds*. The minimum and maximum values depend on the microcontroller and all values are not available.

- AVR ATMega 328 (Arduino Uno, Nano, Pro Mini): the minimum value is 16 ms and the maximum value 8000 ms. Possible values are
	- 16 ms
	- 32 ms
	- 64 ms
	- 128 ms
	- 256 ms
	- 512 ms
	- 1024 ms
	- 2048 ms
	- 4096 ms
	- 8192 ms

If *```delay```* does not correspond to an available value, ```setDelay``` chooses the next higher value. For instance on an AVR ATMega 328 a *```delay```* equal to 1500 ms is rounded up to 2000 ms. If *```delay```* is greater than the maximum value, the maximum value is used.

```setDelay``` does not enable the watchdog.

If ```setDelay``` is used while the watchdog is enabled, the new value is taken into account if the watchdog is disabled then enabled.

The default value for the delay, that is the delay which is used if ```enable()``` is called whereas ```setDelay``` has not been called before, is 10000 ms.

### doResetCPU(*doReset*)

By calling ```doResetCPU``` the watchdog will reset the CPU if *```doReset```* is ```true``` and will do not reset the CPU if *```doReset```* is ```false```.

### attachInterrupt(*interruptRoutine*)

```attachInterrupt``` attaches an interrupt routine to the watchdog. if an interrupt routine is attached, it is called when the watchdog is called before resetting the CPU.

### detachInterrupt()

```detachInterrupt``` removes the interrupt routine from the watchdog.

### enable()

if ```doResetCPU``` or ```attachInterrupt``` have been called, ```enable``` enables the watchdog and start the watchdog timer so that when it expires the attached interrupt routine is called and/or a reset is done. If none of these two method has been called, ```enable``` does not do anything. Calling ```enable``` while the watchdog is already enabled has no effect.

### disable()

```disable``` disables the watchdog and stops the watchdog timer. Calling ```disable``` while the watchdog is not enabled has no effect.

### reset()

```reset``` resets the watchdog timer. It has to be called in the sketch with a period lower than the programmed delay so that the normal execution of the sketch does not lead to a reset or to a watchdog interrupt.

## Advanced use : software modules

This is an implementation of a [method described by Philip Koopman](https://betterembsw.blogspot.com/2014/05/proper-watchdog-timer-use.html). The goal is to reset the watchdog if an only if each software module has been visited in the loop. This can also encompass interrupt service routines if any.

Each module has a unique number. Numbers can be declared as constants by using an enum. As many as 32 modules can be defined.

```C
const uint8_t kModuleCount = 4;
enum { CAN_ISR_MODULE, SERVO_MODULE, LED_MODULE, DETECTION_MODULE };
```

### setModuleCount(*count*)

Set the number of module used. By default 0 modules are set. ```count``` should have a value between 1 and 32. If a value outside this interval is given, ```setModuleCount``` has no effect.

### alive(*module*)

```alive``` reports a module is alive. *```module```* shall have a value between 0 and the number of modules minus one. If a value outside this interval is given, ```alive``` has no effect. When all the modules have reported that they are alive, the watchdog is reset and module reporting is cleared.

## Note about the behavior of the watchdog timer on the ATMega 328.

When both CPU reset and interrupt are enabled, the interrupt has priority and, when the delay has elapsed, will be executed without a reset. Once the interrupt has been executed, the CPU returns to the execution of the main program and the watchdog timer delay elapses a second time to lead to the reset.

## Warning

Be careful not to set a too short delay. Indeed, if the OptiBoot boot loader disables the watchdog timer, this is not the case for all bootloaders. It is therefore necessary to have enough time to allow the bootloader to work. 2 seconds is an appropriate value. With a too short delay, the watchdog timer will cause a reset before setup() is executed and the watchdog timer is reprogrammed.
