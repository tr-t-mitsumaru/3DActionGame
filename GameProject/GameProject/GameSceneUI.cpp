#include"ImageDataManager.h"
#include "GameSceneUI.h"
#include"Player.h"


/// <summary>
/// コンストラクタ
/// </summary>
GameSceneUI::GameSceneUI()
    :uiBlendOpacity(0)
    ,currentBlendState(Opaque)
    ,displayHp(Player::MaxHp)
{
    // インスタンスをもってくる
    imageDataManager = ImageDataManager::GetInstance();

    // 画像のハンドルをもってくる
    hpGageHadle = imageDataManager->GetImageHandle(ImageDataManager::HpGage);
    hpFrameHandle = imageDataManager->GetImageHandle(ImageDataManager::HpFrame);
    frameHandle = imageDataManager->GetImageHandle(ImageDataManager::Frame);
    hpBackGageHandle = imageDataManager->GetImageHandle(ImageDataManager::HpBack);
    subHpGageHandle = imageDataManager->GetImageHandle(ImageDataManager::HpGageSub);
    bossHpBackHandle = imageDataManager->GetImageHandle(ImageDataManager::BossHpBack);
    bossHpFrameHandle = imageDataManager->GetImageHandle(ImageDataManager::BossHpFrame);
    bossHpGageHandle = imageDataManager->GetImageHandle(ImageDataManager::BossHpGage);
    bossSubHpGageHandle = imageDataManager->GetImageHandle(ImageDataManager::BossHpGageSub);

    // 画像のサイズを取る
    GetGraphSize(hpGageHadle, &playerHpGageMaxWidth, &playerHpGageMaxHeight);

}

/// <summary>
/// デストラクタ
/// </summary>
GameSceneUI::~GameSceneUI()
{
    // 処理なし
}

/// <summary>
/// 更新処理
/// </summary>
void GameSceneUI::Update(const int playerHp)
{
    // ブレンド率の更新
    UpdateBlendRate();

    // 描画するHPバーの計算
    displayHp += ((float)playerHp - displayHp) * LerpSpeed;

    //描画するプレイヤーのHPバーの幅を計算
    currentPlayerHpGageWidth = (((float)playerHp / (float)(Player::MaxHp)) * playerHpGageMaxWidth);

    // 描画するプレイヤーのサブHPバーの幅を計算
    currentPlayerSubHpGageWidth = ((displayHp / (float)Player::MaxHp) * playerHpGageMaxWidth);

    
}

/// <summary>
/// ブレンド率の更新処理
/// </summary>
void GameSceneUI::UpdateBlendRate()
{
    // 徐々に不透明にしている状態
    if (currentBlendState == FadingIn)
    {
        uiBlendOpacity += FeadInSpeed;

        // 完全に不透明になったらステートを変更 
        if (uiBlendOpacity >= FeadInMax)
        {
            uiBlendOpacity = FeadInMax;
            currentBlendState = Opaque;
        }
    }
    // 徐々に透明にしている状態
    else if (currentBlendState == FadingOut)
    {
        uiBlendOpacity-= FeadOutSpeed;

        // 完全に透明になったらステートを変更
        if (uiBlendOpacity <= 0)
        {
            uiBlendOpacity = 0;
            currentBlendState = Transparent;
        }
    }
}

/// <summary>
/// 描画
/// </summary>
void GameSceneUI::Draw()
{
    // ブレンドモードを有効にする
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, uiBlendOpacity);

    // 画面の枠画像の描画
    DrawGraph(0, 0, frameHandle, TRUE);

    DrawGraph(700, 90, hpBackGageHandle, TRUE);

    DrawExtendGraph(700, 90, 700 + currentPlayerSubHpGageWidth, 90 + playerHpGageMaxHeight, subHpGageHandle, TRUE);

    DrawExtendGraph(700, 90,700 + currentPlayerHpGageWidth,90 + playerHpGageMaxHeight, hpGageHadle, TRUE);

    // HPバーのフレームを描画
    DrawGraph(700, 90, hpFrameHandle, TRUE);


    DrawGraph(600, 800, hpBackGageHandle, TRUE);

    DrawExtendGraph(700, 90, 700 + currentPlayerSubHpGageWidth, 90 + playerHpGageMaxHeight, subHpGageHandle, TRUE);

    DrawExtendGraph(700, 90, 700 + currentPlayerHpGageWidth, 90 + playerHpGageMaxHeight, hpGageHadle, TRUE);

    // HPバーのフレームを描画
    DrawGraph(700, 90, hpFrameHandle, TRUE);

    // ブレンドモードをノーマルに戻す
    SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);


}

/// <summary>
/// /フェードインを開始する
/// </summary>
void GameSceneUI::StartFeadIn()
{
    currentBlendState = FadingIn;
}

/// <summary>
/// フェードアウトを開始する
/// </summary>
void GameSceneUI::StartFeadOut()
{
    currentBlendState = FadingOut;
}
