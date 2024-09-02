#pragma once
#include"StateBase.h"


/// <summary>
/// プレイヤーの攻撃に関するステート
/// </summary>
class PlayerAttack :public StateBase
{
public:
    //コンストラクタ
    PlayerAttack(int modelHandle, int beforeAnimationIndex,Player::AnimationState animationState);
    //デストラクタ
    ~PlayerAttack();

    ///////   メンバ関数   //////

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="modelDirection">モデルの向き</param>
    /// <param name="characterPosition">キャラクターの座標</param>
    void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR playerTargetPosition, VECTOR cameraPosition)override;

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;



private:

    ///////  定数  ///////

    static constexpr float  NormalAttackCollisionCapsuleLineLength = 15.0f;   // 通常攻撃の当たり判定のカプセルの半分の長さ
    static constexpr float  NormalAttackCollisionRadius            = 12.0f;   // 通常攻撃の当たり判定のカプセルの半径
    static const     VECTOR NormalAttackOffsetPositionY;                      // 通常攻撃のプレイヤーと攻撃の当たり判定座標がどれだけずれているか
    static constexpr float  NormalAttackOffsetPositionScale        = 18.0f;   // 通常攻撃のプレイヤーとどれだけ離すかの大きさ
    static constexpr int    NormalAttackCollisionCapsuleAngle      = -30;     // 通常攻撃の当たり判定用カプセルの回転角度
    static constexpr float  NormalAttackCollisionStartAnimationRatio = 0.3;   // 当たり判定を始めるアニメーションの再生率
    static constexpr int    NormalAttackDamageAmount                   =2;    // 通常攻撃の与えるダメージ量
    static constexpr float  StrongAttackCollisionCapsuleLineLength = 15.0f;   // 強攻撃の当たり判定のカプセルの半分の長さ
    static constexpr float  StrongAttackCollisionRadius            = 15.0f;   // 強攻撃の当たり判定のカプセルの半径
    static const     VECTOR StrongAttackOffsetPositionY;                      // 強攻撃のプレイヤーと攻撃の当たり判定座標がどれだけずれているか
    static constexpr float  StrongAttackOffsetPositionScale        = 18.0f;   // 強攻撃のプレイヤーとどれだけ離すかの大きさ
    static constexpr int    StrongAttackCollisionCapsuleAngle      = 0;       // 強攻撃の当たり判定用カプセルの回転角度
    static constexpr float  StrongAttackCollisionStartAnimationRatio = 0.3;   // 強攻撃の当たり判定を始めるアニメーションの再生率
    static constexpr int    StrongAttackDamageAmount                 = 1;     // 強攻撃の与えるダメージ量

    //////           変数            ///////

    float  collisionCapsuleLineLength;       // 当たり判定カプセルの長さ
    float  collisionRadius;                  // 当たり判定のカプセルの半径
    float  collisionCapsuleAngle;            // 当たり判定の回転角度
    float  collisionStratAnimationRatio;     // 当たり判定を設定するアニメーションの再生率
    VECTOR offsetPosition;                   // 当たり判定をプレイヤーの座標からどれだけ動かすか
    float  offsetPositionScale;              // 当たり判定をモデルの向きにどれだけ進めるか
    int   damageAmount;



    ///////         変数         ///////

    VECTOR            position;                      // 攻撃の当たり判定の座標
    CollisionData     collisionData;                 // 当たり判定に必要な情報をまとめたもの
    CollisionManager* collisionManager;              // 当たり判定の管理クラスのポインタ


    //////  メンバ関数  //////

    /// <summary>
    /// 座標などを当たり判定に必要なデータに変換
    /// </summary>
    void UpdateCollisionData(const VECTOR& modelDirection, const VECTOR characterPosition);

    /// <summary>
    /// 当たった時の処理
    /// </summary>
    void OnHit(CollisionData collisionData);

#ifdef _DEBUG
    /// <summary>
    /// 当たり判定を描画する(デバッグ用)
    /// </summary>
    void DrawCollision() override;
#endif 




};
