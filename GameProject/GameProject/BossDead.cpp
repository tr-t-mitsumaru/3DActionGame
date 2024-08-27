#include"BossDead.h"
#include"BossIdle.h"
#include"SoundManager.h"


///<summary>
///コンストラクタ
///</summary>
BossDead::BossDead(int& InitializeModelHandle, const int beforeAnimationIndex)
    :StateBase(InitializeModelHandle, Boss::Dead, beforeAnimationIndex)
    ,playedDeadSoundEffect(false)
{
    //アニメーション速度の初期化
    animationSpeed = InitializeAnimationSpeed;

    // 音管理クラスのインスタンスをもってくる
    soundManager = SoundManager::GetInstance();

}

/// <summary>
/// デストラクタ
/// </summary>
BossDead::~BossDead()
{
    //処理なし
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">自身のモデルの向き</param>
/// <param name="position">自身のキャラクターの座標</param>
/// <param name="bossTargetPosition">ボスから見てターゲットになる座標</param>
/// <param name="targetPosition">敵対しているキャラの座標</param>
void BossDead::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition,VECTOR cameraPosition)
{
    // 一度だけボスが死んだ際の音を流す
    if (!playedDeadSoundEffect)
    {
        soundManager->PlaySoundEffect(SoundManager::BossDead);
        playedDeadSoundEffect = true;
    }

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
void BossDead::ChangeState()
{

    nextState = this;
}
