//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		�C���N���[�h�t�@�C��
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include	<revolution.h>
#include	<demo.h>
#include<revolution/wpad.h>
#include<revolution/kpad.h>
#include "MEMAllocator.h"
#include "KeyPad.h"
#include "sound.h"
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		�萔��`
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define	SCREEN_WIDTH		(640)						//�X�N���[����
#define	SCREEN_HEIGHT	(480)						//�X�N���[������
#define	LAND_WIDTH		(1024)						//�X�N���[����
#define	LAND_HEIGHT	(512)						//�X�N���[������
#define DEFSHOTY		(5.0f)
#define ENEMYSHOOTSIZE		(6)
#define SHOOTGAP		(15)
#define INVINCIBLE_SIZE (90)
#define ENEMYNUM (3)
#define ENEMY_HEIGHT	(50)
#define	ENEMY_WIDTH	(70)
#define ENEMY0DEFY (SCREEN_HEIGHT / 2 - 150)
#define ENEMY1DEFY  (SCREEN_HEIGHT / 2)
#define ENEMY2DEFY (SCREEN_HEIGHT / 2 + 150)
#define ENEMYSHOOTMAX (255)
#define PLAYERSPEED (6)
#define SWORDENEMYSPEED (3)
#define HEATSIZE		(20.0f)
#define HEATGAP		(30.0f)

#define PLAYERTEX "Player.tpl"
//#define FONTTEX "sysfont.tpl"
#define SCRENTEX	"scren.tpl"
#define LANDPIXELTEX	"LandPixel.tpl"
#define FOOTINGTEX	"Footing.tpl"
#define	WINTEX	 "WIN.tpl"
#define	LOSETEX	 "LOSE.tpl"
#define	TITLETEX	 "title.tpl"
#define	TITLEINTTEX		"titleint.tpl"
#define HeadBackgroundTex	"HeadBackground.tpl"
#define HeadTex	"Head.tpl"
#define PlayerHPTEX	"PlayerHP.tpl"
#define PlayerHPC1TEX	"PlayerHPColor1.tpl"
#define PlayerHPC2TEX	"PlayerHPColor2.tpl"
#define	GRASSTEX "Grass.tpl"
#define LANDTEX	"Land.tpl"
#define ENEMYTEX "Enemy.tpl"


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		�O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		�O���[�o���ϐ�
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

enum {
	TITLE,
	GAME_START,
	GAME_PLAY,
	RESULT,
	GAME_OVER,
	GAME_WIN
};
/*
enum {
	StationAnime,
	RunAnime,
	JumpAnime
};
*/
enum {
	StationStatus,
	RunStatus,
	JumpStatus,
	DefenseStatus,
	AttackStatus
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
	ReturnMod
};
class PlayerClass{
public:
	f32 X;	//�N��
	f32 Y;	//�U����
	int	nHp;	//�̗�
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
	void Init();
	void Update();
	void Draw();
	bool FallHitTest(float, float, float, float);
	void Operation();
	void Jump();
	void Attack();
	void Animetion();
	bool HitStair();
	void AllHitTest();
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
		StopTime = 0;
	
		 
	}
private:
	TPLPalettePtr Tex;
	GXTexObj TexObj;
	TPLPalettePtr DeadTex;
	GXTexObj DeadTexObj;
	TPLPalettePtr InvincibleTex;
	GXTexObj InvincibleTexObj;
	f32 JumpStartY;
	f32 Width;
	f32 Height;
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
	//static 	const u8 *Anime_data[6];// = { AnimeStation, AnimeRun, AnimeJump, AnimeDefense, AnimeAttack, AnimeHit };

	u8 cnt;
	u8 JumpCnt;
	u8 StatusStyle;
	bool FacedRight;
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
};


class EnemyClass {


public:
	f32 DisplayX;
	f32 DisplayY;
	f32 X;	//�N��
	f32 Y;	//�U����
	f32 HitBox_X() {
		if (FacedLeft) {
			return (f32)DisplayX - 3;
		}
		else {
			return (f32)DisplayX - 35;
		}
	}
	f32 HitBox_Y() {
		return (f32)DisplayY - 39;
	}
	f32 HitBox_Wdith;
	f32 HitBox_Height;

	f32 AttBox_X() {
		if (FacedLeft) {
			return (f32)DisplayX - 61;
		}
		else {
			return (f32)DisplayX + 4;
		}
	}
	f32 AttBox_Y() {
		return (f32)Y - 39;
	}
	f32 AttBox_Wdith;
	f32 AttBox_Height;
	int Hp;	//�̗�
	void AllInit();
	void AllUpdate();
	void AllDraw();
	//void Sync(DisplayClass Display) {
	//	DisplayX = X - Display.MoveDistance.x;
	//}
	u8 ActionMod;
	void HitOn();
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
		Hp = 5;
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
	u8 HitCnt;
	static const u8 AnimeRun[64]; 
	static const u8 AnimeAttack[64];  
	static const u8 AnimeHit[64];
	//const u8 *Anime_data[3] = { AnimeRun, AnimeAttack, AnimeHit };
	u8 StatusStyle;
	u8 cnt;
};


typedef struct {
	u16 height;
	u16 width;
}ImaginaryBackground;

class DisplayClass {
public:
	f32 Fix_x;
	f32 Fix_y;
	f32 MoveDistance_x;
	f32 MoveDistance_y;
	u16 height;
	u16 width;
	bool ShockOn;
	void Init(ImaginaryBackground Background) {
		height = SCREEN_HEIGHT;
		width = SCREEN_WIDTH;
		Fix_x = 50;
		Fix_y = Background.height - height;
		MoveDistance_x = 0;
		MoveDistance_y = 0;
		ShockOn = false;
		cnt = 0;
	}
	void Shock(){
		if (cnt >= 30) {
			ShockOn = false;
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
			MoveDistance_x += 2;
			break;
		default:
			MoveDistance_x -= 2;
			break;
		}
		/*if (cnt < 20) {
		if (cnt % 2 == 0) {

		}else{ MoveDistance.x -= 5; }
		cnt += 1;}*/

		cnt += 1;
	}
	void Update(ImaginaryBackground Background) {
		if (ShockOn){
			Shock();
		}
		Fix_x = MoveDistance_x;
		Fix_y = Background.height - height + MoveDistance_y;
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
	}
	f32 Width;
	f32 Height;
	TPLPalettePtr Tex;
	GXTexObj TexObj;
		f32 Ustart;
	f32 Vstart;
	f32 Uwidth;
	f32 Vheight;
private:
	


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
//}SHOOT;



static f32 LandUMove = 0.0f;
u8 score_draw[5] = {0,0,0,0,0}; 
 
static u16 score = 0; 


//u8	ShootFlag[SHOOTNUM] = {HITOFF,HITOFF,HITOFF,HITOFF,HITOFF};
//f32 ShootX[SHOOTNUM];	// = {5.0f,10.0f,15.0f,20.0f,25.0f};
//f32	DefShotX[SHOOTNUM];
//f32 ShootY[SHOOTNUM];// = {DEFSHOTY,DEFSHOTY,DEFSHOTY,DEFSHOTY,DEFSHOTY};
//
//u8	HitSwitch = HITOFF;
GXColor	BG_COLOR = {0xfe,0xf2,0x63};	//�w�i�F- 
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		�v���g�^�C�v�錾
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern PlayerClass Player;
extern ImageClass *Image;
extern DisplayClass Display;
extern ImaginaryBackground Background;
extern UIClass *GameUI;
extern EnemyClass *Enemy;

/*
ImageClass WIN;
ImageClass LOSE;
//SHOOT Shoot[SHOOTNUM];
const u8 AnimeStation[64] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0xff };//0xff�F�I���R�[�h
const u8 AnimeRun[64] = { 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 9, 9, 9, 9, 9, 9, 9, 0xff };
const u8 AnimeJump[64] = { 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 0xff };
const u8 AnimeDefense[64] = { 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26, 25, 25, 25, 25, 25, 25, 25, 25, 25, 0xff };//0xff�F�I���R�[�h
const u8 AnimeAttack[64] = { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 34, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 0xff };//0xff�F�I���R�[�h
const u8 *Anime_data[5] = { AnimeStation, AnimeRun, AnimeJump, AnimeDefense, AnimeAttack };
*/
u8 fcnt;
u8 Status;
//u8 Anime_style;// = 0;
//const u8 *Animept = Anime_data[Anime_style];
//	�T�E���h�p�ϐ�
void       *arenaMem2Lo;		//�T�E���h�p���������ʃA�h���X
void       *arenaMem2Hi;		//�T�E���h�p��������ʃA�h���X
void       *axBuffer;			//�T�E���h�p�������擪�A�h���X
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		�֐��錾
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//f32		UpMove(f32 ,f32 ,f32 ,f32 );				//��ړ��֐�
//f32		RightMove(f32 ,f32 ,f32 ,f32 );			//���ړ��֐�
//f32		LeftMove(f32 ,f32 ,f32 ,f32  );				//���ړ��֐�
//f32		DownMove(f32 ,f32 ,f32 ,f32 );				//���ړ��֐�
bool	HitTest(f32 ,f32 ,f32 ,f32 ,f32 ,f32 , f32 ,f32 );
void	ScreenInitialization();
void	Draw2DCharacter(GXTexObj, f32, f32, f32, f32, f32, f32, f32, f32);
void	DrawPlayerRev(GXTexObj, f32, f32, f32, f32, f32, f32, f32, f32);
void	WiiMainInit();
void	GameUpdate();
void	GameDraw();
//==========================================================================================================
//		�v���O�����{��
//==========================================================================================================
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		�G���g���[�|�C���g
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int main(void){
	

	WiiMainInit(); //������
	
	//		�Q�[�����[�u
	while(1)
	{
		GameUpdate();
		GameDraw();
		//		�����_�����O�I��
		DEMODoneRender();
	}
	//		�Q�[���I��
	OSHalt("End of sample");
}


//==========================================================================================================
//		�֐���`
//==========================================================================================================



