/*==============================================================================================
| Project	: Digital Move
|
| File		: io.h
|
| Purpose	: I/Oの定義
|
| Compiler	: ARM - 
|
| History	: Version	Date		Content											Author
|			  V*.**		2020/--/--	First Edition									N.Ishikawa
+==============================================================================================*/
#ifndef IO_H_
#define IO_H_


/*==============================================================================================
| Port
+==============================================================================================*/
// +++ MBED Debug LED +++
extern DigitalOut LED1_P;				// MBED Debug LED1 出力
extern DigitalOut LED2_P;				// MBED Debug LED2 出力
extern DigitalOut LED3_P;				// MBED Debug LED3 出力
extern DigitalOut LED4_P;				// MBED Debug LED4 出力

// +++ Motor Port +++
extern InterruptIn MOTORAINT_P;			// Motor A 割り込み
extern InterruptIn MOTORBINT_P;			// Motor B 割り込み
extern InterruptIn MOTORCINT_P;			// Motor C 割り込み
extern InterruptIn MOTORDINT_P;			// Motor D 割り込み
extern DigitalIn   MOTORADIR_P;			// Motor A 方向入力 (0:正転, 1:逆転)
extern DigitalIn   MOTORBDIR_P;			// Motor B 方向入力 (0:正転, 1:逆転)
extern DigitalIn   MOTORCDIR_P;			// Motor C 方向入力 (0:正転, 1:逆転)
extern DigitalIn   MOTORDDIR_P;			// Motor D 方向入力 (0:正転, 1:逆転)

// +++ LED Port +++
extern InterruptIn LED1INT_P;			// LED1 割り込み
extern InterruptIn LED2INT_P;			// LED2 割り込み
extern InterruptIn LED3INT_P;			// LED3 割り込み

// +++ PTRC Port +++
extern DigitalOut PTRC_P;				// PTRC 出力

// +++ LED Parameter Port +++
extern DigitalIn LEDTIM1_P;				// LEDTIME(b1)
extern DigitalIn LEDTIM0_P;				// LEDTIME(b0)
extern DigitalIn PTRADJ3_P;				// PTRADJ(b3)
extern DigitalIn PTRADJ2_P;				// PTRADJ(b2)
extern DigitalIn PTRADJ1_P;				// PTRADJ(b1)
extern DigitalIn PTRADJ0_P;				// PTRADJ(b0)
extern DigitalIn LEDADJ1_P;				// LEDADJ(b1)
extern DigitalIn LEDADJ0_P;				// LEDADJ(b0)

// +++ Decode IC Port +++
extern InterruptIn DENAINT_P;			// ENA 割り込み
extern InterruptIn DTCKINT_P;			// TCK 割り込み
extern DigitalIn   DDATA_P;				// Data

// +++ Drive IC Port +++
extern InterruptIn DENAINT_P;			// ENA 割り込み
extern InterruptIn DTCKINT_P;			// TCK 割り込み
extern DigitalIn   DCOM_P;				// COM


/*==============================================================================================
| Timer
+==============================================================================================*/
extern Timeout timeout_led1;
extern Timeout timer_motor_a;
extern Timeout timer_motor_b;
extern Timeout timer_motor_c;
extern Timeout timer_motor_d;


#endif /* IO_H_ */

