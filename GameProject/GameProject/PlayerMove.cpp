#include"PlayerMove.h"
#include"InputManager.h"
#include"Player.h"
#include"PlayerHit.h"
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
    // 現在のステートを入れる
    nowStateTag = Player::MoveState;

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
void PlayerMove::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR playerTargetPosition, VECTOR cameraPosition)
{

    velocity = VGet(0, 0, 0);

    //正規化した移動方向を決める
    VECTOR direction = DecisionDirection(cameraPosition,position);

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

    //アニメーションの再生時間のセット
    UpdateAnimation();

    //ステートの切り替え処理を呼ぶ
    ChangeState();



    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();

}

/// <summary>
/// ステートの切り替え処理をまとめたもの
/// </summary>
void PlayerMove::ChangeState()
{
    // ダメージを受けていたらヒットステートに移行
    if (lifeState == Player::Damaged)
    {
        nextState = new PlayerHit(modelhandle, animationIndex, Player::Impact);
        return;
    }
    //RBのキーが押されていれば攻撃ステートに変更
    else if (inputManager->GetKeyPushState(InputManager::X) == InputManager::Push ||
        inputManager->GetKeyPushState(InputManager::RB) == InputManager::Push)
    {
        //押されたボタンによって強攻撃のアニメーションにするか
        //通常攻撃のアニメーションにするか変更する
        Player::AnimationState animationState;
        if (inputManager->GetKeyPushState(InputManager::X) == InputManager::Push)
        {
            animationState = Player::ComboAttack;
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
    //Bキーが押されていれば回避状態のステート
    else if (inputManager->GetKeyPushState(InputManager::A) == InputManager::Push)
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
/// 移動方向の設定
/// </summary>
/// <param name="cameraPosition">カメラの座標</param>
/// <param name="characterPosition">自身の座標</param>
/// <returns>設定された座標</returns>
VECTOR PlayerMove::DecisionDirection(const VECTOR cameraPosition,const VECTOR characterPosition)
{
    VECTOR direction = VGet(0, 0, 0);

    // カメラからキャラクターのベクトルを出す
    VECTOR cameraVector = VSub(characterPosition, cameraPosition);

    // Y座標は移動させたくないので0にする
    cameraVector.y = 0;

    // 任意のベクトル (0, 1, 0) を使用して新しいX軸を定義
    VECTOR xAxis = VNorm(VCross(VGet(0.0f, 1.0f, 0.0f), cameraVector));

    // 新しいY軸を計算
    VECTOR yAxis = VNorm(VCross(cameraVector, xAxis));

    //キーに合わせて移動を行う
    if (inputManager->GetKeyPushState(InputManager::Left) == InputManager::Push)
    {
        direction = VAdd(direction,VScale(xAxis, -1));
    }
    if (inputManager->GetKeyPushState(InputManager::Right) == InputManager::Push)
    {
        direction = VAdd(direction,xAxis);
    }
    if (inputManager->GetKeyPushState(InputManager::Up) == InputManager::Push)
    {
        direction = VAdd(direction,VNorm(cameraVector));
    }
    if (inputManager->GetKeyPushState(InputManager::Down) == InputManager::Push)
    {
        direction = VAdd(direction,VScale(VNorm(cameraVector), -1));
    }
    

    // 正規化
    if (VSquareSize(direction) > 0)      //directionのサイズを2乗にして返す(二乗にすることでdirectionに値が入っていればifに入る
    {
        direction = VNorm(direction);    //各成分のサイズを１にする
    }

    return direction;

}
