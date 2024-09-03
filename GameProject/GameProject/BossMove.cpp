#include"StateBase.h"
#include"Boss.h"
#include"BossIdle.h"
#include"BossMove.h"
#include"BossDefaultAttack.h"



/// <summary>
/// コンストラクタ
/// </summary>
BossMove::BossMove(int& InitializeModelHandle, const int beforeAnimationIndex)
    :StateBase(InitializeModelHandle, Boss::Walk, beforeAnimationIndex)
{
    //アニメーション速度の初期化
    animationSpeed = InitializeAnimationSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
BossMove::~BossMove()
{
    //処理なし
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">プレイヤーモデルの向き</param>
void BossMove::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition, VECTOR cameraPosition)
{
    // フレームカウントを増やす
    moveFrameCount++;

    //ステートの切り替え処理を呼ぶ
    ChangeState();

    // 移動量を出す
    VECTOR direction = CalculateTargetDirection(bossTargetPosition, position);

    // モデルの向きを反映させる
    modelDirection = direction;

    // 移動量を出す
    velocity = VScale(direction, MoveSpeed);

    //アニメーションの再生時間のセット
    UpdateAnimation(BlendSpeed);

    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();

}


/// <summary>
/// ステートの切り替え処理をまとめたもの
/// </summary>
void BossMove::ChangeState()
{
    // 設定したフレーム数を超えた場合はステートをIdleにする
    if (moveFrameCount >= MoveFrameLimit)
    {
        nextState = new BossIdle(modelhandle, this->GetAnimationIndex(),Boss::Move,isChangingMove);

    }
    else
    {
        nextState = this;
    }
}

