#pragma once
#include"SceneBase.h"

class Stage;
class Camera;
class Player;
class Boss;
class CollisionManager;
class ShotManager;
class EffectManager;
class GameSceneUI;


/// <summary>
/// ゲーム中のシーン
/// </summary>
class GameScene:public SceneBase
{
public:

    ///////          定数            ////////

    static constexpr float ShakeStartBossAnimationRatio = 0.45f;    // 画面の揺らしを開始するボスのアニメーション再生率   

    /// <summary>
    /// ゲームシーンの中での状態
    /// </summary>
    enum GameSceneState
    {
        Start  = 0,  // ゲームが開始してプレイヤーが動けるようになる前の状態
        Battle = 1,  // 戦いが開始した状態
    };
    
    //コンストラクタ
    GameScene();
    //デストラクタ
    ~GameScene();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update()override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

private:


    ///////          変数            ///////

    GameSceneState currentGameScneState;        // 現在のゲームシーン中の状態


    ///////        クラス参照        ///////

    Stage*            stage;            //ステージ
    Camera*           camera;           //カメラ
    Player*           player;           //プレイヤー
    Boss*             boss;             //ボス
    CollisionManager* collisionManager; //コリジョンマネージャーのインスタンスにアクセスるポインタ
    ShotManager*      shotManager;      // ショットマネージャーにアクセスするポインタ
    EffectManager*    effectManager ;   // エフェクトマネージャーにアクセスするポインタ
    GameSceneUI*      gameSceneUI;      // ゲームシーンのUI
    
};

