#include"StateBase.h"
#include"BossDefaultAttack.h"
#include"BossMove.h"
#include"BossAreaAttack.h"
#include"BossShotAttack.h"
#include"BossRunAttack.h"
#include"BossMoveChange.h"
#include"BossDead.h"
#include"BossIdle.h"
#include"Boss.h"



///<summary>
///コンストラクタ
///</summary>
BossIdle::BossIdle(int& InitializeModelHandle,const int beforeAnimationIndex,const NextStateList initializePreviousStateName, const bool beforeIsChangintMove)
    :StateBase(InitializeModelHandle,Boss::Idle,beforeAnimationIndex)
    ,isIdleWaitCountInitialzie(false)
    ,nextStateName(Idle)
    ,idleWaitCount(0)
{
    //アニメーション速度の初期化
    animationSpeed = InitializeAnimationSpeed;

    // 前回のステートを保存
    previousStateName = initializePreviousStateName;

    isChangingMove = beforeIsChangintMove;
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
    // 体力に合わせて静止状態で止めておく時間を変更して初期化する
    InitializeIdleWaitCount();

    // ターゲットとの距離を出す
    float targetDistance = VSize(VSub(bossTargetPosition,position));

    // カウントを増やす
    idleWaitCount++;

    // 静止状態が一定のカウントを超えたら次の行動に移る
    if (idleWaitCount >= idleWaitCountLimit)
    {
        // どの行動に移るかの選択
        SelectActionPattern(targetDistance);
    }

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
    // 体力が無い時点で死亡ステートに移行
    if (lifeState == Player::NoLife)
    {
        nextState = new BossDead(modelhandle, animationIndex);
    }
    else
    {
        switch (nextStateName)
        {
            // 通常攻撃への移行
        case BossIdle::DefaultAttack:

            nextState = new BossDefaultAttack(modelhandle, animationIndex);

            break;
            // 移動ステートに移行
        case BossIdle::Move:

            nextState = new BossMove(modelhandle, animationIndex);

            break;
            // 範囲攻撃のステートに移行
        case BossIdle::AreaAttack:

            nextState = new BossAreaAttack(modelhandle, animationIndex);

            break;
            // 遠距離攻撃に移行
        case BossIdle::ShotAttack:

            nextState = new BossShotAttack(modelhandle, animationIndex);

            break;
            // 突進攻撃に移行
        case BossIdle::RunAttack:

            nextState = new BossRunAttack(modelhandle, animationIndex);

            break;
        case BossIdle::MoveChange:

            nextState = new BossMoveChange(modelhandle, animationIndex);

            break;
        default:

            nextState = this;

            break;
        }
    }

}

/// <summary>
/// 行動パターンを選択する
/// </summary>
void BossIdle::SelectActionPattern(const float targetDistance)
{
    bool isMatchPreviousState = true;

    // 行動を変えるフラグがたっていればその時点で切り替える
    if (isChangingMove)
    {
        nextStateName = MoveChange;
    }
    else
    {
        while (isMatchPreviousState)
        {
            // 行動選択のためのランダムな値を入れる
            int randPattern = GetRand(RandRange);

            // ターゲットとの距離が近かったら
            if (targetDistance <= ShortRange)
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
            }
            // 中距離だった場合
            else if (targetDistance > ShortRange && targetDistance <= MiddleRange)
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
        }
    }





    printfDx("%f\n", targetDistance);
}

/// <summary>
/// 体力に合わせて静止状態にしておくカウントを変えて初期化
/// </summary>
void BossIdle::InitializeIdleWaitCount()
{
    if (!isIdleWaitCountInitialzie)
    {
        switch (currentHpState)
        {
            // 体力が多い状態での初期化
        case Boss::High:

            idleWaitCountLimit = EasyIdleWaitCountLimit;

            break;
            // 体力が通常状態での初期化
        case Boss::Middle:

            idleWaitCountLimit = NormalIdleWaitCountLimit;
            break;
            // 体力が少ない状態での初期化
        case Boss::Low:

            idleWaitCountLimit = HardIdleWaitCountLimit;
            break;
        default:


            break;
        }

        isIdleWaitCountInitialzie = true;
    }

}
