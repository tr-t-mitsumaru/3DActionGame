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

    //ステートの切り替え処理を呼ぶ
    ChangeState();

    //アニメーションの再生時間のセット
    UpdateAnimation();

    //一回のループが終わればアニメーションの更新を止める
    if (currentPlayAnimationState == FirstRoopEnd)
    {
        StopAnimation();
    }

    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();
}

void PlayerDefense::ChangeState()
{

    // ダメージを受けていたらヒットステートに移行
    if (lifeState == Player::Damaged)
    {
        nextState = new PlayerHit(modelhandle, animationIndex, Player::BlockingImpact);
    }
    //LTのキーが押されていればデフェンスステートに移行する
    else if (inputManager->GetKeyPushState(InputManager::LT) == InputManager::Push)
    {
        nextState = this;
    }
    else
    {
        nextState = new PlayerIdle(modelhandle, this->GetAnimationIndex());
    }
}
