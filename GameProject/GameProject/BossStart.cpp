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
    animationSpeed = InitializeAnimationSpeed;

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
/// <param name="targetPosition">敵対しているキャラの座標</param>
void BossStart::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR targetPosition,VECTOR cameraPosition)
{


    //アニメーションの再生時間のセット
    UpdateAnimation();

    //アニメーションの状態を変更させる
    ChangeStartMoveState();

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
    if (inputManager->GetKeyPushState(InputManager::LeftStick) == InputManager::JustRelease)
    {
        //ボスの移動ステートに移行
        nextState = new BossIdle(modelhandle, this->GetAnimationIndex());
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
        else if (this->GetAnimationNowTime() >= this->GetAnimationLimitTime())
        {
            currentStartMoveState == Stand;
        }
    }
}



