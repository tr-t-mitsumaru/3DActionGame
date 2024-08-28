#include"DxLib.h"
#include"GameClearSceneUI.h"
#include "GameClearScene.h"
#include"TitleScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameClearScene::GameClearScene()
{
    // メモリを確保
    gameClearSceneUI = new GameClearSceneUI();

}

/// <summary>
/// デストラクタ
/// </summary>
GameClearScene::~GameClearScene()
{
    // メモリの開放
    delete gameClearSceneUI;
}

/// <summary>
/// 更新処理
/// </summary>
void GameClearScene::Update()
{
    // UIの更新
    gameClearSceneUI->Update();

    //デバッグ時だけキー入力でシーン遷移するように
#ifdef _DEBUG

    //Xキーが離された瞬間ならタイトルシーンに以降
    if (inputManager->GetKeyPushState(InputManager::X) == InputManager::JustRelease)
    {
        fadeInOut->StartFadeOut();
    }
    else
    {
        nextScene = this;
    }

    // フェードインとアウトの更新
    fadeInOut->FadeIn();
    fadeInOut->FadeOut();

    // フェードアウトが終了していたらタイトルシーンに移行
    if (fadeInOut->GetCurrentFadeInOutState() == FadeInOut::EndFadeOut)
    {
        nextScene = new TitleScene();
    }
#endif
}

/// <summary>
/// 描画
/// </summary>
void GameClearScene::Draw()
{
    // UIの描画
    gameClearSceneUI->Draw();
    fadeInOut->Draw();

}


