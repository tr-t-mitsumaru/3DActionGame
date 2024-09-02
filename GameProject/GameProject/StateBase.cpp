#include"DxLib.h"
#include"StateBase.h"
#include"Utility.h"




/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
/// <param name="animationState">アニメーションの状態</param>
StateBase::StateBase(int& modelHandle,const int animationState,const int beforeAnimationIndex)
    :velocity(VGet(0,0,0))
    ,animationBlendRate(0.0f)
    ,beforeAnimationIndex(-1)
    ,currentPlayAnimationState(BlendStart)
    ,lifeState(Player::NoDamage)
{
    //もってきたモデルハンドルを代入
    this->modelhandle = modelHandle;
    //アニメーションをアタッチ
    animationIndex = MV1AttachAnim(this->modelhandle, animationState, -1, FALSE);
    //アニメーションの総再生時間を取得
    animationLimitTime = MV1GetAttachAnimTotalTime(this->modelhandle, animationIndex);
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
    if (nextState != this && beforeAnimationIndex != -1)
    {
        MV1DetachAnim(modelhandle, beforeAnimationIndex);
    }
}

/// <summary>
/// ダメージを受けた際の関数
/// </summary>
void StateBase::OnDamage()
{
    // ダメージを受けた状態に設定
    lifeState = Player::Damaged;
}

/// <summary>
/// ライフが0になった状態に設定する
/// </summary>
void StateBase::SetNoLifeState()
{
    // ライフが0の状態に設定
    lifeState = Player::NoLife;
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

