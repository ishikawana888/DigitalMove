/*==============================================================================================
| Project	: Digital Move
|
| File		: sc0uart.c
|
| Purpose	: UART通信I/F (PC)
|
| Compiler	: ARM - 
|
| History	: Version	Date		Content											Author
|			  V*.**		2018/--/--	First Edition									N.Ishikawa
+==============================================================================================*/
#include "sc0uart.h"
#include "mbed.h"
#include "Serial.h"

Serial sc0uart(USBTX, USBRX);

/*==============================================================================================
| External Variable
+==============================================================================================*/
U0_CMD u0_recv_cmd;													// 受信コマンド
U0_RECV_STS u0_recv_sts;											// 受信ステータス
U0_SEND_STS u0_send_sts;											// 送信ステータス


/*==============================================================================================
| Internal Variable
+==============================================================================================*/
// +++++ Buffer +++++
U0_CMD u0_recv_buf;													// 受信データバッファ
uchar u0_send_buf[U0_SENDDAT_SIZE];									// 送信データバッファ
uchar u0_recv_que[U0_RECVQUE_SIZE];									// 受信キュー
uchar u0_send_que[U0_SENDQUE_SIZE];									// 送信キュー

// +++++ Receive +++++
ushort u0_recv_quesetp;												// 受信キュー書込みポインタ
ushort u0_recv_quegetp;												// 受信キュー読出しポインタ
ushort u0_recv_quecnt;												// 受信キューデータ数
ushort u0_recv_ptr;													// 受信データバッファポインタ
uchar  u0_recv_phase;												// 受信フェーズ
uchar  u0_recv_reg;													// 受信レジスタデータ
uchar  u0_recv_bcc;													// 受信BCCデータ
uchar  u0_recv_dummy;												// 受信ダミー

// +++++ Send +++++
ushort u0_send_quesetp;												// 送信キュー書込みポインタ
ushort u0_send_quegetp;												// 送信キュー読出しポインタ
ushort u0_send_quecnt;												// 送信キューデータ数
uchar  u0_send_reg;													// 送信レジスタデータ
uchar  u0_send_len;													// 送信データ長
uchar  u0_send_cnt;													// 送信カウンタ
uchar  u0_send_ret;													// 応答送信コード (ACK or NAK)
uchar  u0_send_retry;												// 再送カウンタ

// +++++ Timer +++++
uchar  u0_recv_tmr;													// 受信タイマカウンタ
uchar  u0_send_tmr;													// 送信タイマカウンタ
uchar  u0_send_ans_tmr;												// 応答送信待ちタイマカウンタ


/*==============================================================================================
| Prototype
+==============================================================================================*/
// +++ Initialize +++
void init_sc0uart(void);											// 通信の初期化
void sc0uart_baudrate(uchar rate, uchar sysclk);					// ボーレート設定

// +++ Receive Interrupt Function +++
void sc0uart_recv_int(void);										// 受信割り込み処理
//void sc0uart_recv_int(MODSERIAL_IRQ_INFO *q);						// 受信割り込み処理
//void sc0uart_recverr_int(void);									// 受信エラー割り込み処理
void sc0uart_wait_stx(void);										// STX 待ち処理
void sc0uart_wait_etx(void);										// ETX 待ち処理
void sc0uart_wait_bcc(void);										// BCC 待ち処理
uchar sc0uart_resp_check(void);										// 応答要求の有無確認
void sc0uart_recv_clear(void);										// 受信クリア

// +++ Send Interrupt Function +++
void sc0uart_send_int(void);										// 送信割り込み処理
//void sc0uart_send_int(MODSERIAL_IRQ_INFO *q);						// 送信割り込み処理
void sc0uart_send_retry(void);										// 再送処理
void sc0uart_send_start(void);										// 送信開始
void sc0uart_send_clear(void);										// 送信クリア
void sc0uart_send_answer(void);										// 応答送信 ACK/NAK
void sc0uart_send_char(uchar send_reg);								// 1キャラクタ送信

