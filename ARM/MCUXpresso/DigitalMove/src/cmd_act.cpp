/*==============================================================================================
| Project	: Digital Move
|
| File		: cmd_act.c
|
| Purpose	: コマンド処理
|
| Compiler	: ARM - 
|
| History	: Version	Date		Content											Author
|			  V*.**		2018/--/--	First Edition									N.Ishikawa
+==============================================================================================*/
#include "common.h"
#include "gram.h"

#include "cmd_act.h"
#include "bios.h"
#include "lcd_act.h"
#include "sc0uart.h"
#include "sub_func.h"
#include "version.h"


/*==============================================================================================
| Prototype
+==============================================================================================*/
void cmd_recv(void);
void cmd_send(void);
void cmd_analyze(uchar *pdat);
void cmd_resp_store(uchar *pdat);

void cmd_get_version(uchar *pdat);
void cmd_get_count(uchar *pdat);
void cmd_set_reset(uchar *pdat);
void cmd_set_motor(uchar *pdat);
void cmd_set_led(uchar *pdat);
void cmd_set_apply(uchar *pdat);
void cmd_set_needlegap(uchar *pdat);
void cmd_clear_motcnt(void);


/*==============================================================================================
| Variable
+==============================================================================================*/


/*==============================================================================================
| Table
+==============================================================================================*/
/*======================================================================================
| コマンドテーブル
|	const char *str		-> Command pointer
|	void (*set)(uchar*)	-> Function call
|	void (*get)(uchar*)	-> Function call
+======================================================================================*/
const COMMAND_TBL command_t[] = {
	{ "AA",		NULL,				cmd_get_version		},				// バージョン情報
	{ "DA",		NULL,				cmd_get_count		},				// カウントリクエスト
	{ "DB",		cmd_set_reset,		NULL				},				// リセット
	{ "DC",		cmd_set_motor,		NULL				},				// モータ設定
	{ "DD",		cmd_set_led,		NULL				},				// LED設定
	{ "DE",		cmd_set_needlegap,	NULL				},				// 針ズレ設定
	{ "DZ",		cmd_set_apply,		NULL				},				// 設定適用
	
	{ NULL,		NULL,				NULL				},
};


/*==============================================================================================
| +++ RECEIVE +++
+==============================================================================================*/
/*======================================================================================
| Declaration :	cmd_recv
| Description : コマンド受信
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void cmd_recv(void)
{
	sc0uart_recv();
	if(u0_recv_sts._recv){												// 受信あり
		u0_recv_sts._recv = 0;
		cmd_analyze(&u0_recv_cmd.arry[0]);								// コマンド解析
	}
}

/*======================================================================================
| Declaration :	cmd_analyze
| Description : コマンド解析
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void cmd_analyze(uchar *pdat)
{
	const COMMAND_TBL *ptbl;
	void (*set)(uchar*);
	void (*get)(uchar*);
	
	ptbl = command_t;
	do {
		if(ptbl->str[0] == *(pdat+1) && ptbl->str[1] == *(pdat+2)){		// コマンド一致？
			switch(*pdat){
			case 'R':													// 取得コマンド
				get = ptbl->get;
				if(get){
					(*get)(pdat+1);
					cmd_status |= SEND_REQ;								// 送信要求
				}
				break;
			case 'W':													// 設定コマンド
				set = ptbl->set;
				if(set){
					(*set)(pdat+3);
					cmd_resp_store(pdat+1);
					cmd_status |= SEND_REQ;								// 送信要求
				}
				break;
			}
			break;
		}
		++ptbl;
	} while(ptbl->str);
}

/*======================================================================================
| Declaration :	cmd_get_version
| Description : バージョン情報取得
| Parameters  :	uchar *pdat
| Returns	  : n/a
+======================================================================================*/
void cmd_get_version(uchar *pdat)
{
	uchar *ptr;
	
	ptr = resp_cmd;
	*ptr++ = *pdat++;									// First Code  'A'
	*ptr++ = *pdat++;									// Second Code 'A'
	memcpy(ptr, (uchar*)&cal_name[0], CAL_NAME_SIZE);	// Cal Name
	ptr += CAL_NAME_SIZE;
	memcpy(ptr, (uchar*)&firm_ver[0], VER_INFO_SIZE);	// Version
	ptr += VER_INFO_SIZE;
	
	*ptr = U0_CMDTERM;									// Term
}

