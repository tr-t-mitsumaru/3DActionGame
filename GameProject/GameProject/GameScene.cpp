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
    :currentGameScneState(Tutorial)
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

    // 霧の設定を初期化する
    InitializeFog();

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
/// 霧の設定の初期化
/// </summary>
void GameScene::InitializeFog()
{
    // 霧を有効化する
    SetFogEnable(true);

    // 霧の色を指定
    SetFogColor(FogColor, FogColor, FogColor);

    // 霧の開始距離、終了距離を設定
    SetFogStartEnd(0.0f, FogDistance);
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

        // ムービースキップの描画を開始させる
        gameSceneUI->StartMovieSkipImageDraw();

        // Xキーが押されていれば登場演出をスキップ
        if (inputManager->GetKeyPushState(InputManager::X) == InputManager::JustRelease)
        {
            player->InitializeBattleStart(boss->GetPosition());
            boss->InitializeBattleStart();

            // 開始時のBGMを止める
            soundManager->StopBGM(SoundManager::Start);

            // ゲーム中の音を再生する
            soundManager->PlayBGM(SoundManager::Game);

            currentGameScneState = Battle;
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

    // チュートリアル中の更新処理
    else if (currentGameScneState == Tutorial)
    {
        gameSceneUI->Blinking();

        // チュートリアル画像の描画を開始
        gameSceneUI->StartTutorialImageDraw();

        // 最初に再生する
        if (camera->GetStartCameraState() == Camera::StartView)
        {
            soundManager->PlayBGM(SoundManager::Start);
        }

        if (inputManager->GetKeyPushState(InputManager::X) == InputManager::JustRelease)
        {
            // チュートリアル画像の描画を終了させる
            gameSceneUI->EndTutorialImageDraw();
            currentGameScneState = Start;
        }
    }

    // バトル中のみ行うアップデート
    else if (currentGameScneState == Battle)
    {
        // 体力などの描画されていない状態なら
        if (gameSceneUI->GetCurrentBlendState() == GameSceneUI::Opaque)
        {
            // UIのフェードインを開始させる
            gameSceneUI->StartFeadIn();

            // ゲームシーンが始まっているのでムービースキップの描画をやめる
            gameSceneUI->EndMovieSkipImageDraw();

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


    if (boss->GetEndedBossDeadMove())
    {
        nextSceneState = GameClear;
        fadeInOut->StartFadeOut();
    }
    // UIのゲームオーバー時の更新が終わっていればシーンを切り替え
    else if (gameSceneUI->GetEndedGameOvetUpdate())
    {
        nextSceneState = GameOver;
        fadeInOut->StartFadeOut();
    }

    //フェードインとフェードアウトの更新処理
    fadeInOut->FadeIn();
    fadeInOut->FadeOut();


    if (fadeInOut->GetCurrentFadeInOutState() == FadeInOut::EndFadeOut)
    {
        if (nextSceneState == GameClear)
        {
            nextScene = new GameClearScene();
        }
        else
        {
            nextScene = new GameOverScene();
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
void GameScene::Draw()
{
    //各クラスの描画を行う
    stage->Draw();
    player->Draw();
    boss->Draw();
    shotManager->Draw();
    effectManager->Draw();
    gameSceneUI->Draw();
    fadeInOut->Draw();
}