// +++ Receive +++
void sc0uart_recv(void);											// 受信処理

// +++ Send +++
void sc0uart_send(void);											// 送信処理
void sc0uart_send_packet(void);										// 送信パケット生成

// +++ Receive Queue +++
void sc0uart_recv_store(uchar *cptr);								// 受信データの格納
void sc0uart_recv_setque(uchar data);								// 受信データをキューに書込み
uchar sc0uart_recv_getque(void);									// 受信データをキューから読込み
void sc0uart_recv_term_search(void);								// 受信データ終端検索
void sc0uart_recv_error(void);										// 受信エラー

// +++ Send Queue +++
void sc0uart_send_store(uchar *cptr);								// 送信データの格納
void sc0uart_send_setque(uchar data);								// 送信データをキューに書込み
uchar sc0uart_send_getque(void);									// 送信データをキューから読込み
void sc0uart_send_term_search(void);								// 送信データ終端検索
void sc0uart_send_error(void);										// 送信エラー

// +++ Timer +++
void sc0uart_timer_10ms(void);										// Interval Timer 10ms (16bit Timer #2 Interrupt)
void sc0uart_timer_1ms(void);										// Interval Timer 1ms  (16bit Timer #1 Interrupt)


/*==============================================================================================
| +++ Table +++
+==============================================================================================*/


/*==============================================================================================
| +++ Initialize +++
+==============================================================================================*/
/*======================================================================================
| Declaration :	init_sc0uart
| Description : 通信の初期化
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void init_sc0uart(void)
{
	// Variable
	u0_recv_phase = WAITSTX;										// UART Receive phase
	u0_recv_phase = WAITETX;										// UART Receive phase
	
	// Register
	sc0uart.baud(115200);											// ボーレート
	sc0uart.attach(&sc0uart_recv_int, Serial::RxIrq);				// 受信割込み
}

/*======================================================================================
| Declaration :	sc0uart_baudrate
| Description : ボーレート設定
| Parameters  :	uchar rate : ボーレート
| Returns	  : n/a
+======================================================================================*/
void sc0uart_baudrate(uchar rate, uchar sysclk)
{
}