/*======================================================================================
| Declaration :	cmd_get_count
| Description : カウントリクエスト
| Parameters  :	uchar *pdat
| Returns	  : n/a
+======================================================================================*/
void cmd_get_count(uchar *pdat)
{
	MOTPARAM *pmotprm;
	ulong *pmotcnt, tmpl;
	uchar *ptr, *pndetprm;
	
	ptr = resp_cmd;
	*ptr++ = *pdat++;									// First Code  'D'
	*ptr++ = *pdat++;									// Second Code 'A'
	
	// 針検情報
	pndetprm = (uchar*)&ndetprm;
	*ptr++ = hex_asc(*pndetprm >> 4);					// 針検結果(b7)状態(b6)
	*ptr++ = hex_asc(*pndetprm++);						// CH
	*ptr++ = hex_asc(*pndetprm >> 4);					// Rank			(b7-4)
	*ptr++ = hex_asc(*pndetprm++);						//				(b3-0)
	*ptr++ = hex_asc(*pndetprm >> 4);					// 針穴検出回数	(b7-4)
	*ptr++ = hex_asc(*pndetprm++);						//			 	(b3-0)
	
	// Motor Count
	pmotprm = motprm;									// モータパラメータ (通常)
	pmotcnt = ndetprm.motcnt;							// モータカウンタ (針検中)
	do {												// Motor A~D
		if(ndetprm.status._busy){						// 針検中 ?
			tmpl = *pmotcnt++ & 0xffffff;				// b23-0
		}
		else{											// 針検中でない ?
			tmpl = pmotprm->cnt & 0xffffff;				// b23-0
		}
		*ptr++ = hex_asc(tmpl >> 20);					// b23-20
		*ptr++ = hex_asc(tmpl >> 16);					// b19-16
		*ptr++ = hex_asc(tmpl >> 12);					// b15-12
		*ptr++ = hex_asc(tmpl >> 8);					// b11-8
		*ptr++ = hex_asc(tmpl >> 4);					// b7-4
		*ptr++ = hex_asc(tmpl);							// b3-0
		pmotprm++;
	} while(pmotprm->max);
	
	*ptr = U0_CMDTERM;									// Term
	
	__disable_irq();									// 割込み禁止
	ndetprm.status._busy = false;
	__enable_irq();										// 割込み許可
}

/*======================================================================================
| Declaration :	cmd_set_reset
| Description : リセット
| Parameters  :	uchar *pdat
| Returns	  : n/a
+======================================================================================*/
void cmd_set_reset(uchar *pdat)
{
	cmd_clear_motcnt();									// モータカウンタクリア
}

/*======================================================================================
| Declaration :	cmd_set_motor
| Description : モータ設定
| Parameters  :	uchar *pdat
| Returns	  : n/a
+======================================================================================*/
void cmd_set_motor(uchar *pdat)
{
	MOTPARAM *ptr;
	uchar id;
	
	id = asc_hex(*pdat++);								// Motor ID (0(A)-25(Z))
	id <<= 4;
	id |= asc_hex(*pdat++);
	ptr = &motprm[id];
	
	ptr->max = asc_hex(*pdat++);						// Motor Step Max
	ptr->max <<= 4;
	ptr->max |= asc_hex(*pdat++);
	ptr->max <<= 4;
	ptr->max |= asc_hex(*pdat++);
	ptr->max <<= 4;
	ptr->max |= asc_hex(*pdat++);
	ptr->max <<= 4;
	ptr->max |= asc_hex(*pdat++);
	ptr->max <<= 4;
	ptr->max |= asc_hex(*pdat++);
}

/*======================================================================================
| Declaration :	cmd_set_led
| Description : LED設定
| Parameters  :	uchar *pdat
| Returns	  : n/a
+======================================================================================*/
void cmd_set_led(uchar *pdat)
{
	LEDPARAM *ptr;
	uchar id;
	
	id = asc_hex(*pdat++);								// LED ID
	id <<= 4;
	id |= asc_hex(*pdat++);
	ptr = &ledprm[id];
	
	ptr->motid = asc_hex(*pdat++);						// Motor ID (0(A)-25(Z))
	ptr->motid <<= 4;
	ptr->motid |= asc_hex(*pdat++);
	
	ptr->period = asc_hex(*pdat++);						// Motor Rap Step
	ptr->period <<= 4;
	ptr->period |= asc_hex(*pdat++);
	ptr->period <<= 4;
	ptr->period |= asc_hex(*pdat++);
	ptr->period <<= 4;
	ptr->period |= asc_hex(*pdat++);
	ptr->period <<= 4;
	ptr->period |= asc_hex(*pdat++);
	ptr->period <<= 4;
	ptr->period |= asc_hex(*pdat++);
	
	ptr->offset = asc_hex(*pdat++);						// Hole Offset
	ptr->offset <<= 4;
	ptr->offset |= asc_hex(*pdat++);
	ptr->offset <<= 4;
	ptr->offset |= asc_hex(*pdat++);
	ptr->offset <<= 4;
	ptr->offset |= asc_hex(*pdat++);
	ptr->offset <<= 4;
	ptr->offset |= asc_hex(*pdat++);
	ptr->offset <<= 4;
	ptr->offset |= asc_hex(*pdat++);
	
	ptr->ndettype = asc_hex(*pdat++);					// 針検種別
	ptr->ndettype <<= 4;
	ptr->ndettype |= asc_hex(*pdat++);
	
	ptr->holenone = asc_hex(*pdat++);					// 穴なし位置有無
	ptr->holenone <<= 4;
	ptr->holenone |= asc_hex(*pdat++);
	
	ptr->holenonepos = asc_hex(*pdat++);				// 穴なし位置
	ptr->holenonepos <<= 4;
	ptr->holenonepos |= asc_hex(*pdat++);
	ptr->holenonepos <<= 4;
	ptr->holenonepos |= asc_hex(*pdat++);
	ptr->holenonepos <<= 4;
	ptr->holenonepos |= asc_hex(*pdat++);
	ptr->holenonepos <<= 4;
	ptr->holenonepos |= asc_hex(*pdat++);
	ptr->holenonepos <<= 4;
	ptr->holenonepos |= asc_hex(*pdat++);
}

