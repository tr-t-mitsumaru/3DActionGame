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
#include"GameSceneUI.h"




/// <summary>
/// コンストラクタ
/// </summary>
GameScene::GameScene()
    :currentGameScneState(Start)
{
    //メモリの確保
    stage       = new Stage();
    camera      = new Camera();
    player      = new Player();
    boss        = new Boss();
    gameSceneUI = new GameSceneUI();
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
    delete gameSceneUI;

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

    // 登場シーンのアップデート
    if (currentGameScneState == Start)
    {
        // 登場シーンで使用する各アップデート
        camera->UpdateStartScene(playerBossDistance,boss->GetPosition(),player->GetPosition());
        player->UpdateStartScene(playerBossDistance);
        boss->UpdateStartScene();

        // カメラがボスに注目している場合
        if (camera->GetStartCameraState() == Camera::FoucusOnBoss)
        {
            // ボスのアップデートを始める
            boss->StartUpdateStartScene();
        }

        // ボスが威嚇を始めたら
        if (boss->GetCurrentStartMoveState() == Boss::IntimidationStart)
        {
            // カメラの注目点をプレイヤーに変更する
            camera->ChangeForcusPlayer();
        }

        // ボスの威嚇アニメーションが一定まで進んでいたら
        if (boss->GetAnimationNowTime() / boss->GetAnimationLimitTime() >= ShakeStartBossAnimationRatio)
        {
            // カメラを揺らす
            camera->StartCameraShake();
        }

        // ボスの移動が終了していたら
        if (boss->GetCurrentStartMoveState() == Boss::EndMove)
        {
            // カメラの揺れを止める
            camera->StopCameraShake();

            // 登場シーンからバトルシーンに変更する
            currentGameScneState = Battle;
        }
    }

    // バトル中のみ行うアップデート
    else if (currentGameScneState == Battle)
    {
        if (gameSceneUI->GetCurrentBlendState() == GameSceneUI::Opaque)
        {
            gameSceneUI->StartFeadIn();
        }
        player->Update(boss->GetPosition(),camera->GetPosition());
        boss->Update(player->GetPosition(), camera->GetPosition());
        camera->UpdatePlayerCamera(player->GetPosition());
        gameSceneUI->Update(player->GetHp());
    }

    //各クラスのアップデートを呼ぶ
    shotManager->Update();
    //当たり判定全体の更新処理を行う
    collisionManager->Update();

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
    gameSceneUI->Draw();
}
