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

    ///////  定数  ///////

    static constexpr float InitializeAnimationSpeed     = 0.6f;    // アニメーションの初期速度
    static constexpr float CollisionStratAnimationRatio = 0.1f;    // 当たり判定を作成するアニメーションの再生率
    static constexpr float OffsetCollisionPositonScale  = 0.0;   // 当たり判定をプレイヤーの向いている方向にどれだけ動かすか
    static const VECTOR    OffsetCollisionPosition;                // 当たり判定をプレイヤーの位置からどれだけずらすか
    static constexpr float CollisionRadius              = 60.0f;   // 当たり判定の半径
    static constexpr int   DamageAmount                 = 1;       // ダメージ量

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
    /// <param name="modelDirection">モデルの向いている方向</param>
    /// <param name="characterPosition">キャラクターのポジション</param>
    void UpdateCollisionData(const VECTOR& modelDirection, const VECTOR characterPosition);

    /// <summary>
    /// 当たった時の処理
    /// </summary>
    /// <param name="targetCollisionData">当たった相手の当たり判定情報</param>
    void OnHit(CollisionData targetCollisionData);

#ifdef _DEBUG
    /// <summary>
    /// 当たり判定を描画する(デバッグ用)
    /// </summary>
    void DrawCollision() override;
#endif 

};
