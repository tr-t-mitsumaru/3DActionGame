#include"PlayerJump.h"
#include"Player.h"
#include"CollisionManager.h"
#include"PlayerIdle.h"


/// <summary>
/// コンストラクタ
/// </summary>
PlayerJump::PlayerJump(int initialModelHandle,int beforeAnimationIndex,VECTOR beforeVelocity)
    :StateBase(initialModelHandle,Player::Jump,beforeAnimationIndex)
    ,falllSpeed(0.0f)
    ,jumpState(Ground)
{
    // 現在のステートを入れる
    nowStateTag = Player::JumpState;

    //アニメーション速度の初期化
    animationSpeed = 0.7f;

    //落下速度の初期化
    falllSpeed = JumpPower;

    //前回のステートで移動があった場合ジャンプ中もそのまま移動させる
    velocity = beforeVelocity;

    //ジャンプはすぐにアニメーションをしてほしいので
    currentPlayAnimationState = StateBase::BlendEnd;
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerJump::~PlayerJump()
{
    //処理なし
}


/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">自身のモデルの向き</param>
/// <param name="position">自身のキャラクターの座標</param>
/// <param name="playerTargetPosition">敵対しているキャラの座標</param>
void PlayerJump::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR playerTargetPosition, VECTOR cameraPosition)
{
    //落下速度を徐々に上げていく
    falllSpeed -= Gravity;

    //落下速度を移動量に反映させる
    velocity.y = velocity.y + falllSpeed;

    //地面にめり込まないように調節
    velocity = CollisionManager::AdjustGroundToWardVelocity(velocity, position);

    if (jumpState == Jump)
    {
        //移動量を足した時の座標を計算する
        float afterPositionY = position.y + velocity.y;

        //int型にcastして誤差を無くして0になったら終了する
        if ((int)(afterPositionY) == 0)
        {
            jumpState = EndJump;
        }
    }

    //アニメーションが一定ラインまで再生が終わったら
    if (currentPlayAnimationState == StateBase::FirstLoop)
    {
        if (this->GetAnimationNowTime() / this->GetAnimationLimitTime() >= AnimationStopLine)
        {
            StopAnimation();
        }
    }
    //ループ再生されないように一回再生されたら止める
    if (currentPlayAnimationState == StateBase::FirstLoopEnd)
    {
        StopAnimation();
    }

    //地面に近くなったらアニメーションを開始する
    if (position.y < 1)
    {
        StartAnimation();
    }
    //アニメーションの再生時間のセット
    UpdateAnimation();

    //ステートの切り替え
    ChangeState();


    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();

    //一度目の処理が終わったらジャンプ中にステートを切り替える
    if (jumpState == Ground)
    {
        jumpState = Jump;
    }



}

/// <summary>
/// ステートの切り替え処理をまとめたもの
/// </summary>
void PlayerJump::ChangeState()
{
    //ジャンプが終了していたらステートを静止状態に切り替える
    if (jumpState == EndJump)
    {
        nextState = new PlayerIdle(modelhandle, this->GetAnimationIndex());
    }
    else
    {
        nextState = this;
    }
}
