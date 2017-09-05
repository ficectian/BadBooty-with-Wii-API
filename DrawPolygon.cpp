//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//		描画関数
//		曜氷
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include	"main.h"
#include "DrawPolygon.h"
#include "Quantitative.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		Character表示関数定義
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void		Draw2DCharacter(GXTexObj TexObj,f32 X,f32 Y,f32 Width,f32 Height,f32 Ustart,f32 Vstart,f32 Uwidth,f32 Vheight)
{
	GXLoadTexObj(&TexObj, GX_TEXMAP0);
	GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 4);

	//		矩形角その１（頂点0）
	GXPosition3f32(X - Width / 2, Y + Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart, Vstart+ Vheight);            //2017.5.19追加
														
	//		矩形角その１（頂点1）
	GXPosition3f32(X - Width / 2, Y - Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart, Vstart);            //2017.5.19追加

	 //		矩形角その１（頂点2）
	GXPosition3f32(X + Width / 2, Y - Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart+ Uwidth, Vstart);            //2017.5.19追加

	 //		矩形角その１（頂点3）
	GXPosition3f32(X + Width / 2, Y + Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart+ Uwidth, Vstart + Vheight);            //2017.5.19追加

	GXEnd();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		PlayerRev表示関数定義
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void		DrawPlayerRev(GXTexObj TexObj, f32 X, f32 Y, f32 Width, f32 Height, f32 Ustart, f32 Vstart, f32 Uwidth, f32 Vheight)
{
	GXLoadTexObj(&TexObj, GX_TEXMAP0);
	GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 4);

	//		矩形角その１（頂点0）
	GXPosition3f32(X - Width / 2, Y + Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart + Uwidth, Vstart + Vheight);            //2017.5.19追加

	//		矩形角その１（頂点1）
	GXPosition3f32(X - Width / 2, Y - Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart + Uwidth, Vstart);            //2017.5.19追加

	//		矩形角その１（頂点2）
	GXPosition3f32(X + Width / 2, Y - Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart, Vstart);            //2017.5.19追加

	//		矩形角その１（頂点3）
	GXPosition3f32(X + Width / 2, Y + Height / 2, 0.0f);	//座標
	GXColor3u8(0xff, 0xff, 0xff);
	GXTexCoord2f32(Ustart, Vstart + Vheight);            //2017.5.19追加

	GXEnd();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//		画面初期化関数定義
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void		ScreenInitialization() {
	//		プロジェクション行列の設定
	Mtx44	projMatrix;	//	プロジェクション行列

						//		正射影行列の作成
	MTXOrtho(projMatrix, 0, SCREEN_HEIGHT, 0, SCREEN_WIDTH, 0.0f, 1.0f);
	
GXColor	BG_COLOR = {0xfe,0xf2,0x63};	//背景色- 

	//		正射影行列をプロジェクション行列として設定
	GXSetProjection(projMatrix, GX_ORTHOGRAPHIC);

	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//		描画処理
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//		画面クリア
	GXSetCopyClear(BG_COLOR, GX_MAX_Z24);

	//		レンダリング開始
	DEMOBeforeRender();

	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//		頂点属性設定
	//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	GXClearVtxDesc();																												//頂点属性情報クリア
																																	//GXSetVtxDesc(GX_VA_POS,GX_DIRECT);
																																	//GXSetVtxDesc(GX_VA_CLR0,GX_DIRECT);																	//座標はダイレクト参照
																																	//GXSetVtxAttrFmt(GX_VTXFMT0,GX_VA_POS,GX_POS_XYZ,GX_F32,0);		//頂点座標属性フォーマット設定
																																	//GXSetVtxAttrFmt(GX_VTXFMT0,GX_VA_CLR0,GX_CLR_RGB,GX_RGB8,0);
																																	//GXSetNumChans(1);
																																	//GXSetNumTevStages(1);
																																	//GXSetTevOrder(GX_TEVSTAGE0,GX_TEXCOORD_NULL,GX_TEXMAP_NULL,GX_COLOR0A0);
																																	//GXSetTevOp(GX_TEVSTAGE0,GX_PASSCLR);

	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);     //座標はダイレクト参照
	GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);    //単一の属性（色）とその属性に対する参照の仕方を指定
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);    //2017.5.19 テクスチャの設定
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);	//頂点座標属性フォーマットの設定
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0);//頂点色属性フォーマットの設定
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);  //頂点属性の設定   2017.5.19追加
	GXSetNumChans(1);            //カラーチャンネルの設定
	GXSetNumTevStages(1);        //TEVステージ数の設定
	GXSetNumTexGens(1);          //2017.5.19追加
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);  //色・テクスチャの合成準備 2017.5.19修正
																		 //	GXSetTevOp(GX_TEVSTAGE0,GX_PASSCLR);  //TEVのカラーの合成設定
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);  //2017.5.19修正
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);  //2017.5.19追加
																				  //	GXLoadTexObj(&texObj, GX_TEXMAP0);     //2017.5.19追加
}
