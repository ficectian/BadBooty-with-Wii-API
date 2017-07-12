//=======================================================================================
//
//  [ KeyPad ] Wii sample program
//  Wii コントローラーサンプルプログラム
//
//=======================================================================================

#include <revolution.h>
#include <math.h>
#include <stdio.h>
#include <revolution/wpad.h>     // WPAD library
#include <revolution/kpad.h>     // KPAD library
#include <revolution/mem.h> 

#include "MEMAllocator.h"
#include "KeyPad.h"


//-------------------------------------------------------------
//  調整項目
//-------------------------------------------------------------
static f32 pointing_scale   = 200.0f;  // 画面ポインティングスケール

static f32 obj_interval     = 0.20f;   // ＴＶ側の発光点設置間隔 (メートル)

static f32 pos_play_radius  = 0.00f;   // pos 反応具合設定
static f32 pos_sensitivity  = 1.00f;
static f32 pos_play_mode    = (f32)KPAD_PLAY_MODE_LOOSE;

static f32 hori_play_radius = 0.00f;   // horizon 反応具合設定
static f32 hori_sensitivity = 1.00f;
static f32 hori_play_mode   = (f32)KPAD_PLAY_MODE_LOOSE;

static f32 dist_play_radius = 0.00f;   // dist 反応具合設定
static f32 dist_sensitivity = 1.00f;
static f32 dist_play_mode   = KPAD_PLAY_MODE_LOOSE;

static f32 acc_play_radius  = 0.00f;   // acc 反応具合設定
static f32 acc_sensitivity  = 1.00f;
static f32 acc_play_mode    = KPAD_PLAY_MODE_LOOSE;

static f32 repeat_delay_sec = 0.75f;   // キーリピート設定
static f32 repeat_pulse_sec = 0.25f;



#define EDIT_DATA_MAX 16 // 編集データ数



typedef struct {
    f32 *vp;   // 変数へのポインタ
    f32 min;   // 変数の最小値
    f32 max;   // 変数の最大値
    u8  y;     // Ｙカーソル位置
    u8  x[3];  // それぞれの桁のＸカーソル位置
    f32 v[3];  // それぞれの桁での数値変化量
} EditData;

static EditData  edit_data[ EDIT_DATA_MAX ] = {
    { &pointing_scale,   0.0f,900.0f,  3, {10,11,12}, {100.0f, 10.0f, 1.0f} },
    { &obj_interval,     0.00f,9.00f,  5, { 9,11,12}, {1.00f, 0.10f, 0.01f} },
    { &pos_play_radius,  0.00f,9.00f,  8, { 4, 6, 7}, {1.00f, 0.10f, 0.01f} },
    { &pos_sensitivity,  0.00f,1.00f,  8, { 9,11,12}, {1.00f, 0.10f, 0.01f} },
    { &pos_play_mode,    0.00f,1.00f,  8, {14,14,14}, {1.00f, 1.00f, 1.00f} },
    { &hori_play_radius, 0.00f,9.00f,  9, { 4, 6, 7}, {1.00f, 0.10f, 0.01f} },
    { &hori_sensitivity, 0.00f,1.00f,  9, { 9,11,12}, {1.00f, 0.10f, 0.01f} },
    { &hori_play_mode,   0.00f,1.00f,  9, {14,14,14}, {1.00f, 1.00f, 1.00f} },
    { &dist_play_radius, 0.00f,9.00f, 10, { 4, 6, 7}, {1.00f, 0.10f, 0.01f} },
    { &dist_sensitivity, 0.00f,1.00f, 10, { 9,11,12}, {1.00f, 0.10f, 0.01f} },
    { &dist_play_mode,   0.00f,1.00f, 10, {14,14,14}, {1.00f, 1.00f, 1.00f} },
    { &acc_play_radius,  0.00f,9.00f, 11, { 4, 6, 7}, {1.00f, 0.10f, 0.01f} },
    { &acc_sensitivity,  0.00f,1.00f, 11, { 9,11,12}, {1.00f, 0.10f, 0.01f} },
    { &acc_play_mode,    0.00f,1.00f, 11, {14,14,14}, {1.00f, 1.00f, 1.00f} },
    { &repeat_delay_sec, 0.00f,9.00f, 14, { 4, 6, 7}, {1.00f, 0.10f, 0.01f} },
    { &repeat_pulse_sec, 0.00f,9.00f, 14, { 9,11,12}, {1.00f, 0.10f, 0.01f} },
};


