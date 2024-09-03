#pragma once
#include"CollisionData.h"
#include"EffectData.h"

class ModelDataManager;
class CollisionManager;
class StateBase;
class EffectManager;


/// <summary>
/// ボスのクラス
/// </summary>
class Boss
{
public:

    /// <summary>
    /// アニメーションの種類
    /// </summary>
    enum AnimationType
    {
        Attack       = 0,    //攻撃
        Dead         = 1,    //死亡
        FireBleth    = 2,    //ブレス攻撃
        GroundAttack = 3,    //地面に攻撃
        Idle         = 4,    //静止
        Intimidation = 5,    //登場時の威嚇
        JumpAttack   = 6,    //ジャンプして攻撃
        LeftShot     = 7,    //左手で撃つショット
        LeftTrun     = 8,    //左に向きを切り替える
        RightShot    = 9,    //右手で撃つショット
        RightTurn    = 10,   //右向き切り替え
        Run          = 11,   //走る
        Sitting      = 12,   //座っている状態
        Standing     = 13,   //立つ動作
        Walk         = 14    //歩き
    };

    /// <summary>
    /// 行動しているかの状態
    /// </summary>
    enum StartMoveState
    {
        StopMove          = 0,  // 動いていない状態
        StandingNow       = 1,  // 立ち上がっている途中
        Stand             = 2,  // 立ち上がった
        IntimidationStart = 3,  // 威嚇の動きが始まった状態
        EndMove           = 4,  // 行動終了
    };

    /// <summary>
    /// ボスの行動の状態
    /// </summary>
    enum  BossActionState
    {
        DefaultAttack = 0,  // 通常攻撃
        Move          = 1,  // 移動
        AreaAttack    = 2,  // 範囲攻撃
        ShotAttack    = 3,  // 弾を撃つ攻撃
        RunAttack     = 4,  // 突進攻撃
        None          = 5,  // ステートが存在しない場合
    };

    ///////  ゲッター  ///////

    // 座標を渡す
    const VECTOR GetPosition() const { return position; }

    /// <summary>
    /// 現在のボスの登場シーンでの状態を返す
    /// </summary>
    /// <returns>登場シーンでの状態</returns>
    const StartMoveState GetCurrentStartMoveState()const { return currentStartMoveState; }

    /// <summary>
    /// アニメーションの総再生時間を返す
    /// </summary>
    /// <returns>アニメーションの総再生時間</returns>
    const float GetAnimationLimitTime() const { return animationLimitTime; }

    /// <summary>
    /// アニメーションの再生時間を返す
    /// </summary>
    /// <returns>アニメーションの再生時間</returns>
    const float GetAnimationNowTime() const { return animationNowTime; }

    //コンストラクタ
    Boss();
    //デストラクタ
    ~Boss();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update(const VECTOR bossTargetPosition,const VECTOR cameraPosition);

    /// <summary>
    /// 登場シーンでのアップデート
    /// </summary>
    void UpdateStartScene();

    /// <summary>
    /// 登場シーンのボスのアップデートを開始させる
    /// </summary>
    void StartUpdateStartScene();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

private:

    // アニメーションの再生状態
    enum PlayAnimationState
    {
        BlendNow = 0,   // ブレンド中
        BlendEnd = 1,   // ブレンド終了
        Play     = 2,   // 再生中
        Stop     = 3,   // 停止
    };

    ////定数////

    static const VECTOR InitialPosition;             //初期座標
    static const VECTOR OffsetModelPosition;         //モデルの位置調整用の値

