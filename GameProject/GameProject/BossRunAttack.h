#pragma once

#include"StateBase.h"
#include"InputManager.h"

/// <summary>
/// ボスの静止ステート
/// </summary>
class BossRunAttack :public StateBase
{
public:
    ///////  メンバ関数  /////////

    //コンストラクタ
    BossRunAttack(int& modelHandle, const int beforeAnimationIndex);
    //デストラクタ
    ~BossRunAttack();

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="position">プレイヤーモデルの向き</param>
    void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition, VECTOR cameraPosition)override;

private:

    /// <summary>
    /// 走っている状態の種類
    /// </summary>
    enum RunState
    {
        RunStart = 0,   // 走り始め
        Run      = 1,   // 走っている途中
        RunEnd   = 2,   // 走り終わり
    };

    ///////  定数  ///////

    static constexpr float InitializeAnimationSpeed     = 0.6f;     // アニメーションの初期速度
    static constexpr float CollisionStratAnimationRatio = 0.1f;     // 当たり判定を作成するアニメーションの再生率
    static constexpr float OffsetCollisionPositonScale  = 0.01f;    // 当たり判定をプレイヤーの向いている方向にどれだけ動かすか
    static const VECTOR    OffsetCollisionPosition;                 // 当たり判定をプレイヤーの位置からどれだけずらすか
    static constexpr float CollisionRadius              = 6.0f;     // 当たり判定の半径
    static constexpr float TargetOffsetDistance         = 10.0f;     // ターゲットとの距離からどれだけずらすか

    // 攻撃に必要なパラメーター
    static constexpr float EasyAnimationBlendSpeed      = 0.008f;   // 弱い時のアニメーションのブレンドスピード
    static constexpr float NormalAnimationBlendSpeed    = 0.015f;   // 通常時のアニメーションのブレンドスピード
    static constexpr float HardAnimationBlendSpeed      = 0.03f;    // 強い時のアニメーションのブレンドスピード
    static constexpr int   EasyDamageAmount             = 70;       // 弱い時のダメージ量
    static constexpr int   NormalDamageAmount           = 120;      // 通常時のダメージ量
    static constexpr int   HardDamageAmount             = 140;      // 強い時のダメージ量
    static constexpr float EasyMoveSpeed                = 0.5f;     // 弱い時の移動スピード
    static constexpr float NormalMoveSpeed              = 0.6f;     // 通常時の移動スピード
    static constexpr float HardMoveSpeed                = 0.8f;     // 強い時の移動スピード




    ///////  メンバ変数  ////////

    RunState currentRunState;             // 現在の走りステートの状態
    VECTOR   direction;                   // 移動する方向
    VECTOR   startPosition;               // 突進を開始した座標
    float    targetLength;                // ターゲットとの距離
    bool     isAttackParameterInitialize; // 攻撃に必要なパラメーターを初期化したか
    int      damageAmount;                // 与えるダメージ量
    float    animationBlendSpeed;         // アニメーションのブレンドスピード
    float      moveSpeed;                   // 移動スピード



    //ToDo
    //AIを作成したら入力は必要ないので削除
    InputManager* inputManager;      //入力管理クラス

    VECTOR            position;                      // 攻撃の当たり判定の座標
    CollisionData     collisionData;                 // 当たり判定に必要な情報をまとめたもの
    CollisionManager* collisionManager;              // 当たり判定の管理クラスのポインタ

    ///////  メンバ関数  //////

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;


    /// <summary>
    /// 座標などを当たり判定に必要なデータに変換
    /// </summary>
    /// <param name="modelDirection">モデルの向いている方向</param>
    /// <param name="characterPosition">キャラクターのポジション</param>
    void UpdateCollisionData(const VECTOR& modelDirection, const VECTOR characterPosition);

    /// <summary>
    /// 当たった時の処理
    /// </summary>
    /// <param name="targetCollisionData">当たった相手の当たり判定情報</param>
    void OnHit(CollisionData targetCollisionData);

    /// <summary>
    /// 走る時に必要な情報の初期化
    /// </summary>
    /// <param name="targetPosition">目標の座標</param>
    /// <param name="position">自身の座標</param>
    void InitializeRunPrameters(const VECTOR targetPosition, const VECTOR position);

    /// <summary>
    /// 体力に合わせて攻撃する強さのパラメーターを変更
    /// </summary>
    void InitializeAttackPrameters();

#ifdef _DEBUG
    /// <summary>
    /// 当たり判定を描画する(デバッグ用)
    /// </summary>
    void DrawCollision() override;
#endif 

};
