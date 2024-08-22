#pragma once
#include"StateBase.h"
#include"InputManager.h"

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
    void Update(VECTOR& modelDirection, VECTOR& characterPosition)override;


private:

    ///////  定数  ///////

    static constexpr float  InitializeAnimationSpeed               = 0.5f;     //アニメーション速度
    static constexpr float  CollisionCapsuleLineHalfLength         = 50.0f;    //当たり判定のカプセルの半分の大きさ
    static constexpr float  CollisionRadius                        = 35.0f;    //当たり判定の半径
    static const     VECTOR OffsetPosition;                                    //プレイヤーと攻撃の当たり判定座標がどれだけずれているか
    static constexpr float  OffsetPositionScale                    = 20.0f;    //プレイヤーとどれだけ離すかの大きさ
    static constexpr float  CollisionCapsuleAngle                  = 50;       //当たり判定のカプセルの角度
    static constexpr float  InitializeCollisionStartAnimationRatio = 0.3f;     //当たり判定を始めるアニメーションの再生率
    static constexpr int    DamageAmount                           = 1;        //与えるダメージ量

    ///////  メンバ変数  ////////

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

