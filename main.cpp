//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		インクルードファイル
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include	<revolution.h>
#include	<demo.h>
#include<revolution/wpad.h>
#include<revolution/kpad.h>
#include "MEMAllocator.h"
#include "KeyPad.h"
#include "sound.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		定数定義
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define	SCREEN_WIDTH		(640)						//スクリーン幅
#define	SCREEN_HEIGHT	(480)						//スクリーン高さ
#define	LAND_WIDTH		(1024)						//スクリーン幅
#define	LAND_HEIGHT	(512)						//スクリーン高さ
#define DEFSHOTY		(5.0f)
#define ENEMYSHOOTSIZE		(6)
#define SHOOTGAP		(15)
#define INVINCIBLE_SIZE (90)
#define ENEMYNUM (3)
#define ENEMY_HEIGHT	(50)
#define	ENEMY_WIDTH	(70)
#define ENEMY0DEFY (SCREEN_HEIGHT / 2 - 150)
#define ENEMY1DEFY  (SCREEN_HEIGHT / 2)
#define ENEMY2DEFY (SCREEN_HEIGHT / 2 + 150)
#define ENEMYSHOOTMAX (255)
#define PLAYERSPEED (4)
#define HEATSIZE		(20.0f)
#define HEATGAP		(30.0f)

#define PLAYERTEX "Player.tpl"
//#define FONTTEX "sysfont.tpl"
#define SCRENTEX	"scren.tpl"
#define LANDPIXELTEX	"LandPixel.tpl"
#define FOOTINGTEX	"Footing.tpl"
#define	WINTEX	 "WIN.tpl"
#define	LOSETEX	 "LOSE.tpl"
#define	TITLETEX	 "title.tpl"
#define	TITLEINTTEX		"titleint.tpl"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		グローバル変数
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		グローバル変数
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class PlayerClass{
public:
	f32 X;	//年齢
	f32 Y;	//攻撃力
	int	nHp;	//体力
	u8 InvincibleState;
	void Init();
	void Update();
	void Draw();
	bool FallHitTest(float, float, float, float);
	void Operation();
	void Jump();
	void Attack();
	void Animetion();
private:
	TPLPalettePtr Tex;
	GXTexObj TexObj;
	TPLPalettePtr DeadTex;
	GXTexObj DeadTexObj;
	TPLPalettePtr InvincibleTex;
	GXTexObj InvincibleTexObj;
	f32 JumpStartY;
	f32 Width;
	f32 Height;
	f32 Ustart;
	f32 Vstart;
	f32 Uwidth;
	f32 Vheight;
	bool InDoubleJumpStatus;
	u8 cnt;
	u8 JumpCnt;
	u8 StatusStyle;
	bool FacedRight;
	f32 Initial_x;
	f32 Initial_y;
	bool InFall;

};


//typedef struct {
//	TPLPalettePtr Tex;
//	GXTexObj TexObj;
//	TPLPalettePtr DeadTex;
//	GXTexObj DeadTexObj;
//	f32 X;	
//	f32 Y;	
//	f32 Width;
//	f32 Height;
//	f32 Ustart;
//	f32 Vstart;
//	f32 Uwidth;
//	f32 Vheight;
//	int nHp;	//体力
//	f32 Flg;
//	u8 Hitting;
//	void Init(u8);
//	void Update(u8);
//	void Draw();
//	u8 cnt;
//}ENEMY;


typedef struct {
	u16 height;
	u16 width;
}ImaginaryBackground;

class DisplayClass {
public:
	f32 Fix_x;
	f32 Fix_y;
	f32 MoveDistance_x;
	f32 MoveDistance_y;
	u16 height;
	u16 width;
	bool ShockOn;
	void Init(ImaginaryBackground Background) {
		height = SCREEN_HEIGHT;
		width = SCREEN_WIDTH;
		Fix_x = 50;
		Fix_y = Background.height - height;
		MoveDistance_x = 0;
		MoveDistance_y = 0;
		ShockOn = false;
		cnt = 0;
	}
	void Shock(){
		if (cnt >= 30) {
			ShockOn = false;
			cnt = 0;
		}
		switch (cnt)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
			MoveDistance_x += 2;
			break;
		default:
			MoveDistance_x -= 2;
			break;
		}
		/*if (cnt < 20) {
		if (cnt % 2 == 0) {

		}else{ MoveDistance.x -= 5; }
		cnt += 1;}*/

