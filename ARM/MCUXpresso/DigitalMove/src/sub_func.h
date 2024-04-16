/*==============================================================================================
| Project	: Digital Move
|
| File		: sub_func.h
|
| Purpose	: セットアップ
|
| Toolchain	: H8S, H8/300H V6.1.2.0
|
| History	: Version	Date		Content											Author
|			  v1.00		2013/10/30	First Edition									N.Ishikawa
+==============================================================================================*/
#ifndef SUB_FUNC_H_
#define SUB_FUNC_H_


/*===============================================================================================
 | External Function
 +==============================================================================================*/
uchar asc_hex(uchar src_data);
uchar hex_asc(uchar cnv_dat);
ulong asc_dec(uchar *dat_ptr, uchar len);
void dec_asc(uchar *dat_ptr, ulong num, uchar len);
uchar bit_reverse(uchar src_data);
uchar bit_on_count(uchar dat);
uchar bit_on_check(ulong dat, uchar num);
void ftoa(char *string, double fdat, int figure);



#endif /* SUB_FUNC_H_ */

