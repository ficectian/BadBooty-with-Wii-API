//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		UIèàóùä÷êî
//		ójïX
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#include	"main.h"
#include "DrawPolygon.h"
#include "Player.h"
#include "Background.h"
#include "Quantitative.h"

//==========================================================================================================
//		íËã`
//==========================================================================================================
UIClass *GameUI;
UIClass	PlayerHeadBackground;
UIClass	PlayerHead;
UIClass	PlayerHP;
UIClass	PlayerHP1;
UIClass	PlayerHP2;
UIClass	EnemyHP;
UIClass	EnemyHPBackground;
extern bool Inited;

void UIClass::Init(){
	if(!Inited)
	{
		TPLGetPalette(&PlayerHeadBackground.Tex, HeadBackgroundTex);
		TPLGetGXTexObjFromPalette(PlayerHeadBackground.Tex, &PlayerHeadBackground.TexObj, 0);

		TPLGetPalette(&PlayerHead.Tex, HeadTex);
		TPLGetGXTexObjFromPalette(PlayerHead.Tex, &PlayerHead.TexObj, 0);
		TPLGetPalette(&PlayerHP.Tex, PlayerHPTEX);
		TPLGetGXTexObjFromPalette(PlayerHP.Tex, &PlayerHP.TexObj, 0);
		TPLGetPalette(&PlayerHP1.Tex, PlayerHPC1TEX);
		TPLGetGXTexObjFromPalette(PlayerHP1.Tex, &PlayerHP1.TexObj, 0);
		TPLGetPalette(&PlayerHP2.Tex, PlayerHPC2TEX);
		TPLGetGXTexObjFromPalette(PlayerHP2.Tex, &PlayerHP2.TexObj, 0);
		TPLGetPalette(&EnemyHPBackground.Tex, ENEMYHPBGTEX);
		TPLGetGXTexObjFromPalette(EnemyHPBackground.Tex, &EnemyHPBackground.TexObj, 0);
		TPLGetPalette(&EnemyHP.Tex, ENEMYHPTEX);
		TPLGetGXTexObjFromPalette(EnemyHP.Tex, &EnemyHP.TexObj, 0);
		
		PlayerHeadBackground.Width = 270;
		PlayerHeadBackground.Height = 77;
		PlayerHeadBackground.X = 10 + PlayerHeadBackground.Width / 2;
		PlayerHeadBackground.Y = 10 + PlayerHeadBackground.Height / 2;

		PlayerHead.Width = 68;
		PlayerHead.Height = 68;
		PlayerHead.X = 10 + 5 + PlayerHead.Width / 2;
		PlayerHead.Y = 10 + 5 + PlayerHead.Height / 2;

		PlayerHP.Width = 10;
		PlayerHP.Height = 22;
		PlayerHP.X = 10 + 68 + PlayerHP.Width / 2;
		PlayerHP.Y = 10 + 44 + PlayerHP.Height / 2;
		PlayerHP.Uwidth = (f32)1 / 2;
		PlayerHP.Vheight = (f32)1 / 2;
		
		EnemyHPBackground.Width = 82;
		EnemyHPBackground.Height = 6;
		EnemyHP.Width = 80/10;
		EnemyHP.Height = 4;

	}

}

void UIClass::Draw(int HP) {
	extern EnemyClass SwordEnemy[10];
	extern u8 SwordEnemyNum;
	u8 EnemyHPUI;
	for (u8 i = 0; i < SwordEnemyNum; i++) {
		if (SwordEnemy[i].Hp > 0) {
			EnemyHPUI = (u8)(SwordEnemy[i].Hp * 10 / SwordEnemy[i].MaxHp);
			Draw2DCharacter(EnemyHPBackground.TexObj, SwordEnemy[i].DisplayX, SwordEnemy[i].DisplayY - 70, EnemyHPBackground.Width, EnemyHPBackground.Height, EnemyHPBackground.Ustart, EnemyHPBackground.Vstart, EnemyHPBackground.Uwidth, EnemyHPBackground.Vheight);
			for (int n = 0; n < EnemyHPUI; n++) {
				Draw2DCharacter(EnemyHP.TexObj,(SwordEnemy[i].DisplayX - 40 + EnemyHP.Width / 2) + n*EnemyHP.Width, SwordEnemy[i].DisplayY - 70, EnemyHP.Width, EnemyHP.Height, EnemyHP.Ustart, EnemyHP.Vstart, EnemyHP.Uwidth, EnemyHP.Vheight);
			}
		}
		
	}
	if (HP >= 20) {
		for (u8 i = 0; i < 20; i++) {
			Draw2DCharacter(PlayerHP1.TexObj, PlayerHP.X + i*PlayerHP.Width, PlayerHP.Y, PlayerHP.Width, PlayerHP.Height, PlayerHP.Ustart, PlayerHP.Vstart, PlayerHP.Uwidth, PlayerHP.Vheight);
		}
		for (u8 i = 0; i < HP - 20; i++) {
			Draw2DCharacter(PlayerHP2.TexObj, PlayerHP.X + i*PlayerHP.Width, PlayerHP.Y, PlayerHP.Width, PlayerHP.Height, PlayerHP.Ustart, PlayerHP.Vstart, PlayerHP.Uwidth, PlayerHP.Vheight);
		}
	}
	else
	{
		for (u8 i = 0; i < HP; i++) {
			Draw2DCharacter(PlayerHP1.TexObj, PlayerHP.X + i*PlayerHP.Width, PlayerHP.Y, PlayerHP.Width, PlayerHP.Height, PlayerHP.Ustart, PlayerHP.Vstart, PlayerHP.Uwidth, PlayerHP.Vheight);
		}
	}
	//Draw2DCharacter(PlayerHP.TexObj, 10 + 68 + 100, PlayerHP.Y, PlayerHP.Width * 20, PlayerHP.Height, PlayerHP.Ustart, PlayerHP.Vstart, 0.5f, PlayerHP.Vheight);

	Draw2DCharacter(PlayerHeadBackground.TexObj, PlayerHeadBackground.X, PlayerHeadBackground.Y, PlayerHeadBackground.Width, PlayerHeadBackground.Height, PlayerHeadBackground.Ustart, PlayerHeadBackground.Vstart, PlayerHeadBackground.Uwidth, PlayerHeadBackground.Vheight);
	Draw2DCharacter(PlayerHead.TexObj, PlayerHead.X, PlayerHead.Y, PlayerHead.Width, PlayerHead.Height, PlayerHead.Ustart, PlayerHead.Vstart, PlayerHead.Uwidth, PlayerHead.Vheight);


}

