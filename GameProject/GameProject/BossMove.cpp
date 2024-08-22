#include"StateBase.h"
#include"Boss.h"
#include"BossMove.h"
#include"BossDefaultAttack.h"



/// <summary>
/// コンストラクタ
/// </summary>
BossMove::BossMove(int& InitializeModelHandle, const int beforeAnimationIndex)
    :StateBase(InitializeModelHandle, Boss::Walk, beforeAnimationIndex)
{
    //アニメーション速度の初期化
    animationSpeed = InitializeAnimationSpeed;

    inputManager = InputManager::GetInstance();
}

/// <summary>
/// デストラクタ
/// </summary>
BossMove::~BossMove()
{
    //処理なし
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">プレイヤーモデルの向き</param>
void BossMove::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition, VECTOR cameraPosition)
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
void BossMove::ChangeState()
{
    //ToDo
    //BossのAIを作るまではボタンでステートが遷移するようにしている
    if (inputManager->GetKeyPushState(InputManager::LeftStick) == InputManager::JustRelease)
    {
        nextState = new BossDefaultAttack(modelhandle, this->GetAnimationIndex());
    }
    else
    {
        nextState = this;
    }
}
