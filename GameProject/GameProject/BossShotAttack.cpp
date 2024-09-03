#include"InitializeShotData.h"
#include"BossIdle.h"
#include"BossDead.h"
#include"BossShotAttack.h"
#include"EffectManager.h"


///<summary>
///コンストラクタ
///</summary>
BossShotAttack::BossShotAttack(int& InitializeModelHandle, const int beforeAnimationIndex)
    :StateBase(InitializeModelHandle, Boss::LeftShot, beforeAnimationIndex)
    ,isAnimationSwitch(false)
    ,shotState(WaitLeftShot)
    ,isAttackParameterInitialize(false)
{
    //アニメーション速度の初期化
    animationSpeed = InitializeAnimationSpeed;

    // ショットマネージャーのインスタンスをもってくる
    shotManager = ShotManager::GetInstance();

}

/// <summary>
/// デストラクタ
/// </summary>
BossShotAttack::~BossShotAttack()
{
    //処理なし
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">プレイヤーモデルの向き</param>
void BossShotAttack::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition, VECTOR cameraPosition)
{
    // 体力によって攻撃に必要なパラメーターを初期化
    InitializeAttackParameter();

    //ステートの切り替え処理を呼ぶ
    ChangeState();

    // アニメーションの再生率を見て弾を作成する
    SpawnShotByAnimationTime(position, bossTargetPosition, modelDirection);

    // アニメーションの切り替え
    SwitchAnimation();

    //アニメーションの再生時間のセット
    UpdateAnimation(animationBlendSpeed);



    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();

}


/// <summary>
/// ステートの切り替え処理をまとめたもの
/// </summary>
void BossShotAttack::ChangeState()
{
    // 既にステートが切り替えてあれば早期リターン
    if (changedState)
    {
        return;
    }
    else if (shotState == RightSHot && currentPlayAnimationState == FirstLoopEnd)
    {
        //ボスの突進攻撃ステートに移行
        nextState = new BossIdle(modelhandle, this->GetAnimationIndex(),Boss::ShotAttack,isChangingMove);
    }
    else
    {
        nextState = this;
    }
}


/// <summary>
/// アニメーションの切り替え
/// </summary>
void BossShotAttack::SwitchAnimation()
{
    // アニメーションの1ループが終了したら
    if (currentPlayAnimationState == StateBase::FirstLoopEnd && shotState == LeftShot)
    {
        // 前のステートのアニメーションをデタッチ
        MV1DetachAnim(modelhandle, beforeAnimationIndex);

        // 現在のアニメーションインデックスを前のインデックスに入れる
        beforeAnimationIndex = animationIndex;

        //アニメーションをアタッチ
        animationIndex = MV1AttachAnim(modelhandle, Boss::RightShot, -1, FALSE);

        // アニメーションの総再生時間を取得
        animationLimitTime = MV1GetAttachAnimTotalTime(modelhandle, animationIndex);

        //アニメーションの再生時間の初期化
        animationNowTime = 0.0f;

        currentPlayAnimationState = StateBase::BlendStart;

        shotState = WaitRightShot;
    }
}

/// <summary>
/// アニメーションの再生率によってショットを作成
/// </summary>
/// <param name="position">自身の座標</param>
void BossShotAttack::SpawnShotByAnimationTime(const VECTOR position, const VECTOR bossTargetPosition, VECTOR & modelDirection)
{
    // アニメーションの再生率が規定値を超えたら
    if (animationNowTime / animationLimitTime >= ShotCreateAnimationRatio &&
        shotState == WaitLeftShot || shotState == WaitRightShot)
    {
        // 初期化用のデータを作成
        InitializeShotData initializeShotData = AssignInitializeShotData(position, bossTargetPosition);

        // 弾を撃った方向にモデルを回転させる
        modelDirection = initializeShotData.direction;

        // 必要な情報を代入して弾を作成
        shotManager->SpawnShot(initializeShotData);

        // 弾を撃った状態を変更する
        if (shotState == WaitLeftShot)
        {
            shotState = LeftShot;
        }
        else
        {
            shotState = RightSHot;
        }
    }
}


/// <summary>
/// ショットの初期化用データを代入
/// </summary>
InitializeShotData BossShotAttack::AssignInitializeShotData(const VECTOR position,const VECTOR bossTargetPosition)
{
    // 初期化用のデータを作成
    InitializeShotData initializeShotData;

    // 自身とターゲットのポジションから移動方向を作成
    VECTOR direction = VSub(bossTargetPosition, position);
    direction = VNorm(direction);

    // 座標
    initializeShotData.position = position;

    //方向
    initializeShotData.direction = direction;

    // 速度
    initializeShotData.speed = shotSpeed;

    // 弾の半径
    initializeShotData.radius = shotRadius;

    // 弾の種類
    initializeShotData.shooterTag = CollisionManager::BossShot;

    // 弾のダメージ
    initializeShotData.damageAmount = shotDamageAmount;

    // ボスのショット攻撃で初期化する
    initializeShotData.effectTag = EffectManager::BossShot;

    // エフェクトの回転率を設定
    initializeShotData.effectRotationRate = VGet(0, 0, 0);

    // エフェクトのサイズを設定
    initializeShotData.effectScalingRate = effectScale;

    // エフェクトの再生速度の設定
    initializeShotData.effectPlaySpeed = EffectPlaySpeed;

    // 初期化したデータを返す
    return initializeShotData;
    
}

/// <summary>
/// 攻撃に必要なパラメータの初期化
/// </summary>
void BossShotAttack::InitializeAttackParameter()
{
    if (!isAttackParameterInitialize)
    {
        switch (currentHpState)
        {
            // 体力が多い状態での初期化
        case Boss::High:

            animationBlendSpeed = EasyAnimationBlendSpeed;
            shotSpeed           = EasyShotSpeed;
            shotRadius          = EasyShotRadius;
            shotDamageAmount    = EasyShotDamageAmount;
            effectScale         = VGet(EasyEffectDefaultScale, EasyEffectDefaultScale, EasyEffectDefaultScale);

            break;
            // 体力が通常状態での初期化
        case Boss::Middle:

            animationBlendSpeed = NormalAnimationBlendSpeed;
            shotSpeed           = NormalShotSpeed;
            shotRadius          = NormalShotRadius;
            shotDamageAmount    = NormalShotDamageAmount;
            effectScale         = VGet(NormalEffectDefaultScale, NormalEffectDefaultScale, NormalEffectDefaultScale);
            break;
            // 体力が少ない状態での初期化
        case Boss::Low:

            animationBlendSpeed = HardAnimationBlendSpeed;
            shotSpeed           = HardShotSpeed;
            shotRadius          = HardShotRadius;
            shotDamageAmount    = HardShotDamageAmount;
            effectScale         = VGet(HardEffectDefaultScale, HardEffectDefaultScale, HardEffectDefaultScale);
            break;
        default:


            break;
        }

        isAttackParameterInitialize = true;
    }
}
