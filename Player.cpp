//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		Player 処理関数
//		曜氷
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include	"main.h"
#include "DrawPolygon.h"
#include "Player.h"
#include "Background.h"
#include "Quantitative.h"

//==========================================================================================================
//		定義
//==========================================================================================================
PlayerClass Player;
extern DisplayClass Display;
//ENEMY Enemy[ENEMYNUM];
//SHOOT Shoot[SHOOTNUM];
//ENEMYSHOOT EnemyShoot[ENEMYSHOOTMAX];
const u8 PlayerClass::AnimeStation[64] = { 0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0xff };//0xff：終了コード
const u8 PlayerClass::AnimeRun[64] = {8,8,8,8,8,8,8,8,8,9,9,9,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,11,11,11,0xff };
const u8 PlayerClass::AnimeJump[64] = { 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 0xff };
const u8 PlayerClass::AnimeDefense[64] ={ 24,24,24,24,24,24,24,24,24,24,24,24,25,25,25,25,25,25,25,25,25,25,25,25,0xff };//0xff：終了コード
const u8 PlayerClass::AnimeAttack[64] = { 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 34, 34, 34, 34, 34, 34, 34, 34, 34, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 0xff };//0xff：終了コード
const u8 PlayerClass::AnimeHit[64] = { 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 0xff };//0xff：終了コード
const u8 PlayerClass::AnimeClimb[64] = { 40,40,40,40,40,40,41,41,41,41,42,42,42,42,42,42,43,43,43,43,0xff };

