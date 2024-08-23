#include"DxLib.h"
#include"TitleSceneUI.h"
#include"Camera.h"
#include"EffectManager.h"
#include"TitleScene.h"
#include"GameScene.h"


/// <summary>
/// コンストラクタ
/// </summary>
TitleScene::TitleScene()
{
    // メモリの確保
    camera       = new Camera();
    titleSceneUI = new TitleSceneUI();

    // エフェクト管理クラスのインスタンスをもってくる
    effectManager = EffectManager::GetInstance();
}

/// <summary>
/// デストラクタ
/// </summary>
TitleScene::~TitleScene()
{
    // エフェクトを全て止める
    effectManager->StopAllEffect();
    delete camera;
    delete titleSceneUI;
}

/// <summary>
/// 更新処理
/// </summary>
void TitleScene::Update()
{
    // カメラの更新
    camera->UpdateTitleScene();

    // UIの更新
    titleSceneUI->Update();

    // エフェクトの更新を行う
    effectManager->Update();

    //Xが離されたらゲームシーンに移行
    if (inputManager->GetKeyPushState(InputManager::X) == InputManager::JustRelease)
    {
        // ドアを空け始める
        titleSceneUI->StartDoorOpen();

        // カメラを前に進み始める
        camera->StartMovingForward();
    }

    // ドアが開ききったらシーンを切り替える
    if (titleSceneUI->GetCurrentDoorOpenState() == TitleSceneUI::Opened)
    {
        nextScene = new GameScene();
    }
    else
    {
        nextScene = this;
    }
}


/// <summary>
/// 描画
/// </summary>
void TitleScene::Draw()
{
    titleSceneUI->Draw();

    // エフェクトの描画
    effectManager->Draw();
}
