#include <EEPROM.h>

//#define _NAMIKI_MOTOR	 //for Namiki 22CL-103501PG80:1
/********************************************************************/
/*


				        Power Switch

				        Sonar0x11

				 -------------------------
				/                         \
			       /		           \
			      /			            \
			M3   /			             \ M2
		       INT0 /			              \INT1
			   /			               \
			  /			                \
			 /			                 \
			 \			                 /
			  \			                /
		  	   \			               /
			    \			              /
		  Sonar0x12  \		  	             / Sonar0x13
			      \		                    /
			       \	                   /
				--------------------------
					    M1

 */


#include <fuzzy_table.h>
#include <PID_Beta6.h>

#include <PinChangeInt.h>
#include <PinChangeIntConfig.h>

#include <MotorWheel.h>
#include <Omni3WD.h>

//#include <SONAR.h>

// Motors

irqISR(irq1,isr1);
MotorWheel wheel1(9,8,6,7,&irq1);        // Pin9:PWM, Pin8:DIR, Pin6:PhaseA, Pin7:PhaseB

irqISR(irq2,isr2);
MotorWheel wheel2(10,11,14,15,&irq2);    // Pin10:PWM, Pin11:DIR, Pin14:PhaseA, Pin15:PhaseB

irqISR(irq3,isr3);
MotorWheel wheel3(3,2,4,5,&irq3);        // Pin3:PWM, Pin2:DIR, Pin4:PhaseA, Pin5:PhaseB

Omni3WD Omni(&wheel1,&wheel2,&wheel3);
/******************************************/

/******************************************/
// demo
void demoNoSensors(unsigned int speedMMPS) {   
  Omni.wheelBackSetSpeedMMPS(0, DIR_ADVANCE);
  Omni.wheelRightSetSpeedMMPS(speedMMPS, DIR_BACKOFF);
  Omni.wheelLeftSetSpeedMMPS(speedMMPS, DIR_ADVANCE);

  Omni.PIDRegulate();
}

/*****************************************/
// setup()
void setup() {
    TCCR1B=TCCR1B&0xf8|0x01;    // Pin9,Pin10 PWM 31250Hz
    TCCR2B=TCCR2B&0xf8|0x01;    // Pin3,Pin11 PWM 31250Hz
    
    Omni.PIDEnable(0.26,0.02,0,10);
    
}

/****************************************/
// loop()
void loop() {
    demoNoSensors(100);
}
