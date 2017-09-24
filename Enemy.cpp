//+++++++++++++++++++++//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		敵AI関数
//		曜氷
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



#include	"main.h"
#include <stdlib.h> 
#include <time.h>  
#include "DrawPolygon.h"
#include "Player.h"
#include "Background.h"
#include "Quantitative.h"

//==========================================================================================================
//		定義
//==========================================================================================================
extern DisplayClass Display;
extern ImaginaryBackground Background;
extern bool Inited;
EnemyClass *Enemy;
EnemyClass SwordEnemy[64];
u8 SwordEnemyNum;
const u8 EnemyClass::AnimeRun[64] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0xff };//0xff：終了コード
const u8 EnemyClass::AnimeAttack[128] = { 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,6,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0xfe };
const u8 EnemyClass::AnimeHit[64] = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0xff };
const u8 EnemyClass::Ball::bornBall[64] = { 0,0,0, 0,0,0,1,1,1,2,2,2,1,1,1,2,2,2,1,1,1,2,2,2,3,3,3,4,4,4,3,3,3,4,4,4,3,3,3,4,4,4,3,3,3,4,4,4,0xff };//0xff：終了コード	const byte deathBall[64] = { 9,9,9,8,8,8, 9,9,9,8,8,8,9,9,9,8,8,8,9,9,9,8,8,8,7,7,7,6,6,6,7,7,7,6,6,6,7,7,7,6,6,6,5,5,5,5,5,5,0xff };
const u8 EnemyClass::Ball::deathBall[64] = { 9,9,9,8,8,8, 9,9,9,8,8,8,9,9,9,8,8,8,9,9,9,8,8,8,7,7,7,6,6,6,7,7,7,6,6,6,7,7,7,6,6,6,5,5,5,5,5,5,0xff };
const u8 EnemyClass::Ball::loseBall[128] = { 14,14,14,13,13,13, 14,14,14,13,13,13, 14,14,14,13,13,13, 14,14,14,13,13,13,14,14,14,13,13,13,14,14,14,13,13,13,12,12,12,11,11,11,12,12,12,11,11,11,12,12,12,11,11,11,12,12,12,11,11,11,12,12,12,11,11,11,10,10,10,10,10,10,0xff };
		

