#include"InputManager.h"
#include"Player.h"
#include"PlayerIdle.h"
#include"PlayerMove.h"
#include"PlayerAttack.h"
#include"PlayerDefense.h"
#include"PlayerRolling.h"
#include"PlayerShotMagic.h"
#include"PlayerJump.h"


/// <summary>
/// コンストラクタ
/// </summary>
PlayerIdle::PlayerIdle(int& modelHandle,const int beforeAnimationIndex)
    :StateBase(modelHandle,Player::Idle,beforeAnimationIndex)
    , isGround(true)
{
    //インプットマネージャーのアドレスを取得
    inputManager = InputManager::GetInstance();

    //アニメーション速度の初期化
    animationSpeed = 0.5f;
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerIdle::~PlayerIdle()
{
    //処理なし
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">自身のモデルの向き</param>
/// <param name="position">自身のキャラクターの座標</param>
/// <param name="playerTargetPosition">敵対しているキャラの座標</param>
void PlayerIdle::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR playerTargetPosition)
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
void PlayerIdle::ChangeState()
{

    //何かしらの移動キーが押されていた場合移動ステートに切り返る
    if (inputManager->GetKeyPushState(InputManager::Move) == InputManager::Push)
    {
        nextState = new PlayerMove(modelhandle,this->GetAnimationIndex());
    }
    //RBのキーかRTキーが押されていれば攻撃ステートに変更
    else if (inputManager->GetKeyPushState(InputManager::RB) == InputManager::Push ||
             inputManager->GetKeyPushState(InputManager::RT) == InputManager::Push)
    {
        //押されたボタンによって強攻撃のアニメーションにするか
        //通常攻撃のアニメーションにするか変更する
        Player::AnimationState animationState;
        if (inputManager->GetKeyPushState(InputManager::RB) == InputManager::Push)
        {
            animationState = Player::Slash;
        }
        else
        {
            animationState = Player::Clash;
        }
        nextState = new PlayerAttack(modelhandle, this->GetAnimationIndex(), animationState);
    }
    //LTのキーが押されていればデフェンスステートに移行する
    else if (inputManager->GetKeyPushState(InputManager::LT) == InputManager::Push)
    {
        nextState = new PlayerDefense(modelhandle, this->GetAnimationIndex());
    }
    //Aキーが押されて接地していれば
    else if (inputManager->GetKeyPushState(InputManager::A) == InputManager::Push && isGround)
    {
        nextState = new PlayerJump(modelhandle, this->GetAnimationIndex(), velocity);
        isGround = false;
    }
    //Bキーが押されていれば回避状態のステート
    else if (inputManager->GetKeyPushState(InputManager::B) == InputManager::Push)
    {
        nextState = new PlayerRolling(modelhandle, this->GetAnimationIndex());
    }
    //LBキーで射撃ステートに移行
    else if (inputManager->GetKeyPushState(InputManager::LB) == InputManager::Push)
    {
        nextState = new PlayerShotMagic(modelhandle, this->GetAnimationIndex());
    }
    //ステート移行が無ければ自身のポインタを渡す
    else
    {
        nextState = this;
    }
    //ToDo
    //他にも死亡時と被弾時があるが当たり判定作成時に作ります
}

