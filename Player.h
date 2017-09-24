//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		character Hear File
//		曜氷
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

enum PlayerAttackMod
{
	defaultMod,
	swordMod
};

//=========================================
//		構造体定義
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
	f32 swordBox_Wdith;
	f32 swordBox_Height;
	f32 swordBox_X() {
		if (FacedRight) {
			return (f32)X - 19;
		}else {
			return (f32)X + 19 - 100;

		}
	}
	f32 swordBox_Y() {
		return (f32)Y - 20;
	}
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
	const u8  *Anime_data[9] = { AnimeStation, AnimeRun, AnimeJump, AnimeDefense, AnimeAttack, AnimeHit, AnimeClimb, AnimeEvilHit, AnimeSwordHit };
		const u8 *ptAnime;// = Anime_data[StatusStyle];
		if (StatusStyle != AttackStatus) {
			ptAnime = Anime_data[StatusStyle];
		}else{
			switch (attackMod)
			{
			case swordMod:
				ptAnime = Anime_data[8];
				break;
			default:
				ptAnime = Anime_data[StatusStyle];
				break;
			}

		}
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
		swordBox_Wdith = 100.0f;
		swordBox_Height = 45.0f;
		EvilBox_Wdith = 69.0f;
		EvilBox_Height = 42.0f;
		StopTime = 0;
		EnemyInPlayerRight = false;
		attackMod = defaultMod;
		//attackMod = swordMod;
		 
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
	PlayerAttackMod attackMod;

	static const u8 AnimeStation[64];
	static const u8 AnimeRun[64];
	static const u8 AnimeJump[64];
	static const u8 AnimeDefense[64];
	static const u8 AnimeAttack[64]; 
	static const u8 AnimeHit[64]; 
	static	const u8 AnimeClimb[64];
	static	const u8 AnimeEvilHit[64];
	static	const u8 AnimeSwordHit[64];
	//static 	const u8 *Anime_data[6];// = { AnimeStation, AnimeRun, AnimeJump, AnimeDefense, AnimeAttack, AnimeHit };

	u8 cnt;
	u8 JumpCnt;

	f32 Initial_x;
	f32 Initial_y;
	bool InFall;
	int OnStairNum;
	bool MoveHit();
	bool AttHit(f32 x, f32 y, f32 w, f32 h) {
		f32 X, Y, width, height;
			switch (attackMod)
			{
			case defaultMod:
				X = AttBox_X();
				Y = AttBox_Y();
				width = AttBox_Wdith;
				height = AttBox_Height;
				break;
			case swordMod:
				X = swordBox_X();
				Y = swordBox_Y();
				width = swordBox_Wdith;
				height = swordBox_Height;
				break;
			default:
				break;
			}
			if (((Y + height >= y) && (Y - h <= y)) && ((X + width >= x) && (X - w <= x)))
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
	f32 X;	//年齢
	f32 Y;	//攻撃力
	bool inited;
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
	int Hp;	//体力
	int MaxHp;
	void AllInit();
	void AllUpdate();
	void AllDraw();
	bool AllHaveHp();
	//void Sync(DisplayClass Display) {
	//	DisplayX = X - Display.MoveDistance.x;
	//}
	void Init();
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
		broned = 0;
		deaded = 0;
		losed = 0;
		inited = false;
	//	ball = new Ball;
	}
	~EnemyClass() {
//		delete ball;
	}
	u8 AnimeCnt() {
		const u8 *Anime_data[3] = { AnimeRun,AnimeAttack,AnimeHit };
		const u8 *ptEnemyAnime = Anime_data[StatusStyle];
		return *(ptEnemyAnime + cnt);
	}
