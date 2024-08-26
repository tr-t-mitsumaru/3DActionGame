#include"InitializeShotData.h"
#include"PlayerShotMagic.h"
#include"PlayerHit.h"
#include"PlayerIdle.h"
#include"EffectData.h"
#include"EffectManager.h"
#include"CollisionUtility.h"

const VECTOR PlayerShotMagic::OffsetEffectPosition = VGet(0.0f, 1.5f, 0.0f);


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="InitalModelHandle">モデルハンドル</param>
PlayerShotMagic::PlayerShotMagic(int InitalModelHandle, int beforeAnimationIndex)
    :StateBase(InitalModelHandle, Player::Spell, beforeAnimationIndex)
    ,isShotFired(false)
    ,isPlaiedEffect(false)
{
    // 現在のステートを入れる
    nowStateTag = Player::ShotState;

    //アニメーション速度の初期化
    animationSpeed = 1.0f;

    // ショットマネージャーのインスタンスをもってくる
    shotManager = ShotManager::GetInstance();

    // エフェクトマネージャーのインスタンスをもってくる
    effectManager = EffectManager::GetInstance();
}


/// <summary>
/// デストラクタ
/// </summary>
PlayerShotMagic::~PlayerShotMagic()
{
    // 再生していたエフェクトを停止させる
    effectManager->StopEffect(effectData);
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">自身のモデルの向き</param>
/// <param name="position">自身のキャラクターの座標</param>
/// <param name="playerTargetPosition">敵対しているキャラの座標</param>
void PlayerShotMagic::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR playerTargetPosition, VECTOR cameraPosition)
{
    //アニメーションの再生時間のセット
    UpdateAnimation();

    //ステートの切り替え処理を呼ぶ
    ChangeState();

    // アニメーションの再生時間に合わせてショットを生成する
    CreateShotByAnimationRatio(position, playerTargetPosition, modelDirection);

    // エフェクトデータの更新
    UpdateEffectData(position, modelDirection);

    // 前のアニメーションとのブレンドが終了していたら
    // エフェクトを再生する
    if (currentPlayAnimationState == BlendEnd && ! isPlaiedEffect)
    {
        effectManager->PlayEffect(&effectData);

        // エフェクトを再生したフラグをたてる
        isPlaiedEffect = true;
    }

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


/// <summary>
/// アニメーションの再生率によってショットを作成する
/// </summary>
/// <param name="position">座標</param>
/// <param name="targetPosition">目標の座標</param>
/// <param name="modelDirection">モデルの向き</param>
void PlayerShotMagic::CreateShotByAnimationRatio(const VECTOR position, const VECTOR targetPosition, VECTOR& modelDirection)
{
    if (animationNowTime / animationLimitTime >= ShotCreateAnimationRatio && !isShotFired)
    {
        // 初期化用のデータを作成
        InitializeShotData initializeShotData = AssignInitializeShotData(position, targetPosition);

        // 弾を撃った方向にモデルを回転させる
        modelDirection = initializeShotData.direction;

        // 必要な情報を代入して弾を作成
        shotManager->SpawnShot(initializeShotData);

        // ショットを売ったフラグをたてる
        isShotFired = true;

    }
}


/// <summary>
/// 弾の作成に必要な情報を代入する
/// </summary>
/// <param name="position">自身の座標</param>
/// <param name="targetPosition">弾を撃つ目標の座標</param>
/// <returns>初期化した弾のデータ</returns>
InitializeShotData PlayerShotMagic::AssignInitializeShotData(const VECTOR position, const VECTOR targetPosition)
{
    // 初期化用のデータを作成
    InitializeShotData initializeShotData;

    // 自身とターゲットのポジションから移動方向を作成
    VECTOR direction = VSub(targetPosition, position);
    direction = VNorm(direction);


    // 座標
    initializeShotData.position = VAdd(position,VGet(0.0f,2.0f,0.0f));

    //方向
    initializeShotData.direction = direction;

    // 速度
    initializeShotData.speed = ShotSpeed;

    // 弾の半径
    initializeShotData.radius = ShotRadius;

    // 弾の種類
    initializeShotData.shooterTag = CollisionManager::PlayerShot;

    // 弾のダメージ
    initializeShotData.damageAmount = ShotDamageAmount;

    // プレイヤーのショット攻撃で初期化する
    initializeShotData.effectTag = EffectManager::PlayerShot;

    // エフェクトの回転率を設定
    initializeShotData.effectRotationRate = VGet(0, 0, 0);

    // エフェクトのサイズを設定
    initializeShotData.effectScalingRate = VGet(EffectDefaultScale, EffectDefaultScale, EffectDefaultScale);

    // エフェクトの再生速度の設定
    initializeShotData.effectPlaySpeed = EffectPlaySpeed;

    // 初期化したデータを返す
    return initializeShotData;
}

/// <summary>
/// エフェクトデータの初期化
/// </summary>
void PlayerShotMagic::UpdateEffectData(const VECTOR characterPosition, const VECTOR modelDirection)
{
    // エフェクトの種類
    effectData.effectTag = EffectManager::PlayerMagicCircle;

    // エフェクトを描画する座標
    effectData.position = CollisionUtility::TransrateCollisionCapsulePosition(characterPosition, modelDirection,
        OffsetEffectPosition, OffsetEffectPositionScale);

    float angle = atan2(modelDirection.x, modelDirection.z);

    // エフェクトの回転率
    effectData.rotationRate = VGet(0.0f, angle, 0.0f);

    // エフェクトの種類
    effectData.effectTag = EffectManager::PlayerMagicCircle;

    // エフェクトのサイズ
    effectData.scalingRate = VGet(EffectDefaultScale, EffectDefaultScale, EffectDefaultScale);

    // エフェクトの再生速度
    effectData.playSpeed = EffectPlaySpeed;
}
