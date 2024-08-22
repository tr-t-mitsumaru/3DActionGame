#include"PlayerMove.h"
#include"InputManager.h"
#include"Player.h"
#include"PlayerIdle.h"
#include"PlayerAttack.h"
#include"PlayerDefense.h"
#include"PlayerRolling.h"
#include"PlayerShotMagic.h"
#include"PlayerJump.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerMove::PlayerMove(int modelHandle,int beforeAnimationIndex)
    :StateBase(modelHandle,Player::Walk,beforeAnimationIndex)
    , isGround(true)
{
    //インプットマネージャーのアドレスを取得
    inputManager = InputManager::GetInstance();

    //アニメーション速度の初期化
    animationSpeed = 1.4f;


}

/// <summary>
/// デストラクタ
/// </summary>
PlayerMove::~PlayerMove()
{
    //処理なし
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">自身のモデルの向き</param>
/// <param name="position">自身のキャラクターの座標</param>
/// <param name="playerTargetPosition">敵対しているキャラの座標</param>
void PlayerMove::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR playerTargetPosition)
{
    velocity = VGet(0, 0, 0);

    //正規化した移動方向を決める
    VECTOR direction = DecisionDirection();

    //移動キーのどれかのビットがたっていれば方向をモデルに反映させる
    if (inputManager->GetKeyPushState(InputManager::Move) == InputManager::Push)
    {
        modelDirection = direction;
    }

    //地面に接地していたら
    if ((int)(position.y) == 0)
    {
        isGround = true;
    }

    // 移動量を出す
    velocity = VScale(direction, MoveSpeed);


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
void PlayerMove::ChangeState()
{

    //RBのキーが押されていれば攻撃ステートに変更
    if (inputManager->GetKeyPushState(InputManager::RB) == InputManager::Push ||
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
    else if (inputManager->GetKeyPushState(InputManager::A) && isGround)
    {
        nextState = new PlayerJump(modelhandle,this->GetAnimationIndex(), velocity);
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
    else if(inputManager->GetKeyPushState(InputManager::Move) == InputManager::Push)
    {
        nextState = this;
    }
    //上記の状態にならなかったら静止状態に戻す
    else
    {
        nextState = new PlayerIdle(modelhandle, this->GetAnimationIndex());
    }
}

/// <summary>
/// 移動方向の選択
/// </summary>
/// <param name="index">複数押される場合があるのでそれの添え字</param>
/// <returns>調整された移動方向</returns>
VECTOR PlayerMove::DecisionDirection()
{
    VECTOR direction = VGet(0, 0, 0);

    //キーに合わせて移動を行う
    if (inputManager->GetKeyPushState(InputManager::Left) == InputManager::Push)
    {
        direction = VAdd(direction, VGet(-1, 0, 0));
    }
    if (inputManager->GetKeyPushState(InputManager::Right) == InputManager::Push)
    {
        direction = VAdd(direction, VGet(1, 0, 0));
    }
    if (inputManager->GetKeyPushState(InputManager::Up) == InputManager::Push)
    {
        direction = VAdd(direction, VGet(0, 0, 1));
    }
    if (inputManager->GetKeyPushState(InputManager::Down) == InputManager::Push)
    {
        direction = VAdd(direction, VGet(0, 0, -1));
    }
    

    // 正規化
    if (VSquareSize(direction) > 0)      //directionのサイズを2乗にして返す(二乗にすることでdirectionに値が入っていればifに入る
    {
        direction = VNorm(direction);    //各成分のサイズを１にする
    }

    return direction;

}
