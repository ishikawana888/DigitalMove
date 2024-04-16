/*==============================================================================================
| Project	: Cal.NS30
|
| File		: sc0uart.h
|
| Purpose	: UART通信I/F (PC)
|
| Compiler	: ARM - 
|
| History	: Version	Date		Content											Author
|			  V*.**		2018/--/--	First Edition									N.Ishikawa
+==============================================================================================*/
#ifndef SC0UART_H_
#define SC0UART_H_

typedef	unsigned char	uchar;
typedef	unsigned int 	uint;
typedef	unsigned short	ushort;
typedef	unsigned long	ulong;


/*==============================================================================================
| CPU Register
+==============================================================================================*/
// UART (RxD0/TxD0)
#if 0
#define		U0_BRR_R				SCI0.BRR							// Bit Rate Register
#define		U0_TIE_r				SCI0.SCR.BIT.TIE					// Transmit Intrrupt Enable
#define		U0_TDR_R				SCI0.TDR							// Transmit Data Register
#define		U0_SSR_R				SCI0.SSR.BYTE						// Serial Status Register
#define		U0_TDRE_r				SCI0.SSR.BIT.TDRE					// Transmit Data Register Empty (1:Empty)
#define		U0_RDR_R				SCI0.RDR							// Receive Data Register
#define		U0_CKS_r				SCI0.SMR.BIT.CKS					// Clock Select (0:f,1:f/4,f/16,f/64)
#endif

/*==============================================================================================
| Define
+==============================================================================================*/
// Control Code
#ifndef STX
#define		SOH	    				0x01								// SOH
#define		STX	    				0x02								// STX
#define		ETX	    				0x03								// ETX
#define		EOT	    				0x04								// EOT
#define		ENQ						0x05								// ENQ
#define		ACKK    				0x06								// ACK
#define		BS						0x08								// BS
#define		LF						0x0A								// LF
#define		CR						0x0D								// CR
#define		NAK	    				0x15								// NAK
#define		SYN						0x16								// SYN
#define		ESC						0x1B								// ESC
#endif

// Receive Phase
#define		WAITSOH					0									// Wait for SOH
#define		WAITSTX					1									// Wait for STX
#define		WAITETX					2									// Wait for ETX
#define		WAITBCC	    			3									// Wait for BCC

// Module Code
#define		U0_CMDTERM				0x00								// Termination of Command

// Handshake
#define		U0_HANDSHAKE			0x00								// Handshake (U0_RES_SEND/U0_RES_RECVとOR選択)
#define		U0_RES_SEND				0x01								// ACK/NAK応答送信 (0:なし, 1:あり)
#define		U0_RES_RECV				0x02								// ACK/NAK応答受信 (0:なし, 1:あり)

// Boudrate
#define		U0_BASECLK_SCLK			0									// BaseClock SCLK
#define		U0_BASECLK_HCLK			1									// BaseClock HCLK
#define		U0_BAUDRATE_110			0									// Baudrate 110bps
#define		U0_BAUDRATE_150			1									// Baudrate 150bps
#define		U0_BAUDRATE_300			2									// Baudrate 300bps
#define		U0_BAUDRATE_600			3									// Baudrate 600bps
#define		U0_BAUDRATE_1200		4									// Baudrate 1200bps

// Communication Status
#define		U0_RECV					0x01								// データ受信通知

// Module Buffer Size
#define		U0_RECVQUE_SIZE			128									// 受信キューサイズ
#define		U0_SENDQUE_SIZE			128									// 送信キューサイズ
#define		U0_RECVCMD_SIZE			64									// 受信コマンドサイズ
#define		U0_SENDCMD_SIZE			64									// 送信コマンドサイズ
#define		U0_SENDDAT_SIZE			(U0_SENDCMD_SIZE + 4)				// 送信データ最大バイト数 (CMD + (STX+ETX+BCC+NULL))

// Send
#define		U0_SEND_RETRY_MAX		2									// 再送最大回数

// Commnucation Timer
#define		U0_RECV_CONTINUE_TIME	6									// 受信処理継続期間 (x10ms)
#define		U0_SEND_CONTINUE_TIME	2									// 送信処理継続期間 (x10ms)
#define		U0_SEND_DISABLE_TIME	5									// 送信禁止期間     (x10ms)
#define		U0_SEND_ANSWAIT_TIME	4									// 応答送信待ち期間 (x1msec)

// Return Code
#define		R_RES_ENB				0x00								// 応答要求あり
#define		R_RES_DSB				0xff								// 応答要求なし
#define		R_SENDREQ_OK			0x00								// 送信要求OK
#define		R_SENDREQ_NG			0xff								// 送信要求NG


/*==============================================================================================
| Union
+==============================================================================================*/
typedef union {															// RS-232C Command
	uchar arry[U0_RECVCMD_SIZE];
	struct {															//	Type 1 -> Code + Parameter
		uchar code;
		uchar param[U0_RECVCMD_SIZE - 1];
	} t1;
//	struct {															//	Type 2 -> "R/W" + Code + Code + Parameter
//		uchar rw;
//		uchar code1;
//		uchar code2;
//		uchar param[U0_RECVCMD_SIZE - 3];
//	} st;
} U0_CMD;


/*==============================================================================================
| Structure
+==============================================================================================*/
typedef struct {														// RS-232C Boudrate
	uchar duty;															//	デューティ		(0:1:1, 1:1:N)
	uchar base;															//	ベースクロック	(0:HCLK, 1:SCLK)
	uchar clock;														//	カウントクロック
	uchar compare;														//	コンペアレジスタ
} U0_BAUDRATE;

typedef struct {
	const U0_BAUDRATE *baud_t;
} U0_BAUDRATE_TBL;

typedef struct {														// RS-232C Recv Status
	uchar _err	:1;														//	b7		: Recv Error
	uchar _rsv	:6;														//	b6-1	: n/a
	uchar _recv	:1;														//	b0		: Receive Arrival
} U0_RECV_STS;

typedef struct {														// RS-232C Send Status
	uchar _err	:1;														//	b7		: Send Error
	uchar _rsv	:6;														//	b6-1	: n/a
	uchar _send	:1;														//	b0		: Send Busy (Command or ACK/NAK)
} U0_SEND_STS;


/*==============================================================================================
| External Variable
+==============================================================================================*/
extern U0_CMD u0_recv_cmd;												// 受信コマンド
extern U0_RECV_STS u0_recv_sts;											// 受信ステータス
extern U0_SEND_STS u0_send_sts;											// 送信ステータス


/*==============================================================================================
| External Function
+==============================================================================================*/
// Initialize
void init_sc0uart(void);												// 通信の初期化
void sc0uart_baudrate(uchar rate, uchar sysclk);						// ボーレート設定

// Interrupt
void sc0uart_recverr_int(void);											// 受信エラー割り込み処理
//void sc0uart_recv_int(void);											// 受信割り込み処理
//void sc0uart_send_int(void);											// 送信割り込み処理
void sc0uart_send_char(uchar send_reg);									// 1キャラクタ送信

// Receive
void sc0uart_recv(void);												// 受信処理

// Send
void sc0uart_send(void);												// 送信処理
void sc0uart_send_store(uchar *cptr);									// 送信データの格納

// Timer
void sc0uart_timer_10ms(void);											// Interval Timer 10ms (16bit Timer #2 Interrupt)
void sc0uart_timer_1ms(void);											// Interval Timer 1ms  (16bit Timer #1 Interrupt)


#endif /* SC0UART_H_ */