		cnt += 1;
	}
	void Update(ImaginaryBackground Background) {
		if (ShockOn){
			Shock();
		}
		Fix_x = MoveDistance_x;
		Fix_y = Background.height - height + MoveDistance_y;
	}
private:
	u8 cnt;

};

class ImageClass{
public:

	f32	X;
	f32	Y;
	f32 DisplayX;
	f32 DisplayY;
	void Init();
	void Update();
	void TitleInit();
	void TitleDraw(int);
	void BackDraw();
	void UpDraw(int);
	void Sync(DisplayClass Display) {
		DisplayX = X - Display.MoveDistance_x;
	}
	f32 Width;
	f32 Height;
	TPLPalettePtr Tex;
	GXTexObj TexObj;
		f32 Ustart;
	f32 Vstart;
	f32 Uwidth;
	f32 Vheight;
private:
	


};
//typedef struct {
//	TPLPalettePtr Tex;
//	GXTexObj TexObj;
//	u8		Flag;
//	f32	X;
//	f32	DefX;
//	f32	Y;
//	bool	Disp;
//	bool notshooting;
//	void Init(u8);
//	void Update(u8);
//	void Hit(u8);
//	void Draw();
//}SHOOT;

enum {
	TITLE,
	GAME_START,
	GAME_PLAY,
	RESULT,
	GAME_OVER,
	GAME_WIN
};

enum {
	StationAnime,
	RunAnime,
	JumpAnime
};

enum {
	StationStatus,
	RunStatus,
	JumpStatus,
	DefenseStatus,
	AttackStatus
};

static f32 LandUMove = 0.0f;
u8 score_draw[5] = {0,0,0,0,0}; 
 
static u16 score = 0; 


//u8	ShootFlag[SHOOTNUM] = {HITOFF,HITOFF,HITOFF,HITOFF,HITOFF};
//f32 ShootX[SHOOTNUM];	// = {5.0f,10.0f,15.0f,20.0f,25.0f};
//f32	DefShotX[SHOOTNUM];
//f32 ShootY[SHOOTNUM];// = {DEFSHOTY,DEFSHOTY,DEFSHOTY,DEFSHOTY,DEFSHOTY};
//
//u8	HitSwitch = HITOFF;
GXColor	BG_COLOR = {0xfe,0xf2,0x63};	//背景色- 
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		プロトタイプ宣言
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern PlayerClass Player;
extern ImageClass *Image;
extern DisplayClass Display;
extern ImaginaryBackground Background;
/*
ImageClass WIN;
ImageClass LOSE;*/
//SHOOT Shoot[SHOOTNUM];
const u8 AnimeStation[64] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0xff };//0xff：終了コード
const u8 AnimeRun[64] = { 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 9, 9, 9, 9, 9, 9, 9, 0xff };
const u8 AnimeJump[64] = { 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 0xff };
const u8 AnimeDefense[64] = { 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26, 25, 25, 25, 25, 25, 25, 25, 25, 25, 0xff };//0xff：終了コード
const u8 AnimeAttack[64] = { 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 33, 33, 34, 34, 34, 34, 34, 34, 33, 33, 33, 33, 33, 33, 33, 32, 32, 32, 32, 32, 32, 0xff };//0xff：終了コード
const u8 *Anime_data[5] = { AnimeStation, AnimeRun, AnimeJump, AnimeDefense, AnimeAttack };

