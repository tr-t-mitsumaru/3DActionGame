#include"Player.h"
#include"PlayerDead.h"
#include"PlayerIdle.h"
#include"PlayerHit.h"
#include"SoundManager.h"



/// <summary>
/// コンストラクタ
/// </summary>
PlayerHit::PlayerHit(int InitalModelHandle, int beforeAnimationIndex, Player::AnimationState animationState)
    :StateBase(InitalModelHandle,animationState, beforeAnimationIndex)
    ,playedHitVoice(false)
{
    // 音管理クラスのインスタンスをもってくる
    soundManager = SoundManager::GetInstance();

    // 現在のステートを入れる
    nowStateTag = Player::HitState;

    // アニメーションの速度を代入
    animationSpeed = AnimationSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerHit::~PlayerHit()
{
    // 処理なし
}


/// <summary>
/// 更新処理
/// </summary>
/// <param name="modelDirection">モデルの向き</param>
/// <param name="characterPosition">キャラクターの座標</param>
void PlayerHit::Update(VECTOR& modelDirection, VECTOR& position, const VECTOR targetPosition, VECTOR cameraPosition)
{
    // ボイスを再生していない場合は再生する
    if (! playedHitVoice)
    {
        soundManager->PlaySoundEffect(SoundManager::DamageVoice);
        playedHitVoice = true;
    }
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
void PlayerHit::ChangeState()
{
    if (!changedState)
    {
        //アニメーションの再生が終了したらステートを切り替える
        if (currentPlayAnimationState == FirstLoopEnd)
        {
            nextState = new PlayerIdle(modelhandle, this->GetAnimationIndex());
        }
        else
        {
            nextState = this;
        }
    }
}