/*======================================================================================
| Declaration :	sc0uart_start
| Description : UART通信開始
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_start(void)
{
}

/*======================================================================================
| Declaration :	sc0uart_stop
| Description : UART通信終了
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_stop(void)
{
}


/*==============================================================================================
| +++ Receive Interrupt Function +++
+==============================================================================================*/
/*======================================================================================
| Declaration :	sc0uart_recv_int
| Description : 受信割り込み処理
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
//void sc0uart_recv_int(MODSERIAL_IRQ_INFO *q)
void sc0uart_recv_int(void)
{
//	if(sc0uart.readable()){
		u0_recv_reg = sc0uart.getc();								// 受信データ取得
		switch(u0_recv_phase){
		case WAITSTX:												// Wait for STX
			sc0uart_wait_stx();
			break;
		case WAITETX:												// Wait for EXT
			sc0uart_wait_etx();
			break;
		case WAITBCC:												// Wait for BCC
			sc0uart_wait_bcc();
			break;
		}
//	}
}


#if 0
/*======================================================================================
| Declaration :	sc0uart_recverr_int
| Description : 受信エラー割り込み処理
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_recverr_int(void)
{
	register sts;
	
	sts = U0_SSR_R;
	U0_SSR_R = sts & 0x87;											// Reset Error
	sc0uart_recv_clear();											// 受信クリア
}
#endif

/*======================================================================================
| Declaration :	sc0uart_wait_stx
| Description : STX 待ち処理
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_wait_stx(void)
{
	switch(u0_recv_reg){
	case STX:														// STX Received
		u0_recv_ptr = 0;
		u0_recv_bcc = 0;
		u0_recv_phase = WAITETX;									// To wait for ETX
		break;
	case ETX:														// ETX Received
		sc0uart_recv_clear();
		break;
	default:
		break;
	}
}

/*======================================================================================
| Declaration :	sc0uart_wait_etx
| Description : ETX 待ち処理
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_wait_etx(void)
{
	u0_recv_bcc ^= u0_recv_reg;										// Math BCC
	switch(u0_recv_reg){
	case STX:														// STX Received
		sc0uart_wait_stx();
		break;
	case ETX:														// ETX Received
		if(u0_recv_ptr < U0_RECVCMD_SIZE){
			u0_recv_buf.arry[u0_recv_ptr] = U0_CMDTERM;
		}
		u0_recv_phase = WAITBCC;									// To wait for BCC
		break;
	default:
		if(u0_recv_ptr < U0_RECVCMD_SIZE){							// コマンドの長さがバッファ内
			u0_recv_buf.arry[u0_recv_ptr] = u0_recv_reg;			// 受信コマンドをバッファにストア
			++u0_recv_ptr;											// コマンドポインタをインクリメント
		}
		else{
			sc0uart_recv_clear();
		}
		break;
	}
}

/*======================================================================================
| Declaration :	sc0uart_wait_bcc
| Description : BCC 待ち処理
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_wait_bcc(void)
{
	uchar ret;
	
	ret = sc0uart_resp_check();										// 応答要求有無のチェック
	if(u0_recv_bcc == u0_recv_reg){									// BCC OK?
		if(ret == R_RES_ENB){										// 応答要求あり？
			u0_send_ret = ACKK;										// ACK 応答
			u0_send_ans_tmr = U0_SEND_ANSWAIT_TIME;					// 応答送信待ちタイマ起動
		}
		sc0uart_recv_store(&u0_recv_buf.arry[0]);
	}
	else{															// BCC Error?
		if(ret == R_RES_ENB){										// 応答要求あり？
			u0_send_ret = NAK;										// NAK 応答
			u0_send_ans_tmr = U0_SEND_ANSWAIT_TIME;					// 応答送信待ちタイマ起動
		}
	}
	sc0uart_recv_clear();
}

/*======================================================================================
| Declaration :	sc0uart_resp_check
| Description : RS-232C ACK/NAK Response Check
| Parameters  :	n/a
| Returns	  : R_RES_DSB : 応答要求なし
|				R_RES_ENB : 応答要求あり
+======================================================================================*/
uchar sc0uart_resp_check(void)
{
	if(U0_HANDSHAKE & U0_RES_SEND){									// ACK/NAK 応答送信あり？
		return R_RES_ENB;
	}
	else{															// ACK/NAK 応答送信なし？
		return R_RES_DSB;
	}
}

/*======================================================================================
| Declaration :	sc0uart_recv_clear
| Description : 受信クリア処理
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_recv_clear(void)
{
	u0_recv_ptr = 0;
	u0_recv_bcc = 0;
	u0_recv_tmr = 0;
	u0_recv_phase = WAITSTX;
}


/*==============================================================================================
| +++ Send Interrupt Function +++
+==============================================================================================*/
/*======================================================================================
| Declaration :	sc0uart_send_int
| Description : 送信割り込み処理
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_send_int(void)
{
	if(u0_send_cnt < u0_send_len){									// 送信データあり
		u0_send_reg = u0_send_buf[u0_send_cnt++];					// 送信データセット
		sc0uart.attach(&sc0uart_send_int, Serial::TxIrq);			// 送信バッファ空き割込み
		sc0uart.putc(u0_send_reg);									// データ送信
//		u0_send_tmr = U0_SEND_CONTINUE_TIME;						// 送信タイマ再起動
	}
	else{
		sc0uart.attach(NULL, Serial::TxIrq);						// 送信バッファ空き割込み
		sc0uart_send_clear();
	}
}

/*======================================================================================
| Declaration :	sc0uart_send_retry
| Description :	再送処理 (タイマ割り込み)
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_send_retry(void)
{
	if(++u0_send_retry <= U0_SEND_RETRY_MAX){						// 再送実行？
		sc0uart_send_start();
	}
	else{															// 再送終了？
		u0_send_sts._err = 1;										// 送信エラー
		sc0uart_send_clear();
	}
}

/*======================================================================================
| Declaration :	sc0uart_send_start
| Description : 送信開始 (タイマ割り込み)
| Parameters  :	n/a
| Returns     :	n/a
+======================================================================================*/
void sc0uart_send_start(void)
{
	u0_send_sts._send = 1;											// 送信中セット
	u0_send_sts._err = 0;											// 送信エラークリア
	u0_send_cnt = 0;												// 送信カウンタクリア
	
	u0_send_reg = u0_send_buf[u0_send_cnt++];						// 送信データセット
	sc0uart.attach(&sc0uart_send_int, Serial::TxIrq);				// 送信バッファ空き割込み
	sc0uart.putc(u0_send_reg);										// データ送信
}

