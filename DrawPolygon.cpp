//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		�`��֐�
//		�j�X
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include	"main.h"
#include "DrawPolygon.h"
#include "Quantitative.h"

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
	
GXColor	BG_COLOR = {0xfe,0xf2,0x63};	//�w�i�F- 

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