u8 fcnt;
u8 Status;
//u8 Anime_style;// = 0;
//const u8 *Animept = Anime_data[Anime_style];
//	サウンド用変数
void       *arenaMem2Lo;		//サウンド用メモリ下位アドレス
void       *arenaMem2Hi;		//サウンド用メモリ上位アドレス
void       *axBuffer;			//サウンド用メモリ先頭アドレス
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		関数宣言
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//f32		UpMove(f32 ,f32 ,f32 ,f32 );				//上移動関数
//f32		RightMove(f32 ,f32 ,f32 ,f32 );			//左移動関数
//f32		LeftMove(f32 ,f32 ,f32 ,f32  );				//左移動関数
//f32		DownMove(f32 ,f32 ,f32 ,f32 );				//下移動関数
bool	HitTest(f32 ,f32 ,f32 ,f32 ,f32 ,f32 , f32 ,f32 );
void	ScreenInitialization();
void	Draw2DCharacter(GXTexObj, f32, f32, f32, f32, f32, f32, f32, f32);
void	DrawPlayerRev(GXTexObj, f32, f32, f32, f32, f32, f32, f32, f32);
void	WiiMainInit();
void	GameUpdate();
void	GameDraw();
//==========================================================================================================
//		プログラム本編
//==========================================================================================================
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		エントリーポイント
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(void){
	WiiMainInit(); //初期化
	
	//		ゲームルーブ
	while(1)
	{
		GameUpdate();
		GameDraw();
		//		レンダリング終了
		DEMODoneRender();
	}
	//		ゲーム終了
	OSHalt("End of sample");
}


//==========================================================================================================
//		関数定義
//==========================================================================================================



