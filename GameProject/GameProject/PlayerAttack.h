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
    void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR playerTargetPosition)override;

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;



private:
    ///////  定数  ///////

    static constexpr float  CollisionCapsuleLineHalfLength = 15.0f;        //当たり判定のカプセルの半分の長さ
    static constexpr float  CollisionRadius                = 7.0f;         //当たり判定のカプセルの半径
    static const     VECTOR OffsetPosition;                                //プレイヤーと攻撃の当たり判定座標がどれだけずれているか
    static constexpr float  OffsetPositionScale            = 20.0f;        //プレイヤーとどれだけ離すかの大きさ
    static constexpr int    CollisionCapsuleAngle          = -30;          //当たり判定用カプセルの回転角度
    static constexpr float  InitializeCollisionStartAnimationRatio = 0.3;  //当たり判定を始めるアニメーションの再生率
    static constexpr int    DamageAmount                   = 1;            //与えるダメージ量

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
