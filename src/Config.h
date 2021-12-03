/**********************************************************************

Config.h
COPYRIGHT (c) 2013-2016 Gregg E. Berman
Adapted for DcDcc by Thierry PARIS

Part of DCC++ BASE STATION for the Arduino

Adapté pat David Goudard pour l'ajout carte multiplex pour senseurs IR

**********************************************************************/
#ifndef __config_h
#define __config_h

#include "Arduino.h"

#ifdef USE_SENSORMUXCARD
	#include "SensorMuxCard.h"
#endif 

/**	Use it as an argument to specify an unused pin. */
#define UNDEFINED_PIN	255

/////////////////////////////////////////////////////////////////////////////////////
//
// DEFINE NUMBER OF MAIN TRACK REGISTER

/** Number of track registers for the main line. 0 for transient orders, the others for continual orders. */
#define MAX_MAIN_REGISTERS 12
/** Number of track registers for the programming line. 0 for transient orders, the two others for continual orders for the only loco on this track. */
#define MAX_PROG_REGISTERS 3

/////////////////////////////////////////////////////////////////////////////////////
//
// DEFINE PINS ACCORDING TO MOTOR SHIELD MODEL
//

#ifdef ARDUINO_AVR_MEGA                   // is using Mega 1280, define as Mega 2560 (pinouts and functionality are identical)
#define ARDUINO_AVR_MEGA2560
#define MAX_MAIN_REGISTERS 100
#endif

#if (defined(ARDUINO_AVR_UNO) || defined(ARDUINO_AVR_NANO))      // Configuration for UNO or NANO

/** Interruption pin for main track.*/
#define DCC_SIGNAL_PIN_MAIN 10          // Arduino Uno  - uses OC1B
/** Interruption pin for programming track.*/
#define DCC_SIGNAL_PIN_PROG 5           // Arduino Uno  - uses OC0B

#elif defined(ARDUINO_AVR_MEGA2560)

#define DCC_SIGNAL_PIN_MAIN 12          // Arduino Mega - uses OC1B
#define DCC_SIGNAL_PIN_PROG 2           // Arduino Mega - uses OC3B

#endif

#define EmergencyStop A3           // allready defined in CurrentMonitor.h line 47
// ***** Pour activer le bouton d'arret d'urgence sur A3, placer un bouton poussoir connecté avec la masse

// ***** Pour utiliser CDM-Rail avec le bus S88, dé-commenter la ligne 16 de S88.h *****

/////////////////////////////////////////////////////////////////////////////////////
// COMM SETUP - ***** Please select a COMM type and an Ethernet interface if needed *****
/////////////////////////////////////////////////////////////////////////////////////

#define COMM_TYPE 0                 // Serial (USB) & NANO or UNO or MEGA
//#define COMM_TYPE 1                 // Ethernet & MEGA only ==> ***** you must choose an interface in DCCpp.h line 376-379 and an IP address line 149 *****
//                                                                ***** selectionner une interface dans DCCpp.h ligne 376-379 et une adresse IP ligne 149 *****

/////////////////////////////////////////////////////////////////////////////////////
// SELECT MOTOR SHIELD
/////////////////////////////////////////////////////////////////////////////////////

/** Interruption pin for main track.*/
#define MOTOR_SHIELD_SIGNAL_ENABLE_PIN_MAIN 3
#define MOTOR_SHIELD_SIGNAL_ENABLE_PIN_PROG 11

#define MOTOR_SHIELD_CURRENT_MONITOR_PIN_MAIN A0
#define MOTOR_SHIELD_CURRENT_MONITOR_PIN_PROG A1

#define MOTOR_SHIELD_DIRECTION_MOTOR_CHANNEL_PIN_A 12
#define MOTOR_SHIELD_DIRECTION_MOTOR_CHANNEL_PIN_B 13

#define POLOLU_SIGNAL_ENABLE_PIN_MAIN 9
#define POLOLU_SIGNAL_ENABLE_PIN_PROG 11

#define POLOLU_CURRENT_MONITOR_PIN_MAIN A0
#define POLOLU_CURRENT_MONITOR_PIN_PROG A1

#define POLOLU_DIRECTION_MOTOR_CHANNEL_PIN_A 7
#define POLOLU_DIRECTION_MOTOR_CHANNEL_PIN_B 8

////////////////////////////////////////////////////////////////////////////
// SELECT PINS MULTIPLEX CARD
////////////////////////////////////////////////////////////////////////////
#ifdef USE_SENSORMUXCARD
  #define MUX_CARD_NB 1 // nombre de carte multiplexeurs/demultiplexeurs
  #define MUX_CARD_NB_VOIES 16 // nombre de voies sur carte multiplexeurs/demultiplexeurs

  #define MUX_ENABLE_PIN 2
  #define MUX_S0_PIN 3
  #define MUX_S1_PIN 4
  #define MUX_S2_PIN 5
  #define MUX_S3_PIN 6

  #define MUX_CARD_PIN_1 A2
  #define MUX_CARD_PIN_2 A5
   
#endif

#ifdef USE_ETHERNET
enum EthernetProtocol
{
	HTTP,
	TCP
};
#endif

struct DCCppConfig
{
#ifdef USE_ETHERNET
	static uint8_t EthernetIp[4];
	static uint8_t EthernetMac[6];
	static int EthernetPort;
	static EthernetProtocol Protocol;
#endif

	static byte SignalEnablePinMain;	// PWM : *_SIGNAL_ENABLE_PIN_MAIN
	static byte CurrentMonitorMain;		// Current sensor : *_CURRENT_MONITOR_PIN_MAIN

	static byte SignalEnablePinProg;	// PWM : *_SIGNAL_ENABLE_PIN_PROG
	static byte CurrentMonitorProg;		// Current sensor : *_CURRENT_MONITOR_PIN_PROG

	// Only for shields : indirection of the signal from SignalPinMain to DirectionMotor of the shield
	static byte DirectionMotorA;		// *_DIRECTION_MOTOR_CHANNEL_PIN_A
	static byte DirectionMotorB;		// *_DIRECTION_MOTOR_CHANNEL_PIN_B
};

/////////////////////////////////////////////////////////////////////////////////////

#endif