//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		GAME System�̏������֐�
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void WiiMainInit(){
	//		�e�평����
	DEMOInit(NULL);						//��v���W���[���������ŏ���������܂��oos,pad,gx,vi}
	init_memory2();	//�������m��
	init_KPAD();	//�����R��������
	Status = GAME_START;
	fcnt = 0;
	Image->TitleInit();
	// �T�E���h�p�������m��
	// initialize Exp Heap on MEM2
	arenaMem2Lo = OSGetMEM2ArenaLo();
	arenaMem2Hi = OSGetMEM2ArenaHi();
	hExpHeap = MEMCreateExpHeap(arenaMem2Lo, (u32)arenaMem2Hi - (u32)arenaMem2Lo);

	// initialize AI & AX
	axBuffer = MEMAllocFromExpHeapEx(hExpHeap, AXGetMemorySize(AX_MAX_VOICES), 32);

	AIInit(NULL);
	AXInitSpecifyMem(AX_MAX_VOICES, axBuffer);

	// Load Voice data into MRAM
	LoadSamples();

	// Register Callback with AX for audio processing
	AXRegisterCallback(&AudioFrameCallback);

	for (int i = 0; i < AX_MAX_VOICES; i++)
	{
		Voices[i].state = VOICE_STATE_STOP;
	}

	// �T�E���h�p�����������@�����܂�

	// �e�N�X�`���̃��[�h�����͂���ȍ~�ɍs������
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		GAME�̍X�V����
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GameUpdate(){
	fcnt += 1;
	if (fcnt > 60) { fcnt = 0; }
	work_controller();
	switch (Status){
	case TITLE:
		
		if (kpads[0][0].trig & KPAD_BUTTON_A){
			Status = GAME_START; 
			//PlaySample(&Samples[2]);
		}
		break;
	case GAME_START:
		Background.height = 1024;
		Background.width = 2048;
		Display.Init(Background);
		Image->Init();
		Player.Init();
		Enemy->AllInit();

		GameUI->Init();

		Status = GAME_PLAY;
		break;
	case GAME_PLAY:
		if (Player.StopTime == 0) {
			Image->Update();
			Enemy->AllDraw();

			Player.Update();
			Enemy->AllUpdate();	}else{
			Player.StopTime -= 1;
		}

		if (Player.nHp <= -10) { 
			Status = GAME_OVER;	
			/*
			TPLGetPalette(&LOSE.Tex, LOSETEX);
			TPLGetGXTexObjFromPalette(LOSE.Tex, &LOSE.TexObj, 0); */
		}
		//if (Enemy[0].nHp <= -5 && Enemy[1].nHp <= -5 && Enemy[2].nHp <= -5) { Status = GAME_WIN;	TPLGetPalette(&WIN.Tex, WINTEX);	TPLGetGXTexObjFromPalette(WIN.Tex, &WIN.TexObj, 0); }
		break;
	case GAME_OVER:
		
		if (kpads[0][0].trig & KPAD_BUTTON_A) { Status = TITLE; }
		break;
	case GAME_WIN:
		if (kpads[0][0].trig & KPAD_BUTTON_A){ Status = TITLE; }
		break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		GAME�̉�ʕ`��
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GameDraw(){
	ScreenInitialization();//��ʏ�����

	switch (Status){
	case TITLE:
		Image->TitleDraw(fcnt);
		break;
	case GAME_PLAY:
		/*
		Draw2DCharacter(texScrenObj, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1, 1);//sky
		Draw2DCharacter(texLandObj, LAND_WIDTH / 2, SCREEN_HEIGHT - LAND_HEIGHT / 2, LAND_WIDTH, LAND_HEIGHT, 0 + LandUMove, 0, 1, 1);


		Draw2DCharacter(Cloud.TexObj, Cloud.X, Cloud.Y, Cloud.Width, Cloud.Height, 0, 0, 1, 1);//sky
		*/
		Image->BackDraw();
		Enemy->AllDraw();
		Player.Draw();
		Image->UpDraw(Player.nHp);
		GameUI->Draw(Player.nHp);
		/*for (u8 i = 0; i < ENEMYNUM; i++) {Enemy[i].Draw();}*/
	
		/*for(u8 i=0;i<5;i++){
		Draw2DCharacter(texFontObj, i*50.0f+200.0f, 50.0f, 50.0f, 50.0f, (score_draw[i]+16)%16*0.0625f, 0.0625f, ((score_draw[i]+16)/16)*0.0625f, 0.0625f);
		}*/
		break;
		/*
	case GAME_OVER:
		Image->BackDraw();
		Draw2DCharacter(LOSE.TexObj, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1, 1);
		if (fcnt >30 == 0) {
			Draw2DCharacter(Titleint.TexObj, Titleint.X, Titleint.Y, Titleint.Width, Titleint.Height, 0, 0, 1, 1);
		}
		break;
	case GAME_WIN:
		Image->BackDraw();
		Draw2DCharacter(WIN.TexObj, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 1, 1);
		if (fcnt >30 == 0) {
			Draw2DCharacter(Titleint.TexObj, Titleint.X, Titleint.Y, Titleint.Width, Titleint.Height, 0, 0, 1, 1);
		}
		break;*/
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		�����蔻�菈���֐���`
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool			HitTest(f32 X1,f32 Y1,f32 W1,f32 H1,f32 X2,f32 Y2, f32 W2,f32 H2)
{
	f32 XX1 =X1-W1/2;
	f32	XX2 =X2-W2/2;
	f32 YY1 = Y1-H1/2;
	f32 YY2 = Y2-H2/2;

	if(((YY1+H1 >= YY2) && (YY1 -H2<= YY2)) && ((XX1+ W1 >= XX2) && (XX1-W2 <= XX2)))
	{
		return true;
	}
	else
	{
		return	 false;
	}

}


//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		Character�\���֐���`
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void		Draw2DCharacter(GXTexObj TexObj,f32 X,f32 Y,f32 Width,f32 Height,f32 Ustart,f32 Vstart,f32 Uwidth,f32 Vheight)
{
	GXLoadTexObj(&TexObj, GX_TEXMAP0);
	GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 4);

	//		��`�p���̂P�i���_0�j
	GXPosition3f32(X - Width / 2, Y + Height / 2, 0.0f);	//���W
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart, Vstart+ Vheight);            //2017.5.19�ǉ�
														
	//		��`�p���̂P�i���_1�j
	GXPosition3f32(X - Width / 2, Y - Height / 2, 0.0f);	//���W
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart, Vstart);            //2017.5.19�ǉ�

	 //		��`�p���̂P�i���_2�j
	GXPosition3f32(X + Width / 2, Y - Height / 2, 0.0f);	//���W
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart+ Uwidth, Vstart);            //2017.5.19�ǉ�

	 //		��`�p���̂P�i���_3�j
	GXPosition3f32(X + Width / 2, Y + Height / 2, 0.0f);	//���W
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart+ Uwidth, Vstart + Vheight);            //2017.5.19�ǉ�

	GXEnd();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		PlayerRev�\���֐���`
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void		DrawPlayerRev(GXTexObj TexObj, f32 X, f32 Y, f32 Width, f32 Height, f32 Ustart, f32 Vstart, f32 Uwidth, f32 Vheight)
{
	GXLoadTexObj(&TexObj, GX_TEXMAP0);
	GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 4);

	//		��`�p���̂P�i���_0�j
	GXPosition3f32(X - Width / 2, Y + Height / 2, 0.0f);	//���W
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart + Uwidth, Vstart + Vheight);            //2017.5.19�ǉ�

	//		��`�p���̂P�i���_1�j
	GXPosition3f32(X - Width / 2, Y - Height / 2, 0.0f);	//���W
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart + Uwidth, Vstart);            //2017.5.19�ǉ�

	//		��`�p���̂P�i���_2�j
	GXPosition3f32(X + Width / 2, Y - Height / 2, 0.0f);	//���W
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart, Vstart);            //2017.5.19�ǉ�

	//		��`�p���̂P�i���_3�j
	GXPosition3f32(X + Width / 2, Y + Height / 2, 0.0f);	//���W
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart, Vstart + Vheight);            //2017.5.19�ǉ�

	GXEnd();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		��ʏ������֐���`
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void		ScreenInitialization() {
	//		�v���W�F�N�V�����s��̐ݒ�
	Mtx44	projMatrix;	//	�v���W�F�N�V�����s��

						//		���ˉe�s��̍쐬
	MTXOrtho(projMatrix, 0, SCREEN_HEIGHT, 0, SCREEN_WIDTH, 0.0f, 1.0f);

	//		���ˉe�s����v���W�F�N�V�����s��Ƃ��Đݒ�
	GXSetProjection(projMatrix, GX_ORTHOGRAPHIC);

	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//		�`�揈��
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//		��ʃN���A
	GXSetCopyClear(BG_COLOR, GX_MAX_Z24);

	//		�����_�����O�J�n
	DEMOBeforeRender();

	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//		���_�����ݒ�
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	GXClearVtxDesc();																												//���_�������N���A
																																	//GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
																																	//GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);																	//���W�̓_�C���N�g�Q��
																																	//GXSetVtxAttrFmt(GX_VTXFMT0,GX_VA_POS,GX_POS_XYZ,GX_F32,0);		//���_���W�����t�H�[�}�b�g�ݒ�
																																	//GXSetVtxAttrFmt(GX_VTXFMT0,GX_VA_CLR0,GX_CLR_RGB,GX_RGB8,0);
																																	//GXSetNumChans(1);
																																	//GXSetNumTevStages(1);
																																	//GXSetTevOrder(GX_TEVSTAGE0,GX_TEXCOORD_NULL,GX_TEXMAP_NULL,GX_COLOR0A0);
																																	//GXSetTevOp(GX_TEVSTAGE0,GX_PASSCLR);

	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);     //���W�̓_�C���N�g�Q��
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);    //�P��̑����i�F�j�Ƃ��̑����ɑ΂���Q�Ƃ̎d�����w��
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);    //2017.5.19 �e�N�X�`���̐ݒ�
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);	//���_���W�����t�H�[�}�b�g�̐ݒ�
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0);//���_�F�����t�H�[�}�b�g�̐ݒ�
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);  //���_�����̐ݒ�   2017.5.19�ǉ�
	GXSetNumChans(1);            //�J���[�`�����l���̐ݒ�
	GXSetNumTevStages(1);        //TEV�X�e�[�W���̐ݒ�
	GXSetNumTexGens(1);          //2017.5.19�ǉ�
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);  //�F�E�e�N�X�`���̍������� 2017.5.19�C��
																		 //	GXSetTevOp(GX_TEVSTAGE0,GX_PASSCLR);  //TEV�̃J���[�̍����ݒ�
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);  //2017.5.19�C��
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);  //2017.5.19�ǉ�
																				  //	GXLoadTexObj(&texObj, GX_TEXMAP0);     //2017.5.19�ǉ�
}


