#include"BossIdle.h"
#include"Boss.h"
#include"StateBase.h"
#include"BossMove.h"



///<summary>
///コンストラクタ
///</summary>
BossIdle::BossIdle(int& InitializeModelHandle,const int beforeAnimationIndex)
    :StateBase(InitializeModelHandle,Boss::Idle,beforeAnimationIndex)
{
    //アニメーション速度の初期化
    animationSpeed = InitializeAnimationSpeed;
    //インプットマネージャーのインスタンスをもってくる
    inputManager = InputManager::GetInstance();
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
void BossIdle::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition)
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
void BossIdle::ChangeState()
{
    //ToDo
    //BossのAIを作るまではボタンでステートが遷移するようにしている
    if (inputManager->GetKeyPushState(InputManager::LeftStick)== InputManager::JustRelease)
    {
        //ボスの移動ステートに移行
        nextState = new BossMove(modelhandle, this->GetAnimationIndex());
    }
    else
    {
        nextState = this;
    }
}