//==========================================================================================================
//		全体敵行為処理定義
//==========================================================================================================
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	全体敵初期化関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void	EnemyClass::AllInit() {
	//extern DisplayClass Display;
	extern u8 GameLoop;
	SwordEnemyNum = 5;
	
	if (GameLoop == 0) {
		if(!SwordEnemy[0].inited)
		{
			TPLGetPalette(&SwordEnemy[0].ptTex, ENEMYPTTEX);
			TPLGetGXTexObjFromPalette(SwordEnemy[0].ptTex, &SwordEnemy[0].ptTexObj, 0);
			TPLGetPalette(&SwordEnemy[0].ball.Tex, LIGHTBALLTEX);
			TPLGetGXTexObjFromPalette(SwordEnemy[0].ball.Tex, &SwordEnemy[0].ball.TexObj, 0);
			TPLGetPalette(&SwordEnemy[0].Tex, ENEMYTEX);
			TPLGetGXTexObjFromPalette(SwordEnemy[0].Tex, &SwordEnemy[0].TexObj, 0);
			SwordEnemy[0].MaxHp = 5;
			SwordEnemy[0].inited = true;
		}
		
		SwordEnemy[0].Init();
		//SwordEnemy[0].Hp = SwordEnemy[0].MaxHp;
		SwordEnemy[0].InitialX = 1200;
		SwordEnemy[0].X = SwordEnemy[0].InitialX;
	}
	else
	{
		SwordEnemy[GameLoop].Init();
		//SwordEnemy[GameLoop].Hp = SwordEnemy[GameLoop].MaxHp;
	}
	/*
		if(!Inited)
		{
			TPLGetPalette(&SwordEnemy[0].Tex, ENEMYTEX);
			TPLGetGXTexObjFromPalette(SwordEnemy[0].Tex, &SwordEnemy[0].TexObj, 0);
			for(u8 i = 0;i<SwordEnemyNum;i++)
			{
				SwordEnemy[i].TexObj=SwordEnemy[0].TexObj;
			}
		}
	
	
	for(u8 i=0 ;i<SwordEnemyNum ;i++)
	{
		SwordEnemy[i].Hp = 0;
	}
	
	switch (GameLoop)
	{
	case 0:
		SwordEnemy[0].InitialX = 1200;
		SwordEnemy[0].InitialY =  (float)(Background.height - InitialPlayerHeight - SwordEnemy[0].Height / 2);
		SwordEnemy[0].X = SwordEnemy[0].InitialX;
		SwordEnemy[0].Y = SwordEnemy[0].InitialY;
		SwordEnemy[0].DisplayX = SwordEnemy[0].X;
		SwordEnemy[0].DisplayY = SwordEnemy[0].Y;
		SwordEnemy[0].MaxHp = 3;
		SwordEnemy[0].Hp = SwordEnemy[0].MaxHp;
		SwordEnemy[0].StatusStyle = EnemyRunAnime;
		SwordEnemy[0].ActionMod = PatrolMod;
		break;
	case 1:
		
		SwordEnemy[1].InitialX = 500;
		SwordEnemy[1].InitialY = (float)(Background.height - InitialPlayerHeight - SwordEnemy[0].Height / 2);
		SwordEnemy[1].X = SwordEnemy[1].InitialX;
		SwordEnemy[1].Y = SwordEnemy[1].InitialY;
		SwordEnemy[1].DisplayX = SwordEnemy[1].X;
		SwordEnemy[1].DisplayY = SwordEnemy[1].Y;
		SwordEnemy[1].Hp = SwordEnemy[1].MaxHp;
		SwordEnemy[1].ActionMod = PatrolMod;
		SwordEnemy[1].StatusStyle = EnemyRunAnime;
		break;
	case 3:
		SwordEnemy[2].InitialX = 1000;
		SwordEnemy[2].InitialY = (float)(Background.height - InitialPlayerHeight - SwordEnemy[0].Height / 2);
		SwordEnemy[2].X = SwordEnemy[2].InitialX;
		SwordEnemy[2].Y = SwordEnemy[2].InitialY;
		SwordEnemy[2].DisplayX = SwordEnemy[2].X;
		SwordEnemy[2].DisplayY = SwordEnemy[2].Y;
		SwordEnemy[2].MaxHp = 10;
		SwordEnemy[2].Hp = SwordEnemy[2].MaxHp;
		SwordEnemy[2].ActionMod = PatrolMod;
		SwordEnemy[2].StatusStyle = EnemyRunAnime;
		
	default:
		break;
	}*/

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	全体敵更新関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EnemyClass::AllUpdate() {
	//	剣敵の更新
	for (int i = 0; i < SwordEnemyNum; i++) {
		SwordEnemy[i].Update();
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	全体敵更新関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void	EnemyClass::AllDraw() {
	for (int i = 0; i < SwordEnemyNum; i++) {
		SwordEnemy[i].Draw();
		
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	全体敵HP判定関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool	EnemyClass::AllHaveHp() {
	for (int i = 0; i < SwordEnemyNum; i++) {
		if (SwordEnemy[i].broned != 0 && SwordEnemy[i].deaded != 2) { return true; }	
	}

	return false;
}

//==========================================================================================================
//		個体敵行為処理定義（剣敵）
//==========================================================================================================
void EnemyClass::Init() {
	int x;
	srand((unsigned)time(NULL));
	x = (int)(rand() % 3);

	srand((unsigned)time(NULL));
	x = (int)(rand() % 3);
	if(!inited)
	{
		TPLGetPalette(&ptTex, ENEMYPTTEX);
		TPLGetGXTexObjFromPalette(ptTex, &ptTexObj, 0);
		TPLGetPalette(&ball.Tex, LIGHTBALLTEX);
		TPLGetGXTexObjFromPalette(ball.Tex, &ball.TexObj, 0);
		if (x == 1) {
			TPLGetPalette(&Tex, ENEMYSPTEX);
			TPLGetGXTexObjFromPalette(Tex, &TexObj, 0);
			MaxHp = 10;
		}else{
			TPLGetPalette(&Tex, ENEMYTEX);
			TPLGetGXTexObjFromPalette(Tex, &TexObj, 0);
			MaxHp = 5;
		}
		
		inited = true;
	}

	InitialX = (f32)(rand() % Background.width);
	InitialY = (f32)(Background.height - InitialPlayerHeight - SwordEnemy[0].Height / 2);
	Y = InitialY;
	DisplayX = X;
	DisplayY = Y;
	broned = 1;
	deaded = 0;
	losed = 0;
	StatusStyle = EnemyRunAnime;
	ActionMod = PatrolMod;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	剣敵更新関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void	EnemyClass::Update() {

if(broned!=0 && deaded != 2)
{
		//**********************************************************************
		//	描画座標の更新
		//**********************************************************************
		DisplayX = X - Display.MoveDistance_x;
		DisplayY = Y - Display.MoveDistance_y;
}
if (broned==2 && deaded == 0) {
	

		if (InvincibleTime != 0) { InvincibleTime -= 1; }  //	無敵時間の処理
		switch (ActionMod)
		{
		case PatrolMod:
			Patrol();
			break;  //	巡回
		case HitMod:
			Hit();
			break;  //	damage受ける
		case TrackMod:
			Track();
			break;  //	敵探す
		case ReturnMod:
			Return();
			break;  //戻る
		case EvilMod:
			Evil();
			break;
		default:
			break;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	剣敵Animetion関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EnemyClass::Animetion() {
	
	if (broned == 2 && deaded == 0) {
		const u8 *Anime_data[3] = { AnimeRun,AnimeAttack,AnimeHit };
		const u8 *ptAnime = Anime_data[StatusStyle];
		if (*(ptAnime + cnt) == 0xff) {
			cnt = 0;
		}
		if (*(ptAnime + cnt) == 0xfe) {
			StatusStyle = EnemyRunAnime;
			cnt = 0;
		}
		Ustart = ((*(ptAnime + cnt)) % (int)(1 / Uwidth))*Uwidth;
		Vstart = ((*(ptAnime + cnt)) / (int)(1 / Vheight))*Vheight;
		cnt += 1;
	}
	
	if (broned == 1) {
		if (ball.runBronAnime()) {
			broned = 2;
			Hp = MaxHp;
		}	
	}
	if (losed == 1) {
		if (ball.runLoseAnime()) {
			losed = 2;
		//	deaded=1;
		}
	}
	if (deaded == 1) {
		if (ball.runDeadAnime()) {
			deaded = 2;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	剣敵関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EnemyClass::Draw() {
	if (broned == 2 && deaded == 0) {
		Animetion();
		if (FacedLeft) {
			Draw2DCharacter(TexObj, DisplayX, DisplayY, Width, Height, Ustart, Vstart, Uwidth, Vheight);
		}
		else {
			DrawPlayerRev(TexObj, DisplayX, DisplayY, Width, Height, Ustart, Vstart, Uwidth, Vheight);
		}
	}
	
	if (broned == 1 || deaded == 1) {
		Animetion();
		ball.Draw(DisplayX, DisplayY);
	}
	if (losed == 1) {
		Animetion();
		ball.Draw2(DisplayX, DisplayY);
	}

	
	//HP
	if (broned != 0 && deaded != 2) {
		if (DisplayX>Display.width)
		{
			Draw2DCharacter(ptTexObj, Display.width - EnemyPtSize, DisplayY, EnemyPtSize, EnemyPtSize, 0.5, 0, 0.5, 1);

		}
		else if (DisplayX<0)
		{
			Draw2DCharacter(ptTexObj, EnemyPtSize, DisplayY, EnemyPtSize, EnemyPtSize, 0, 0, 0.5, 1);

		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	剣敵damage受ける開始関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void	EnemyClass::HitOn() {
	HitCnt = 0;
	cnt = 0;
	StatusStyle = EnemyHitAnime;
	ActionMod = HitMod;
	InvincibleTime = 30;

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	剣敵damage受ける処理関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EnemyClass::Hit() {
	extern PlayerClass Player;
	HitCnt += 1;
	if (Player.X <= X) {
		X += 4;
	}
	else
	{
		X -= 4;
	}
	Y -= 10 - 0.98f*HitCnt;
	if (Y > InitialY) {
		Y = InitialY;
		HitCnt = 0;
		cnt = 0;
		Hp -= 1;
		if (Hp <= 0) {
			Hp = 0;
			deaded = 1;
		}
		StatusStyle = EnemyRunAnime;
		ActionMod = PatrolMod;
	}

}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	剣敵damage受ける開始関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void	EnemyClass::EvilOn() {
	extern PlayerClass Player;
	HitCnt = 0;
	cnt = 0;
	StatusStyle = EnemyHitAnime;
	ActionMod = EvilMod;
	InvincibleTime = 30;
	if (Player.FacedRight) {
		X = Player.X + 49 - 16;
	}else{
		X = Player.X - 49 + 16;
	}
	losed = 1;
	Y = Player.Y -16;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	剣敵EvilHit受ける処理関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EnemyClass::Evil() {
	extern PlayerClass Player;
	HitCnt += 1;
	if (HitCnt > 15) {
		HitCnt = 0;
		Hp = 0;
		deaded = 1;
		ActionMod = PatrolMod;
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	剣敵敵探す関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EnemyClass::Track() {
	extern PlayerClass Player;
	if (StatusStyle != EnemyAttAnime) {
		if (Player.X >= X) {
			if (FacedLeft) { 
				FacedLeft = false; 
				X += 32;
			}
			if (!PlayerHit()) { X += SWORDENEMYSPEED; }
		}
		else {
			if (!FacedLeft) { 
				FacedLeft = true; 
				X -= 32;
			}
			if (!PlayerHit()) { X -= SWORDENEMYSPEED; }
		}
		if (X - Player.X > 600 || X - Player.X < -600) {
			ActionMod = ReturnMod;
		}  //	Player が離せば戻る
		if ((X - Player.X > 0 && X - Player.X < 64) || (X - Player.X<0 && X - Player.X >-64)) {
			StatusStyle = EnemyAttAnime;
			cnt = 0;
		}  
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	剣敵巡回関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EnemyClass::Patrol() {
	extern PlayerClass Player;

	if (FacedLeft) {
		if (!PlayerHit()) { X -= SWORDENEMYSPEED; }
		if (X < InitialX - 150 || X <= 0) {
			if (!PlayerHit()) { X += SWORDENEMYSPEED * 2; }
			FacedLeft = !FacedLeft;
			X += 32;
		}
	}
	else {
		if (!PlayerHit()) { X += SWORDENEMYSPEED; }
		if (X > InitialX + 150 || X >= Background.width) {
			if (!PlayerHit()) { X -= SWORDENEMYSPEED * 2; }
			FacedLeft = !FacedLeft;
			X -= 32;
		}
	}

	if ((X - Player.X>0 &&X - Player.X < 300) || (X - Player.X<0 &&X - Player.X >-300)) {
		ActionMod = TrackMod;
	}//	Player が接近すると「敵探す」を処理


}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	剣敵戻る関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EnemyClass::Return() {
	extern PlayerClass Player;
	if (InitialX >= X) {
		if (FacedLeft) { FacedLeft = false; }
		if (!PlayerHit()) { X += SWORDENEMYSPEED; }
	}
	else {
		if (!FacedLeft) { FacedLeft = true; }
		if (!PlayerHit()) { X -= SWORDENEMYSPEED; }
	}

	if ((X - InitialX>0 && X - X < 30) || (X - InitialX<0 && X - InitialX >-30)) {
		ActionMod = PatrolMod;
	}
	if ((X - Player.X>0 && X - Player.X < 300) || (X - Player.X<0 && X - Player.X >-300)) {
		ActionMod = TrackMod;
	}
}




void EnemyClass::Ball::Draw(float x, float y) {
	Draw2DCharacter(TexObj, x, y, Width, Height, Ustart, Vstart, Uwidth, Vheight);
}

void EnemyClass::Ball::Draw2(float x, float y) {
	extern PlayerClass Player;
	float X;
	if (Player.FacedRight) {
		X = x - cnt2;
	}else {
		X = x + cnt2;
	}
	Draw2DCharacter(TexObj, X, y, Width, Height, Ustart2, Vstart2, Uwidth, Vheight);
}
