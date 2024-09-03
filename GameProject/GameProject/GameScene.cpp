#include"DxLib.h"
#include "GameScene.h"
#include"GameOverScene.h"
#include"GameClearScene.h"
#include"Camera.h"
#include"Stage.h"
#include"Player.h"
#include"Boss.h"
#include"CollisionManager.h"
#include"ShotManager.h"
#include"EffectManager.h"




/// <summary>
/// コンストラクタ
/// </summary>
GameScene::GameScene()
    :currentGameScneState(Start)
{
    //メモリの確保
    stage  = new Stage();
    camera = new Camera();
    player = new Player();
    boss   = new Boss();
    collisionManager = CollisionManager::GetInstance();
    shotManager = ShotManager::GetInstance();
    effectManager = EffectManager::GetInstance();
}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene()
{
    //メモリの解放
    delete stage;
    delete camera;
    delete player;
    delete boss;

    // シングルトンクラスで作成したものを全て削除する
    collisionManager->DeleteAllCollisionDataList();
    shotManager->DeleteAllShot();
    effectManager->StopAllEffect();
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update()
{
    // プレイヤーとボスとの距離を図る
    float playerBossDistance = VSize(VSub(boss->GetPosition(), player->GetPosition()));

    if (currentGameScneState == Start)
    {
        camera->UpdateStartScene(playerBossDistance,boss->GetPosition(),player->GetPosition());
        player->UpdateStartScene(playerBossDistance);
        boss->UpdateStartScene();
        if (camera->GetStartCameraState() == Camera::FoucusOnBoss)
        {
            boss->StartUpdateStartScene();
        }
        if (boss->GetCurrentStartMoveState() == Boss::IntimidationStart)
        {
            camera->ChangeForcusPlayer();
        }
        if (boss->GetAnimationNowTime() / boss->GetAnimationLimitTime() >= ShakeStartBossAnimationRatio)
        {
            camera->StartCameraShake();
        }
        if (boss->GetCurrentStartMoveState() == Boss::EndMove)
        {
            camera->StopCameraShake();
            currentGameScneState = Battle;
        }
    }
    else if (currentGameScneState == Battle)
    {
        player->Update(boss->GetPosition(),camera->GetPosition());
        boss->Update(player->GetPosition(), camera->GetPosition());
        camera->UpdatePlayerCamera(player->GetPosition());
    }

    //各クラスのアップデートを呼ぶ
    shotManager->Update();
    //当たり判定全体の更新処理を行う
    collisionManager->Update();
    // camera->Update(player->GetPosition());

    // エフェクト全体の更新
    effectManager->Update();


    //デバッグ時だけキー入力でシーン遷移するように
#ifdef _DEBUG

    //Xキーが離されていればゲームクリアシーンに移行
    if (inputManager->GetKeyPushState(InputManager::X) == InputManager::JustRelease)
    {
        nextScene = new GameClearScene();
    }
    //Yキーが離されていればゲームオーバーシーンに以降
    else if (inputManager->GetKeyPushState(InputManager::Y) == InputManager::JustRelease)
    {
        nextScene = new GameOverScene();
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
void GameScene::Draw()
{
    //各クラスの描画を行う
    stage->Draw();
    player->Draw();
    boss->Draw();
    shotManager->Draw();
    effectManager->Draw();
}
