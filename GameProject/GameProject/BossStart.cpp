#include"Boss.h"
#include"StateBase.h"
#include "BossStart.h"
#include"BossIdle.h"


///<summary>
///コンストラクタ
///</summary>
BossStart::BossStart(int& InitializeModelHandle, const int beforeAnimationIndex)
    :StateBase(InitializeModelHandle, Boss::Standing, beforeAnimationIndex)
    ,currentStartMoveState(SitDown)
{
    //アニメーション速度の初期化
    animationSpeed = StandingAnimationSpeed;

    inputManager = InputManager::GetInstance();

    //アニメーションを止めた状態で始める
    currentPlayAnimationState = Stop;
}

/// <summary>
/// デストラクタ
/// </summary>
BossStart::~BossStart()
{
    //処理なし
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">自身のモデルの向き</param>
/// <param name="position">自身のキャラクターの座標</param>
/// <param name="bossTargetPosition">敵対しているキャラの座標</param>
void BossStart::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition, VECTOR cameraPosition)
{


    //アニメーションの再生時間のセット
    UpdateAnimation(0.02f);

    //アニメーションの状態を変更させる
    ChangeStartMoveState();

    SwitchAnimation();

    //ステートの切り替え処理を呼ぶ
    ChangeState();


    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();

}


/// <summary>
/// ステートの切り替え処理をまとめたもの
/// </summary>
void BossStart::ChangeState()
{
    //ToDo
    //BossのAIを作るまではボタンでステートが遷移するようにしている
    //本来はcurrentStartMoveStateがStandになったら状態を遷移ささせる
    if (currentPlayAnimationState == FirstRoopEnd)
    {
        //ボスの移動ステートに移行
        nextState = new BossIdle(modelhandle, this->GetAnimationIndex(),BossIdle::None);
    }
    else
    {
        nextState = this;
    }
}

/// <summary>
/// 行動開始前のアニメーションの状態変更させるための関数
/// </summary>
void BossStart::ChangeStartMoveState()
{
    //カウントが一定のラインに達するまで動きを止めておく
    if (currentStartMoveState == SitDown)
    {
        startCount++;
        //カウントが超えたら立ち上がらせる
        if (startCount >= StartCountLimit)
        {
            currentStartMoveState = Standing;
        }
    }
    //立ち上がっている状態の処理
    else if (currentStartMoveState == Standing)
    {
        //アニメーションが開始されていなければ開始させる
        if (currentPlayAnimationState == Stop)
        {
            StartAnimation();
        }
        //立ち上がり終わっていたら状態を変更させる
        else if (animationNowTime / animationLimitTime >= SwitchAnimationRatio)
        {
            currentStartMoveState = Stand;
            animationSpeed = 0.5f;
        }
    }
}

/// <summary>
/// アニメーションの切り替え
/// </summary>
void BossStart::SwitchAnimation()
{
    // アニメーションの1ループが終了したら
    if (animationNowTime/animationLimitTime >= SwitchAnimationRatio && currentStartMoveState == Stand)
    {
        // 前のステートのアニメーションをデタッチ
        MV1DetachAnim(modelhandle, beforeAnimationIndex);

        // 現在のアニメーションインデックスを前のインデックスに入れる
        beforeAnimationIndex = animationIndex;

        //アニメーションをアタッチ
        animationIndex = MV1AttachAnim(modelhandle, Boss::Intimidation, -1, FALSE);

        // アニメーションの総再生時間を取得
        animationLimitTime = MV1GetAttachAnimTotalTime(modelhandle, animationIndex);

        //アニメーションの再生時間の初期化
        animationNowTime = 0.0f;

        currentPlayAnimationState = StateBase::BlendStart;

        currentStartMoveState = Intimidation;
    }
}



