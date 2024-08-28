#include"DxLib.h"
#include "FadeInOut.h"
#include"ImageDataManager.h"
#include"Game.h"

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
    // 処理なし
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
    if (currentFadeInOutState != NowFadeOut)
    {
        return;
    }
    else
    {
        //少しずつ透明な画像を元に戻す
        blendCount += FadeOutSpeed;
        //フェードアウトが完了したら
        if (blendCount > BlendMax)
        {
            blendCount = BlendMax;
            currentFadeInOutState = EndFadeOut;
        }
    }
}

/// <summary>
/// フェードインに関する処理
/// </summary>
void FadeInOut::FadeIn()
{
    //フェードインが始まるフラグが立っていないなら即リターン
    if (currentFadeInOutState != NowFadeIn)
    {
        return;
    }
    else
    {
        //少しずつ画像を透明にしていく
        blendCount -= FadeInSpeed;
        if (blendCount < 0)
        {
            blendCount = 0;
            currentFadeInOutState = EndFadeIn;
        }

    }
}

/// <summary>
/// フェードインフェードアウトで使う黒い画像の描画
/// </summary>
void FadeInOut::Draw()
{
    //透過率の変更
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendCount);
    //黒い画像の描画
    DrawExtendGraph(0, 0, Game::ScreenWidth + 1, Game::ScreenHeight, blackImage, TRUE);
    //他に影響がないように元に戻す
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}


