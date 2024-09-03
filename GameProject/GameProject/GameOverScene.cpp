#include"DxLib.h"
#include "GameOverScene.h"
#include"GameScene.h"
#include"TitleScene.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameOverScene::GameOverScene()
{
    //処理なし
}

/// <summary>
/// デストラクタ
/// </summary>
GameOverScene::~GameOverScene()
{
    //処理なし
}

/// <summary>
/// 更新処理
/// </summary>
void GameOverScene::Update()
{

    //デバッグ時だけキー入力でシーン遷移するように
#ifdef _DEBUG


    //Xキーが離されていればゲームシーンに移行
    if (inputManager->GetKeyPushState(InputManager::X) == InputManager::JustRelease)
    {
       nextScene = new GameScene();
    }
    //右矢印キーが離されていればタイトルシーンに以降
    else if (inputManager->GetKeyPushState(InputManager::Y) == InputManager::JustRelease)
    {
        nextScene = new TitleScene();
    }
    else
    {
        nextScene = this;
    }
#endif
}

/// <summary>
/// 描画
/// </summary>
void GameOverScene::Draw()
{
#ifdef _DEBUG
    DrawString(500, 500, "ゲームオーバーシーン", GetColor(255, 255, 255));
#endif
}