void PlayerClass::Init() {
	extern DisplayClass Display;

	TPLGetPalette(&Player.Tex, PLAYERTEX);
	TPLGetGXTexObjFromPalette(Player.Tex, &Player.TexObj, 0);   //2017.5.19追加
	Initial_x = (f32)100+Display.width / 2;
	Initial_y = (f32)(Display.height - InitialPlayerHeight - 64);
	X = Initial_x;
	Y = Initial_y;
	DisplayX = X;
	DisplayY = Y;
//	Width = 128;
//	Height = 128;
	MaxHp = 28;
	Hp = MaxHp;
	InvincibleState = 0;
	/*
	Ustart = 0.0f;
	Uwidth = (f32)1/8;
	Vstart = 0.0f;
	Vheight = (f32)1/8;
	*/
	FacedRight = true;
	StatusStyle = StationStatus;
	cnt = 0;
	JumpCnt = 0;
	InDoubleJumpStatus = false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Game LoopにPlayer更新関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PlayerClass::Update() {
	//**********************************************************************
	//	描画座標の更新
	//**********************************************************************
	DisplayX = X - Display.MoveDistance_x;
	DisplayY = Y - Display.MoveDistance_y;

	if (InvincibleTime != 0) { InvincibleTime -= 1; }  //	無敵時間の処理
	
	if (Hp > 0) {
		Climb();
		if (StatusStyle != ClimbStatus) {
			if (StatusStyle == AttackStatus) {
				Attack();

			}
			else if (StatusStyle == HitStatus) {
				Hit();
			}
			else {
				Operation();
				Jump();
			}
		}
		
		AllHitTest();		
	}

}


void PlayerClass::Draw() {
	Animetion();
	

	if (FacedRight){
		Draw2DCharacter(TexObj, DisplayX, DisplayY, Width, Height, Ustart, Vstart, Uwidth, Vheight);
	}else{
		DrawPlayerRev(TexObj, DisplayX, DisplayY, Width, Height, Ustart, Vstart, Uwidth, Vheight);
	}




}


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Player操作関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void	PlayerClass::Operation() {
	extern DisplayClass Display;
	//**********************************************************************
	//	Player 防御
	//**********************************************************************
	if (kpads[0][0].hold & KPAD_BUTTON_B){
		if (StatusStyle != JumpStatus) {
			if (StatusStyle != DefenseStatus) {
				cnt = 0; 
				StatusStyle = DefenseStatus;
			}
		}
		//**********************************************************************
		//	Player 防御移動
		//**********************************************************************
		if (kpads[0][0].hold & KPAD_BUTTON_UP) {
			/*if (!FacedRight) {
				FacedRight = !FacedRight;
				X += 64;
			}*/
			X -= PLAYERSPEED/2;
			MoveHit();
		}else if (kpads[0][0].hold & KPAD_BUTTON_DOWN) {
			/*if (FacedRight) {
				FacedRight = !FacedRight;
				X -= 64;
			}*/
			X += PLAYERSPEED/2;
			MoveHit();
		}else{
			cnt = 0;
		}
	}
	else
	{
		//**********************************************************************
		//	Player 移動
		//**********************************************************************
		if (kpads[0][0].hold & KPAD_BUTTON_UP) {
		if (FacedRight) {
					FacedRight = !FacedRight;
					X -= 28;
				}
				X -= PLAYERSPEED;
				MoveHit();
				if (StatusStyle != JumpStatus) {
					if (StatusStyle != RunStatus) {
						cnt = 0;
						StatusStyle = RunStatus;
					}
				}							//左移動
		}
		if (kpads[0][0].hold & KPAD_BUTTON_DOWN) {
		if (!FacedRight) { 
					FacedRight = !FacedRight;
					X += 28;
				}
				X += PLAYERSPEED;
				MoveHit();
				if (StatusStyle != JumpStatus) {
					if (StatusStyle != RunStatus) {
						cnt = 0;
						StatusStyle = RunStatus;
					}
				}				//右移動
		}
	}


	//**********************************************************************
	//	Player 攻撃
	//**********************************************************************
	if (kpads[0][0].trig & KPAD_BUTTON_1){
		if (StatusStyle != JumpStatus) {
			cnt = 0;
			StatusStyle = AttackStatus;
		}
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
	if (kpads[0][0].release & (KPAD_BUTTON_UP | KPAD_BUTTON_DOWN|KPAD_BUTTON_B))
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
const u8 *Anime_data[7] = { AnimeStation, AnimeRun, AnimeJump, AnimeDefense, AnimeAttack, AnimeHit ,AnimeClimb};

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
				if (Player.FallHitTest(Footing[i].X, Footing[i].Y, Footing[i].Width, Footing[i].Height)) {
					Y = Footing[i].Y - Footing[i].Height / 2 - Height / 2 + 10;

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
				if (Player.FallHitTest(Footing[i].X, Footing[i].Y, Footing[i].Width, Footing[i].Height)) {
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
const u8 *Anime_data[7] = { AnimeStation, AnimeRun, AnimeJump, AnimeDefense, AnimeAttack, AnimeHit ,AnimeClimb};

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
const u8 *Anime_data[7] = { AnimeStation, AnimeRun, AnimeJump, AnimeDefense, AnimeAttack, AnimeHit ,AnimeClimb};

	const u8 *ptAnime = Anime_data[StatusStyle];
	if (*(ptAnime + cnt) == 0xff) {

		if (StatusStyle == JumpStatus) {
			cnt = 16;
		}
		else {
			cnt = 0;
		}
	}
	Ustart = ((*(ptAnime + cnt)) % (u8)(1 / Uwidth))*Uwidth;
	Vstart = ((*(ptAnime + cnt)) / (u8)(1 / Vheight))*Vheight;
	if (StatusStyle != ClimbStatus) { cnt += 1; }
	//if (*(ptAnime + cnt) == 0xff) { cnt = 0; }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Player damage受ける開始関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void	PlayerClass::HitOn() {
	if (Display.ShockOn == 0) {
		Display.ShockOn = 2;
	}
		
		HitCnt = 0;
		cnt = 0;
		StatusStyle = HitStatus;
		InvincibleTime = 30;
		JumpStartY = Initial_y;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Player damage受ける処理関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PlayerClass::Hit() {
	HitCnt += 1;

	if (EnemyInPlayerRight) {
		X -= PLAYERSPEED;
		BoundaryHitTest();
	}
	else
	{
		X += PLAYERSPEED;
		BoundaryHitTest();
	}
	Y -= 10 - 0.98f*HitCnt;
	if (Y > JumpStartY) {
		Y = JumpStartY;
		HitCnt = 0;
		cnt = 0;
		Hp -= 3;
		StatusStyle = StationStatus;
		InDoubleJumpStatus = false;
	}

}



//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Player 処理関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PlayerClass::Climb() {
const u8 *Anime_data[7] = { AnimeStation, AnimeRun, AnimeJump, AnimeDefense, AnimeAttack, AnimeHit ,AnimeClimb};
	extern u8 FootingNum;
	extern ImageClass Footing[256];
	//**********************************************************************
	//	Player 登る
	//**********************************************************************
	if (StatusStyle != ClimbStatus) {
		if (kpads[0][0].hold & KPAD_BUTTON_RIGHT){
			cnt = 0; 
			StatusStyle = ClimbStatus; 
			Y -= 20;
		}
		if (kpads[0][0].hold & KPAD_BUTTON_LEFT) {
			cnt = 0; 
			StatusStyle = ClimbStatus; 
			Y += 20;
		}
	}else{
			if (kpads[0][0].hold & KPAD_BUTTON_RIGHT) {
			cnt += 1;
			if ((AnimeCnt(0) != AnimeCnt(-1)) && ((AnimeCnt(0) == 40) || (AnimeCnt(0) == 42) || (AnimeCnt(0) == 0xff))) {
				Y -= 20;
			}
		}
			if (kpads[0][0].hold & KPAD_BUTTON_LEFT){
			cnt += 1;
			if ((AnimeCnt(0) != AnimeCnt(-1)) && ((AnimeCnt(0) == 40) || (AnimeCnt(0) == 42) || (AnimeCnt(0) == 0xff))) {
				Y += 20;
			}
		}

		//落下
		
		if ((kpads[0][0].hold & KPAD_BUTTON_UP)||(kpads[0][0].hold & KPAD_BUTTON_DOWN)||(kpads[0][0].hold & KPAD_BUTTON_2)){
			if (Y != Initial_y ) {
				InFall = true;
				for (int i = 0; i < FootingNum; i++) {
					if (Player.FallHitTest(Footing[i].X, Footing[i].Y, Footing[i].Width, Footing[i].Height)) {
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
	
}
