#include"ImageDataManager.h"
#include "GameSceneUI.h"
#include"Player.h"
#include"Boss.h"
#include"Font.h"


/// <summary>
/// コンストラクタ
/// </summary>
GameSceneUI::GameSceneUI()
    :uiBlendOpacity(0)
    ,currentBlendState(Opaque)
    ,playerDisplayHp(0)
    ,bossDisplayHp(0)
    ,endedGameOvetUpdate(false)
{
    // インスタンスをもってくる
    imageDataManager = ImageDataManager::GetInstance();

    // メモリの確保
    font = new Font();

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
    gameOvetTextImage = imageDataManager->GetImageHandle(ImageDataManager::GameOverText);


    // 画像のサイズを取る
    GetGraphSize(hpGageHadle, &playerHpGageMaxWidth, &playerHpGageMaxHeight);
    GetGraphSize(bossHpGageHandle, &bossHpGageMaxWidth, &bossHpGageMaxHeight);


}

/// <summary>
/// デストラクタ
/// </summary>
GameSceneUI::~GameSceneUI()
{
    // メモリの開放
    delete font;
}

/// <summary>
/// 更新処理
/// </summary>
void GameSceneUI::Update(const int playerHp,const int bossHp)
{
    // ブレンド率の更新
    UpdateBlendRate();

    // ゲームオーバーテキストの更新
    UpdateGameOvetTextBlendRate();

    // ゲームオーバーテキストを描画する時間の更新
    UpdateGameOverTextDisplayCount();

    // 描画するプレイヤーのHPバーの計算
    playerDisplayHp += ((float)playerHp - playerDisplayHp) * LerpSpeed;

    // 描画するボスのHPバーの計算
    bossDisplayHp += ((float)bossHp - bossDisplayHp) * LerpSpeed;

    // 描画するプレイヤーのHPバーの幅を計算
    currentPlayerHpGageWidth = (((float)playerHp / (float)(Player::MaxHp)) * playerHpGageMaxWidth);

    // 描画するボスのHPバーの幅を計算
    currentBossHpGageWidth = (((float)bossHp / (float)(Boss::MaxHp)) * bossHpGageMaxWidth);


    // 描画するプレイヤーのサブHPバーの幅を計算
    currentPlayerSubHpGageWidth = ((playerDisplayHp / (float)Player::MaxHp) * playerHpGageMaxWidth);

    // 描画するプレイヤーのサブHPバーの幅を計算
    currentBossSubHpGageWidth = ((bossDisplayHp / (float)Boss::MaxHp) * bossHpGageMaxWidth);


    
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

    // プレイヤーに関するUIの追加
    DrawPlayerUI();

    // ボスに関するUIの追加
    DrawBossUI();

    // ブレンドモードをノーマルに戻す
    SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, gameOverTextBlendRate);

    DrawGraph(0, 0, gameOvetTextImage, TRUE);

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

/// <summary>
/// ボスに関するUIの描画
/// </summary>
void GameSceneUI::DrawPlayerUI()
{
    // プレイヤーのHPゲージの下地
    DrawGraph(PlayerHpGageXPosition, PlayerHpGageYPosition, hpBackGageHandle, TRUE);

    // 徐々に減っていくHPゲージ
    DrawExtendGraph(PlayerHpGageXPosition, PlayerHpGageYPosition, PlayerHpGageXPosition + currentPlayerSubHpGageWidth, PlayerHpGageYPosition + playerHpGageMaxHeight, subHpGageHandle, TRUE);

    // 即座に減るHPゲージ
    DrawExtendGraph(PlayerHpGageXPosition, PlayerHpGageYPosition, PlayerHpGageXPosition + currentPlayerHpGageWidth, PlayerHpGageYPosition + playerHpGageMaxHeight, hpGageHadle, TRUE);

    // HPバーのフレームを描画
    DrawGraph(PlayerHpGageXPosition, PlayerHpGageYPosition, hpFrameHandle, TRUE);
}

/// <summary>
/// プレイヤーに関するUIの描画
/// </summary>
void GameSceneUI::DrawBossUI()
{
    // HPゲージの下地
    DrawGraph(BossHpGageXPosition, BossHpGageYPosition, bossHpBackHandle, TRUE);

    // 徐々に減っていくHPゲージ
    DrawExtendGraph(BossHpGageXPosition, BossHpGageYPosition, BossHpGageXPosition + currentBossSubHpGageWidth, BossHpGageYPosition + bossHpGageMaxHeight, bossSubHpGageHandle, TRUE);

    // 即座に減るHPゲージ
    DrawExtendGraph(BossHpGageXPosition, BossHpGageYPosition, BossHpGageXPosition + currentBossHpGageWidth, BossHpGageYPosition + bossHpGageMaxHeight, bossHpGageHandle, TRUE);

    // HPバーのフレームを描画
    DrawGraph(BossHpGageXPosition, BossHpGageYPosition, bossHpFrameHandle, TRUE);

    // ボスの名前
    DrawStringToHandle(BossNameStringXPosition, BossNameStringYPosition, "FIRE GOLEM", GetColor(210, 210, 210), font->GetBossNameHandle());
}


/// <summary>
/// ゲームオーバーテキストの描画を開始させる
/// </summary>
void GameSceneUI::StartGameOverTextDraw()
{
    currentGameOvetTextBlendState = FadingIn;
}


/// <summary>
/// ゲームオーバーテキストのブレンド率を更新
/// </summary>
void GameSceneUI::UpdateGameOvetTextBlendRate()
{
    // だんだん不透明にしていく
    if (currentGameOvetTextBlendState == FadingIn)
    {
        gameOverTextBlendRate += FeadInSpeed;

        // 完全に不透明になったらステートを変更 
        if (gameOverTextBlendRate >= FeadInMax)
        {
            gameOverTextBlendRate = FeadInMax;
            currentGameOvetTextBlendState = Opaque;
        }
    }
}

/// <summary>
/// ゲームオーバテキストを描画するカウントの更新
/// </summary>
void GameSceneUI::UpdateGameOverTextDisplayCount()
{
    if (currentGameOvetTextBlendState == Opaque)
    {
        gameOvetTextDisplayCount++;
        if (gameOvetTextDisplayCount >= GameOvetTextDisplayCountLimit)
        {
            endedGameOvetUpdate = true;
        }
    }
}
