/*==============================================================================================
| Project	: Digital Move
|
| File		: boot.cpp
|
| Purpose	: 起動処理
|
| Compiler	: ARM - 
|
| History	: Version	Date		Content											Author
|			  V*.**		2020/--/--	First Edition									N.Ishikawa
+==============================================================================================*/
#define	_BOOT_

#include "common.h"
#include "gram.h"

#include "bios.h"
#include "main.h"
#include "lcd_act.h"
#include "sc0uart.h"


/*==============================================================================================
| Prototype
+==============================================================================================*/
void boot(void);															// 起動処理
void init_cpu(void);														// CPUの初期化
void init_variable(void);													// 変数の初期化


/*==============================================================================================
| Variable
+==============================================================================================*/


/*==============================================================================================
| Table
+==============================================================================================*/


/*==============================================================================================
| Function
+==============================================================================================*/
/*======================================================================================
| Declaration :	boot
| Description : 起動処理
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void boot(void)
{
	__disable_irq();								// 割込み禁止

	init_cpu();
	init_variable();
	init_sc0uart();
	
	__enable_irq();									// 割込み許可
}

/*======================================================================================
| Declaration :	init_cpu
| Description : CPUの初期化
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void init_cpu(void)
{
	// MBED LED
	LED1_P = 0;
	LED2_P = 0;
	LED3_P = 0;
	LED4_P = 0;
	
	// Motor Port
	MOTORAINT_P.mode(PullDown);
	MOTORADIR_P.mode(PullDown);
	MOTORBINT_P.mode(PullDown);
	MOTORBDIR_P.mode(PullDown);
#if (!CALSELECT || CALSELECT & CALH800 || CALSELECT & CALF150)	// General/H800/F150 ?
	MOTORCINT_P.mode(PullDown);
	MOTORCDIR_P.mode(PullDown);
	MOTORDINT_P.mode(PullDown);
	MOTORDDIR_P.mode(PullDown);
#endif
	
	// LED Port
#if (!CALSELECT || CALSELECT & CALH800)							// General/H800/F150 ?
	LED1INT_P.mode(PullUp);
	LED2INT_P.mode(PullUp);
	LED3INT_P.mode(PullUp);
#elif !((CALSELECT & CALH296) || (CALSELECT & CALF150))
	LED1INT_P.mode(PullUp);
#endif
	
	// PTRC Port
	PTRC_P = 1;
	
	// Interrupt Motor
	MOTORAINT_P.rise(&interrupt_motora);						// Motor A
	MOTORBINT_P.rise(&interrupt_motorb);						// Motor B
#if (!CALSELECT || CALSELECT & CALH800 || CALSELECT & CALF150)	// General/H800/F150 ?
	MOTORCINT_P.rise(&interrupt_motorc);						// Motor C
	MOTORDINT_P.rise(&interrupt_motord);						// Motor D
#endif
	
	// Interrupt LED
#if (CALSELECT & CALH800)										// H800 ?
	LED1INT_P.fall(&interrupt_fall_led1);						// LED1 Fall
	LED1INT_P.rise(&interrupt_rise_led1);						// LED1 Rise
	LED2INT_P.fall(&interrupt_fall_led2);						// LED2 Fall
	LED2INT_P.rise(&interrupt_rise_led2);						// LED2 Rise
#elif !(CALSELECT & CALH296 || CALSELECT & CALF150)
	LED1INT_P.fall(&interrupt_fall_led1);						// LED1 Fall
	LED1INT_P.rise(&interrupt_rise_led1);						// LED1 Rise
#endif
	
#if (CALSELECT & CALH296)										// H296
	regist_interrupt_decode();									// デコードIC通信ポート
#endif
	
#if (CALSELECT & CALF150)										// F150
	regist_interrupt_drive();									// ドライブIC通信ポート
#endif

}

/*======================================================================================
| Declaration :	init_variable
| Description : 変数の初期化
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void init_variable(void)
{
	// ポート割当IDクリア
	//memset(port_assign, 0, sizeof(port_assign));
	
	// モータ設定クリア
	memset(motprm, 0, sizeof(motprm));
	
	// LED設定クリア
	memset(ledprm, 0, sizeof(ledprm));
	
	// 針検パラメータクリア
	memset(&ndetprm, 0, sizeof(ndetprm));

	// LED CH クリア
	*(uchar*)&periled = 0;

	// LCD Event
	syssts.lcdevent = LCD_RESET;
}


