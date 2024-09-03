#include"BossIdle.h"
#include"BossAreaAttack.h"
#include "BossDefaultAttack.h"
#include"BossDead.h"
#include"Utility.h"
#include"CollisionUtility.h"
#include"EffectManager.h"
#include"SoundManager.h"


const VECTOR BossDefaultAttack::OffsetPosition = VGet(3.0f, 4.0f, 0.0f);

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="InitializeModelHandle">ボスのモデルハンドル</param>
/// <param name="beforeAnimationIndex">前のステートでのアニメーション</param>
BossDefaultAttack::BossDefaultAttack(int& InitializeModelHandle, const int beforeAnimationIndex)
    :StateBase(InitializeModelHandle, Boss::Attack, beforeAnimationIndex)
    ,isAttackParameterInitialize(false)
    ,isChangedAnimationSpeed(false)
{
    //アニメーション速度の初期化
    animationSpeed = InitializeAnimationSpeed;

    // コリジョンマネージャーのインスタンスをもってくる
    collisionManager = CollisionManager::GetInstance();

    // エフェクトマネージャーのインスタンスをもってくる
    effectManager = EffectManager::GetInstance();

    // 音管理クラスのインスタンスをもってくる
    soundManager = SoundManager::GetInstance();

    //当たり判定がまだ生成されていない状態
    collisionData.collisionState = CollisionData::NoCollision;
}

/// <summary>
/// デストラクタ
/// </summary>
BossDefaultAttack::~BossDefaultAttack()
{
    // エフェクトの停止
    effectManager->StopEffect(effectData);
}


/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">プレイヤーモデルの向き</param>
void BossDefaultAttack::Update(VECTOR& modelDirection, VECTOR& characterPosition,const VECTOR bossTargetPosition, VECTOR cameraPosition)
{
    // 攻撃に必要なパラメータの初期化
    InitializeAttackParameter();

    //ステートの切り替え処理を呼ぶ
    ChangeState();

    //アニメーションの再生時間のセット
    UpdateAnimation();


    // アニメーションスピード
    ChangeAnimationSpeed();

    // 向く方向を計算
    VECTOR direction = CalculateTargetDirection(bossTargetPosition, characterPosition);

    // 向きの変更
    modelDirection = direction;

    // アニメーションの再生率が一定のラインを超えたら当たり判定をなくす
    if (animationNowTime / animationLimitTime >= CollisionEndAnimationRatio)
    {
        collisionData.collisionState = CollisionData::CollisionEnded;
    }
    //当たり判定に必要な情報の更新
    UpdateCollisionData(modelDirection,characterPosition);

    // エフェクトの再生に必要な情報の更新
    UpdateEffectData(modelDirection, characterPosition);

    // 当たり判定が有効になった入ればCollisionManagerに送信
    if (collisionData.collisionState == CollisionData::NoCollision)
    {
        //アニメーションの再生割合を調べて当たり判定情報をCollisionManagerに送信する
        collisionData.collisionState = CollisionUtility::SendCollisionDataByAnimationTime(GetAnimationNowTime(), GetAnimationLimitTime(),
            collisionData.collisionState, InitializeCollisionStartAnimationRatio);

        if (collisionData.collisionState == CollisionData::CollisionActive)
        {
            collisionManager->RegisterCollisionData(&collisionData);
            effectManager->PlayEffect(&effectData);
        }
    }

    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();
}

/// <summary>
/// ステートの切り替え処理をまとめたもの
/// </summary>
void BossDefaultAttack::ChangeState()
{
    // 既にステートが切り替えてあれば早期リターン
    if (changedState)
    {
        return;
    }
    else if (currentPlayAnimationState == FirstLoopEnd)
    {
        //ボスの移動ステートに移行
        nextState = new BossIdle(modelhandle, this->GetAnimationIndex(),Boss::DefaultAttack,isChangingMove);
    }
    else
    {
        nextState = this;
    }
}

/// <summary>
/// 当たった時の処理
/// </summary>
void BossDefaultAttack::OnHit(CollisionData collisionData)
{
    //当たり判定を消す
    this->collisionData.collisionState = CollisionData::CollisionEnded;
}

