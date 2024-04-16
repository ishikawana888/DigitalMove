/*==============================================================================================
| Project	: Digital Move
|
| File		: bios.c
|
| Purpose	: 基本I/O処理
|
| Compiler	: ARM - 
|
| History	: Version	Date		Content											Author
|			  V*.**		2020/--/--	First Edition									N.Ishikawa
+==============================================================================================*/
#include "common.h"
#include "gram.h"

#include "bios.h"
#include "main.h"

/*==============================================================================================
| Prototype
+==============================================================================================*/
void regist_interrupt(void);
void regist_interrupt_motor(uchar port);
void regist_interrupt_led(uchar port);
void regist_interrupt_decode(void);
void regist_interrupt_drive(void);


/*======================================================================================
| Declaration :	regist_interrupt
| Description : 割り込みの登録
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void regist_interrupt(void)
{
#if false
	MOTPARAM *pmotprm;
	LEDPARAM *pledprm;
	
	pmotprm = motprm;											// モータ割り込み登録
	while(pmotprm->port){										// A(0),B(1),C(2),D(3)連続で割り当てることが条件
		regist_interrupt_motor(pmotprm->port);
		++pmotprm;
	}
	
	pledprm = ledprm;											// LED割り込み登録
	while(pledprm->port){
		regist_interrupt_led(pledprm->port);
		++pledprm;
	}
#endif
}

/*======================================================================================
| Declaration :	regist_interrupt_motor
| Description : モータ割り込みの登録
| Parameters  :	uchar port -> 該当のポート番号
| Returns	  : n/a
+======================================================================================*/
void regist_interrupt_motor(uchar port)
{
	switch(port){
	case MOTPORT1:
		MOTORAINT_P.rise(&interrupt_motora);
		break;
	case MOTPORT2:
		MOTORBINT_P.rise(&interrupt_motorb);
		break;
	case MOTPORT3:
		break;
	case MOTPORT4:
		break;
	default:
		break;
	}
}

/*======================================================================================
| Declaration :	regist_interrupt_led
| Description : LED割り込みの登録
| Parameters  :	uchar port -> 該当のポート番号
| Returns	  : n/a
+======================================================================================*/
void regist_interrupt_led(uchar port)
{
#if !(CALSELECT & CALH296)
	switch(port){
	case LEDPORT1:
		LED1INT_P.fall(&interrupt_fall_led1);
		LED1INT_P.rise(&interrupt_rise_led1);
		break;
	default:
		break;
	}
#endif
}

/*======================================================================================
| Declaration :	regist_interrupt_decode
| Description : デコードIC割り込みの登録
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void regist_interrupt_decode(void)
{
#if (CALSELECT & CALH296)
	DENAINT_P.fall(&interrupt_fall_decena);
	DENAINT_P.rise(&interrupt_rise_decena);
	DTCKINT_P.rise(&interrupt_rise_dectck);
#endif
}

/*======================================================================================
| Declaration :	regist_interrupt_drive											<V1.02>
| Description : ドライブIC割り込みの登録
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void regist_interrupt_drive(void)
{
#if (CALSELECT & CALF150)
	DENAINT_P.fall(&interrupt_fall_drvena);
	DENAINT_P.rise(&interrupt_rise_drvena);
	DTCKINT_P.rise(&interrupt_rise_drvtck);
#endif
}



