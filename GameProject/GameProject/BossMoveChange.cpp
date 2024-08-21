#include"BossMoveChange.h"
#include"EffectManager.h"
#include"BossIdle.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
/// <param name="beforeAnimationIndex">前のステートのアニメーションの種類</param>
BossMoveChange::BossMoveChange(int& modelHandle, const int beforeAnimationIndex)
    :StateBase(modelHandle,Boss::Intimidation,beforeAnimationIndex)
    ,isPlaedEffect(false)
{
    // アニメーションスピードの初期化
    animationSpeed = InitializeAnimationSpeed;

    // エフェクトマネージャーのインスタンスをもってくる
    effectManager = EffectManager::GetInstance();
}

/// <summary>
/// デストラクタ
/// </summary>
BossMoveChange::~BossMoveChange()
{
    // 処理なし
}

/// 更新処理
/// </summary>
/// <param name="position">自身のモデルの向き</param>
/// <param name="position">自身のキャラクターの座標</param>
/// <param name="targetPosition">敵対しているキャラの座標</param>
void BossMoveChange::Update(VECTOR& modelDirection, VECTOR& position, const VECTOR targetPosition, VECTOR cameraPosition)
{
    // ステートの切り替え処理を呼ぶ
    ChangeState();

    // アニメーションの再生時間のセット
    UpdateAnimation();

    // エフェクトデータの更新
    UpdateEffectData(position);

    // アニメーションに合わせてエフェクトを再生する
    PlayEffectByAnimationTime();

    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();
}

/// <summary>
/// ステートの切り替え処理をまとめたもの
/// </summary>
void BossMoveChange::ChangeState()
{
    // アニメーションの最初のループが終了したら
    if (currentPlayAnimationState == FirstRoopEnd)
    {
        //ボスの静止ステートに移行
        nextState = new BossIdle(modelhandle, this->GetAnimationIndex(), BossIdle::MoveChange,isChangingMove);
    }
    else
    {
        nextState = this;
    }
}

/// <summary>
/// エフェクトの再生に必要なデータの更新
/// </summary>
/// <param name="characterPosition">キャラクターの座標</param>
void BossMoveChange::UpdateEffectData(const VECTOR characterPosition)
{
    // エフェクトの座標を代入
    effectData.position = characterPosition;

    // エフェクトの回転率はいらないので0で初期化
    effectData.rotationRate = VGet(0, 0, 0);

    // ボスの威嚇時のエフェクトにする
    effectData.effectTag = EffectManager::BossIntimidation;

    // ボスのエフェクトのサイズを代入する
    effectData.scalingRate =  EffectDefaultScale;

    // エフェクトの再生速度を設定する
    effectData.playSpeed = EffectPlaySpeed;

}

/// <summary>
/// アニメーションの再生率に合わせてエフェクトを再生する
/// </summary>
void BossMoveChange::PlayEffectByAnimationTime()
{
    if (animationNowTime / animationLimitTime >= EffectPlayAnimationRatio && !isPlaedEffect)
    {
        effectManager->PlayEffect(&effectData);
        isPlaedEffect = true;
    }
}
