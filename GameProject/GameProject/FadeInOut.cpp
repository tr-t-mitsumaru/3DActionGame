#include"DxLib.h"
#include "FadeInOut.h"
#include"ImageDataManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
FadeInOut::FadeInOut()
    :blendCount(BlendMax)
    , currentFadeInOutState(NowFadeIn)
{
    // 画像の管理クラスのインスタンスをもってくる
    ImageDataManager* imageDataManager = ImageDataManager::GetInstance();

    // 黒い画像をもってくる
    blackImage = imageDataManager->GetImageHandle(ImageDataManager::BlackBack);
}

/// <summary>
/// デストラクタ
/// </summary>
FadeInOut::~FadeInOut()
{
    //画像の削除
    DeleteGraph(blackImage);
}

/// <summary>
/// フェードアウトを始めるためにフラグを切り替える関数
/// </summary>
void FadeInOut::StartFadeOut()
{
    currentFadeInOutState = NowFadeOut;
}


/// <summary>
/// フェードアウトに関する処理
/// </summary>
void FadeInOut::FadeOut()
{
    //フェードアウトが始まるフラグが立っていないなら即リターン
    if (currentFadeInOutState != START_FADE_OUT)
    {
        return;
    }
    else
    {
        //少しずつ透明な画像を元に戻す
        blendNum += FADE_OUT_SPEED;
        //フェードアウトが完了したら
        if (blendNum > BLEND_MAX)
        {
            blendNum = BLEND_MAX;
            currentFadeInOutState = END_FADE_OUT;
        }
    }
}

/// <summary>
/// フェードインに関する処理
/// </summary>
void FadeInOut::FadeIn()
{
    //フェードインが始まるフラグが立っていないなら即リターン
    if (currentFadeInOutState != START_FADE_IN)
    {
        return;
    }
    else
    {
        //少しずつ画像を透明にしていく
        blendNum -= FADE_IN_SPEED;
        if (blendNum < 0)
        {
            blendNum = 0;
            currentFadeInOutState = END_FADE_IN;
        }

    }
}

/// <summary>
/// フェードインフェードアウトで使う黒い画像の描画
/// </summary>
void FadeInOut::Draw()
{
    //透過率の変更
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendNum);
    //黒い画像の描画
    DrawExtendGraph(0, 0, SCREEN_WIDTH + 1, SCREEN_HEIGHT, blackImage, TRUE);
    //他に影響がないように元に戻す
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


