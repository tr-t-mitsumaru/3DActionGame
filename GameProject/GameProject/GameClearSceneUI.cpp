#include"DxLib.h"
#include"ImageDataManager.h"
#include"Utility.h"
#include"GameClearSceneUI.h"
#include"SoundManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
GameClearSceneUI::GameClearSceneUI()
    :scrollAnimationCount(0)
    ,logoBlendRate(0)
    ,currentUIState(OpeningScroll)
{
    // 画像管理クラスのインスタンスをもってくる
    ImageDataManager* imageDataManager = ImageDataManager::GetInstance();

    // 画像ハンドルに代入
    backGroundImage = imageDataManager->GetImageHandle(ImageDataManager::ClearSceneBack);
    logoImage = imageDataManager->GetImageHandle(ImageDataManager::ClearLogo);
    returnTitleTextImage = imageDataManager->GetImageHandle(ImageDataManager::ReturnTitleText);

    // 分割画像のハンドルを代入
    scrollImage = imageDataManager->GetDibisionImageHandle(ImageDataManager::ClearSceneScroll);

    // 音管理クラスのインスタンスをもってくる
    soundManager = SoundManager::GetInstance();

}


/// <summary>
/// デストラクタ
/// </summary>
GameClearSceneUI::~GameClearSceneUI()
{
    soundManager->StopBGM(SoundManager::ClearBgm);
}

/// <summary>
/// 更新処理
/// </summary>
void GameClearSceneUI::Update()
{
    // 巻物画像の更新
    UpdateScroll();

    // ロゴの更新
    UpdateLogo();

    // 点滅表示
    Blinking();
}

/// <summary>
/// 描画
/// </summary>
void GameClearSceneUI::Draw()
{
    // 背景画像の描画
    DrawGraph(0, 0, backGroundImage, TRUE);

    // 巻物画像の描画
    DrawGraph(100, 0, scrollImage[scrollAnimationCount / 5], TRUE);

    // ブレンドモードを有効にする
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, logoBlendRate);

    // ロゴの描画
    DrawGraph(LogoPositionX, LogoPositionY, logoImage,TRUE);

    if (display)
    {
        DrawGraph(ReturnTitleTextPositionX, ReturnTitleTextPositionY, returnTitleTextImage, TRUE);
    }

    // ブレンドモードをノーマルに戻す
    SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
}


/// <summary>
/// ロゴの更新処理
/// </summary>
void GameClearSceneUI::UpdateLogo()
{
    if (currentUIState == NowLogoUpdate)
    {
        soundManager->PlayBGM(SoundManager::ClearBgm);

        // ロゴのブレンド率を上げていく
        logoBlendRate += LogoBlendSpeed;
        if (logoBlendRate >= BlendMax)
        {
            logoBlendRate = BlendMax;

            currentUIState = EndedLogoUpdate;

        }
    }

}

    /// <summary>
    /// 巻物の更新を行う
    /// </summary>
void GameClearSceneUI::UpdateScroll()
{
    // 巻物が開き終わってなければあアニメーションを進めて開く
    if (currentUIState == OpeningScroll)
    {
        soundManager->PlaySoundEffect(SoundManager::ScrollOpen);
        // 巻物のアニメーションを進める
        scrollAnimationCount++;

        // アニメーションが最後まで進んだら
        if (scrollAnimationCount >= ScrollAnimationCountLimit)
        {
            // 最後のカウントで停止させる
            scrollAnimationCount = ScrollAnimationCountLimit - 1;

            // 開き終わったフラグを立てる
            currentUIState = NowLogoUpdate;
            soundManager->StopSoundEffect(SoundManager::ScrollOpen);
        }

    }
    else
    {
        soundManager->StopSoundEffect(SoundManager::ScrollOpen);

    }
}


/// <summary>
/// 点滅表示
/// </summary>
void GameClearSceneUI::Blinking()
{
    blinkingCount++;
    //文字を表示する時間のカウント
    if (blinkingCount <= DisplayTime)
    {
        display = true;
    }
    //文字を消す時間のカウント
    else if (blinkingCount > DisplayTime && blinkingCount <= InbisibleTime)
    {
        display = false;
        if (blinkingCount == InbisibleTime)
        {
            blinkingCount = 0;
        }
    }
}
