/*==============================================================================================
| Project	: Digital Move
|
| File		: sub_func.c
|
| Purpose	: 汎用機能
|
| Compiler	: ARM -
|
| History	: Version	Date		Content											Author
|			  V*.**		2018/--/--	First Edition									N.Ishikawa
+==============================================================================================*/
#include <stdio.h>
#include "common.h"


/*===============================================================================================
 | Prototype
 +==============================================================================================*/
uchar asc_hex(uchar src_data);
uchar hex_asc(uchar cnv_dat);
ulong asc_dec(uchar *dat_ptr, uchar len);
void dec_asc(uchar *dat_ptr, ulong num, uchar len);
uchar bit_reverse(uchar src_data);
uchar bit_on_count(uchar dat);
uchar bit_on_check(ulong dat, uchar num);
void ftoa(char *string, double fdat, int figure);


/*======================================================================================
| Declaration :	asc_hex
| Description :	ASCII Code -> HEX
| Parameters  :	uchar src_data
| Returns	  :	OK:HEX/NG:0xff
+======================================================================================*/
uchar asc_hex(uchar src_data)
{
	uchar ret;
	
	ret = 0xFF;
	if((src_data >= '0') && (src_data <= '9')){
		ret = src_data - '0';
	}else{
		src_data &= ~BIT5;											// 'a'～'f' -> 'A'～'F'
		if((src_data >= 'A') && (src_data <= 'F')){
			ret = src_data - ('0' + 7);
		}
	}
	return ret;
}

/*======================================================================================
| Declaration :	hex_asc
| Description :	HEX -> ASCII Code
| Parameters  :	uchar cnv_data (Lower 4bit)
| Returns	  :	ASCII Data
+======================================================================================*/
uchar hex_asc(uchar cnv_dat)
{
	cnv_dat &= 0x0F;
	cnv_dat += 0x30;
	if(cnv_dat > '9'){												// 0Ah ～ 0Fh
		cnv_dat += 7;
	}
	return cnv_dat;
}

/*======================================================================================
| Declaration :	asc_dec
| Description :	ASCII Code -> DEC
| Parameters  :	uchar *dat_ptr
|				uchar len
| Returns	  :	DEC Data
+======================================================================================*/
ulong asc_dec(uchar *dat_ptr, uchar len)
{
	ulong ret = 0, keta = 1;
	
	dat_ptr += (len - 1);											// 1位のﾃﾞｰﾀﾎﾟｲﾝﾀ取得
	while(len--){													// ﾃﾞｰﾀ計算
		if((*dat_ptr >= '0') && (*dat_ptr <= '9')){
			ret += (*dat_ptr - 0x30) * keta;
		}
		keta = (keta << 3) + (keta << 1);							// keta * 10
		--dat_ptr;
	}
	return ret;
}

/*======================================================================================
| Declaration :	dec_asc
| Description :	DEC -> ASCII Code
| Parameters  :	uchar *dat_ptr
|				ulong num
|				uchar len
| Returns     :	n/a
+======================================================================================*/
void dec_asc(uchar *dat_ptr, ulong num, uchar len)
{
	uchar i, j;
	ulong keta;
	
	for(i = 1; i <= len; ++i){
		keta = 1;
		for(j = 0; j < len - i; ++j){								// 位の基数を計算する
			keta = (keta << 3) + (keta << 1);						// keta * 10
		}
		for(j = 0; j < 10; j++){									// 位の値を求める
			if(num < keta){
				break;
			}
			num -= keta;
		}
		*dat_ptr++ = j + 0x30;
	}
}

/*======================================================================================
| Declaration :	bit_reverse
| Description :	ビット位置逆転  (d0,d1,...d7 -> d7,d6...d0)
| Parameters  :	uchar src_data > ソースデータ
| Returns:		uchar rev_data > 逆転データ
+======================================================================================*/
uchar bit_reverse(uchar src_data)
{
	uchar i, rev_data = 0x00;
	
	for(i=0; i<8; ++i){
		rev_data <<= 1;
		rev_data |= src_data & BIT0;
		src_data >>= 1;
	}
	return rev_data;
}

/*======================================================================================
| Declaration :	bit_on_count
| Description : Bit count of ON
| Parameters  :	uchar dat > 検査データ
| Returns	  : ONビット数
+======================================================================================*/
uchar bit_on_count(uchar dat)
{
	uchar i, num;
	
	num = 0;
	for (i=0; i<8; ++i){
		if((dat >> i) & BIT0){
			++num;													// Count bit ON
		}
	}
	return num;
}

/*======================================================================================
| Declaration :	bit_on_check
| Description : ビットON位置の検査
| Parameters  :	ulong dat > 検査データ
|				uchar num > 検査ビット数
| Returns	  : ビットONの位置 (0 org)
|				(ビットONなし 0xFF)
+======================================================================================*/
uchar bit_on_check(ulong dat, uchar num)
{
	uchar i;
	
	for(i = 0; i < num; ++i){
		if(dat & BIT0){						/* ﾋﾞｯﾄOn検査 */
			return i;
		}
		dat >>= 1;
	}
	return 0xFF;
}


/*======================================================================================
| Declaration :	ftoa
| Description : 浮動小数点を文字列に変換
| Parameters  :	char *string > 文字列ポインタ
|				double fdat	  > 変換する小数
|				int figure	  > 小数の桁数
| Returns	  : n/a
+======================================================================================*/
void ftoa(char *string, double fdat, int figure)
{
	sprintf(string, "%.*f", figure, fdat);		// stdio.hを使わずできるか検討 ###
}



