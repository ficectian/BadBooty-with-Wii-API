//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		character Hear File
//		—j•X
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include "Quantitative.h"

enum {
	StationStatus,
	RunStatus,
	JumpStatus,
	DefenseStatus,
	AttackStatus,
	HitStatus,
	ClimbStatus,
	EvilHitStatus
};
enum {
	EnemyRunAnime,
	EnemyAttAnime,
	EnemyHitAnime
};

enum {
	PatrolMod,
	HitMod,
	TrackMod,
	ReturnMod,
	EvilMod
};

//=========================================
//		\‘¢‘Ì’è‹`
//=========================================
class PlayerClass{
public:
	f32 X;
	f32 Y;
	f32 DisplayX;
	f32 DisplayY;
	f32 Width;
	f32 Height;
	int	Hp;
	u8 InvincibleState;

	f32 AttBox_X() {
		if (FacedRight) {
			return (f32)X + 1;
		}
		else{
			return (f32)X - 45;
		}
	}
	f32 AttBox_Y() {
		return (f32)Y - 61;
	}
	f32 AttBox_Wdith;
	f32 AttBox_Height;
	f32 EvilBox_Wdith;
	f32 EvilBox_Height;
	f32 EvilBox_X(){
		if (FacedRight) {
			return (f32)X - 5;
		}
		else {
			return (f32)X -64;
		}
	}
	f32 EvilBox_Y() {
		return (f32)Y - 32;
	}
	f32 HitBox_X() {
		if (FacedRight) {
			return (f32)X - 43;
		}
		else {
			return (f32)X;
		}
	}
	f32 HitBox_Y() {
		return (f32)Y - 47;
	}
	f32 HitBox_Wdith;
	f32 HitBox_Height;
	u8 StatusStyle;
	void Init();
	void Update();
	void Draw();
	bool FallHitTest(f32, f32, f32, f32);
	void Operation();
	void Jump();
	void Attack();
	void Animetion();
	bool HitStair();
	void AllHitTest();
	bool BoundaryHitTest();
	bool FacedRight;
	void Climb();
	void EvilHit();
	u8 AnimeCnt(s8 n) {
		const u8  *Anime_data[8] = { AnimeStation, AnimeRun, AnimeJump, AnimeDefense, AnimeAttack, AnimeHit ,AnimeClimb,AnimeEvilHit};
		const u8 *ptAnime = Anime_data[StatusStyle];
		return *(ptAnime + cnt+n);
	}
	u8 StopTime;
	PlayerClass() {
		InvincibleTime = 0;
		Width = 128;
		Height = 128;
		Ustart = 0.0f;
		Uwidth = (f32)1 / 8;
		Vstart = 0.0f;
		Vheight = (f32)1 / 8;
		FacedRight = true;
		StatusStyle = StationStatus;
		cnt = 0;
		JumpCnt = 0;
		InDoubleJumpStatus = false;
		InFall = false;
		AttBox_Wdith = 46.0f;
		AttBox_Height = 69.0f;
		HitBox_Wdith = 43.0f;
		HitBox_Height = 111.0f;
		EvilBox_Wdith = 69.0f;
		EvilBox_Height = 42.0f;
		StopTime = 0;
		EnemyInPlayerRight = false;
		 
	}
private:
	TPLPalettePtr Tex;
	GXTexObj TexObj;
	TPLPalettePtr DeadTex;
	GXTexObj DeadTexObj;
	TPLPalettePtr InvincibleTex;
	GXTexObj InvincibleTexObj;
	f32 JumpStartY;
	int MaxHp;
	f32 Ustart;
	f32 Vstart;
	f32 Uwidth;
	f32 Vheight;
	bool InDoubleJumpStatus;

	static const u8 AnimeStation[64];
	static const u8 AnimeRun[64];
	static const u8 AnimeJump[64];
	static const u8 AnimeDefense[64];
	static const u8 AnimeAttack[64]; 
	static const u8 AnimeHit[64]; 
	static	const u8 AnimeClimb[64];
	static	const u8 AnimeEvilHit[64];
	//static 	const u8 *Anime_data[6];// = { AnimeStation, AnimeRun, AnimeJump, AnimeDefense, AnimeAttack, AnimeHit };

	u8 cnt;
	u8 JumpCnt;

