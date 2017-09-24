//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		”wŒiˆ—ŠÖ”
//		—j•X
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include	"main.h"
#include "DrawPolygon.h"
#include "Player.h"
//#include "input.h"
#include "Background.h"
#include "Quantitative.h"

//==========================================================================================================
//		’è‹`
//==========================================================================================================
//==========================================================================================================
extern ImaginaryBackground Background;	// ‰¼‘z”wŒi–ÊÏ‘ã“ü
extern DisplayClass Display;	// ‰¼‘zcamera‘ã“ü
extern u8 Status;	// game Mode‘ã“ü
extern StairClass *Stair;

ImageClass *Image;
ImageClass Scren;
//ImageClass Land;
ImageClass Title;
ImageClass TitleOP;
ImageClass *thankPlay; 
ImageClass Titleint;
ImageClass Grass[128];
ImageClass LandPixel[256];
ImageClass Footing[256];
u8 LandNum;
u8 FootingNum;
u8 GrassNum;
ImaginaryBackground Background;
DisplayClass Display;
bool Bingging;
f32 LOGOBigging;
extern bool Inited;

void ImageClass::Init() {
switch (Status)
	{
		//**********************************************************************
		//	Title‰æ–Ê‰Šú‰»
		//**********************************************************************
	case TITLE:
	if(!Inited)
	{
			TPLGetPalette(&Title.Tex, TITLETEX);
		TPLGetGXTexObjFromPalette(Title.Tex, &Title.TexObj, 0);
		TPLGetPalette(&Titleint.Tex, TITLEINTTEX);
		TPLGetGXTexObjFromPalette(Titleint.Tex, &Titleint.TexObj, 0);
		TPLGetPalette(&TitleOP.Tex, TITLEOPTEX);
		TPLGetGXTexObjFromPalette(TitleOP.Tex, &TitleOP.TexObj, 0);


		Title.Height = Display.height;
		Title.Width = Display.width;
		Title.Y = Display.height / 2;
		Title.DisplayY = Title.Y;
		Title.X = Display.width/2;
		Title.DisplayX = Title.X;
		
		TitleOP.Height = Display.height;
		TitleOP.Width = Display.width;
		TitleOP.Y = Display.height / 2;
		TitleOP.DisplayY = TitleOP.Y;
		TitleOP.X = Display.width/2;
		TitleOP.DisplayX = TitleOP.X;
		
		Titleint.Width = 200;
		Titleint.Height = 30;
		Titleint.X = Display.width - 50 - Titleint.Width / 2;
		Titleint.Y = Display.height - 143 - Titleint.Height / 2;
	}
	
		LOGOBigging = 1.0f;
		Bingging = true;
		break;
	case GAME_START:
		if(!Inited)
		{
				TPLGetPalette(&Scren.Tex, SCRENTEX);
	TPLGetGXTexObjFromPalette(Scren.Tex, &Scren.TexObj, 0);

	TPLGetPalette(&LandPixel[0].Tex, LANDPIXELTEX);
	TPLGetGXTexObjFromPalette(LandPixel[0].Tex, &LandPixel[0].TexObj, 0);

	TPLGetPalette(&Footing[0].Tex, LANDTEX);
	TPLGetGXTexObjFromPalette(Footing[0].Tex, &Footing[0].TexObj, 0);
	TPLGetPalette(&Grass[0].Tex, GRASSTEX);
	TPLGetGXTexObjFromPalette(Grass[0].Tex, &Grass[0].TexObj, 0);

	Stair->Init();
	LandPixel[0].Width = 120;
	LandNum = Background.width / LandPixel[0].Width + 1;
	//LandNum = 41;
	for (int i = 0; i < LandNum; i++) {
		LandPixel[i].Height = 200;
			LandPixel[i].Width = 120;
		LandPixel[i].Y = Background.height - LandPixel[i].Height / 2+ BleedSize+20;
		LandPixel[i].DisplayY = LandPixel[i].Y;
		LandPixel[i].X = LandPixel[i].Width / 2 + i*LandPixel[i].Width;
		LandPixel[i].DisplayX = LandPixel[i].X;
	}
	Grass[0].Width = 120;
	GrassNum = Background.width / Grass[0].Width + 1;
	for (int i = 0; i < GrassNum; i++) {
		Grass[i].Height = 148;
			Grass[i].Width = 120;
		Grass[i].Y = Background.height - Grass[i].Height / 2+ BleedSize+20;
		Grass[i].DisplayY = Grass[i].Y;
		Grass[i].X = Grass[i].Width / 2 + i*Grass[i].Width;
		Grass[i].DisplayX = Grass[i].X;
	}
	Scren.Width = SCREEN_WIDTH;
	Scren.Height = SCREEN_HEIGHT;
	Scren.X = Scren.Width / 2;
	Scren.Y = Scren.Height / 2;
	FootingNum = 3;
	Footing[0].X = 1024;
	Footing[0].Y = Background.height - 220;
	Footing[1].X = 1224;
	Footing[1].Y = Background.height - 270;
	Footing[2].X = 1200;
	Footing[2].Y = Background.height - 760;
	for (int i = 0; i < FootingNum; i++) {
		Footing[i].Height = 20;
		Footing[i].Width = 100;
		Footing[i].DisplayY = Footing[i].Y;
		Footing[i].DisplayX = Footing[i].X;
	}
	Footing[2].Width = 400;
		}
		

		break;
	case GAME_WIN:
		if (thankPlay == NULL) {
		thankPlay = new ImageClass; 
		TPLGetPalette(&thankPlay->Tex, THANKPLAYTEX);
		TPLGetGXTexObjFromPalette(thankPlay->Tex, &thankPlay->TexObj, 0);
		}
		thankPlay->Width = 339.0f;
		thankPlay->Height = 110.0f;
		thankPlay->X = Display.width / 2;
		thankPlay->Y = Display.height / 2;
		thankPlay->DisplayX = thankPlay->X;
		thankPlay->DisplayY = thankPlay->Y;
		break;
	default:
		break;
	}


}