/*======================================================================================
| Declaration :	cmd_set_apply
| Description : 設定適用
| Parameters  :	uchar *pdat
| Returns	  : n/a
+======================================================================================*/
void cmd_set_apply(uchar *pdat)
{
	cmd_clear_motcnt();									// モータカウンタクリア
}

/*======================================================================================
| Declaration :	cmd_set_needlegap
| Description : 針ズレ設定
| Parameters  :	uchar *pdat
| Returns	  : n/a
+======================================================================================*/
void cmd_set_needlegap(uchar *pdat)
{
	MOTPARAM *ptr;
	ulong tmpl;
	uchar id, dir;
	
	id = asc_hex(*pdat++);								// Motor ID (0(A)-25(Z))
	id <<= 4;
	id |= asc_hex(*pdat++);
	
	dir = *pdat++;										// Plus or Minus
	
	tmpl = asc_hex(*pdat++);							// Gap Step
	tmpl <<= 4;
	tmpl |= asc_hex(*pdat++);
	tmpl <<= 4;
	tmpl |= asc_hex(*pdat++);
	tmpl <<= 4;
	tmpl |= asc_hex(*pdat++);
	tmpl <<= 4;
	tmpl |= asc_hex(*pdat++);
	tmpl <<= 4;
	tmpl |= asc_hex(*pdat++);
	
	__disable_irq();									// 割込み禁止
	ptr = &motprm[id];
	if (dir == '+'){									// Plus ?
		tmpl += ptr->cnt;
		if (tmpl >= ptr->max) {
			ptr->cnt = tmpl - ptr->max;
		}
		else{
			ptr->cnt = tmpl;
		}
	}
	else{												// Minus ?
		if (tmpl > ptr->cnt) {
			tmpl -= ptr->cnt;
			ptr->cnt = ptr->max - tmpl;
		}
		else{
			ptr->cnt -= tmpl;
		}
	}
	__enable_irq();										// 割込み許可
}

/*======================================================================================
| Declaration :	cmd_clear_motcnt
| Description : モータカウンタクリア
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void cmd_clear_motcnt(void)
{
	MOTPARAM *pmotprm;
	
	pmotprm = motprm;
	do {
		pmotprm->cnt = 0;
		pmotprm++;
	} while(pmotprm->max);
	
	syssts.lcdevent = LCD_MOTOR;						//LCDのカウントを再表示
}

/*======================================================================================
| Declaration :	cmd_resp_store
| Description : 応答データ格納
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void cmd_resp_store(uchar *pdat)
{
	uchar *ptr;
	
	ptr = resp_cmd;										// 応答データ
	*ptr++ = *pdat++;									// Code MSB
	*ptr++ = *pdat;										// Code LSB
	*ptr = U0_CMDTERM;
}


#if 0
/*****************************************************************************
** Function name:	send_message
** Descriptions:	1byte の返事の実行
** parameters:		
** Returned value:	
*****************************************************************************/
void send_message(unsigned char length_mes){

	unsigned char i =0 ;

	// 前回コマンドとの比較
	if (send_buf[0] != u0_recv_buf._cmd){
		lcd_event = LCD_CMDRES;
	}

	// 先頭の1byte
	send_buf[0] = u0_recv_buf._cmd[0];

	for (i = 0; i< length_mes; i=i+1) {
		pc.putc(send_buf[i]);
	}
}
#endif


