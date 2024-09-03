#include"Player.h"
#include"PlayerDead.h"


/// <summary>
/// コンストラクタ
/// </summary>
PlayerDead::PlayerDead(int initalModelHandle, int beforeAnimationIndex)
    :StateBase(initalModelHandle, Player::Death, beforeAnimationIndex)
{
    // 現在のステートを入れる
    nowStateTag = Player::DeadState;

    // アニメーションの速度を代入
    animationSpeed = AnimationSpeed;
}


/// <summary>
/// デストラクタ
/// </summary>
PlayerDead::~PlayerDead()
{
    // 処理なし
}


/// <summary>
/// 更新処理
/// </summary>
/// <param name="modelDirection">モデルの向き</param>
/// <param name="characterPosition">キャラクターの座標</param>
void PlayerDead::Update(VECTOR& modelDirection, VECTOR& position, const VECTOR targetPosition, VECTOR cameraPosition)
{
    //ステートの切り替え処理を呼ぶ
    ChangeState();

    // 一度アニメーションが再生され終わったら止める
    if (currentPlayAnimationState == FirstLoopEnd)
    {
        currentPlayAnimationState = Stop;
    }

    //アニメーションの再生時間のセット
    UpdateAnimation();

    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();

}

/// <summary>
/// ステートの切り替え処理をまとめたもの
/// </summary>
void PlayerDead::ChangeState()
{
    // 死んだらステートをそのままにする
    nextState = this;
}
