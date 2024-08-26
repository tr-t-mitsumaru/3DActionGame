#pragma once
#include"StateBase.h"


/// <summary>
/// プレイヤーの回避に関するステート
/// </summary>
class PlayerRolling :public StateBase
{
public:
    //コンストラクタ
    PlayerRolling(int modelHandle, int beforeAnimationIndex);
    //デストラクタ
    ~PlayerRolling();

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="position">自身のモデルの向き</param>
    /// <param name="position">自身のキャラクターの座標</param>
    /// <param name="playerTargetPosition">敵対しているキャラの座標</param>
    void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR playerTargetPosition, VECTOR cameraPosition)override;

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;

private:

    static constexpr float MoveSpeed = 0.25f;    //  回避時の移動スピード

    /// <summary>
    /// 回避時の移動量を決める
    /// </summary>
    VECTOR SetMovement(const VECTOR modelDirection);

};
