#pragma once
#include"StateBase.h"
#include"InputManager.h"

/// <summary>
/// ボスの通常移動に関するステート
/// </summary>
class BossMove:public StateBase
{
public:

    ///////  メンバ関数  //////

    //コンストラクタ
    BossMove(int& InitializeModelHandle, const int beforeAnimationIndex);
    //デストラクタ
    ~BossMove();

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="position">プレイヤーモデルの向き</param>
    void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition, VECTOR cameraPosition)override;


private:

    ///////  定数  ///////

    static constexpr float InitializeAnimationSpeed = 0.3f;   // アニメーション速度
    static constexpr float MoveSpeed                = 0.08f;   // 移動スピード
    static constexpr float BlendSpeed               = 0.06f;  // 前のアニメーションとのブレンドの速さ
    static constexpr int   MoveFrameLimit           = 200;    // 移動させるフレーム数の上限

    ///////  メンバ変数  ////////

    int moveFrameCount;              // 移動させるフレーム数のカウント
    

    ///////  メンバ関数  //////

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;

};