	f32 Initial_x;
	f32 Initial_y;
	bool InFall;
	int OnStairNum;
	bool MoveHit();
	bool AttHit(f32 x, f32 y, f32 w, f32 h) {
		if (((AttBox_Y() + AttBox_Height >= y) && (AttBox_Y() - h <= y)) && ((AttBox_X() + AttBox_Wdith >= x) && (AttBox_X() - w <= x)))
		{
			return true;
		}
		else
		{
			return 	false;
		}
	}
	bool EvilHit(float x, float y, float w, float h) {
		if (((EvilBox_Y() + EvilBox_Height >= y) && (EvilBox_Y() - h <= y)) && ((EvilBox_X() + EvilBox_Wdith >= x) && (EvilBox_X() - w <= x)))
		{
			return true;
		}
		else
		{
			return 	false;
		}
	}
	bool HitHit(f32 x, f32 y, f32 w, f32 h) {
		if (((HitBox_Y() + HitBox_Height >= y) && (HitBox_Y() - h <= y)) && ((HitBox_X() + HitBox_Wdith >= x) && (HitBox_X() - w <= x)))
		{
			return true;
		}
		else
		{
			return 	false;
		}
	}
	void HitOn();
	void Hit();
	u8 HitCnt;
	u8 InvincibleTime;
	bool EnemyInPlayerRight;
};


class EnemyClass {


public:
	f32 DisplayX;
	f32 DisplayY;
	f32 X;	//”N—î
	f32 Y;	//UŒ‚—Í
	f32 HitBox_X() {
		if (FacedLeft) {
			return (f32)X - 3;
		}
		else {
			return (f32)X - 35;
		}
	}
	f32 HitBox_Y() {
		return (f32)Y - 39;
	}
	f32 HitBox_Wdith;
	f32 HitBox_Height;

	f32 AttBox_X() {
		if (FacedLeft) {
			return (f32)X - 61;
		}
		else {
			return (f32)X + 4;
		}
	}
	f32 AttBox_Y() {
		return (f32)Y - 39;
	}
	f32 AttBox_Wdith;
	f32 AttBox_Height;
	int Hp;	//‘Ì—Í
	int MaxHp;
	void AllInit();
	void AllUpdate();
	void AllDraw();
	bool AllHaveHp();
	//void Sync(DisplayClass Display) {
	//	DisplayX = X - Display.MoveDistance.x;
	//}
	u8 ActionMod;
	void HitOn();
	void EvilOn();
	bool FacedLeft;
	u8 InvincibleTime;
	void Track();
	void Return();
	bool PlayerHit() {
		extern PlayerClass Player;
		if (((HitBox_Y() + HitBox_Height >= Player.HitBox_Y()) && (HitBox_Y() - Player.HitBox_Height <= Player.HitBox_Y())) && ((HitBox_X() + HitBox_Wdith >= Player.HitBox_X()) && (HitBox_X() - Player.HitBox_Wdith <= Player.HitBox_X())))
		{
			return true;
		}
		else
		{
			return 	false;
		}
	}
	EnemyClass() {
		//bool bret = DXLoadTexture(ENEMYTEX, &Tex);
		Width = 128;
		Height = 128;
		InvincibleTime = 0;
		MaxHp = 5;
		Hp = 0;
		Ustart = 0.0f;
		Uwidth = (f32)1 / 4;
		Vstart = 0.0f;
		Vheight = (f32)1 / 4;
		FacedLeft = true;
		StatusStyle = EnemyRunAnime;
		cnt = 0;
		ActionMod = PatrolMod;
		HitBox_Wdith = 38.0f;
		HitBox_Height = 103.0f;
		AttBox_Wdith = 57.0f;
		AttBox_Height = 60.0f;
	}
	u8 AnimeCnt() {
		const u8 *Anime_data[3] = { AnimeRun,AnimeAttack,AnimeHit };
		const u8 *ptEnemyAnime = Anime_data[StatusStyle];
		return *(ptEnemyAnime + cnt);
	}
private:
	TPLPalettePtr Tex;
	GXTexObj TexObj;
	TPLPalettePtr DeadTex;
	GXTexObj DeadTexObj;
	f32 InitialX;
	f32 InitialY;

	f32 Width;
	f32 Height;
	f32 Ustart;
	f32 Vstart;
	f32 Uwidth;
	f32 Vheight;


	void Animetion();
	void	Patrol();
	void Update();
	void Draw();
	void Hit();
	void Evil();
	u8 HitCnt;
	static const u8 AnimeRun[64]; 
	static const u8 AnimeAttack[128];  
	static const u8 AnimeHit[64];
	//const u8 *Anime_data[3] = { AnimeRun, AnimeAttack, AnimeHit };
	u8 StatusStyle;
	u8 cnt;
};



bool	HitTest(f32 X1, f32 Y1, f32 W1, f32 H1, f32 X2, f32 Y2, f32 W2, f32 H2);