void ImageClass::Update(){
	switch (Status)
	{
	case TITLE:
		Titleint.cnt += 1;
		if (Titleint.cnt >= 60) { Titleint.cnt = 0; }
		if (Bingging) {
			LOGOBigging += 0.01f;
			if (LOGOBigging >= 1.2f) { Bingging = false; }
		}else{
			LOGOBigging -= 0.01f;
			if (LOGOBigging <= 0.8f) { Bingging = true; }
		}
		

		break;
	case GAME_PLAY:
		Display.Update(Background);
		//Land.Ustart += 0.0003f;
		Stair->Update();
		for (u8 i = 0; i < LandNum; i++) {
			LandPixel[i].Sync(Display);
		}
		for (u8 i = 0; i < FootingNum; i++) {
			Footing[i].Sync(Display);
		}
		for (u8 i = 0; i < GrassNum; i++) {
			Grass[i].Sync(Display);
		}
		break;
	case GAME_WIN:
		if (Bingging) {
			LOGOBigging += 0.01f;
			if (LOGOBigging >= 1.2f) { Bingging = false; }
		}
		else {
			LOGOBigging -= 0.01f;
			if (LOGOBigging <= 0.8f) { Bingging = true; }
		}
		break;
	default:
		break;
	}

}

void ImageClass::TitleDraw(bool LookOP) {
	if(!LookOP)
	{
		Draw2DCharacter(Title.TexObj, Title.X, Title.Y, Title.Width, Title.Height, 0, 0, 1, 1);

	}else{
		Draw2DCharacter(TitleOP.TexObj, TitleOP.X, TitleOP.Y, TitleOP.Width, TitleOP.Height, 0, 0, 1, 1);
	}
	Draw2DCharacter(Titleint.TexObj, Titleint.X, Titleint.Y, Titleint.Width*LOGOBigging, Titleint.Height, 0, 0, 1, 1);
}


void ImageClass::BackDraw() {
	switch (Status)
	{
	case GAME_PLAY:
		Draw2DCharacter(Scren.TexObj, Scren.X, Scren.Y, Scren.Width, Scren.Height, 0, 0, 1, 1);//sky
		Stair->Draw();
		for (int i = 0; i < LandNum; i++) {
			Draw2DCharacter(LandPixel[0].TexObj, LandPixel[i].DisplayX, LandPixel[i].DisplayY, LandPixel[i].Width, LandPixel[i].Height, LandPixel[i].Ustart, LandPixel[i].Vstart, LandPixel[i].Uwidth, LandPixel[i].Vheight);
		}

		for (int i = 0; i < FootingNum; i++) {
			Draw2DCharacter(Footing[0].TexObj, Footing[i].DisplayX, Footing[i].DisplayY, Footing[i].Width, Footing[i].Height, Footing[i].Ustart, Footing[i].Vstart, Footing[i].Uwidth, Footing[i].Vheight);
		}
		break;
	case GAME_WIN:
		Draw2DCharacter(Scren.TexObj, Scren.X, Scren.Y, Scren.Width, Scren.Height, 0, 0, 1, 1);//sky
		for (int i = 0; i < LandNum; i++) {
			Draw2DCharacter(LandPixel[0].TexObj, LandPixel[i].DisplayX, LandPixel[i].DisplayY, LandPixel[i].Width, LandPixel[i].Height, LandPixel[i].Ustart, LandPixel[i].Vstart, LandPixel[i].Uwidth, LandPixel[i].Vheight);
		}

		Draw2DCharacter(Titleint.TexObj, Titleint.X, Titleint.Y, Titleint.Width*LOGOBigging, Titleint.Height, 0, 0, 1, 1);
		Draw2DCharacter(thankPlay->TexObj, thankPlay->X, thankPlay->Y, thankPlay->Width, thankPlay->Height, 0, 0, 1, 1);

		break;
	default:
		break;
	}



}



void ImageClass::UpDraw() {
	for (int i = 0; i < GrassNum; i++) {
		Draw2DCharacter(Grass[0].TexObj, Grass[i].DisplayX, Grass[i].DisplayY, Grass[i].Width, Grass[i].Height, Grass[i].Ustart, Grass[i].Vstart, Grass[i].Uwidth, Grass[i].Vheight);
	}
}

