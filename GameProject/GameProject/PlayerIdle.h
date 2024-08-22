#pragma once
#include"StateBase.h"
#include"InputManager.h"

/// <summary>
/// プレイヤーの待機状態ステート
/// </summary>
class PlayerIdle :public StateBase
{
public:
    //コンストラクタ
    PlayerIdle(int& modelHandle, const int beforeAnimationIndex);
    //デストラクタ
    ~PlayerIdle();

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="position">自身のモデルの向き</param>
    /// <param name="position">自身のキャラクターの座標</param>
    /// <param name="targetPosition">敵対しているキャラの座標</param>
    void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR targetPosition,VECTOR cameraPosition)override;


private:
    static constexpr float InitialAnimationSpeed = 1.0f;    
    
    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;


    //メンバ変数
    InputManager* inputManager;    //インプットマネージャーにアクセスするためのポインタ

    bool          isGround;        //接地しているか


};