//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		GAME Systemの初期化関数
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void WiiMainInit(){
	//		各種初期化
	DEMOInit(NULL);						//主要モジュールも内部で初期化されます｛os,pad,gx,vi}
	init_memory2();	//メモリ確保
	init_KPAD();	//リモコン初期化
	Status = TITLE;
	fcnt = 0;
	Image->TitleInit();
	// サウンド用メモリ確保
	// initialize Exp Heap on MEM2
	arenaMem2Lo = OSGetMEM2ArenaLo();
	arenaMem2Hi = OSGetMEM2ArenaHi();
	hExpHeap = MEMCreateExpHeap(arenaMem2Lo, (u32)arenaMem2Hi - (u32)arenaMem2Lo);

	// initialize AI & AX
	axBuffer = MEMAllocFromExpHeapEx(hExpHeap, AXGetMemorySize(AX_MAX_VOICES), 32);

	AIInit(NULL);
	AXInitSpecifyMem(AX_MAX_VOICES, axBuffer);

	// Load Voice data into MRAM
	LoadSamples();

	// Register Callback with AX for audio processing
	AXRegisterCallback(&AudioFrameCallback);

	for (int i = 0; i < AX_MAX_VOICES; i++)
	{
		Voices[i].state = VOICE_STATE_STOP;
	}

	// サウンド用初期化処理　ここまで

	// テクスチャのロード処理はこれ以降に行うこと
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		GAMEの更新処理
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GameUpdate(){
	fcnt += 1;
	if (fcnt > 60) { fcnt = 0; }
	work_controller();
	switch (Status){
	case TITLE:
		
		if (kpads[0][0].trig & KPAD_BUTTON_A){
			Status = GAME_START; 
			//PlaySample(&Samples[2]);
		}
		break;
	case GAME_START:
		Background.height = 1024;
		Background.width = 2048;
		Display.Init(Background);
		Image->Init();
		Player.Init();
		Status = GAME_PLAY;
		break;
	case GAME_PLAY:
		Image->Update();
		Player.Update();
		if (Player.nHp <= -10) { 
			Status = GAME_OVER;	
			/*
			TPLGetPalette(&LOSE.Tex, LOSETEX);
			TPLGetGXTexObjFromPalette(LOSE.Tex, &LOSE.TexObj, 0); */
		}
		//if (Enemy[0].nHp <= -5 && Enemy[1].nHp <= -5 && Enemy[2].nHp <= -5) { Status = GAME_WIN;	TPLGetPalette(&WIN.Tex, WINTEX);	TPLGetGXTexObjFromPalette(WIN.Tex, &WIN.TexObj, 0); }
		break;
	case GAME_OVER:
		
		if (kpads[0][0].trig & KPAD_BUTTON_A) { Status = TITLE; }
		break;
	case GAME_WIN:
		if (kpads[0][0].trig & KPAD_BUTTON_A){ Status = TITLE; }
		break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		GAMEの画面描画
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GameDraw(){
	ScreenInitialization();//画面初期化

	switch (Status){
	case TITLE:
		Image->TitleDraw(fcnt);
		break;
	case GAME_PLAY:
		/*
		Draw2DCharacter(texScrenObj, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1, 1);//sky
		Draw2DCharacter(texLandObj, LAND_WIDTH / 2, SCREEN_HEIGHT - LAND_HEIGHT / 2, LAND_WIDTH, LAND_HEIGHT, 0 + LandUMove, 0, 1, 1);


		Draw2DCharacter(Cloud.TexObj, Cloud.X, Cloud.Y, Cloud.Width, Cloud.Height, 0, 0, 1, 1);//sky
		*/
		Image->BackDraw();
		Player.Draw();
		Image->UpDraw(Player.nHp);
		/*for (u8 i = 0; i < ENEMYNUM; i++) {Enemy[i].Draw();}*/
	
		/*for(u8 i=0;i<5;i++){
		Draw2DCharacter(texFontObj, i*50.0f+200.0f, 50.0f, 50.0f, 50.0f, (score_draw[i]+16)%16*0.0625f, 0.0625f, ((score_draw[i]+16)/16)*0.0625f, 0.0625f);
		}*/
		break;
		/*
	case GAME_OVER:
		Image->BackDraw();
		Draw2DCharacter(LOSE.TexObj, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1, 1);
		if (fcnt >30 == 0) {
			Draw2DCharacter(Titleint.TexObj, Titleint.X, Titleint.Y, Titleint.Width, Titleint.Height, 0, 0, 1, 1);
		}
		break;
	case GAME_WIN:
		Image->BackDraw();
		Draw2DCharacter(WIN.TexObj, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1, 1);
		if (fcnt >30 == 0) {
			Draw2DCharacter(Titleint.TexObj, Titleint.X, Titleint.Y, Titleint.Width, Titleint.Height, 0, 0, 1, 1);
		}
		break;*/
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		当たり判定処理関数定義
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool			HitTest(f32 X1,f32 Y1,f32 W1,f32 H1,f32 X2,f32 Y2, f32 W2,f32 H2)
{
	f32 XX1 =X1-W1/2;
	f32	XX2 =X2-W2/2;
	f32 YY1 = Y1-H1/2;
	f32 YY2 = Y2-H2/2;

	if(((YY1+H1 >= YY2) && (YY1 -H2<= YY2)) && ((XX1+ W1 >= XX2) && (XX1-W2 <= XX2)))
	{
		return true;
	}
	else
	{
		return	 false;
	}

}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		Character表示関数定義
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void		Draw2DCharacter(GXTexObj TexObj,f32 X,f32 Y,f32 Width,f32 Height,f32 Ustart,f32 Vstart,f32 Uwidth,f32 Vheight)
{
	GXLoadTexObj(&TexObj, GX_TEXMAP0);
	GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 4);

	//		矩形角その１（頂点0）
	GXPosition3f32(X - Width / 2, Y + Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart, Vstart+ Vheight);            //2017.5.19追加
														
	//		矩形角その１（頂点1）
	GXPosition3f32(X - Width / 2, Y - Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart, Vstart);            //2017.5.19追加

	 //		矩形角その１（頂点2）
	GXPosition3f32(X + Width / 2, Y - Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart+ Uwidth, Vstart);            //2017.5.19追加

	 //		矩形角その１（頂点3）
	GXPosition3f32(X + Width / 2, Y + Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart+ Uwidth, Vstart + Vheight);            //2017.5.19追加

	GXEnd();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		PlayerRev表示関数定義
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void		DrawPlayerRev(GXTexObj TexObj, f32 X, f32 Y, f32 Width, f32 Height, f32 Ustart, f32 Vstart, f32 Uwidth, f32 Vheight)
{
	GXLoadTexObj(&TexObj, GX_TEXMAP0);
	GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 4);

	//		矩形角その１（頂点0）
	GXPosition3f32(X - Width / 2, Y + Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart + Uwidth, Vstart + Vheight);            //2017.5.19追加

	//		矩形角その１（頂点1）
	GXPosition3f32(X - Width / 2, Y - Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart + Uwidth, Vstart);            //2017.5.19追加

	//		矩形角その１（頂点2）
	GXPosition3f32(X + Width / 2, Y - Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart, Vstart);            //2017.5.19追加

	//		矩形角その１（頂点3）
	GXPosition3f32(X + Width / 2, Y + Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart, Vstart + Vheight);            //2017.5.19追加

	GXEnd();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		画面初期化関数定義
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void		ScreenInitialization() {
	//		プロジェクション行列の設定
	Mtx44	projMatrix;	//	プロジェクション行列

						//		正射影行列の作成
	MTXOrtho(projMatrix, 0, SCREEN_HEIGHT, 0, SCREEN_WIDTH, 0.0f, 1.0f);

	//		正射影行列をプロジェクション行列として設定
	GXSetProjection(projMatrix, GX_ORTHOGRAPHIC);

	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//		描画処理
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//		画面クリア
	GXSetCopyClear(BG_COLOR, GX_MAX_Z24);

	//		レンダリング開始
	DEMOBeforeRender();

	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//		頂点属性設定
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	GXClearVtxDesc();																												//頂点属性情報クリア
																																	//GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
																																	//GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);																	//座標はダイレクト参照
																																	//GXSetVtxAttrFmt(GX_VTXFMT0,GX_VA_POS,GX_POS_XYZ,GX_F32,0);		//頂点座標属性フォーマット設定
																																	//GXSetVtxAttrFmt(GX_VTXFMT0,GX_VA_CLR0,GX_CLR_RGB,GX_RGB8,0);
																																	//GXSetNumChans(1);
																																	//GXSetNumTevStages(1);
																																	//GXSetTevOrder(GX_TEVSTAGE0,GX_TEXCOORD_NULL,GX_TEXMAP_NULL,GX_COLOR0A0);
																																	//GXSetTevOp(GX_TEVSTAGE0,GX_PASSCLR);

	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);     //座標はダイレクト参照
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);    //単一の属性（色）とその属性に対する参照の仕方を指定
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);    //2017.5.19 テクスチャの設定
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);	//頂点座標属性フォーマットの設定
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0);//頂点色属性フォーマットの設定
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);  //頂点属性の設定   2017.5.19追加
	GXSetNumChans(1);            //カラーチャンネルの設定
	GXSetNumTevStages(1);        //TEVステージ数の設定
	GXSetNumTexGens(1);          //2017.5.19追加
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);  //色・テクスチャの合成準備 2017.5.19修正
																		 //	GXSetTevOp(GX_TEVSTAGE0,GX_PASSCLR);  //TEVのカラーの合成設定
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);  //2017.5.19修正
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);  //2017.5.19追加
																				  //	GXLoadTexObj(&texObj, GX_TEXMAP0);     //2017.5.19追加
}


