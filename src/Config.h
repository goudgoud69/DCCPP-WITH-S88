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


/** Number of track registers for the programming line. 0 for transient orders, the two others for continual orders for the only loco on this track. */
#define MAX_PROG_REGISTERS 3

// ***** Pour activer le bouton d'arret d'urgence sur A3, placer un bouton poussoir connecté avec la masse
#define EmergencyStop A3           // allready defined in CurrentMonitor.h line 47


// ***** Pour utiliser CDM-Rail avec le bus S88, dé-commenter la ligne 16 de S88.h *****

/////////////////////////////////////////////////////////////////////////////////////
// COMM SETUP - ***** Please select a COMM type and an Ethernet interface if needed *****
/////////////////////////////////////////////////////////////////////////////////////

#define COMM_TYPE 0                 // Serial (USB) & NANO or UNO or MEGA
//#define COMM_TYPE 1                 // Ethernet & MEGA only ==> ***** you must choose an interface in DCCpp.h line 376-379 and an IP address line 149 *****
//                                                                ***** selectionner une interface dans DCCpp.h ligne 376-379 et une adresse IP ligne 149 *****


#ifdef USE_ETHERNET
enum EthernetProtocol
{
	HTTP,
	TCP
};
#endif

////////////////////////////////////////////////////////
// DCCppConfig
////////////////////////////////////////////////////////
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

#endif

/////////////////////////////////////////////////////////
// Sensord via multiplex card(s)
#ifdef USE_SENSORMUXCARD
  #define MUX_CARD_NB 2 // nombre de carte multiplexeurs/demultiplexeurs
  #define MUX_CARD_NB_VOIES 16 // nombre de voies sur carte multiplexeurs/demultiplexeurs
#endif

//////////////////////////////////////////////////////////////////////////////////////
// CONFIGURATION SPECIFIQUE SUIVANT LA CARTE
//////////////////////////////////////////////////////////////////////////////////////
#if defined(ARDUINO_AVR_UNO) || (ARDUINO_AVR_NANO)                         // Configuration for UNO or NANO
  #include "Config_UNO_NANO.h"

#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega644__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__)                  // is using Mega 1280, define as Mega 2560 (pinouts and functionality are identical)
   #include "Config_MEGA.h"
#endif



