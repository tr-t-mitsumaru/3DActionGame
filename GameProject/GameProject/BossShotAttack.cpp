#include"InitializeShotData.h"
#include"BossIdle.h"
#include"BossShotAttack.h"


///<summary>
///コンストラクタ
///</summary>
BossShotAttack::BossShotAttack(int& InitializeModelHandle, const int beforeAnimationIndex)
    :StateBase(InitializeModelHandle, Boss::LeftShot, beforeAnimationIndex)
    ,isAnimationSwitch(false)
    ,shotState(WaitLeftShot)
{
    //アニメーション速度の初期化
    animationSpeed = InitializeAnimationSpeed;

    //インプットマネージャーのインスタンスをもってくる
    inputManager = InputManager::GetInstance();

    // ショットマネージャーのインスタンスをもってくる「
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
    //ステートの切り替え処理を呼ぶ
    ChangeState();

    // アニメーションの再生率を見て弾を作成する
    SpawnShotByAnimationTime(position, bossTargetPosition, modelDirection);

    // アニメーションの切り替え
    SwitchAnimation();

    //アニメーションの再生時間のセット
    UpdateAnimation(AnimationBlendSpeed);



    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();

}


/// <summary>
/// ステートの切り替え処理をまとめたもの
/// </summary>
void BossShotAttack::ChangeState()
{
    //ToDo
    //BossのAIを作るまではボタンでステートが遷移するようにしている
    if (shotState == RightSHot && currentPlayAnimationState == FirstLoopEnd)
    {
        //ボスの突進攻撃ステートに移行
        nextState = new BossIdle(modelhandle, this->GetAnimationIndex(),BossIdle::ShotAttack);
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
    initializeShotData.speed = ShotSpeed;

    // 弾の半径
    initializeShotData.radius = ShotRadius;

    // 弾の種類
    initializeShotData.shooterTag = CollisionManager::BossShot;

    // 弾のダメージ
    initializeShotData.damageAmount = ShotDamageAmount;

    // 初期化したデータを返す
    return initializeShotData;
    
}
