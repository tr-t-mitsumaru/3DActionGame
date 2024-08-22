#pragma once
#include"StateBase.h"
#include"InputManager.h"

class ShotManager;
struct InitializeShotData;

/// <summary>
/// ボスの静止ステート
/// </summary>
class BossAreaAttack :public StateBase
{
public:
    ///////  メンバ関数  /////////

    //コンストラクタ
    BossAreaAttack(int& modelHandle, const int beforeAnimationIndex);
    //デストラクタ
    ~BossAreaAttack();

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="position">自身のモデルの向き</param>
    /// <param name="position">自身のキャラクターの座標</param>
    /// <param name="bossTargetPosition">敵対しているキャラの座標</param>
    void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition, VECTOR cameraPosition)override;

    /// <summary>
    /// 攻撃用のショットを作成する
    /// </summary>
    void CreateAttackShot(VECTOR characterPosition);

private:

    enum AttackState
    {
        NoAttack    = 0,
        StartAttack = 1,
        EndAttack   = 2
    };

    ///////  定数  ///////

    // 攻撃時のパラメーター
    static constexpr float EasyAnimationBlendSpeed   = 0.02f;        // 弱い時のアニメーションのブレンドスピード
    static constexpr float NormalAnimationBlendSpeed = 0.04f;        // 通常状態でのアニメーションのブレンドスピード
    static constexpr float HardAnimationBlendSpeed   = 0.07f;        // 強い時のアニメーションブレンドスピード
    static constexpr float EasyShotRadius            = 15.0f;        // 弱い時の弾の大きさ
    static constexpr float NormalShotRadius          = 20.0f;        // 普通の時の弾の大きさ
    static constexpr float HardShotRadius            = 24.0f;        // 強い時の弾の大きさ
    static constexpr float EasyShotSpeed             = 3.5f;         // 弱い時の弾の速度
    static constexpr float NormalShotSpeed           = 5.0f;         // 普通の時の弾の速度
    static constexpr float HardShotSpeed             = 7.0f;         // 強い時の弾の速度
    static constexpr int   EasyShotDamageAmount      = 40;           // 弱い時の弾のダメージ
    static constexpr int   NormalShotDamageAmount    = 60;           // 普通の時の弾のダメージ
    static constexpr int   HardShotDamageAmount      = 80;           // 強い時の弾のダメージ
    static constexpr float EasyAnimationSpeed        = 0.4f;         // 弱い時のアニメーションの速さ
    static constexpr float NormalAnimationSpeed      = 0.6f;         // 普通の時のアニメーションの速さ
    static constexpr float HardAnimationSpeed        = 0.8f;         // 強い時のアニメーションの速さ
    static constexpr float EasyEffectScale           = 15.0f;        // 弱い時のエフェクトの大きさ
    static constexpr float NormalEffectScale         = 20.0f;        // 通常時のエフェクトの大きさ
    static constexpr float HardEffectScale           = 25.0f;        // 強い時のエフェクトの大きさ



    static constexpr float InitializeAnimationSpeed  = 0.6f;         // アニメーションの初期速度
    static constexpr int   SpawnShotCount            = 50;           // 範囲攻撃で生成する弾の数
    static constexpr float FullCircleRadian          = 2 * DX_PI_F;  // １回転分のラジアン
    static constexpr float ShotCreateAnimationRatio  = 0.5f;         // ショットを撃つアニメーションの再生率
    static constexpr float EffectPlaySpeed           = 1.0f;         // エフェクトの再生スピード

    ///////  メンバ変数  ////////

    //ToDo
    //AIを作成したら入力は必要ないので削除
    InputManager* inputManager;      //入力管理クラス

    //攻撃した時の当たり判定に必要な情報をまとめたもの
    CollisionData collisionData;           //当たり判定に必要な情報をまとめたもの
    CollisionManager* collisionManager;    //当たり判定の管理クラスのポインタ
    //ショット関連
    ShotManager* shotManager;                 // ショットの管理クラス
    AttackState  attackState;                 // 現在の攻撃の状態
    float        shotSpeed;                   // ショットのスピード
    bool         isAttackParameterInitialize; // 攻撃に必要なパラメーターを初期化したか
    float        animationBlendSpeed;         // アニメーションのブレンドスピード
    float        shotRadius;                  // 生成する弾のサイズ
    int          shotDamageAmount;            // 弾のダメージ
    VECTOR       effectScale;                 // エフェクトのサイズ

    ///////  メンバ関数  //////

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;

    /// <summary>
    /// 弾のデータを初期化
    /// </summary>
    /// <param name="index">何番目の弾かの添え字</param>
    /// <param name="characterPosition">基準となる座標</param>
    /// <returns>初期化用データ</returns>
    InitializeShotData InitializeShot(const int index, const VECTOR characterPosition);


    /// <summary>
    /// アニメーションの再生率によってショットを作成
    /// </summary>
    /// <param name="position">自身の座標</param>
    void CreateShotByAnimationTime(const VECTOR position);

    /// <summary>
    /// 体力に合わせて攻撃する強さのパラメーターを変更
    /// </summary>
    void InitializeAttackParameter();
};
