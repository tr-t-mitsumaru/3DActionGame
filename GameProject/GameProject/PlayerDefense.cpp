#include"PlayerDefense.h"
#include"PlayerHit.h"
#include"PlayerIdle.h"


/// <summary>
/// コンストラクタ
/// </summary>
PlayerDefense::PlayerDefense(int modelHandle, int beforeAnimationIndex)
    :StateBase(modelHandle, Player::StartBlocking, beforeAnimationIndex)
{
    // 現在のステートを入れる
    nowStateTag = Player::DefenseState;

    //インプットマネージャーのアドレスを取得
    inputManager = InputManager::GetInstance();

    //アニメーション速度の初期化
    animationSpeed = 1.0f;

}


/// <summary>
/// デストラクタ
/// </summary>
PlayerDefense::~PlayerDefense()
{
    //処理なし
}


/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">プレイヤーモデルの向き</param>
void PlayerDefense::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR playerTargetPosition, VECTOR cameraPosition)
{
    //アニメーションの再生時間のセット
    UpdateAnimation();

    //ステートの切り替え処理を呼ぶ
    ChangeState();

    //一回のループが終わればアニメーションの更新を止める
    if (currentPlayAnimationState == FirstLoopEnd)
    {
        StopAnimation();
    }

    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();
}

void PlayerDefense::ChangeState()
{
    // 既にChangeState以外でステートが切り替えられていなければ
    if (! changedState)
    {
        //LTのキーが押されていればデフェンスステートに移行する
        if (inputManager->GetKeyPushState(InputManager::LT) == InputManager::Push)
        {
            nextState = this;
        }
        else
        {
            nextState = new PlayerIdle(modelhandle, this->GetAnimationIndex());
        }
    }
}