//==========================================================================================================
//		End of sample
//==========================================================================================================
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


void ImageClass::Init() {
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
		LandPixel[i].Height = 100;
		LandPixel[i].Width = 60;
		LandPixel[i].Ustart = 0.0f;
		LandPixel[i].Uwidth = 1.0f;
		LandPixel[i].Vstart = 0.0f;
		LandPixel[i].Vheight = 1.0f;
		LandPixel[i].Y = SCREEN_HEIGHT - LandPixel[i].Height / 2;
		LandPixel[i].DisplayY = LandPixel[i].Y;
		LandPixel[i].X = LandPixel[i].Width / 2 + i*LandPixel[i].Width;
		LandPixel[i].DisplayX = LandPixel[i].X;
	}
	Grass[0].Width = 60;
	GrassNum = Background.width / Grass[0].Width + 1;
	for (int i = 0; i < GrassNum; i++) {
		Grass[i].Height = 60;
		Grass[i].Width = 60;
		Grass[i].Ustart = 0.0f;
		Grass[i].Uwidth = 1.0f;
		Grass[i].Vstart = 0.0f;
		Grass[i].Vheight = 1.0f;
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
	Footing[0].Y = SCREEN_HEIGHT - 120;
	Footing[1].X = 1224;
	Footing[1].Y = SCREEN_HEIGHT - 170;
	for (int i = 0; i < FootingNum; i++) {
		Footing[i].Height = 20;
		Footing[i].Width = 100;
		Footing[i].Ustart = 0.0f;
		Footing[i].Uwidth = 1.0f;
		Footing[i].Vstart = 0.0f;
		Footing[i].Vheight = 1.0f;
		Footing[i].DisplayY = Footing[i].Y;
		Footing[i].DisplayX = Footing[i].X;
	}

}