private:
	TPLPalettePtr Tex;
	GXTexObj TexObj;
	TPLPalettePtr ptTex;
	GXTexObj ptTexObj;
	u8 broned;
	u8 deaded;
	u8 losed;
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
	
	class Ball
	{
	public:
		TPLPalettePtr Tex;
		GXTexObj TexObj;

		Ball() {
			Height = 64.0f;
			Width = 64.0f;
			Ustart = 0.0f;
			Vstart = 0.0f;
			Ustart2 = 0.0f;
			Vstart2 = 0.0f;
			Uwidth = (float)1 / 4;
			Vheight = (float)1 / 4;
			cnt = 0;
			cnt2 = 0;
		}
		//~Ball();
		f32 Width;
		f32 Height;
		f32 Ustart;
		f32 Vstart;
		f32 Ustart2;
		f32 Vstart2;
		f32 Uwidth;
		f32 Vheight;
		static	const u8 bornBall[64];// = { 0,0,0, 0,0,0,1,1,1,2,2,2,1,1,1,2,2,2,1,1,1,2,2,2,3,3,3,4,4,4,3,3,3,4,4,4,3,3,3,4,4,4,3,3,3,4,4,4,0xff };//0xff：終了コード
		static	const u8 deathBall[64];// = { 9,9,9,8,8,8, 9,9,9,8,8,8,9,9,9,8,8,8,9,9,9,8,8,8,7,7,7,6,6,6,7,7,7,6,6,6,7,7,7,6,6,6,5,5,5,5,5,5,0xff };
		static	const u8 loseBall[128];// = { 14,14,14,13,13,13, 14,14,14,13,13,13, 14,14,14,13,13,13, 14,14,14,13,13,13,14,14,14,13,13,13,14,14,14,13,13,13,12,12,12,11,11,11,12,12,12,11,11,11,12,12,12,11,11,11,12,12,12,11,11,11,12,12,12,11,11,11,10,10,10,10,10,10,0xff };
		//const byte loseBall[64] = { 14,14,14,13,13,13, 14,14,14,13,13,13,12,12,12,11,11,11,10,10,10,0xff };

		//const byte *ballAnime_data[3] = { bornBall,deathBall,loseBall };
		u8 cnt;
		u8 cnt2;
		bool runBronAnime() {
			const u8 *ballAnime_data[3] = { bornBall,deathBall,loseBall };
			const u8 *ptBallAnime = ballAnime_data[0];
			cnt += 1;
			if (*(ptBallAnime + cnt) == 0xff) {
				cnt = 0;
				return true;
			}
			Ustart = ((*(ptBallAnime + cnt)) % (int)(1 / Uwidth))*Uwidth;
			Vstart = ((*(ptBallAnime + cnt)) / (int)(1 / Vheight))*Vheight;
			return false;
		}

		bool runDeadAnime() {
			const u8 *ballAnime_data[3] = { bornBall,deathBall,loseBall };
			const u8 *ptBallAnime = ballAnime_data[1];
			cnt += 1;
			if (*(ptBallAnime + cnt) == 0xff) {
				cnt = 0;
				return true;
			}
			Ustart = ((*(ptBallAnime + cnt)) % (int)(1 / Uwidth))*Uwidth;
			Vstart = ((*(ptBallAnime + cnt)) / (int)(1 / Vheight))*Vheight;
			return false;
		}

		bool runLoseAnime() {
			const u8 *ballAnime_data[3] = { bornBall,deathBall,loseBall };
			const u8 *ptBallAnime = ballAnime_data[2];
			cnt2 += 1;
			if (*(ptBallAnime + cnt2) == 0xff) {
				cnt2 = 0;
				return true;
			}
			Ustart2 = ((*(ptBallAnime + cnt2)) % (int)(1 / Uwidth))*Uwidth;
			Vstart2 = ((*(ptBallAnime + cnt2)) / (int)(1 / Vheight))*Vheight;
			return false;
		}
		void Draw(f32 x, f32 y);
		void Draw2(f32 x, f32 y);
	};
	Ball ball;

};



bool	HitTest(f32 X1, f32 Y1, f32 W1, f32 H1, f32 X2, f32 Y2, f32 W2, f32 H2);