    static constexpr float CollisionCapsuleLineLength = 60.0f;      // 当たり判定に必要なカプセルの線分の長さ
    static constexpr float CollisionRadius            = 35.0f;      // 当たり判定に必要なカプセルの半径の大きさ
    static constexpr float HalfLength                 = 0.5f;       // 中央座標を出す際の長さを半分にするための定数
    static constexpr int   NotRegisterTag             = -1;         // レジスタの識別番号が代入されていないことをしめす
    static constexpr float DefaultScale               = 0.2f;       // 基本的なスケール
    static constexpr float AngleSpeed                 = 0.15f;      // モデルの向きを変えるスピード
    static constexpr float ShotHitEffectScale         = 10.0f;      // 弾が当たった時のエフェクトのサイズ
    static constexpr float PlayerAttackHitEffectScale = 10.0f;      // プレイヤーの攻撃が当たった際のエフェクトのサイズ
    static constexpr int   MoveStartCountLimit        = 40;         // 動き出すまでの時間
    static constexpr float SwitchAnimationRatio       = 0.4f;       // アニメーションを切り替える再生率
    static constexpr float FirstAnimationSpeed        = 0.4f ;      // 最初のアニメーションの再生速度
    static constexpr float SecondAnimationSpeed       = 0.5f;       // ２個目のアニメーションの速度
    static constexpr float AnimationBlendSpeed        = 0.01f;      // アニメーションのブレンドスピード
    static constexpr float IntimidationEffectScale    = 40.0f;      // 威嚇時のエフェクトの大きさ

    ////         メンバ変数           ////

    //モデル
    int               modelHandle;      //モデルハンドル
    VECTOR            modelDirection;   //モデルの向くべき方向
    float             angle;            //モデルの向きを変更させる際の値
    VECTOR            position;         //座標
    CollisionData     collisionData;    //当たり判定に必要な情報
    CollisionManager* collisionManager; //当たり判定管理クラスにアクセスするポインタ
    int               hp;

    //ステート
    StateBase* nowState;                //現在のステートを保存するポインタ
    StateBase* nextState;               //次のループでのステートをホゾンスルポインタ

    // エフェクト
    EffectManager* effectManager;         // エフェクトの管理クラスにアクセスするポインタ
    EffectData shotHitEffectData;         // エフェクトの更新に必要なデータ
    EffectData playerAttackHitEffectData; // プレイヤーの攻撃が当たった際のエフェクトのデータ
    EffectData intimidationEffectData;    // 威嚇時に使用するエフェクトのデータ

    // 登場シーン
    StartMoveState currentStartMoveState;          // 登場シーンでのボスの状態
    int            moveStartCount;                 // 動き出すまでのカウント
    float          animationLimitTime;             // 再生中のアニメーションの総再生時間
    float          animationNowTime;               // 再生中のアニメーションの現在の経過時間
    int            animationIndex;                 // 現在のアニメーションのインデックス
    float          animationSpeed;                 // アニメーションの再生速度
    bool           isEndAnimationFirstRoop;        // アニメーションの初期ループが終わったフラグ
    int            beforeAnimationIndex;           // 前のアニメーションのインデックス
    float          animationBlendRate;             // アニメーションのブレンド率
    bool           isStartUpdateStartScene;        // アップデートが開始されたかのフラグ
    bool           isPlaiedIntimidationEffect;     // 威嚇エフェクトが再生されたか
    PlayAnimationState currentPlayAnimationState;  // アニメーションの状態


    ////メンバ関数////

    /// <summary>
    /// ボスの情報から当たり判定に必要な情報を出して代入
    /// </summary>
    void UpdateCollisionData();

    /// <summary>
    /// オブジェクトに当たった際の処理を書いたもの
    /// </summary>
    /// <param name="">当たり判定に必要な情報をまとめたデータ</param>
    void OnHit(const CollisionData collisionData);

    /// <summary>
    /// 弾と当たった際のエフェクトの初期化
    /// </summary>
    /// <param name="shotPosition">当たった弾の座標</param>
    void InitializeShotHitEffectData(const VECTOR shotPosition);

    /// <summary>
    /// 行動開始前のアニメーションの状態変更させるための関数
    /// </summary>
    void ChangeStartMoveState();


    /// <summary>
    /// アニメーションの更新
    /// </summary>
    void UpdateAnimation();

    /// <summary>
    /// アニメーションの切り替え
    /// </summary>
    void SwitchAnimation();

    /// <summary>
    /// プレイヤーの攻撃に当たった際のエフェクトの初期化
    /// </summary>
    /// <param name="attackPosition">攻撃の座標</param>
    void InitializePlayerAttackHitEffectData(const VECTOR attackPosition);

    /// <summary>
    /// ボスの威嚇時のエフェクトデータの初期化
    /// </summary>
    void InitializeIntimidationEffectData();


    /// <summary>
    /// ステートの移行処理
    /// </summary>
    void ChangeState();

    /// <summary>
    /// プレイヤーの回転制御
    /// </summary>
    void UpdateAngle();

};