/*======================================================================================
| Declaration :	sc0uart_send_clear
| Description : 送信クリア (送信割り込み/タイマ割り込み)
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_send_clear(void)
{
	u0_send_tmr = 0;												// 送信タイマ停止
	u0_send_sts._send = 0;											// 送信中クリア
	u0_send_retry = 0;												// 再送カウンタクリア
}

/*======================================================================================
| Declaration :	sc0uart_send_answer
| Description : 応答送信 ACK/NAK (タイマ割り込み)
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_send_answer(void)
{
	u0_send_reg = u0_send_ret;
	sc0uart.putc(u0_send_reg);										// データ送信
}

/*======================================================================================
| Declaration :	sc0uart_send_char
| Description : 1キャラクタ送信 (タイマ割り込み)
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_send_char(uchar send_reg)
{
	if(u0_send_sts._send){											// 送信中？
		return;
	}
	
	sc0uart.putc(u0_send_reg);										// データ送信
}


/*==============================================================================================
| +++ Receive +++
+==============================================================================================*/
/*======================================================================================
| Declaration :	sc0uart_recv
| Description : 受信データ取得
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_recv(void)
{
	ushort i;
	uchar *p;
	
	if(!u0_recv_quecnt){											// 受信データなし？
		return;
	}
	
	p = &u0_recv_cmd.arry[0];										// 受信データ取得
	i = 0;
	do {
		*p = sc0uart_recv_getque();									// 受信キューからデータ取得
		if(*p == U0_CMDTERM){										// コマンド終了？
			break;
		}
		++p;
	} while(++i < U0_RECVCMD_SIZE);
	
	// Status
	if(!(u0_recv_quecnt & 0x8000)){									// エラーなし？
		u0_recv_sts._recv = 1;										// UART0 受信通知
	}
	else{
		__disable_irq();											// 割込み禁止
		sc0uart_recv_error();
		__enable_irq();												// 割込み許可
	}
}


/*==============================================================================================
| +++ Send +++
+==============================================================================================*/
/*======================================================================================
| Declaration :	sc0uart_send
| Description : 送信処理
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_send(void)
{
	if(u0_send_sts._send){											// 送信中？
		return;
	}
	if(u0_send_ans_tmr){											// 応答送信待ち中？
		return;
	}
	if(!u0_send_quecnt){											// 送信データなし？
		return;
	}
	
	sc0uart_send_packet();											// 送信パケット生成
	
	__disable_irq();												// 割込み禁止
	u0_send_retry = 0;												// 再送カウンタクリア
	sc0uart_send_start();											// 送信開始
	__enable_irq();													// 割込み許可
}

/*======================================================================================
| Declaration :	sc0uart_send_packet
| Description : 送信パケット生成
| Parameters  :	n/a
| Returns     :	n/a
+======================================================================================*/
void sc0uart_send_packet(void)
{
	uchar *p, bcc, i;
	
	bcc = ETX;														// BCC 計算	(CMD~ETX)
	u0_send_len = 3;												// 送信データ長 (STX+ETX+BCC)
	p = &u0_send_buf[0];
	
	// Set to STX
	*p++ = STX;
	
	// Command
	i = 0;
	do {
		*p = sc0uart_send_getque();									// 送信キューからデータ取得
		if(*p == U0_CMDTERM){										// コマンド終了？
			break;
		}
		bcc ^= *p;													// BCC 計算
		++p;
		++u0_send_len;
	} while(++i < U0_SENDCMD_SIZE);
	
	// Set to ETX
	*p++ = ETX;
	
	// Set to BCC
	*p++ = bcc;
	
	// Set to Send Command Term
	*p = U0_CMDTERM;
	
	if(u0_send_quecnt & 0x8000){									// エラー？
		sc0uart_send_error();
	}
}


