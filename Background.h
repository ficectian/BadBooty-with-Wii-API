//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		”wŒiHear File
//		—j•X
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "Quantitative.h"

typedef struct {
	u16 height;
	u16 width;
}ImaginaryBackground;

class DisplayClass {
public:
	f32 X;
	f32 Y;
	f32 Left_X() {
		return X - width / 2;
	}
	f32 Up_Y() {
		return Y - height / 2;
	}
	f32 Right_X() {
		return X + width / 2;
	}
	f32 Down_Y() {
		return Y + height / 2;
	}
//	f32 Fix_x;
//	f32 Fix_y;
	f32 MoveDistance_x;
	f32 MoveDistance_y;
	u16 height;
	u16 width;
	u8 ShockOn;
	void Init(ImaginaryBackground Background) {
		X = Background.width/2;
		Y = Background.height / 2;;
	
	}
	void Shock(u8 ShockLv){
		if (cnt >= 40) {
			ShockOn = 0;
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
		case 30:
		case 31:
		case 32:
		case 33:
		case 34:
			X -= ShockLv;
			Y -= ShockLv - 1.0f;

			break;
		default:
			X += ShockLv;
			Y += ShockLv - 1.0f;

			break;
		}

		cnt += 1;
	}
	void Update(ImaginaryBackground Background) {
		extern PlayerClass Player;
		if (ShockOn != 0) {
			Shock(ShockOn);
		}
		/*
		else
		{*/
			if (Player.X - width / 2 > BleedSize && Player.X + width / 2 < Background.width - BleedSize) {
				if (X > Player.X) {
					if (Player.StatusStyle == DefenseStatus) {
						X -= PLAYERSPEED/2;
					}
					else if (Player.StatusStyle == JumpStatus||Player.StatusStyle == HitStatus) {
						X = Player.X;
					}
					else{
						X -= PLAYERSPEED;
					}
				}
				if (X < Player.X) {
					if (Player.StatusStyle == DefenseStatus) {
						X += PLAYERSPEED / 2;
					}
					else if (Player.StatusStyle == JumpStatus||Player.StatusStyle == HitStatus) {
						X = Player.X;
					}
					else {
						X += PLAYERSPEED;
					}
				}
			}
			if (Player.Y - height / 2 > BleedSize && Player.Y + height / 2 < Background.height - BleedSize) {
				if (Y > Player.Y - height / 2 + InitialPlayerHeight + Player.Height / 2) {
					if (Player.StatusStyle == JumpStatus||Player.StatusStyle == HitStatus) {
						Y = Player.Y - height / 2 + InitialPlayerHeight + Player.Height / 2;
					}else{
						Y -= PLAYERSPEED;
					}
				}
				
				if (Y < Player.Y - height / 2 + InitialPlayerHeight + Player.Height / 2) {
					if (Player.StatusStyle == JumpStatus||Player.StatusStyle == HitStatus) {
						Y = Player.Y - height / 2 + InitialPlayerHeight + Player.Height / 2;
					}else {
						Y += PLAYERSPEED;
					}
				}
				 
			}
			
	//	}
		MoveDistance_x = Left_X();
		MoveDistance_y = Up_Y();
	}
	DisplayClass()
	{
		height = SCREEN_HEIGHT;
		width = SCREEN_WIDTH;
		MoveDistance_x = 0;
		MoveDistance_y = 0;
		ShockOn = false;
		cnt = 0;
	
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
		DisplayY = Y - Display.MoveDistance_y;
	}
	f32 Width;
	f32 Height;
	TPLPalettePtr Tex;
	
	ImageClass() {
		Ustart = 0.0f;
		Uwidth = 1.0f;
		Vstart = 0.0f;
		Vheight = 1.0f;
		cnt = 0;
	}
private:
	GXTexObj TexObj;
	f32 Ustart;
	f32 Vstart;
	f32 Uwidth;
	f32 Vheight;
	u8 cnt;


};

class UIClass {
public:


	void Init();
	/*void Update();
	void BackDraw();*/
	void Draw(int);
	UIClass() {
		Ustart = 0.0f;
		Vstart = 0.0f;
		Uwidth = 1.0f;
		Vheight = 1.0f;
	}
private:
	TPLPalettePtr Tex;
	GXTexObj TexObj;
	f32	X;
	f32	Y;
	f32 Width;
	f32 Height;
	f32 Ustart;
	f32 Vstart;
	f32 Uwidth;
	f32 Vheight;
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

