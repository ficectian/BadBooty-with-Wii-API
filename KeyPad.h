//=======================================================================================
//
//  [ KeyPad ] Wii sample program
//  Wii コントローラーサンプルプログラム
//
//=======================================================================================

//-------------------------------------------------------------
//  To show KPAD callback status
//-------------------------------------------------------------
typedef struct MyCallbackStatus {
    u32 count;
    s32 reason;
} MyCallbackStatus;

#define MY_CALLBACK_MAX         2
#define MY_START_COUNTS_MAX     4
#define MY_START_COUNT_DEFAULT  400

extern s32              MyCallbackLatestIdx[MY_CALLBACK_MAX];
extern MyCallbackStatus MyCallbackStatusBuf[MY_CALLBACK_MAX][MY_START_COUNTS_MAX];

extern vu32 MySamplingCount;


#define KPAD_BUF_SIZE   16

extern  KPADStatus kpads[ WPAD_MAX_CONTROLLERS ][ KPAD_BUF_SIZE * 2 ];              // キーデータバッファ
extern  s32        kpad_reads[ WPAD_MAX_CONTROLLERS ];                              // キーデータ数


//-------------------------------------------------------------
//  関数宣言
//-------------------------------------------------------------
extern  void init_KPAD( void );


extern void	init_controller( void );
extern void	work_controller( void );
extern void draw_controller( void );

extern  void dpd_callback( s32 chan, s32 reason );
extern  void mpls_callback( s32 chan, s32 reason );
extern  void sampling_callback( s32 chan );

