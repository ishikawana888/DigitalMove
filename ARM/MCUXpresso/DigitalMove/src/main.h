/*==============================================================================================
| Project	: Digital Move
|
| File		: main.h
|
| Purpose	: メインループ
|
| Compiler	: ARM - 
|
| History	: Version	Date		Content											Author
|			  V*.**		2020/--/--	First Edition									N.Ishikawa
+==============================================================================================*/
#ifndef MAIN_H_
#define MAIN_H_

/*==============================================================================================
| Define
+==============================================================================================*/


/*==============================================================================================
| Structure
+==============================================================================================*/


/*==============================================================================================
| External Table
+==============================================================================================*/


/*==============================================================================================
| External Function
+==============================================================================================*/
int main(void);													// メインループ

void interrupt_motora(void);
void interrupt_motorb(void);
void interrupt_motorc(void);
void interrupt_motord(void);
void interrupt_fall_led1(void);
void interrupt_rise_led1(void);
void interrupt_fall_led2(void);
void interrupt_rise_led2(void);

#if (CALSELECT & CALH296)
void interrupt_rise_decena(void);
void interrupt_fall_decena(void);
void interrupt_rise_dectck(void);
#endif

#if (CALSELECT & CALF150)
void interrupt_rise_drvena(void);
void interrupt_fall_drvena(void);
void interrupt_rise_drvtck(void);
#endif

#endif /* MAIN_H_ */