/*==============================================================================================
| +++ Receive Queue +++
+==============================================================================================*/
/*======================================================================================
| Declaration :	sc0uart_recv_store
| Description : 受信データ格納
| Parameters  :	uchar *cptr : 受信コマンドポインタ
| Returns	  : n/a
+======================================================================================*/
void sc0uart_recv_store(uchar *cptr)
{
	ushort i;
	
	i = 0;
	do {															// 受信コマンド格納
		if(*cptr == U0_CMDTERM){									// 受信コマンド終了？
			break;
		}
		sc0uart_recv_setque(*cptr++);								// 受信キューへデータセット
	} while(++i < U0_RECVCMD_SIZE);
	
	// Termination of receive queue
	sc0uart_recv_setque(U0_CMDTERM);								// コマンド終端セット
}

/*======================================================================================
| Declaration :	sc0uart_recv_setque
| Description : 受信キューへデータ格納
| Parameters  :	uchar data : 格納データ
| Returns	  : n/a
+======================================================================================*/
void sc0uart_recv_setque(uchar data)
{
	if(u0_recv_quecnt >= U0_RECVQUE_SIZE){							// 受信キュー FULL?
		sc0uart_recv_term_search();									// 最古のデータをスキップする
	}
	u0_recv_que[u0_recv_quesetp] = data;							// 受信キューへデータセット
	if(++u0_recv_quesetp >= U0_RECVQUE_SIZE){
		u0_recv_quesetp = 0;
	}
	++u0_recv_quecnt;
}

/*======================================================================================
| Declaration :	sc0uart_recv_getque
| Description : 受信キューからデータ取得
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
uchar sc0uart_recv_getque(void)
{
	uchar data;
	
	__disable_irq();												// 割込み禁止
	
	data = u0_recv_que[u0_recv_quegetp];							// 受信キューからデータ取得
	if(++u0_recv_quegetp >= U0_RECVQUE_SIZE){
		u0_recv_quegetp = 0;
	}
	--u0_recv_quecnt;												// 受信データ数デクリメント
	
	__enable_irq();													// 割込み許可
	
	return data;
}

/*======================================================================================
| Declaration :	sc0uart_recv_term_search
| Description : 受信データ終端検索
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_recv_term_search(void)
{
	// 次のコマンド終端まで読出しポインタを進める
	do {
		if(++u0_recv_quegetp >= U0_RECVQUE_SIZE){
			u0_recv_quegetp = 0;
		}
		if(--u0_recv_quecnt & 0x8000){								// Error?
			sc0uart_recv_error();
			return;
		}
	} while(u0_recv_que[u0_recv_quegetp]);
	
	// コマンド終端分の処理
	if(++u0_recv_quegetp >= U0_RECVQUE_SIZE){
		u0_recv_quegetp = 0;
	}
	--u0_recv_quecnt;
}

/*======================================================================================
| Declaration :	sc0uart_recv_error
| Description : 受信エラー処理
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_recv_error(void)
{
	u0_recv_quecnt = 0;
	u0_recv_quegetp = u0_recv_quesetp;
}


/*==============================================================================================
| +++ Send Queue +++
+==============================================================================================*/
/*======================================================================================
| Declaration :	sc0uart_send_store
| Description : 送信データ格納
| Parameters  :	uchar *cptr : 送信コマンドポインタ
| Returns	  : n/a
+======================================================================================*/
void sc0uart_send_store(uchar *cptr)
{
	ushort i;
	
	i = 0;
	do {															// 送信コマンド格納
		if(*cptr == U0_CMDTERM){									// 送信コマンド終了？
			break;
		}
		sc0uart_send_setque(*cptr++);								// 送信キューへデータセット
	} while(++i < U0_SENDCMD_SIZE);
	
	// Termination of send queue
	sc0uart_send_setque(U0_CMDTERM);								// コマンド終端セット
}

