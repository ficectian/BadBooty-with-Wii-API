//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		敵AI関数
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
extern DisplayClass Display;
extern ImaginaryBackground Background;
extern bool Inited;
EnemyClass *Enemy;
EnemyClass SwordEnemy[64];
u8 SwordEnemyNum;
const u8 EnemyClass::AnimeRun[64] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0xff };//0xff：終了コード
const u8 EnemyClass::AnimeAttack[128] = { 4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,6,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0xfe };
	
const u8 EnemyClass::AnimeHit[64] = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0xff };

//==========================================================================================================
//		全体敵行為処理定義
//==========================================================================================================
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	全体敵初期化関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void	EnemyClass::AllInit() {
	//extern DisplayClass Display;
	extern u8 GameLoop;
	SwordEnemyNum = 3;
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
	}

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
		if (SwordEnemy[i].Hp > 0) {
			SwordEnemy[i].Draw();
		}
	}
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	全体敵HP判定関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool	EnemyClass::AllHaveHp() {
	for (int i = 0; i < SwordEnemyNum; i++) {
		if (SwordEnemy[i].Hp > 0) { return true; }
	}

	return false;
}

//==========================================================================================================
//		個体敵行為処理定義（剣敵）
//==========================================================================================================
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	剣敵更新関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void	EnemyClass::Update() {
if (Hp > 0) {
		//**********************************************************************
		//	描画座標の更新
		//**********************************************************************
		DisplayX = X - Display.MoveDistance_x;
		DisplayY = Y - Display.MoveDistance_y;

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

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	剣敵関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void EnemyClass::Draw() {
	Animetion();

	if (FacedLeft) {
		Draw2DCharacter(TexObj, DisplayX, DisplayY, Width, Height, Ustart, Vstart, Uwidth, Vheight);
	}
	else {
		DrawPlayerRev(TexObj, DisplayX, DisplayY, Width, Height, Ustart, Vstart, Uwidth, Vheight);
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


