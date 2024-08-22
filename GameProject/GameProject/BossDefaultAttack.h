#pragma once
#include"StateBase.h"
#include"InputManager.h"
#include"EffectData.h"

/// <summary>
/// ボスの通常攻撃のステート
/// </summary>
class BossDefaultAttack :public StateBase
{
public:
    //コンストラクタ
    BossDefaultAttack(int& InitializeModelHandle, const int beforeAnimationIndex);
    //デストラクタ
    ~BossDefaultAttack();

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="position">プレイヤーモデルの向き</param>
    void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition, VECTOR cameraPosition)override;


private:

    ///////  定数  ///////

    static constexpr float  InitializeAnimationSpeed               = 0.5f;     // アニメーション速度
    static constexpr float  CollisionCapsuleLineHalfLength         = 50.0f;    // 当たり判定のカプセルの半分の大きさ
    static constexpr float  CollisionRadius                        = 60.0f;    // 当たり判定の半径
    static const     VECTOR OffsetPosition;                                    // プレイヤーと攻撃の当たり判定座標がどれだけずれているか
    static constexpr float  OffsetPositionScale                    = 70.0f;    // プレイヤーとどれだけ離すかの大きさ
    static constexpr float  CollisionCapsuleAngle                  = 50;       // 当たり判定のカプセルの角度
    static constexpr float  InitializeCollisionStartAnimationRatio = 0.45f;    // 当たり判定を始めるアニメーションの再生率
    static constexpr float  CollisionEndAnimationRatio             = 0.5f;     // 当たり判定を終わらせるアニメーションの再生率
    static constexpr int    DamageAmount                           = 90;       // 与えるダメージ量
    static constexpr float  EffectPlaySpeed                        = 1.0f;     // エフェクトの再生速度
    static constexpr float  EffectDefaultScale                     = 20.0f;    // エフェクトの基本サイズ
    static constexpr float  EasyAnimationSpeed                     = 0.2f;     // 弱い時のアニメーションのスピード
    static constexpr float  NormalAnimationSpeed                   = 0.5f;     // 通常時のアニメーションのスピード
    static constexpr float  HardAnimationSpeed                     = 1.0f;     // 強い時のアニメーションスピード
    static constexpr float  AnimationSpeedChangeRate               = 0.26f;     // アニメーションのスピードを変更する再生率
    static constexpr float  EffectOffsetPositionY                  = 50.0f;    // エフェクトのY座標の調整位置

    ///////  メンバ変数  ////////

    VECTOR            position;                      // 攻撃の当たり判定の座標
    CollisionData     collisionData;                 // 当たり判定に必要な情報をまとめたもの
    CollisionManager* collisionManager;              // 当たり判定の管理クラスのポインタ
    EffectManager*    effectManager;                 // エフェクト管理クラスのポインタ
    EffectData        effectData;                    // エフェクトの再生に必要な情報をまとめたデータ
    bool              isAttackParameterInitialize;   // 攻撃に使うパラメータを初期化したか
    float             animationBlendSpeed;           // 前のアニメーションとブレンドする時間
    bool              isChangedAnimationSpeed;       // アニメーションのスピードを切り替えたかのフラグ


    ///////  メンバ関数  //////

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;

    /// <summary>
    /// 座標などを当たり判定に必要なデータに変換
    /// </summary>
    void UpdateCollisionData(const VECTOR& modelDirection, const VECTOR characterPosition);

    /// <summary>
    /// 当たった時の処理
    /// </summary>
    void OnHit(CollisionData collisionData);

    /// <summary>
    /// エフェクトの再生に必要なデータの更新
    /// </summary>
    /// <param name="modelDirection">モデルの向き</param>
    void UpdateEffectData(const VECTOR modelDirection, const VECTOR characterPosition);

    /// <summary>
    /// 攻撃に必要なパラメータの初期化
    /// </summary>
    void InitializeAttackParameter();

    /// <summary>
    /// アニメーションスピードの切り替え
    /// </summary>
    void ChangeAnimationSpeed();

#ifdef _DEBUG
    /// <summary>
    /// 当たり判定を描画する(デバッグ用)
    /// </summary>
    void DrawCollision() override;
#endif 
};

