/*==============================================================================================
| Project	: Digital Move
|
| File		: version.c
|
| Purpose	: バージョン情報
|
| Compiler	: ARM - 
|
| History	: Information is follows:
|
+==============================================================================================*/
#include "version.h"

//#pragma _section C=Version
/*==============================================================================================
| Calibre Information
+==============================================================================================*/
#if (CALSELECT & CALNS30)													// NS30 ?
	const unsigned char cal_name[] = "NS30";								// Max 4 byte
#elif (CALSELECT & CAL0100)													// 0100 ?
	const unsigned char cal_name[] = "0100";								// Max 4 byte
#elif (CALSELECT & CALH296)													// H296 ?
	const unsigned char cal_name[] = "H296";								// Max 4 byte
#elif (CALSELECT & CALH800)													// H800/H874 ?
	const unsigned char cal_name[] = "H800";								// Max 4 byte
#elif (CALSELECT & CALF150)													// F150 ?
	const unsigned char cal_name[] = "F150";								// Max 4 byte
#else																		// General ?
	const unsigned char cal_name[] = "GNRL";								// Max 4 byte
#endif

/*==============================================================================================
| Firmware Version
+==============================================================================================*/
const unsigned char firm_ver[] = "1.02 ";								// Max 5 byte

/*==============================================================================================
| Firmware Special Note
+==============================================================================================*/
//const unsigned char firm_spnote[] = "";								// Max 5 byte

/*==============================================================================================
| History Firmware Updates
+==============================================================================================*/
/*==============================================================================================
| Version	Content													Date		Name
|-----------------------------------------------------------------------------------------------
| 1.00		Fist Edition (Reverse engineer)							2020.10.15	N.Ishikawa
| 1.01      Support for H800/H874                                   2022.05.15  N.Ishikawa
| 1.02      Support for F150                                       	2023.06.15  N.Ishikawa
|
|-----------------------------------------------------------------------------------------------
| Base Version History
|	QC3148　2014年度　ARM用　プログラム
|	ver3.00 2018-05-15 saitohhi QC_f900_dummyのver3.0からコピー　NS30用に改造
|	ver3.02 2018-06-26 saitohhi LED点灯時待ち時間の調整　0→10us 
|	NS30では針検中　他のモータの動作が無い　あるいは　あっても10usでは1発分
|		と判断し、割り込みハンドラで10us待たせる。
|	ver3.2 2018-11-06 saitohhi
|		Bモータ運針時にAモータが12Hzの0位置にいる場合　LED3_Pを点灯。その他で非点灯。
|	ver3.3 2018-11-07 saitohhi
|		ずれをmotprm[2].cntに表示
|	ver3.4　2018-11-07　saitohhi
|		ずれ量を,motprm[2].cntに表示　LED3_P点灯は廃止
|
+==============================================================================================*/

/*==============================================================================================
| チェックサム調整用データ
+==============================================================================================*/
//volatile unsigned char checksum_zero_adjust = 0xEA;


//#pragma _section C=Copyright
/*==============================================================================================
| Copyright Information
+==============================================================================================*/
//const unsigned char copy_right[] = "Copyright(C) 2019 CITIZEN WATCH Co.,Ltd.";


//#pragma _section C


