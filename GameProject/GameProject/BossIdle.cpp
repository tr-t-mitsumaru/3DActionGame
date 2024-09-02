﻿#include"StateBase.h"
#include"BossDefaultAttack.h"
#include"BossMove.h"
#include"BossAreaAttack.h"
#include"BossShotAttack.h"
#include"BossRunAttack.h"
#include"BossIdle.h"
#include"Boss.h"



///<summary>
///コンストラクタ
///</summary>
BossIdle::BossIdle(int& InitializeModelHandle,const int beforeAnimationIndex,const IdleStateRelatedStates initializePreviousStateName)
    :StateBase(InitializeModelHandle,Boss::Idle,beforeAnimationIndex)
{
    //アニメーション速度の初期化
    animationSpeed = InitializeAnimationSpeed;
    //インプットマネージャーのインスタンスをもってくる
    inputManager = InputManager::GetInstance();

    // 前回のステートを保存
    previousStateName = initializePreviousStateName;
}

/// <summary>
/// デストラクタ
/// </summary>
BossIdle::~BossIdle()
{
    //処理なし
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">プレイヤーモデルの向き</param>
/// <param name="position">自身のキャラクターの座標</param>
/// <param name="bossTargetPosition">敵対している相手の座標</param>
void BossIdle::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition, VECTOR cameraPosition)
{
    // ターゲットとの距離を出す
    float targetDistance = VSize(VSub(bossTargetPosition,position));

    // どの行動に移るかの選択
    SelectActionPatternLoop(targetDistance);

    //ステートの切り替え処理を呼ぶ
    ChangeState();

    //アニメーションの再生時間のセット
    UpdateAnimation(AnimationBlendSpeed);

    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();
}


/// <summary>
/// ステートの切り替え処理をまとめたもの
/// </summary>
void BossIdle::ChangeState()
{
    switch (nextStateName)
    {
        // 通常攻撃への移行
        case BossIdle::DefaultAttack:
        {
            nextState = new BossDefaultAttack(modelhandle, animationIndex);

            break;
        }
        // 移動ステートに移行
        case BossIdle::Move:
        {
            nextState = new BossMove(modelhandle, animationIndex);

            break;
        }
        // 範囲攻撃のステートに移行
        case BossIdle::AreaAttack:
        {
            nextState = new BossAreaAttack(modelhandle, animationIndex);

            break;
        }
        // 遠距離攻撃に移行
        case BossIdle::ShotAttack:
        {
            nextState = new BossShotAttack(modelhandle, animationIndex);

            break;
        }
        // 突進攻撃に移行
        case BossIdle::RunAttack:
        {
            nextState = new BossRunAttack(modelhandle, animationIndex);

            break;
        }
        default:
        {
            nextState = this;

            break;
        }
    }
}

/// <summary>
/// 行動パターンが決定するまでループを続ける
/// </summary>
void BossIdle::SelectActionPatternLoop(const float bossTargetDistance)
{
    bool isMatchPreviousState = true;

    while (isMatchPreviousState)
    {

        // 選ばれた行動が前のステートと同じかを調べる
        // 違えばfalseが帰ってきてループから抜ける
        isMatchPreviousState =  SelectActionOnRange(bossTargetDistance);
    }
}


/// <summary>
/// 距離に合わせて行動パターンを選択
/// </summary>
/// <param name="targetDistance">ボスが攻撃する対象との距離</param>
bool BossIdle::SelectActionOnRange(const float bossTargetDistance)
{
    bool isMatchPreviousState = true;

    // 行動選択のためのランダムな値を入れる
    int randPattern = GetRand(RandRange);

    // ターゲットとの距離が近かったら
    if (bossTargetDistance <= ShortRange)
    {
        // それぞれの確率で処理が分岐する
        if (randPattern < ShortRangeDefaultAttackProbability)
        {
            nextStateName = DefaultAttack;
        }
        else if (randPattern >= ShortRangeDefaultAttackProbability && randPattern < ShortRangeDefaultAttackProbability + ShortRangeAreaAttackProbability)
        {
            nextStateName = AreaAttack;
        }
        else
        {
            nextStateName = Move;
        }
    }
    // 中距離だった場合
    else if (bossTargetDistance > ShortRange && bossTargetDistance <= MiddleRange)
    {
        if (randPattern < MiddleRangeAreaAttackProbability)
        {
            nextStateName = AreaAttack;
        }
        else if (randPattern >= MiddleRangeAreaAttackProbability && randPattern < MiddleRangeShotAttackProbability + MiddleRangeShotAttackProbability)
        {
            nextStateName = ShotAttack;
        }
        else
        {
            nextStateName = Move;
        }
    }
    // 遠距離だった場合
    else
    {
        if (randPattern < LongRangeRunAttackProbability)
        {
            nextStateName = RunAttack;
        }
        else if (randPattern >= LongRangeRunAttackProbability && randPattern < LongRangeShotAttackProbability + LongRangeRunAttackProbability)
        {
            nextStateName = ShotAttack;
        }
        else
        {
            nextStateName = Move;
        }
    }

    if (nextStateName != previousStateName)
    {
        isMatchPreviousState = false;
    }

    return isMatchPreviousState;
}
