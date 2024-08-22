#pragma once
#include"StateBase.h"


/// <summary>
/// プレイヤーの攻撃に関するステート
/// </summary>
class PlayerShotMagic :public StateBase
{
public:
    //コンストラクタ
    PlayerShotMagic(int modelHandle, int beforeAnimationIndex);
    //デストラクタ
    ~PlayerShotMagic();

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="position">自身のモデルの向き</param>
    /// <param name="position">自身のキャラクターの座標</param>
    /// <param name="targetPosition">敵対しているキャラの座標</param>
    void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR targetPosition,VECTOR cameraPosition)override;

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;

private:

};
