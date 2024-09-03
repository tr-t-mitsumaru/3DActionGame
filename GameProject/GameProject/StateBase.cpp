#include"DxLib.h"
#include"StateBase.h"
#include"PlayerHit.h"
#include"PlayerDead.h"
#include"BossDead.h"
#include"Utility.h"





/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
/// <param name="animationState">アニメーションの状態</param>
StateBase::StateBase(int& modelHandle,const int initializeAnimationState,const int beforeAnimationIndex)
    :velocity(VGet(0,0,0))
    ,animationBlendRate(0.0f)
    ,beforeAnimationIndex(-1)
    ,currentPlayAnimationState(BlendStart)
    ,currentHpState(Boss::High)
    ,isChangingMove(false)
    ,attachedAnimation(false)
    ,changedState(false)
{
    //もってきたモデルハンドルを代入
    this->modelhandle = modelHandle;

    animationNumber = initializeAnimationState;

    //アニメーションの再生時間の初期化
    animationNowTime = 0.0f;
    //前回のアニメーションの添え字を代入
    this->beforeAnimationIndex = beforeAnimationIndex;
}

/// <summary>
/// デストラクタ
/// </summary>
StateBase::~StateBase()
{
    //処理なし
}

/// <summary>
/// アニメーションの更新処理
/// </summary>
void StateBase::UpdateAnimation(const float blendSpeed)
{
    if (!attachedAnimation)
    {
        //アニメーションをアタッチ
        animationIndex = MV1AttachAnim(this->modelhandle, animationNumber, -1, FALSE);
        //アニメーションの総再生時間を取得
        animationLimitTime = MV1GetAttachAnimTotalTime(this->modelhandle, animationIndex);

        attachedAnimation = true;
    }
    //前回のアニメーションがある場合
    if (beforeAnimationIndex != -1 && currentPlayAnimationState == BlendStart)
    {
        //前回とのアニメーションをブレンドして表示
        animationBlendRate += blendSpeed;
        //ブレンドが終わったら
        if (animationBlendRate >= 1.0f)
        {
            currentPlayAnimationState = BlendEnd;
            //前のアニメーションをでタッチ
            MV1DetachAnim(modelhandle, beforeAnimationIndex);
            beforeAnimationIndex = -1;
        }
        MV1SetAttachAnimBlendRate(modelhandle, beforeAnimationIndex, 1.0f - animationBlendRate);
        MV1SetAttachAnimBlendRate(modelhandle, animationIndex, animationBlendRate);
    }
    else if(currentPlayAnimationState != Stop)
    {
        // 再生時間を進める
        animationNowTime += animationSpeed;
        // 再生時間をセットする
        MV1SetAttachAnimTime(modelhandle, animationIndex, animationNowTime);
        // 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
        if (animationNowTime >= animationLimitTime)
        {
            animationNowTime = 0.0f;
            currentPlayAnimationState = FirstLoopEnd;
        }
    }

}

/// <summary>
/// アニメーションを止める処理
/// </summary>
void StateBase::StopAnimation()
{
    currentPlayAnimationState = Stop;
}



/// <summary>
/// アニメーションを開始させる処理
/// </summary>
void StateBase::StartAnimation()
{
    currentPlayAnimationState = FirstLoop;
}

/// <summary>
/// シーンが切り替わってた際にアニメーションをデタッチする
/// </summary>
void StateBase::DetachAnimation()
{
    if (nextState != this)
    {
        MV1DetachAnim(modelhandle, beforeAnimationIndex);
        beforeAnimationIndex = -1;
    }
}

/// <summary>
/// ダメージを受けた際の関数
/// </summary>
void StateBase::OnDamage()
{
    // ガード中とそれ以外で渡すアニメーションステートを変える
    if (nowStateTag == Player::DefenseState)
    {
        nextState = new PlayerHit(modelhandle, animationIndex, Player::BlockingImpact);
    }
    else
    {
        nextState = new PlayerHit(modelhandle, animationIndex, Player::Impact);
    }
    changedState = true;
}

/// <summary>
/// ライフが0になった状態に設定する
/// </summary>
void StateBase::SetPlayerNoLifeState()
{
    nextState = new PlayerDead(modelhandle, animationIndex);

    changedState = true;
}

/// <summary>
/// ボスの体力が0になったさいの処理
/// </summary>
void StateBase::SetBossNoLifeState()
{
    nextState = new BossDead(modelhandle, animationIndex);

    changedState = true;
}

/// <summary>
/// 移動方向の設定
/// </summary>
/// <param name="targetPosition">移動したい目標の座標</param>
/// <param name="position">自身の座標</param>
/// <returns>設定した方向</returns>
VECTOR StateBase::CalculateTargetDirection(const VECTOR targetPosition, const VECTOR position)
{
    // 移動させる方向を出す
    VECTOR direction = VSub(targetPosition, position);

    // 正規化
    return  direction = VNorm(direction);
}

/// <summary>
/// 行動を変えるフラグの切り替えをおこなう
/// </summary>
void StateBase::SwitchIsChangingMove()
{
    isChangingMove = true;
}
