﻿#include"Player.h"
#include"PlayerDead.h"
#include"PlayerIdle.h"
#include"PlayerHit.h"



/// <summary>
/// コンストラクタ
/// </summary>
PlayerHit::PlayerHit(int InitalModelHandle, int beforeAnimationIndex, Player::AnimationState animationState)
    :StateBase(InitalModelHandle,animationState, beforeAnimationIndex)
{
    // 現在のステートを入れる
    nowStateTag = Player::HitState;

    // アニメーションの速度を代入
    animationSpeed = AnimationSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerHit::~PlayerHit()
{
    // 処理なし
}


/// <summary>
/// 更新処理
/// </summary>
/// <param name="modelDirection">モデルの向き</param>
/// <param name="characterPosition">キャラクターの座標</param>
void PlayerHit::Update(VECTOR& modelDirection, VECTOR& position, const VECTOR targetPosition, VECTOR cameraPosition)
{
    //ステートの切り替え処理を呼ぶ
    ChangeState();

    //アニメーションの再生時間のセット
    UpdateAnimation();

    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();
}


/// <summary>
/// ステートの切り替え処理をまとめたもの
/// </summary>
void PlayerHit::ChangeState()
{
    if (currentPlayAnimationState == FirstRoopEnd && lifeState == Player::NoLife)
    {
        nextState = new PlayerDead(modelhandle, this->GetAnimationIndex());
    }
    //アニメーションの再生が終了したらステートを切り替える
    else if (currentPlayAnimationState == FirstRoopEnd)
    {
        nextState = new PlayerIdle(modelhandle, this->GetAnimationIndex());

        // ダメージを受けていない状態に戻す
        lifeState = Player::NoDamage;
    }
    else
    {
        nextState = this;
    }
}
