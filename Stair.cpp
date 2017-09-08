//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		äKíièàóùä÷êî
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

StairClass *Stair;
StairClass StairL[128];
u8 StairNum;
extern DisplayClass Display;
extern ImaginaryBackground Background;

void StairClass::Init() {
	TPLGetPalette(&StairL[0].Tex, STAIRTEX);
	TPLGetGXTexObjFromPalette(StairL[0].Tex, &StairL[0].TexObj, 0);


	StairNum = (int)((Background.height - 760 - StairL[0].Height / 2) - InitialPlayerHeight) / StairL[0].Height;
	for (int i = 0; i < StairNum; i++) {
		StairL[i].X = 1400;
		StairL[i].Y = (float)((Background.height - 760) + i*StairL[i].Height);
		StairL[i].DisplayX = StairL[i].X;
		StairL[i].DisplayY = StairL[i].Y;
	}

}

void StairClass::Update() {
	for (int i = 0; i < StairNum; i++) {
		StairL[i].Sync(Display);
	}
}

void StairClass::Draw() {
	for (int i = 0; i < StairNum; i++) {
		Draw2DCharacter(StairL[0].TexObj, StairL[i].DisplayX, StairL[i].DisplayY, StairL[i].Width, StairL[i].Height, StairL[i].Ustart, StairL[i].Vstart, StairL[i].Uwidth, StairL[i].Vheight);

	}

}


