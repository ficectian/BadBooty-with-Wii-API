//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		インクルードファイル
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include	"main.h"
#include "DrawPolygon.h"
#include "Quantitative.h"
#include "Player.h"
#include "Background.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		グローバル変数
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		プロトタイプ宣言
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern PlayerClass Player;
extern ImageClass *Image;
extern DisplayClass Display;
extern ImaginaryBackground Background;
extern UIClass *GameUI;
extern EnemyClass *Enemy;


u8 fcnt;
u8 Status;
bool LoopWaiting;
u8 LoopFPS = 0;
u8 GameLoop = 0;
bool LookOP = false;
bool Inited = false;
//u8 Anime_style;// = 0;
//const u8 *Animept = Anime_data[Anime_style];
//	サウンド用変数
void       *arenaMem2Lo;		//サウンド用メモリ下位アドレス
void       *arenaMem2Hi;		//サウンド用メモリ上位アドレス
void       *axBuffer;			//サウンド用メモリ先頭アドレス
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		関数宣言
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//bool	HitTest(f32 ,f32 ,f32 ,f32 ,f32 ,f32 , f32 ,f32 );
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
switch (Status)
	{
	default:
		//		各種初期化
		DEMOInit(NULL);						//主要モジュールも内部で初期化されます｛os,pad,gx,vi}
		init_memory2();	//メモリ確保
		init_KPAD();	//リモコン初期化
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
		Status = TITLE;
		Image->Init();
		fcnt = 0;
		break;
	case GAME_START:
		LoopWaiting = false;
		Background.height = 1500;
		Background.width = 3000;
		Display.Init(Background);
		Image->Init();
		Player.Init(); //player初期化
		Enemy->AllInit();
		GameUI->Init();
		Inited = true;
		Status = GAME_PLAY;

		
		break;
	}


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
		Image->Update();
		if (kpads[0][0].trig & KPAD_BUTTON_A){
			if(!LookOP)
			{
				LookOP = true;
			}else{
				Status = GAME_START;
				WiiMainInit();
			}
		}
		break;
		/*
	case GAME_START:
	
		Display.Init(Background);
		Image->Init();
		Player.Init();
		Enemy->AllInit();

		GameUI->Init();

		Status = GAME_PLAY;
		break;*/
	case GAME_PLAY:
		if (Player.StopTime == 0) {
			Display.Update(Background);
			Image->Update();
			Player.Update(); //Player Move
			Enemy->AllUpdate();
		}else{
			Player.StopTime -= 1;
		}
		
		if (Player.Hp <= 0) {  
			GameLoop = 0; 
			Enemy->AllInit();
			Status = TITLE;
		}
		if(!Enemy->AllHaveHp()) {  LoopWaiting = true; }
		if(LoopFPS >=180){
			GameLoop += 1;
			Enemy->AllInit();
			LoopWaiting = false;
			LoopFPS = 0;
		}
		if (LoopWaiting) { LoopFPS += 1; }
		if (GameLoop > 3) {
			GameLoop = 0;
			LookOP = false;
			Status = TITLE;
		}
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
		Image->TitleDraw(LookOP);
		break;
	case GAME_PLAY:
		/*
		Draw2DCharacter(texScrenObj, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1, 1);//sky
		Draw2DCharacter(texLandObj, LAND_WIDTH / 2, SCREEN_HEIGHT - LAND_HEIGHT / 2, LAND_WIDTH, LAND_HEIGHT, 0 + LandUMove, 0, 1, 1);


		Draw2DCharacter(Cloud.TexObj, Cloud.X, Cloud.Y, Cloud.Width, Cloud.Height, 0, 0, 1, 1);//sky
		*/
		Image->BackDraw();
		Enemy->AllDraw();
		Player.Draw();
		Image->UpDraw(Player.Hp);
		GameUI->Draw(Player.Hp);
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

//==========================================================================================================
//		End of sample
//==========================================================================================================

