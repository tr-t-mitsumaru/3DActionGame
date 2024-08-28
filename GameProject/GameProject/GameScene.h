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
class SoundManager;


/// <summary>
/// ゲーム中のシーン
/// </summary>
class GameScene:public SceneBase
{
public:

    ///////          定数            ////////

    static constexpr float ShakeStartBossAnimationRatio = 0.45f;    // 画面の揺らしを開始するボスのアニメーション再生率
    static constexpr float MaxDistanceFromCenter        = 40;      // プレイヤーやボスが動ける中央からの最大距離

    /// <summary>
    /// ゲームシーンの中での状態
    /// </summary>
    enum GameSceneState
    {
        Start    = 0,  // ゲームが開始してプレイヤーが動けるようになる前の状態
        Tutorial = 1,  // チュートリアル
        Battle   = 2,  // 戦いが開始した状態
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

    ///////          定数             //////
    static constexpr int   FogColor    = 80;        // 霧の色
    static constexpr float FogDistance = 100.0f;    // 霧との距離


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
    SoundManager*     soundManager;     // サウンドマネージャー

    ///////       関数       ///////

    /// <summary>
    /// 霧の設定の初期化
    /// </summary>
    void InitializeFog();
    
    
};

