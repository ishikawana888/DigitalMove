/*==============================================================================================
| Project	: Digital Move
|
| File		: lcd_act.cpp
|
| Purpose	: LCD表示
|
| Compiler	: ARM - 
|
| History	: Version	Date		Content											Author
|			  V*.**		2018/--/--	First Edition									N.Ishikawa
+==============================================================================================*/
#include "common.h"
#include "gram.h"

#include "lcd_act.h"
#include "sc0uart.h"


//TextLCD lcd(p24, p26, p27, p28, p29, p30,TextLCD::LCD20x4); // rs, e, d4-d7 "MBED" "OLED"

/*==============================================================================================
| Prototype
+==============================================================================================*/
void lcd_act(void);
void lcd_dispmsg(uchar event);


/*==============================================================================================
| Table
+==============================================================================================*/


/*==============================================================================================
| LCD
+==============================================================================================*/
/*======================================================================================
| Declaration :	lcd_act
| Description : LCD表示制御
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void lcd_act(void)
{
	if(syssts.lcdevent){
		lcd_dispmsg(syssts.lcdevent);
		syssts.lcdevent = 0;
	}
}

/*======================================================================================
| Declaration :	lcd _dispmsg
| Description : LCD表示処理
| Parameters  :	uchar event: 表示イベント
| Returns	  : n/a
+======================================================================================*/
void lcd_dispmsg(uchar event)
{
	uchar Pc_Flg;

	time_t seconds = time(NULL);
	struct tm *t=localtime(&seconds);
	
	switch(event){
		case LCD_RESET:
			lcd.locate(0,0);
			//----------01234567890123456789
			lcd.printf("++ QC3148 LPC1768 ++");
			lcd.locate(0,1);
			lcd.printf("++ typNS30  Ver3.40++");
			lcd.locate(0,2);
			lcd.printf("++      2018-11-07++");
			lcd.locate(0,3);
			lcd.printf("++                ++");
			break;
		case LCD_CLEAR1:
			lcd.locate(0,0);
			lcd.printf("                    ");
			break;
		case LCD_CLEAR2:
			lcd.locate(0,1);
			lcd.printf("                    ");
			break;
		case LCD_CLEAR3:
			lcd.locate(0,2);
			lcd.printf("                    ");
			break;
		case LCD_CLEAR4:
			lcd.locate(0,3);
			lcd.printf("                    ");
			break;
		case LCD_MOTOR:
			lcd.locate(0,1);
			lcd.printf("A%06d B%06d     ",motprm[0].cnt,motprm[1].cnt);
			lcd.locate(0,2);
			lcd.printf("C%06d D%06d     ",motprm[2].cnt,motprm[3].cnt);
			lcd.locate(0,3);
			lcd.printf("E%06d F%06d     ",motprm[4].cnt,motprm[5].cnt);
			break;
		case LCD_CMDRES:
			lcd.locate(0,0);
			//----------01234567890123456789
			//lcd.printf("ADR %02d CMD %02x       ",Pc_Flg,u0_recv_buf._cmd[0]);
			break;
		default:
			break;
	}
}