//==========================================================================================================
//		End of sample
//==========================================================================================================
ImageClass *Image;
ImageClass Scren;
//ImageClass Land;
ImageClass Title;
ImageClass Titleint;
ImageClass LandPixel[256];
ImageClass Footing[256];
u8 LandNum;
u8 FootingNum;
ImaginaryBackground Background;
DisplayClass Display;


void ImageClass::Init() {
	TPLGetPalette(&Scren.Tex, SCRENTEX);
	TPLGetGXTexObjFromPalette(Scren.Tex, &Scren.TexObj, 0);

	TPLGetPalette(&LandPixel[0].Tex, LANDPIXELTEX);
	TPLGetGXTexObjFromPalette(LandPixel[0].Tex, &LandPixel[0].TexObj, 0);

	TPLGetPalette(&Footing[0].Tex, LANDPIXELTEX);
	TPLGetGXTexObjFromPalette(Footing[0].Tex, &Footing[0].TexObj, 0);


	LandPixel[0].Width = 50;
	LandNum = Background.width / LandPixel[0].Width + 1;
	//LandNum = 41;
	for (int i = 0; i < LandNum; i++) {
		LandPixel[i].Height = 50;
		LandPixel[i].Width = 50;
		LandPixel[i].Ustart = 0.0f;
		LandPixel[i].Uwidth = 1.0f;
		LandPixel[i].Vstart = 0.0f;
		LandPixel[i].Vheight = 1.0f;
		LandPixel[i].Y = SCREEN_HEIGHT - 25;
		LandPixel[i].DisplayY = LandPixel[i].Y;
		LandPixel[i].X = LandPixel[i].Width / 2 + i*LandPixel[i].Width;
		LandPixel[i].DisplayX = LandPixel[i].X;
	}

	Scren.Width = SCREEN_WIDTH;
	Scren.Height = SCREEN_HEIGHT;
	Scren.X = Scren.Width / 2;
	Scren.Y = Scren.Height / 2;
	FootingNum = 2;
	Footing[0].X = 1024;
	Footing[0].Y = SCREEN_HEIGHT - 120;
	Footing[1].X = 1224;
	Footing[1].Y = SCREEN_HEIGHT - 170;
	for (int i = 0; i < FootingNum; i++) {
		Footing[i].Height = 20;
		Footing[i].Width = 100;
		Footing[i].Ustart = 0.0f;
		Footing[i].Uwidth = 1.0f;
		Footing[i].Vstart = 0.0f;
		Footing[i].Vheight = 1.0f;
		Footing[i].DisplayY = Footing[i].Y;
		Footing[i].DisplayX = Footing[i].X;
	}

}