/*======================================================================================
| Declaration :	sc0uart_send_setque
| Description : 送信キューへデータ格納
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_send_setque(uchar data)
{
	if(u0_send_quecnt >= U0_SENDQUE_SIZE){							// 送信キュー FULL?
		sc0uart_send_term_search();									// 最古のデータをスキップする
	}
	u0_send_que[u0_send_quesetp] = data;							// 送信キューへデータセット
	if(++u0_send_quesetp >= U0_SENDQUE_SIZE){
		u0_send_quesetp = 0;
	}
	++u0_send_quecnt;
}

/*======================================================================================
| Declaration :	sc0uart_send_getque
| Description : 送信キューからデータ取得
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
uchar sc0uart_send_getque(void)
{
	uchar data;
	
	data = u0_send_que[u0_send_quegetp];							// 送信キューからデータ取得
	if(++u0_send_quegetp >= U0_SENDQUE_SIZE){
		u0_send_quegetp = 0;
	}
	--u0_send_quecnt;												// 送信データ数デクリメント
	
	return data;
}

/*======================================================================================
| Declaration :	sc0uart_send_term_search
| Description : 送信データ終端検索
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_send_term_search(void)
{
	// 次のコマンド終端まで読出しポインタを進める
	do {
		if(++u0_send_quegetp >= U0_SENDQUE_SIZE){
			u0_send_quegetp = 0;
		}
		if(--u0_send_quecnt & 0x8000){								// Error?
			sc0uart_send_error();
			return;
		}
	} while(u0_send_que[u0_send_quegetp]);
	
	// コマンド終端分の処理
	if(++u0_send_quegetp >= U0_SENDQUE_SIZE){
		u0_send_quegetp = 0;
	}
	--u0_send_quecnt;
}

/*======================================================================================
| Declaration :	sc0uart_send_error
| Description : 送信エラー処理
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_send_error(void)
{
	u0_send_quecnt = 0;
	u0_send_quegetp = u0_send_quesetp;
}


/*==============================================================================================
| +++ Timer +++
+==============================================================================================*/
/*======================================================================================
| Declaration :	sc0uart_timer_10ms
| Description : Interval Timer 10ms (16bit Timer #2 Interrupt)
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_timer_10ms(void)
{
	if(u0_recv_tmr){												// 受信処理継続中？
		if(!(--u0_recv_tmr)){										// タイムアウト？
			sc0uart_recv_clear();									// 受信処理中止 -> 受信クリアの実行
		}
	}
	if(u0_send_tmr){												// 送信処理継続中？
		if(!(--u0_send_tmr)){										// タイムアウト？
			sc0uart_send_retry();									// 再送処理
		}
	}
}

/*======================================================================================
| Declaration :	sc0uart_timer_1ms
| Description : Interval Timer 1ms (16bit Timer #1 Interrupt)
| Parameters  :	n/a
| Returns	  : n/a
+======================================================================================*/
void sc0uart_timer_1ms(void)
{
	if(u0_send_ans_tmr){											// 応答要求
		if(!(--u0_send_ans_tmr)){
			sc0uart_send_answer();									// 応答送信 ACK/NAK
		}
	}
}


