//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		îwåièàóùä÷êî
//		ójïX
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include	"main.h"
#include "DrawPolygon.h"
#include "Player.h"
//#include "input.h"
#include "Background.h"
#include "Quantitative.h"

//==========================================================================================================
//		íËã`
//==========================================================================================================
//==========================================================================================================
extern ImaginaryBackground Background;	// âºëzîwåiñ êœë„ì¸
extern DisplayClass Display;	// âºëzcameraë„ì¸
extern u8 Status;	// game Modeë„ì¸

ImageClass *Image;
ImageClass Scren;
//ImageClass Land;
ImageClass Title;
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


void ImageClass::Init() {
switch (Status)
	{
		//**********************************************************************
		//	TitleâÊñ èâä˙âª
		//**********************************************************************
	case TITLE:
		TPLGetPalette(&Title.Tex, TITLETEX);
		TPLGetGXTexObjFromPalette(Title.Tex, &Title.TexObj, 0);
		TPLGetPalette(&Titleint.Tex, TITLEINTTEX);
		TPLGetGXTexObjFromPalette(Titleint.Tex, &Titleint.TexObj, 0);

		Title.Height = Display.height;
		Title.Width = Display.width;
		Title.Y = Display.height / 2;
		Title.DisplayY = Title.Y;
		Title.X = Display.width/2;
		Title.DisplayX = Title.X;
		Titleint.Width = 201;
		Titleint.Height = 30;
		Titleint.X = Display.width - 50 - Titleint.Width / 2;
		Titleint.Y = Display.height - 143 - Titleint.Height / 2;
		LOGOBigging = 1.0f;
		Bingging = true;
		break;
	case GAME_START:
			TPLGetPalette(&Scren.Tex, SCRENTEX);
	TPLGetGXTexObjFromPalette(Scren.Tex, &Scren.TexObj, 0);

	TPLGetPalette(&LandPixel[0].Tex, LANDPIXELTEX);
	TPLGetGXTexObjFromPalette(LandPixel[0].Tex, &LandPixel[0].TexObj, 0);

	TPLGetPalette(&Footing[0].Tex, LANDTEX);
	TPLGetGXTexObjFromPalette(Footing[0].Tex, &Footing[0].TexObj, 0);
	TPLGetPalette(&Grass[0].Tex, GRASSTEX);
	TPLGetGXTexObjFromPalette(Grass[0].Tex, &Grass[0].TexObj, 0);

	LandPixel[0].Width = 60;
	LandNum = Background.width / LandPixel[0].Width + 1;
	//LandNum = 41;
	for (int i = 0; i < LandNum; i++) {
		LandPixel[i].Height = 160;
		LandPixel[i].Width = 60;
		LandPixel[i].Y = SCREEN_HEIGHT - LandPixel[i].Height / 2;
		LandPixel[i].DisplayY = LandPixel[i].Y;
		LandPixel[i].X = LandPixel[i].Width / 2 + i*LandPixel[i].Width;
		LandPixel[i].DisplayX = LandPixel[i].X;
	}
	Grass[0].Width = 60;
	GrassNum = Background.width / Grass[0].Width + 1;
	for (int i = 0; i < GrassNum; i++) {
		Grass[i].Height = 120;
		Grass[i].Width = 60;
		Grass[i].Y = SCREEN_HEIGHT - Grass[i].Height / 2;
		Grass[i].DisplayY = Grass[i].Y;
		Grass[i].X = Grass[i].Width / 2 + i*Grass[i].Width;
		Grass[i].DisplayX = Grass[i].X;
	}
	Scren.Width = SCREEN_WIDTH;
	Scren.Height = SCREEN_HEIGHT;
	Scren.X = Scren.Width / 2;
	Scren.Y = Scren.Height / 2;
	FootingNum = 2;
	Footing[0].X = 1024;
	Footing[0].Y = SCREEN_HEIGHT - 220;
	Footing[1].X = 1224;
	Footing[1].Y = SCREEN_HEIGHT - 270;
	for (int i = 0; i < FootingNum; i++) {
		Footing[i].Height = 20;
		Footing[i].Width = 100;
		Footing[i].DisplayY = Footing[i].Y;
		Footing[i].DisplayX = Footing[i].X;
	}

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
	default:
		break;
	}

}
/*
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
*/
void ImageClass::TitleDraw(int fcnt) {
	Draw2DCharacter(Title.TexObj, Title.X, Title.Y, Title.Width, Title.Height, 0, 0, 1, 1);
	Draw2DCharacter(Titleint.TexObj, Titleint.X, Titleint.Y, Titleint.Width*LOGOBigging, Titleint.Height, 0, 0, 1, 1);
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
	for (int i = 0; i < GrassNum; i++) {
		Draw2DCharacter(Grass[0].TexObj, Grass[i].DisplayX, Grass[i].DisplayY, Grass[i].Width, Grass[i].Height, Grass[i].Ustart, Grass[i].Vstart, Grass[i].Uwidth, Grass[i].Vheight);
	}
}