void ImageClass::Update(){

	Display.Update(Background);
	//Land.Ustart += 0.0003f;
	for (int i = 0; i < LandNum; i++) {
		LandPixel[i].Sync(Display);
	}
	for (int i = 0; i < FootingNum; i++) {
		Footing[i].Sync(Display);
	}
}

void ImageClass::TitleInit(){
	TPLGetPalette(&Title.Tex, TITLETEX);
	TPLGetGXTexObjFromPalette(Title.Tex, &Title.TexObj, 0);

	TPLGetPalette(&Titleint.Tex, TITLEINTTEX);
	TPLGetGXTexObjFromPalette(Titleint.Tex, &Titleint.TexObj, 0);
	Title.Width = SCREEN_WIDTH;
	Title.Height = SCREEN_HEIGHT;
	Title.X = Title.Width / 2;
	Title.Y = Title.Height / 2;

	Titleint.Width = 219;
	Titleint.Height = 65;
	Titleint.X = 130;
	Titleint.Y = 400;
}
void ImageClass::TitleDraw(int fcnt) {
	Draw2DCharacter(Title.TexObj, Title.X, Title.Y, Title.Width, Title.Height, 0, 0, 1, 1);

	if (fcnt >30 == 0) {
		Draw2DCharacter(Titleint.TexObj, Titleint.X, Titleint.Y, Titleint.Width, Titleint.Height, 0, 0, 1, 1);
	}
}
void ImageClass::BackDraw() {
	Draw2DCharacter(Scren.TexObj, Scren.X, Scren.Y, Scren.Width, Scren.Height, 0, 0, 1, 1);//sky
	for (int i = 0; i < LandNum; i++) {
		Draw2DCharacter(LandPixel[0].TexObj, LandPixel[i].DisplayX, LandPixel[i].DisplayY, LandPixel[i].Width, LandPixel[i].Height, LandPixel[i].Ustart, LandPixel[i].Vstart, LandPixel[i].Uwidth, LandPixel[i].Vheight);
	}

	for (int i = 0; i < FootingNum; i++) {
		Draw2DCharacter(Footing[0].TexObj, Footing[i].DisplayX, Footing[i].DisplayY, Footing[i].Width, Footing[i].Height, Footing[i].Ustart, Footing[i].Vstart, Footing[i].Uwidth, Footing[i].Vheight);
	}
}

void ImageClass::UpDraw(int HP) {

}








PlayerClass Player;
//ENEMY Enemy[ENEMYNUM];
//SHOOT Shoot[SHOOTNUM];
//ENEMYSHOOT EnemyShoot[ENEMYSHOOTMAX];