/// <summary>
/// 座標などを当たり判定に必要なデータに変換
/// </summary>
/// <param name="modelDirection">モデルの向いている方向</param>
/// <param name="characterPosition">キャラクターのポジション</param>
void BossDefaultAttack::UpdateCollisionData(const VECTOR& modelDirection, const VECTOR characterPosition)
{
    //当たり判定の座標を移動させる
    position = CollisionUtility::TransrateCollisionCapsulePosition(characterPosition, modelDirection,OffsetPosition,OffsetPositionScale);

    //角度からラジアンに変換する
    float radianAngle = Utility::ConvertRadian(CollisionCapsuleAngle);

    //カプセル回転用のベクトルを用意する
    VECTOR capsuleLineVector = CollisionUtility::RotationCollisionCapsule(radianAngle, modelDirection, position, CollisionCapsuleLineHalfLength);

    //中央座標の代入
    collisionData.centerPosition = position;

    //カプセルの下側の座標
    collisionData.bottomPosition = position;

    //カプセルの上側の座標
    collisionData.upPosition = VAdd(position, capsuleLineVector);

    //カプセルの球部分の半径
    collisionData.radius = CollisionRadius;

    //オブジェクトの種類
    collisionData.hitObjectTag = CollisionManager::BossDefaultAttack;

    //当たった際の関数
    collisionData.onHit = std::bind(&BossDefaultAttack::OnHit, this, std::placeholders::_1);

    //当たった際のダメージ量
    collisionData.damageAmount = DamageAmount;
    
}

/// <summary>
/// エフェクトの再生に必要なデータの更新
/// </summary>
/// <param name="modelDirection">モデルの向き</param>
void BossDefaultAttack::UpdateEffectData(const VECTOR modelDirection,const VECTOR characterPosition)
{
    // エフェクトの座標を代入
    effectData.position = VAdd(characterPosition, VGet(0.0f, EffectOffsetPositionY, 0.0f));

    // モデルの向きからY軸の回転率を出す
    float angle = atan2(modelDirection.x, modelDirection.z);

    // エフェクトの回転率
    effectData.rotationRate = VGet(0.0f, angle, 0.0f);

    // エフェクトの種類
    effectData.effectTag = EffectManager::BossClaw;

    // エフェクトのサイズ
    effectData.scalingRate = VGet(EffectDefaultScale, EffectDefaultScale, EffectDefaultScale);

    // エフェクトの再生速度
    effectData.playSpeed = EffectPlaySpeed;
}

/// <summary>
/// 攻撃に必要なパラメータの初期化
/// </summary>
void BossDefaultAttack::InitializeAttackParameter()
{
    if (! isAttackParameterInitialize)
    {
        switch (currentHpState)
        {
        // 体力が多い状態での初期化
        case Boss::High:

            animationSpeed = EasyAnimationSpeed;

            break;
        // 体力が通常状態での初期化
        case Boss::Middle:

            animationSpeed = NormalAnimationSpeed;
            break;
        // 体力が少ない状態での初期化
        case Boss::Low:

            animationSpeed = HardAnimationSpeed;
            break;
        default:


            break;
        }

        isAttackParameterInitialize = true;
    }

}

/// <summary>
/// アニメーションスピードの切り替え
/// </summary>
void BossDefaultAttack::ChangeAnimationSpeed()
{
    // アニメーションが一定まで再生されたら
    if (animationNowTime / animationLimitTime >= AnimationSpeedChangeRate && ! isChangedAnimationSpeed)
    {
        // 切り替えた瞬間に音を流し始める

        soundManager->PlaySoundEffect(SoundManager::BossDefaultAttack);
        // アニメーションスピードを切り替える
        animationSpeed = InitializeAnimationSpeed;

        // アニメーションのスピードを切り替えたのでフラグを切り替える
        isChangedAnimationSpeed = true;
    }
}

#ifdef _DEBUG

void BossDefaultAttack::DrawCollision()
{
    if (collisionData.collisionState == CollisionData::CollisionActive)
    {
        //当たり判定が正しいかの確認用の描画
        DrawCapsule3D(collisionData.upPosition, collisionData.bottomPosition, collisionData.radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
    }
}
#endif // _DEBUG