//-------------------------------------------------------------
//  変数
//-------------------------------------------------------------
s32              MyCallbackLatestIdx[ MY_CALLBACK_MAX ] = { -1, -1 };
MyCallbackStatus MyCallbackStatusBuf[ MY_CALLBACK_MAX ][ MY_START_COUNTS_MAX ];
vu32             MySamplingCount;


//-------------------------------------------------------------
//  キー入力情報
//-------------------------------------------------------------
KPADStatus kpads[ WPAD_MAX_CONTROLLERS ][ KPAD_BUF_SIZE * 2 ];              // キーデータバッファ
s32        kpad_reads[ WPAD_MAX_CONTROLLERS ];                              // キーデータ数
s32        kpad_err;                                                        // キーデータエラー



//-------------------------------------------------------------
//  コントローラ初期化
//-------------------------------------------------------------
void init_KPAD( void )
{
    // パッド関連の初期設定
    WPADRegisterAllocator( alloc32, free32 );           // ワークメモリ取得・解放用のアロケータ関数登録

    KPADInit();                                         // コントローラ初期化
    KPADSetControlDpdCallback ( 0, dpd_callback );      // DPD              コールバック設定
    KPADSetControlMplsCallback( 0, mpls_callback );     // モーションプラス コールバック設定
    KPADSetSamplingCallback   ( 0, sampling_callback ); // サンプリング     コールバック設定

    // アプリケーションの初期設定
    init_controller();
}


//-------------------------------------------------------------
//  KPAD callback
//-------------------------------------------------------------
static void work_callback( int idx, s32 reason )
{
    u32 i;
    u32 smallest = MY_START_COUNT_DEFAULT;
    u32 smallestIdx = 0;

    for( i=0; i<MY_START_COUNTS_MAX; i++ )
    {
        if( MyCallbackStatusBuf[ idx ][ i ].count == 0 )
        {
            break;
        }
        else
        {
            if( smallest > MyCallbackStatusBuf[ idx ][ i ].count )
            {
                smallest = MyCallbackStatusBuf[ idx ][ i ].count;
                smallestIdx = i;
            }
        }
    }

    if ( i == MY_START_COUNTS_MAX )
    {
        i = smallestIdx;
    }

    MyCallbackStatusBuf[ idx ][ i ].count = MY_START_COUNT_DEFAULT;
    MyCallbackStatusBuf[ idx ][ i ].reason = reason;
    MyCallbackLatestIdx[ idx ] = (s32)i;
}

void dpd_callback( s32 chan, s32 reason )
{
#pragma unused(chan)
    work_callback( 0, reason );
}

void mpls_callback( s32 chan, s32 reason )
{
#pragma unused(chan)
    work_callback( 1, reason );
}

void sampling_callback( s32 chan )
{
#pragma unused(chan)
    MySamplingCount++;
}


//-------------------------------------------------------------
//  初期化関数
//-------------------------------------------------------------
void init_controller( void )
{
    s32 i;

    // コントローラ初期設定
    KPADReset();

    for( i=0; i<WPAD_MAX_CONTROLLERS; i++ )
    {
        KPADSetPosParam ( i, pos_play_radius,  pos_sensitivity  );
        KPADSetHoriParam( i, hori_play_radius, hori_sensitivity );
        KPADSetDistParam( i, dist_play_radius, dist_sensitivity );
        KPADSetAccParam ( i, acc_play_radius,  acc_sensitivity  );
        KPADSetBtnRepeat( i, repeat_delay_sec, repeat_pulse_sec );
    }
}


//-------------------------------------------------------------
//  コントローラー処理関数
//-------------------------------------------------------------
void work_controller( void )
{
    u8  btn  = KPAD_BUTTON_PROC_MODE_LOOSE;

    s32 i;

    int init = 0;

    // 情報取得
    for( i=0; i<WPAD_MAX_CONTROLLERS; i++ )
    {
        // バッファにサンプリングされたデータが新しい順に格納されている
        kpad_reads[ i ] = KPADReadEx( i, &kpads[ i ][ 0 ], (u32)( KPAD_BUF_SIZE * ( init + 1 ) ), &kpad_err );
        if( kpad_err == KPAD_READ_ERR_NO_DATA )
        {
            kpad_reads[ i ] = -1;
        }
    }


}