/*======================================================================================
| Standard Command
+======================================================================================*/
/*======================================================================================
| Declaration :	cmd_standard
| Description : 通常コマンド
| Parameters  :	uchar *pdat -> 受信データ
| Returns	  : n/a
+======================================================================================*/
#if 0
void cmd_standard(uchar *pdat)
{
	const COMMAND_TBL *ptbl;
	
	ptbl = command_t;
	do {
		if(ptbl->str[0] == *(pdat+1) && ptbl->str[1] == *(pdat+2)){		// コマンド一致 ?
			switch(*pdat){
			case 'R':													// 取得コマンド
				ptbl->get(pdat+1);
				break;
			case 'W':													// 設定コマンド
				ptbl->set(pdat+3);
				break;
			}
			break;
		}
		++ptbl;
	} while(ptbl->str);
}
#endif

/*======================================================================================
| Declaration :	cmd_get_version
| Description : バージョン情報取得
| Parameters  :	uchar *pdat -> 受信データ
| Returns	  : n/a
+======================================================================================*/
#if 0
void cmd_get_version(uchar *pdat)
{
	uchar rbuf[64], *p;
	
	p = rbuf;
	*p++ = *pdat++;														// コマンドコード1
	*p++ = *pdat++;														// コマンドコード2
	memcpy(p, (uchar*)&cal_name[0], CAL_NAME_SIZE);						// キャリバー
	p += CAL_NAME_SIZE;
	memcpy(p, (uchar*)&firm_ver[0], VER_INFO_SIZE);						// バージョン
	
	if(i2c_status._recv){												// I2C受信 ?
		i2c_send_start(rbuf, 2 + CAL_NAME_SIZE + VER_INFO_SIZE);
	}
}
#endif

/*======================================================================================
| Declaration :	cmd_set_cpu
| Description : CPU ROM/RAM 書込み
| Parameters  :	uchar *pdat -> 受信データ
| Returns	  : n/a
+======================================================================================*/
#if 0
void cmd_set_cpu(uchar *pdat)
{
	ushort addr;
	uchar len, wbuf[32], *p, i;
	
	addr = asc_hex(*pdat++);											// アドレスデコード
	addr <<= 4;
	addr |= asc_hex(*pdat++);
	addr <<= 4;
	addr |= asc_hex(*pdat++);
	addr <<= 4;
	addr |= asc_hex(*pdat++);
	
	len = asc_hex(*pdat++);												// データ長デコード
	len <<= 4;
	len |= asc_hex(*pdat++);
	
	p = wbuf;															// 書込みデータデコード
	for(i=0; i<len; i++){
		*p = asc_hex(*pdat++);
		*p <<= 4;
		*p |= asc_hex(*pdat++);
		++p;
	}
	
	if(addr <= RAM_END){												// RAM領域 (00000-00FFFh)
		memcpy((uchar*)addr, wbuf, len);
	}
	else if(addr >= DATAROM_START && addr <= DATAROM_END){				// DataROM領域 (04100-048FFh)
		reram_write((uchar*)addr, wbuf, len);
	}
}
#endif

/*======================================================================================
| Declaration :	cmd_get_cpu
| Description : CPU ROM/RAM 読込み
| Parameters  :	uchar *pdat -> 受信データ
| Returns	  : n/a
+======================================================================================*/
#if 0
void cmd_get_cpu(uchar *pdat)
{
	ushort addr;
	uchar len, rbuf[64], *p, i;
	
	p = rbuf;
	*p++ = *pdat++;														// コマンドコード1
	*p++ = *pdat++;														// コマンドコード2
	
	addr = asc_hex(*pdat++);											// アドレスデコード
	addr <<= 4;
	addr |= asc_hex(*pdat++);
	addr <<= 4;
	addr |= asc_hex(*pdat++);
	addr <<= 4;
	addr |= asc_hex(*pdat++);
	
	len = asc_hex(*pdat++);												// データ長デコード
	len <<= 4;
	len |= asc_hex(*pdat++);
	
	for(i=0; i<len; i++){												// 読込みデータエンコード
		*p++ = hex_asc(*(uchar*)addr >> 4);
		*p++ = hex_asc(*(uchar*)addr);
		++addr;
	}
	
	if(i2c_status._recv){												// I2C受信 ?
		i2c_send_start(rbuf, (len * 2) + 2);
	}
}
#endif


/*==============================================================================================
| +++ SEND +++
+==============================================================================================*/
/*======================================================================================
| Declaration :	cmd_send
| Description : コマンド送信
| Parameters  :	n/a
| Returns	  :	n/a
+======================================================================================*/
void cmd_send(void)
{
	if(cmd_status & SEND_REQ){												// 送信要求あり？
		cmd_status &= ~SEND_REQ;
		sc0uart_send_store(resp_cmd);										// 送信データの格納
	}
	sc0uart_send();															// DEBUG(UART1) 送信
}



