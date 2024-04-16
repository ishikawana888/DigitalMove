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
#ifndef VERSION_H_
#define VERSION_H_

/*==============================================================================================
| Define
+==============================================================================================*/
#define	CAL_NAME_SIZE			4									// キャリバー名情報サイズ
#define	VER_INFO_SIZE			5									// ファームウェアバージョン情報サイズ
//#define	VER_NOTE_SIZE			5									// ファームウェア注記事項サイズ


/*==============================================================================================
| Software Version Information
+==============================================================================================*/
extern const unsigned char cal_name[];								// Calibre Name
extern const unsigned char firm_ver[];								// Firmware Version
//extern const unsigned char firm_spnote[];							// Firmware Special Note


#endif /* VERSION_H_ */

