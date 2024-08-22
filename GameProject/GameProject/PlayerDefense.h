#pragma once
#include"StateBase.h"
#include"InputManager.h"

/// <summary>
/// プレイヤーのガードに関するステート
/// </summary>
class PlayerDefense:public StateBase
{
public:
    //コンストラクタ
    PlayerDefense(int modelHandle, int beforeAnimationIndex);
    //デストラクタ
    ~PlayerDefense();

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="position">自身のモデルの向き</param>
    /// <param name="position">自身のキャラクターの座標</param>
    /// <param name="playerTargetPosition">敵対しているキャラの座標</param>
    void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR playerTargetPosition)override;

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;
private:

    //メンバ変数
    InputManager* inputManager;              //インプットマネージャーにアクセスするためのポインタ
};
