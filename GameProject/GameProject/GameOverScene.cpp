#include"DxLib.h"
#include "GameOverScene.h"
#include"GameOverSceneUI.h"
#include"GameScene.h"
#include"TitleScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameOverScene::GameOverScene()
{
    // メモリ確保
    gameOverSceneUI = new GameOverSceneUI();
}

/// <summary>
/// デストラクタ
/// </summary>
GameOverScene::~GameOverScene()
{
    // メモリの開放
    delete gameOverSceneUI;
}

/// <summary>
/// 更新処理
/// </summary>
void GameOverScene::Update()
{
    gameOverSceneUI->Update();

    //Xキーが離されていればゲームシーンに移行
    if (inputManager->GetKeyPushState(InputManager::Up) == InputManager::JustRelease
        && gameOverSceneUI->GetCurrentSerectTextState() == GameOverSceneUI::ReturnTitleText)
    {
        gameOverSceneUI->SwitchSerectText();
    }
    //右矢印キーが離されていればタイトルシーンに以降
    else if (inputManager->GetKeyPushState(InputManager::Down) == InputManager::JustRelease &&
        gameOverSceneUI->GetCurrentSerectTextState() == GameOverSceneUI::ContinueText)
    {
        gameOverSceneUI->SwitchSerectText();
    }

    if (inputManager->GetKeyPushState(InputManager::X) == InputManager::JustRelease)
    {
        if (gameOverSceneUI->GetCurrentSerectTextState() == GameOverSceneUI::ContinueText)
        {
            nextScene = new GameScene();
        }
        else
        {
            nextScene = new TitleScene();
        }
    }
    else
    {
        nextScene = this;
    }


}

/// <summary>
/// 描画
/// </summary>
void GameOverScene::Draw()
{
    gameOverSceneUI->Draw();
}
