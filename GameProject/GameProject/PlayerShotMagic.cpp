#include"PlayerShotMagic.h"
#include"PlayerHit.h"
#include"PlayerIdle.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="initalModelHandle">モデルハンドル</param>
PlayerShotMagic::PlayerShotMagic(int initalModelHandle, int beforeAnimationIndex)
    :StateBase(initalModelHandle, Player::Spell, beforeAnimationIndex)
{
    // 現在のステートを入れる
    nowStateTag = Player::ShotState;

    //アニメーション速度の初期化
    animationSpeed = 1.0f;
}


/// <summary>
/// デストラクタ
/// </summary>
PlayerShotMagic::~PlayerShotMagic()
{
    //処理なし
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">自身のモデルの向き</param>
/// <param name="position">自身のキャラクターの座標</param>
/// <param name="playerTargetPosition">敵対しているキャラの座標</param>
void PlayerShotMagic::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR playerTargetPosition, VECTOR cameraPosition)
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
void PlayerShotMagic::ChangeState()
{
    // ダメージを受けていたらヒットステートに移行
    if (lifeState == Player::Damaged)
    {
        nextState = new PlayerHit(modelhandle, animationIndex, Player::Impact);
    }
    //アニメーションの再生が終了したらステートを切り替える
    else if (currentPlayAnimationState == FirstRoopEnd)
    {
        nextState = new PlayerIdle(modelhandle, this->GetAnimationIndex());
    }
    else
    {
        nextState = this;
    }
}
