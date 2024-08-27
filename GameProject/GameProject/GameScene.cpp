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
#include"SoundManager.h"




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

    // シングルトンクラスのインスタンスをもってくる
    collisionManager = CollisionManager::GetInstance();
    shotManager = ShotManager::GetInstance();
    effectManager = EffectManager::GetInstance();
    soundManager = SoundManager::GetInstance();

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
    soundManager->StopBGM(SoundManager::Game);
    soundManager->StopSoundEffect(SoundManager::GameOverSE);
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

        // 最初に再生する
        if (camera->GetStartCameraState() == Camera::StartView)
        {
            soundManager->PlayBGM(SoundManager::Start);
        }

        // カメラがボスに注目している場合
        if (camera->GetStartCameraState() == Camera::FoucusOnBoss)
        {
            // ボスのアップデートを始める
            boss->StartUpdateStartScene();

            // ボスが動き出した音
            soundManager->PlaySoundEffect(SoundManager::BossStandUp);
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

            // 移動可能エリアのエフェクトを再生させる
            stage->StartEffectPaly();

            // ボスが立ち上がっている時の音を止める
            soundManager->StopSoundEffect(SoundManager::BossStandUp);

            // ボスの威嚇音を流す
            soundManager->PlaySoundEffect(SoundManager::BossRoar);

            // 開始時のBGMを止める
            soundManager->StopBGM(SoundManager::Start);

            // ゲーム中の音を再生する
            soundManager->PlayBGM(SoundManager::Game);
        }

        // ボスの移動が終了していたら
        if (boss->GetCurrentStartMoveState() == Boss::EndMove)
        {
            // カメラの揺れを止める
            camera->StopCameraShake();

            // 登場シーンからバトルシーンに変更する
            currentGameScneState = Battle;

            // 開始時にサウンドを止める
            soundManager->StopSoundEffect(SoundManager::BossRoar);
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
        if (player->GetEndedDeadMove())
        {
            gameSceneUI->StartGameOverTextDraw();
            soundManager->StopBGM(SoundManager::Game);
            soundManager->PlaySoundEffect(SoundManager::GameOverSE);
        }

        boss->Update(player->GetPosition(), camera->GetPosition());

        // ボスとプレイヤーの位置を修正
        boss->ApplyBoundaryPushback();
        player->ApplyBoundaryPushback();
        camera->UpdatePlayerCamera(player->GetPosition());
        gameSceneUI->Update(player->GetHp(),boss->GetHp());
        
    }

    stage->Update();

    //各クラスのアップデートを呼ぶ
    shotManager->Update();
    //当たり判定全体の更新処理を行う
    collisionManager->Update();

    // エフェクト全体の更新
    effectManager->Update();


    //デバッグ時だけキー入力でシーン遷移するように
#ifdef _DEBUG

    //Xキーが離されていればゲームクリアシーンに移行
    if (boss->GetEndedBossDeadMove())
    {
        nextScene = new GameClearScene();
    }
    // UIのゲームオーバー時の更新が終わっていればシーンを切り替え
    else if (gameSceneUI->GetEndedGameOvetUpdate())
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