void ImageClass::Update(){

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
}

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
void ImageClass::TitleDraw(int fcnt) {
	Draw2DCharacter(Title.TexObj, Title.X, Title.Y, Title.Width, Title.Height, 0, 0, 1, 1);

	if (fcnt >30 == 0) {
		Draw2DCharacter(Titleint.TexObj, Titleint.X, Titleint.Y, Titleint.Width, Titleint.Height, 0, 0, 1, 1);
	}
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








PlayerClass Player;
//ENEMY Enemy[ENEMYNUM];
//SHOOT Shoot[SHOOTNUM];
//ENEMYSHOOT EnemyShoot[ENEMYSHOOTMAX];
const u8 PlayerClass::AnimeStation[64] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0xff };//0xff�F�I���R�[�h
const u8 PlayerClass::AnimeRun[64] = { 8, 8, 8, 8, 8, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 9, 9, 9, 9, 9, 9, 9, 0xff };
const u8 PlayerClass::AnimeJump[64] = { 16, 16, 16, 16, 16, 16, 16, 16, 16, 17, 17, 17, 17, 17, 17, 17, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 0xff };
const u8 PlayerClass::AnimeDefense[64] = { 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26, 25, 25, 25, 25, 25, 25, 25, 25, 25, 0xff };//0xff�F�I���R�[�h
const u8 PlayerClass::AnimeAttack[64] = { 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 33, 34, 34, 34, 34, 34, 34, 34, 34, 34, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 0xff };//0xff�F�I���R�[�h
const u8 PlayerClass::AnimeHit[64] = { 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 0xff };//0xff�F�I���R�[�h
void PlayerClass::Init() {
	extern DisplayClass Display;

	TPLGetPalette(&Player.Tex, PLAYERTEX);
	TPLGetGXTexObjFromPalette(Player.Tex, &Player.TexObj, 0);   //2017.5.19�ǉ�
	Initial_x = (float)Display.width / 2;
	Initial_y = (float)(Display.height - 50 - 64);
	X = Initial_x;
	Y = Initial_y;
	Width = (u8)(128/(544/480));
	Height = (u8)(128/(544/480));
	Width = 128;
	Height = 128;
	nHp = 30;
	InvincibleState = 0;
	Ustart = 0.0f;
	Uwidth = (f32)1/8;
	Vstart = 0.0f;
	Vheight = (f32)1/8;
	FacedRight = true;
	StatusStyle = StationStatus;
	cnt = 0;
	JumpCnt = 0;
	InDoubleJumpStatus = false;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Player��������֐���`
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool			PlayerClass::FallHitTest(float X1, float Y1, float W, float H)
{
	f32 XX1 = X - 24 / 2;
	if (FacedRight) {
		XX1 -= 5;
	}
	else {
		XX1 += 5;
	}//�C��
	f32	XX2 = X1 - W / 2;
	f32 YY1 = Y + Height / 2 - 10;
	f32 YY2 = Y1 - H / 2;

	if (((YY1 + 10 >= YY2) && (YY1 - H <= YY2)) && ((XX1 + 24 >= XX2) && (XX1 - W <= XX2)))
	{
		return true;
	}
	else
	{
		return 	false;
	}

}

void PlayerClass::Update() {
	
	/*
	if (InvincibleState > 0) {
		InvincibleState -= 1;
	}*/
	if (nHp > 0) {

		if (StatusStyle != AttackStatus) {
			Operation();
			Jump();
		}else {
			Attack();
		}		
	}

}


void PlayerClass::Draw() {
	Animetion();
	

	if (FacedRight){
		Draw2DCharacter(TexObj, X, Y, Width, Height, Ustart, Vstart, Uwidth, Vheight);
	}else{
		DrawPlayerRev(TexObj, X, Y, Width, Height, Ustart, Vstart, Uwidth, Vheight);
	}




}


//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Player����֐���`
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void	PlayerClass::Operation() {
	extern DisplayClass Display;
	
	//**********************************************************************
	//	Player �ړ�
	//**********************************************************************
	if (kpads[0][0].hold & KPAD_BUTTON_UP) {
		if (FacedRight) {
			if (X - Width / 2 < Display.width / 4 && Display.Fix_x + Display.MoveDistance_x > 10) {
				Display.MoveDistance_x -= PLAYERSPEED / 2;
				MoveHit();
			}
			else if (X - Width / 2 > 0) { X -= PLAYERSPEED / 2; MoveHit(); }


			if (StatusStyle != JumpStatus) {
				if (StatusStyle != DefenseStatus) { cnt = 0;  StatusStyle = DefenseStatus; }
			}
		}
		else {
			if (X - Width / 2 < Display.width / 4 && Display.Fix_x + Display.MoveDistance_x > 10) {
				Display.MoveDistance_x -= PLAYERSPEED;
				MoveHit();
			}
			else if (X - Width / 2 > 0) {
				X -= PLAYERSPEED;
				MoveHit();
			}
			if (StatusStyle != JumpStatus) {
				if (StatusStyle != RunStatus) {
					cnt = 0;
					StatusStyle = RunStatus;
				}
			}
		}							//���ړ�
	}
	if (kpads[0][0].hold & KPAD_BUTTON_DOWN) {
		if (FacedRight) {
			if (X + Width / 2 > Display.width * 3 / 4 && Display.MoveDistance_x + Display.width <= Background.width - 10) {
				Display.MoveDistance_x += PLAYERSPEED;
				MoveHit();
			}
			else if (X + Width / 2 < Display.width) { X += PLAYERSPEED; MoveHit(); }

			if (StatusStyle != JumpStatus) {
				if (StatusStyle != RunStatus) {
					cnt = 0;
					StatusStyle = RunStatus;
				}
			}
		}
		else {
			if (X + Width / 2 > Display.width * 3 / 4 && (Display.MoveDistance_x + Display.width) <= Background.width - 10) {
				Display.MoveDistance_x += PLAYERSPEED / 2;
				MoveHit();
			}
			else if (X + Width / 2 < Display.width) { X += PLAYERSPEED / 2; MoveHit(); }
			if (StatusStyle != JumpStatus) { if (StatusStyle != DefenseStatus) { cnt = 0;  StatusStyle = DefenseStatus; } }
		}				//�E�ړ�
	}

	//**********************************************************************
	//	Player �U��
	//**********************************************************************
	if (kpads[0][0].trig & KPAD_BUTTON_1){
		if (StatusStyle != JumpStatus) {
			cnt = 0;
			StatusStyle = AttackStatus;
			Display.ShockOn = true;
		}
	}

	//**********************************************************************
	//	Player �]�g
	//**********************************************************************
	if (kpads[0][0].trig & KPAD_BUTTON_B){
		FacedRight = !FacedRight;
		if (StatusStyle == RunStatus) { StatusStyle = DefenseStatus; }
	}

	//**********************************************************************
	//	Player �@jump
	//**********************************************************************
	if (kpads[0][0].trig & KPAD_BUTTON_2) {
		if (StatusStyle != JumpStatus && !InDoubleJumpStatus) {
			JumpStartY = Initial_y;
			JumpCnt = 0;
			cnt = 0;
			StatusStyle = JumpStatus;
		}
		else if (!InDoubleJumpStatus) {
			JumpCnt = 0;
			cnt = 0;
			InDoubleJumpStatus = true;
		}
	}

	//**********************************************************************
	//	��~
	//**********************************************************************
	if (kpads[0][0].release & (KPAD_BUTTON_UP | KPAD_BUTTON_DOWN))
	{
		if (StatusStyle != JumpStatus) {
			if (StatusStyle != StationStatus) { cnt = 0; StatusStyle = StationStatus; }
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Player jump�Ɨ��������֐���`
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PlayerClass::Jump() {
	extern u8 FootingNum;
	extern ImageClass Footing[256];
	const u8 *Anime_data[6] = { AnimeStation, AnimeRun, AnimeJump, AnimeDefense, AnimeAttack, AnimeHit };

	if (StatusStyle == JumpStatus) {
		JumpCnt += 1;
		//float vg = 10 - 0.98f*fcnt;
		Y -= 14 - 0.98f*JumpCnt;;
		const u8 *ptAnime = Anime_data[StatusStyle];

		if (Y > JumpStartY) {
			Y = JumpStartY;
			JumpCnt = 0;
			cnt = 0;
			JumpStartY = 0.0f;
			InDoubleJumpStatus = false;
			StatusStyle = StationStatus;
			//Display.ShockOn = true;
		}
		if (*(ptAnime + cnt) == 18) {
			for (int i = 0; i < FootingNum; i++) {
				if (Player.FallHitTest(Footing[i].DisplayX, Footing[i].DisplayY, Footing[i].Width, Footing[i].Height)/*HitTest(, Y, 20, Height, Footing[i].DisplayX, Footing[i].DisplayY, Footing[i].Width, Footing[i].Height)*/) {
					Y = Footing[i].DisplayY - Footing[i].Height / 2 - Height / 2 + 10;

					JumpCnt = 0;
					cnt = 0;
					JumpStartY = 0.0f;
					InDoubleJumpStatus = false;
					StatusStyle = StationStatus;
				}
			}
		}
	}
	else {
		//����
		if (Y != Initial_y) {
			InFall = true;
			for (int i = 0; i < FootingNum; i++) {
				if (Player.FallHitTest(Footing[i].DisplayX, Footing[i].DisplayY, Footing[i].Width, Footing[i].Height)) {
					InFall = false;
					break;
				}
			}
			if (InFall) {
				JumpStartY = Initial_y;
				JumpCnt = 16;
				cnt = 16;
				StatusStyle = JumpStatus;
			}

		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Player �U�������֐���`
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PlayerClass::Attack() {
const u8 *Anime_data[6] = { AnimeStation, AnimeRun, AnimeJump, AnimeDefense, AnimeAttack, AnimeHit };

	const u8 *ptAnime = Anime_data[StatusStyle];
	cnt += 1;
	if (*(ptAnime + cnt) == 0xff) {
		cnt = 0;
		StatusStyle = StationStatus;
	}
	else { cnt -= 1; }

}

//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Player Animetion�����֐���`
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PlayerClass::Animetion() {
const u8 *Anime_data[6] = { AnimeStation, AnimeRun, AnimeJump, AnimeDefense, AnimeAttack, AnimeHit };

	const u8 *ptAnime = Anime_data[StatusStyle];
	if (*(ptAnime + cnt) == 0xff) {

		if (StatusStyle == JumpStatus) {
			cnt = 16;
		}
		else {
			cnt = 0;
		}
	}
	Ustart = ((*(ptAnime + cnt)) % (u8)(1 / Uwidth))*Uwidth;
	Vstart = ((*(ptAnime + cnt)) / (u8)(1 / Vheight))*Vheight;
	cnt += 1;
	//if (*(ptAnime + cnt) == 0xff) { cnt = 0; }
}
bool PlayerClass::MoveHit() {
	extern EnemyClass SwordEnemy[10];
	extern u8 SwordEnemyNum;
	for (int i = 0; i < SwordEnemyNum; i++){
		if (SwordEnemy[i].PlayerHit()) {
			if (HitBox_X() < SwordEnemy[i].HitBox_X()) { X -= PLAYERSPEED; }
			else {
				X += PLAYERSPEED;
			}
			return true;
		}
	}

	return false;
}

UIClass *GameUI;
UIClass	PlayerHeadBackground;
UIClass	PlayerHead;
UIClass	PlayerHP;
UIClass	PlayerHP1;
UIClass	PlayerHP2;
void UIClass::Init(){
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

}

void UIClass::Draw(int HP) {
	
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


EnemyClass *Enemy;
EnemyClass SwordEnemy[10];
u8 SwordEnemyNum;
const u8 EnemyClass::AnimeRun[64] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0xff };//0xff�F�I���R�[�h
const u8 EnemyClass::AnimeAttack[64] = { 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0xfe };
const u8 EnemyClass::AnimeHit[64] = { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0xff };


void	EnemyClass::AllInit() {
	extern DisplayClass Display;

	TPLGetPalette(&SwordEnemy[0].Tex, ENEMYTEX);
	TPLGetGXTexObjFromPalette(SwordEnemy[0].Tex, &SwordEnemy[0].TexObj, 0);
	SwordEnemyNum = 1;
	/*SwordEnemy[0].Width = 128;
	SwordEnemy[0].Height = 128;*/
	SwordEnemy[0].InitialX = 1200;
	SwordEnemy[0].InitialY = (float)(Display.height - 50 - SwordEnemy[0].Height / 2);
	SwordEnemy[0].X = SwordEnemy[0].InitialX;
	SwordEnemy[0].Y = SwordEnemy[0].InitialY;
	SwordEnemy[0].DisplayX = SwordEnemy[0].X;
	SwordEnemy[0].DisplayY = SwordEnemy[0].Y;

}
void EnemyClass::AllUpdate() {
	Display.Update(Background);

	for (int i = 0; i < SwordEnemyNum; i++) {
		//SwordEnemy[i].Sync(Display);
		SwordEnemy[i].DisplayX = SwordEnemy[i].X - Display.MoveDistance_x;
		//SwordEnemy[i].DisplayY = SwordEnemy[i].Y;
		SwordEnemy[i].Update();
	}
}
void	EnemyClass::AllDraw() {
	for (int i = 0; i < SwordEnemyNum; i++) {
		if (SwordEnemy[i].Hp > 0) {
			SwordEnemy[i].Draw();
		}
	}
}
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

void EnemyClass::Draw() {
	Animetion();

	if (FacedLeft) {
		Draw2DCharacter(TexObj, DisplayX, DisplayY, Width, Height, Ustart, Vstart, Uwidth, Vheight);
	}
	else {
		DrawPlayerRev(TexObj, DisplayX, DisplayY, Width, Height, Ustart, Vstart, Uwidth, Vheight);
	}
}



void	EnemyClass::Update() {
	if (InvincibleTime != 0) { InvincibleTime -= 1; }
	switch (ActionMod)
	{
	case PatrolMod:
		Patrol();
		break;
	case HitMod:
		Hit();
		break;
	case TrackMod:
		Track();
		break;
	case ReturnMod:
		Return();
		break;
	default:
		break;
	}
}

void	EnemyClass::HitOn() {
	HitCnt = 0;
	cnt = 0;
	StatusStyle = EnemyHitAnime;
	ActionMod = HitMod;
	InvincibleTime = 30;

}

void EnemyClass::Hit() {
	extern PlayerClass Player;
	HitCnt += 1;
	if (Player.X <= DisplayX) {
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

void EnemyClass::Track() {
	extern PlayerClass Player;
	if (StatusStyle != EnemyAttAnime) {
	/*
		if (Player.X >= DisplayX) {
			if (FacedLeft) { FacedLeft = false; }
			if (!PlayerHit()) { X += SWORDENEMYSPEED; }
		}
		else {
			if (!FacedLeft) { FacedLeft = true; }
			if (!PlayerHit()) { X -= SWORDENEMYSPEED; }
		}
		*/
		if (Player.X >= DisplayX) {
			if (FacedLeft) { FacedLeft = false; }
			 X += SWORDENEMYSPEED; 
		}
		else {
			if (!FacedLeft) { FacedLeft = true; }
			X -= SWORDENEMYSPEED; 
		}
		if (DisplayX - Player.X > 600 || DisplayX - Player.X < -600) {
			ActionMod = ReturnMod;
		}
		if ((DisplayX - Player.X > 0 && DisplayX - Player.X < 64) || (DisplayX - Player.X<0 && DisplayX - Player.X >-64)) {
			StatusStyle = EnemyAttAnime;
			cnt = 0;
		}
	}
}

void EnemyClass::Patrol() {
	extern PlayerClass Player;

	if (FacedLeft) {
		if (!PlayerHit()) { X -= SWORDENEMYSPEED; }
		if (X < InitialX - 150 || X <= 0) {
			if (!PlayerHit()) { X += SWORDENEMYSPEED * 2; }
			FacedLeft = !FacedLeft;
		}
	}
	else {
		if (!PlayerHit()) { X += SWORDENEMYSPEED; }
		if (X > InitialX + 150 || X >= Background.width) {
			if (!PlayerHit()) { X -= SWORDENEMYSPEED * 2; }
			FacedLeft = !FacedLeft;
		}
	}

	if ((DisplayX - Player.X>0 && DisplayX - Player.X < 300) || (DisplayX - Player.X<0 && DisplayX - Player.X >-300)) {
		ActionMod = TrackMod;
	}


}

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

	if ((X - InitialX>0 && DisplayX - X < 30) || (X - InitialX<0 && X - InitialX >-30)) {
		ActionMod = PatrolMod;
	}
	if ((DisplayX - Player.X>0 && DisplayX - Player.X < 300) || (DisplayX - Player.X<0 && DisplayX - Player.X >-300)) {
		ActionMod = TrackMod;
	}
}
