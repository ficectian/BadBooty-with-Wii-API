//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		当たり判定関数
//		曜氷
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#include	"main.h"
#include "DrawPolygon.h"
#include "Player.h"
#include "Background.h"
#include "Quantitative.h"

//==========================================================================================================
//		全てのPlayerに関する当たり判定処理定義
//==========================================================================================================
void PlayerClass::AllHitTest() {
	extern DisplayClass Display;  //  仮想camera代入
	extern EnemyClass SwordEnemy[64];	// 剣敵代入
	extern u8 SwordEnemyNum;	// 剣敵数代入
	const u8 *Anime_data[8] = { AnimeStation, AnimeRun, AnimeJump, AnimeDefense, AnimeAttack, AnimeHit ,AnimeClimb,AnimeEvilHit};
	const u8 *ptAnime = Anime_data[StatusStyle];	// 計算用Player Animation Cnt

	//**********************************************************************
	//	敵に攻撃の当たり判定
	//**********************************************************************
	if (*(ptAnime + cnt) == 33 || *(ptAnime + cnt) == 34||*(ptAnime + cnt) == 57 || *(ptAnime + cnt) == 59) {
		// 	剣敵に攻撃の当たり判定
		for (u8 i = 0; i < SwordEnemyNum; i++)
		{
			if (SwordEnemy[i].Hp > 0) {
				if (AttHit(SwordEnemy[i].HitBox_X(), SwordEnemy[i].HitBox_Y(), SwordEnemy[i].HitBox_Wdith, SwordEnemy[i].HitBox_Height)) {
					if (SwordEnemy[i].InvincibleTime == 0) {
						StopTime = 5;
					switch(attackMod)
					{
					case swordMod:
						PlaySample(&Samples[5]);
						break;
					default:
						PlaySample(&Samples[1]);
						break;
					}
						SwordEnemy[i].HitOn();
					}  // 無敵時間内ではない
				}  // 敵に当たる
			}  // 敵が生きている
		}
	}
	
	//**********************************************************************
	//	敵にEvilHitの当たり判定
	//**********************************************************************
	if (*(ptAnime + cnt) == 50 || *(ptAnime + cnt) == 51) {
		// 	剣敵に攻撃の当たり判定
		for (u8 i = 0; i < SwordEnemyNum; i++)
		{
			if (SwordEnemy[i].Hp > 0 && SwordEnemy[i].Hp <= SwordEnemy[i].MaxHp*2/5) {
				if (EvilHit(SwordEnemy[i].HitBox_X(), SwordEnemy[i].HitBox_Y(), SwordEnemy[i].HitBox_Wdith, SwordEnemy[i].HitBox_Height)) {
					if (SwordEnemy[i].InvincibleTime == 0) {
						StopTime = 10;
						SwordEnemy[i].EvilOn();
						PlaySample(&Samples[4]);
						attackMod = swordMod;
						Hp += SwordEnemy[i].Hp * 2;
						if (Hp > MaxHp) { Hp = MaxHp; }
					}  // 無敵時間内ではない
				}  // 敵に当たる
			}  // 敵が生きている
		}
	}

	//**********************************************************************
	//	敵にdamage受けの当たり判定
	//**********************************************************************
	// 	剣敵にdamage受けの当たり判定
	for (u8 i = 0; i < SwordEnemyNum; i++)
	{
		if (SwordEnemy[i].Hp > 0) {
			if (SwordEnemy[i].AnimeCnt() == 5 || SwordEnemy[i].AnimeCnt() == 6) {
				if (HitHit(SwordEnemy[i].AttBox_X(), SwordEnemy[i].AttBox_Y(), SwordEnemy[i].AttBox_Wdith, SwordEnemy[i].AttBox_Height)) {
					if (InvincibleTime == 0) {
						if (StatusStyle != HitStatus) {
							if ((StatusStyle == DefenseStatus) &&( (X < SwordEnemy[i].X && FacedRight) || (X > SwordEnemy[i].X && !FacedRight))) {
								// 防御すれば
								if (Display.ShockOn == 0) {Display.ShockOn = 1;}
								InvincibleTime = 30;
								PlaySample(&Samples[0]);
								Hp -= 1;
							}else {
								// 防御していない
								if (X < SwordEnemy[i].X) {EnemyInPlayerRight = true;}else {
									EnemyInPlayerRight = false;} // 敵方位判定
								PlaySample(&Samples[3]);
								HitOn();
							}
						}
					}  // 無敵時間内ではない
				}  // Playerに当たる
			}  // 敵攻撃Cntの時
		}  // 敵が生きている
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Player移動当たり判定関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PlayerClass::MoveHit() {
	extern EnemyClass SwordEnemy[64];  // 剣敵代入
	extern u8 SwordEnemyNum;	// 剣敵数代入
	
	// 	剣敵に当たり判定
	for (u8 i = 0; i < SwordEnemyNum; i++){
		if (SwordEnemy[i].Hp > 0) {
			if (SwordEnemy[i].PlayerHit()) {
				if (HitBox_X() < SwordEnemy[i].HitBox_X()) { 
					X -= PLAYERSPEED; 
				}else {
					X += PLAYERSPEED;
				}
				return true;
			}
		}  // 敵が生きている
	}
	if(BoundaryHitTest()){ return true; }
	return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Map当たり判定関数定義
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PlayerClass::BoundaryHitTest() {
	extern ImaginaryBackground Background;
	if (X - Width / 2 < BleedSize) {
		X += PLAYERSPEED;
		return true;
	}else if (X + Width / 2 > Background.width - BleedSize)	{
		X -= PLAYERSPEED;
		return true;
	}
	return false;

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
	f32 YY1 = Y;
	f32 YY2 = Y1 - H / 2;

	if (((YY1 + 64 >= YY2) && (YY1 - H <= YY2)) && ((XX1 + 24 >= XX2) && (XX1 - W <= XX2)))
	{
		return true;
	}
	else
	{
		return 	false;
	}

}



////--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
////		当たり判定処理関数定義
////--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool			HitTest(f32 X1, f32 Y1, f32 W1, f32 H1, f32 X2, f32 Y2, f32 W2, f32 H2)
{
	f32 XX1 = X1 - W1 / 2;
	f32	XX2 = X2 - W2 / 2;
	f32 YY1 = Y1 - H1 / 2;
	f32 YY2 = Y2 - H2 / 2;

	if (((YY1 + H1 >= YY2) && (YY1 - H2 <= YY2)) && ((XX1 + W1 >= XX2) && (XX1 - W2 <= XX2)))
	{
		return true;
	}
	else
	{
		return 	false;
	}

}