void PlayerClass::Init() {
	extern DisplayClass Display;

	TPLGetPalette(&Player.Tex, PLAYERTEX);
	TPLGetGXTexObjFromPalette(Player.Tex, &Player.TexObj, 0);   //2017.5.19追加
	Initial_x = (float)Display.width / 2;
	Initial_y = (float)(Display.height - 40 - 64);
	X = Initial_x;
	Y = Initial_y;
	Width = (u8)(128/(544/480));
	Height = (u8)(128/(544/480));
	Width = 128;
	Height = 128;
	nHp = 6;
	InvincibleState = 0;
	Ustart = 0.0f;
	Uwidth = (f32)1/8;
	Vstart = 0.0f;
	Vheight = (f32)1/8;
	FacedRight = true;
	StatusStyle = StationStatus;
	cnt = 0;
	JumpCnt = 0;
	InDoubleJumpStatus = false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Player落下判定関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool			PlayerClass::FallHitTest(float X1, float Y1, float W, float H)
{
	f32 XX1 = X - 24 / 2;
	if (FacedRight) {
		XX1 -= 5;
	}
	else {
		XX1 += 5;
	}//修正
	f32	XX2 = X1 - W / 2;
	f32 YY1 = Y + Height / 2 - 10;
	f32 YY2 = Y1 - H / 2;

	if (((YY1 + 10 >= YY2) && (YY1 - H <= YY2)) && ((XX1 + 24 >= XX2) && (XX1 - W <= XX2)))
	{
		return true;
	}
	else
	{
		return 	false;
	}

}

void PlayerClass::Update() {
	
	/*
	if (InvincibleState > 0) {
		InvincibleState -= 1;
	}*/
	if (nHp > 0) {

		if (StatusStyle != AttackStatus) {
			Operation();
			Jump();
		}else {
			Attack();
		}		
	}

}


void PlayerClass::Draw() {
	Animetion();
	

	if (FacedRight){
		Draw2DCharacter(TexObj, X, Y, Width, Height, Ustart, Vstart, Uwidth, Vheight);
	}else{
		DrawPlayerRev(TexObj, X, Y, Width, Height, Ustart, Vstart, Uwidth, Vheight);
	}




}


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Player操作関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void	PlayerClass::Operation() {
	extern DisplayClass Display;
	
	//**********************************************************************
	//	Player 移動
	//**********************************************************************
	if (kpads[0][0].hold & KPAD_BUTTON_UP) {
		if (FacedRight) {
			if (X - Width / 2 < Display.width / 4 && Display.Fix_x + Display.MoveDistance_x > 10) {
				Display.MoveDistance_x -= PLAYERSPEED / 2;
			}
			else if (X - Width / 2 > 0) { X -= PLAYERSPEED / 2; }


			if (StatusStyle != JumpStatus) {
				if (StatusStyle != DefenseStatus) { cnt = 0;  StatusStyle = DefenseStatus; }
			}
		}
		else {
			if (X - Width / 2 < Display.width / 4 && Display.Fix_x + Display.MoveDistance_x > 10) {
				Display.MoveDistance_x -= PLAYERSPEED;
			}
			else if (X - Width / 2 > 0) {
				X -= PLAYERSPEED;
			}
			if (StatusStyle != JumpStatus) {
				if (StatusStyle != RunStatus) {
					cnt = 0;
					StatusStyle = RunStatus;
				}
			}
		}							//左移動
	}
	if (kpads[0][0].hold & KPAD_BUTTON_DOWN) {
		if (FacedRight) {
			if (X + Width / 2 > Display.width * 3 / 4 && Display.MoveDistance_x + Display.width <= Background.width - 10) {
				Display.MoveDistance_x += PLAYERSPEED;
			}
			else if (X + Width / 2 < Display.width) { X += PLAYERSPEED; }

			if (StatusStyle != JumpStatus) {
				if (StatusStyle != RunStatus) {
					cnt = 0;
					StatusStyle = RunStatus;
				}
			}
		}
		else {
			if (X + Width / 2 > Display.width * 3 / 4 && (Display.MoveDistance_x + Display.width) <= Background.width - 10) {
				Display.MoveDistance_x += PLAYERSPEED / 2;
			}
			else if (X + Width / 2 < Display.width) { X += PLAYERSPEED / 2; }
			if (StatusStyle != JumpStatus) { if (StatusStyle != DefenseStatus) { cnt = 0;  StatusStyle = DefenseStatus; } }
		}				//右移動
	}

	//**********************************************************************
	//	Player 攻撃
	//**********************************************************************
	if (kpads[0][0].trig & KPAD_BUTTON_1){
		if (StatusStyle != JumpStatus) {
			cnt = 0;
			StatusStyle = AttackStatus;
			Display.ShockOn = true;
		}
	}

	//**********************************************************************
	//	Player 転身
	//**********************************************************************
	if (kpads[0][0].trig & KPAD_BUTTON_B){
		FacedRight = !FacedRight;
		if (StatusStyle == RunStatus) { StatusStyle = DefenseStatus; }
	}

	//**********************************************************************
	//	Player 　jump
	//**********************************************************************
	if (kpads[0][0].trig & KPAD_BUTTON_2) {
		if (StatusStyle != JumpStatus && !InDoubleJumpStatus) {
			JumpStartY = Initial_y;
			JumpCnt = 0;
			cnt = 0;
			StatusStyle = JumpStatus;
		}
		else if (!InDoubleJumpStatus) {
			JumpCnt = 0;
			cnt = 0;
			InDoubleJumpStatus = true;
		}
	}

	//**********************************************************************
	//	停止
	//**********************************************************************
	if (kpads[0][0].release & (KPAD_BUTTON_UP | KPAD_BUTTON_DOWN))
	{
		if (StatusStyle != JumpStatus) {
			if (StatusStyle != StationStatus) { cnt = 0; StatusStyle = StationStatus; }
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Player jumpと落下処理関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PlayerClass::Jump() {
	extern u8 FootingNum;
	extern ImageClass Footing[256];
	if (StatusStyle == JumpStatus) {
		JumpCnt += 1;
		//float vg = 10 - 0.98f*fcnt;
		Y -= 14 - 0.98f*JumpCnt;;
		const u8 *ptAnime = Anime_data[StatusStyle];

		if (Y > JumpStartY) {
			Y = JumpStartY;
			JumpCnt = 0;
			cnt = 0;
			JumpStartY = 0.0f;
			InDoubleJumpStatus = false;
			StatusStyle = StationStatus;
			//Display.ShockOn = true;
		}
		if (*(ptAnime + cnt) == 18) {
			for (int i = 0; i < FootingNum; i++) {
				if (Player.FallHitTest(Footing[i].DisplayX, Footing[i].DisplayY, Footing[i].Width, Footing[i].Height)/*HitTest(, Y, 20, Height, Footing[i].DisplayX, Footing[i].DisplayY, Footing[i].Width, Footing[i].Height)*/) {
					Y = Footing[i].DisplayY - Footing[i].Height / 2 - Height / 2 + 10;

					JumpCnt = 0;
					cnt = 0;
					JumpStartY = 0.0f;
					InDoubleJumpStatus = false;
					StatusStyle = StationStatus;
				}
			}
		}
	}
	else {
		//落下
		if (Y != Initial_y) {
			InFall = true;
			for (int i = 0; i < FootingNum; i++) {
				if (Player.FallHitTest(Footing[i].DisplayX, Footing[i].DisplayY, Footing[i].Width, Footing[i].Height)) {
					InFall = false;
					break;
				}
			}
			if (InFall) {
				JumpStartY = Initial_y;
				JumpCnt = 16;
				cnt = 16;
				StatusStyle = JumpStatus;
			}

		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Player 攻撃処理関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PlayerClass::Attack() {
	const u8 *ptAnime = Anime_data[StatusStyle];
	cnt += 1;
	if (*(ptAnime + cnt) == 0xff) {
		cnt = 0;
		StatusStyle = StationStatus;
	}
	else { cnt -= 1; }

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Player Animetion処理関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PlayerClass::Animetion() {
	const u8 *ptAnime = Anime_data[StatusStyle];
	if (*(ptAnime + cnt) == 0xff) {

		if (StatusStyle == JumpStatus) {
			cnt = 16;
		}
		else {
			cnt = 0;
		}
	}
	Ustart = ((*(ptAnime + cnt)) % (int)(1 / Uwidth))*Uwidth;
	Vstart = ((*(ptAnime + cnt)) / (int)(1 / Vheight))*Vheight;
	cnt += 1;
	//if (*(ptAnime + cnt) == 0xff) { cnt = 0; }
}