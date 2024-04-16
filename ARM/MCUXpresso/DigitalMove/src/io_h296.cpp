/*==============================================================================================
| Project	: Digital Move
|
| File		: io.cpp
|
| Purpose	: I/Oの定義
|
| Compiler	: ARM - 
|
| History	: Version	Date		Content											Author
|			  V*.**		2020/--/--	First Edition									N.Ishikawa
+==============================================================================================*/
#include "mbed.h"
#include "io.h"
#include "common.h"


#if (CALSELECT & CALH296)				// Cal.H296 ?
/*==============================================================================================
| PORT I/O
+==============================================================================================*/
// +++ MBED Debug LED +++
DigitalOut LED1_P(LED1);				// MBED Debug LED1
DigitalOut LED2_P(LED2);				// MBED Debug LED2
DigitalOut LED3_P(LED3);				// MBED Debug LED3
DigitalOut LED4_P(LED4);				// MBED Debug LED4

// +++ Motor Port +++
InterruptIn MOTORAINT_P(p5);			// Motor A 割り込み
DigitalIn	MOTORADIR_P(p6);			// Motor A 方向入力 (0:正転, 1:逆転)
InterruptIn MOTORBINT_P(p7);			// Motor B 割り込み
DigitalIn	MOTORBDIR_P(p8);			// Motor B 方向入力 (0:正転, 1:逆転)
#if !(CALSELECT)						// General ?
InterruptIn MOTORCINT_P(p9);			// Motor C 割り込み
DigitalIn	MOTORCDIR_P(p10);			// Motor C 方向入力 (0:正転, 1:逆転)
InterruptIn MOTORDINT_P(p11);			// Motor D 割り込み
DigitalIn	MOTORDDIR_P(p12);			// Motor D 方向入力 (0:正転, 1:逆転)
#endif

// LED
#if !(CALSELECT)						// General ?
InterruptIn LED1INT_P(p13);				// LED1 割り込み
InterruptIn LED2INT_P(p14);				// LED2 割り込み
InterruptIn LED3INT_P(p15);				// LED3 割り込み
#elif !(CALSELECT & CALH296)
InterruptIn LED1INT_P(p13);				// LED1 割り込み
#endif

// +++ PTRC Port +++
DigitalOut PTRC_P(p16);					// P16:OUTPUT PTRC(23)

// +++ LED Parameter Port +++
#if !(CALSELECT)						// General ?
DigitalIn LEDTIM1_P(p17);				// P17:LEDTIME(b1)
DigitalIn LEDTIM0_P(p18);				// P18:LEDTIME(b0)
DigitalIn PTRADJ3_P(p19);				// P19:PTRADJ(b3)
DigitalIn PTRADJ2_P(p20);				// P20:PTRADJ(b2)
DigitalIn PTRADJ1_P(p21);				// P21:PTRADJ(b1)
DigitalIn PTRADJ0_P(p22);				// P22:PTRADJ(b0)
DigitalIn LEDADJ1_P(p23);				// P23:LEDADJ(b1)
DigitalIn LEDADJ0_P(p24);				// P24:LEDADJ(b0)
#elif (CALSELECT & CALNS30)				// NS30 ?
DigitalIn PTRADJ0_P(p9);				// P9: PTRADJ(b0)
DigitalIn PTRADJ1_P(p10);				// P10:PTRADJ(b1)
DigitalIn PTRADJ2_P(p11);				// P11:PTRADJ(b2)
DigitalIn PTRADJ3_P(p12);				// P12:PTRADJ(b3)
DigitalIn LEDADJ0_P(p14);				// P14:LEDADJ(b0)
DigitalIn LEDADJ1_P(p15);				// P15:LEDADJ(b1)
#elif (CALSELECT & CAL0100)				// 0100 ?
DigitalIn LEDADJ0_P(p9);				// P9 :LEDADJ(b0)
DigitalIn LEDADJ1_P(p10);				// P10:LEDADJ(b1)
DigitalIn PTRADJ0_P(p11);				// P11:PTRADJ(b0)
DigitalIn PTRADJ1_P(p12);				// P12:PTRADJ(b1)
DigitalIn PTRADJ2_P(p14);				// P14:PTRADJ(b2)
#endif

// +++ Decode IC Port +++
#if (CALSELECT & CALH296)				// H296 ?
InterruptIn DENAINT_P(p9);				// DecENA 割り込み
InterruptIn DTCKINT_P(p10);				// DecTCK 割り込み
DigitalIn	DDATA_P(p11);				// P11:DecData
#endif


/*==============================================================================================
| Timer
+==============================================================================================*/
#if (CALSELECT & CALH296)				// H296 ?
Timeout timeout_led1;
#endif

#endif  // Cal.H296