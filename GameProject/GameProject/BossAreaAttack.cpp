#include"ShotManager.h"
#include"InitializeShotData.h"
#include"Boss.h"
#include"BossIdle.h"
#include"BossAreaAttack.h"
#include"CollisionManager.h"

///<summary>
///コンストラクタ
///</summary>
BossAreaAttack::BossAreaAttack(int& InitializeModelHandle, const int beforeAnimationIndex)
    :StateBase(InitializeModelHandle, Boss::GroundAttack, beforeAnimationIndex)
    ,attackState(NoAttack)
{
    //アニメーション速度の初期化
    animationSpeed = InitializeAnimationSpeed;

    //コリジョンマネージャーのインスタンスをもってくる
    collisionManager = CollisionManager::GetInstance();

    //ショットマネージャーのインスタンスをもってくる
    shotManager = ShotManager::GetInstance();



}

/// <summary>
/// デストラクタ
/// </summary>
BossAreaAttack::~BossAreaAttack()
{
    //処理なし
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">プレイヤーモデルの向き</param>
void BossAreaAttack::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition, VECTOR cameraPosition)
{
    //ステートの切り替え処理を呼ぶ
    ChangeState();

    // アニメーションの再生率に合わせて弾を作成する
    CreateShotByAnimationTime(position);

    //アニメーションの再生時間のセット
    UpdateAnimation(AnimationBlendSpeed);

    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();

}


/// <summary>
/// ステートの切り替え処理をまとめたもの
/// </summary>
void BossAreaAttack::ChangeState()
{
    if (currentPlayAnimationState == FirstLoopEnd)
    {
        //ボスの移動ステートに移行
        nextState = new BossIdle(modelhandle,this->GetAnimationIndex(),Boss::AreaAttack);
    }
    else
    {
        nextState = this;
    }
}

/// <summary>
/// 攻撃用のショットを作成する
/// </summary>
/// <param name="characterPosition">自身の座標</param>
void BossAreaAttack::CreateAttackShot(VECTOR characterPosition)
{
    // 作成する弾の数だけまわす
    for (int i = 0; i < SpawnShotCount; i++)
    {
        // shotManagerを使用して弾の生成
        shotManager->SpawnShot(InitializeShot(i, characterPosition));
    }
}


/// <summary>
/// 弾のデータを初期化
/// </summary>
/// <param name="index">何番目の弾かの添え字</param>
/// <param name="characterPosition">基準となる座標</param>
/// <returns>初期化用データ</returns>
InitializeShotData BossAreaAttack::InitializeShot(const int index,const VECTOR characterPosition)
{
    //初期化用のデータを作成
    InitializeShotData initializeShotData;

    // 弾を生成する度に進む角度を変更する
    float angle = FullCircleRadian * index / SpawnShotCount;

    //回転用の行列を作成
    MATRIX rotationMatrix = MGetRotY(angle);

    //初期座標を代入
    initializeShotData.position = characterPosition;

    //移動方向を代入
    initializeShotData.direction = VNorm(VTransform(VGet(1.0f, 0.0f, 0.0f), rotationMatrix));

    //弾の半径を代入
    initializeShotData.radius = ShotRadius;

    //弾のダメージ量を代入
    initializeShotData.damageAmount = ShotDamageAmount;

    // 弾のスピードの代入
    initializeShotData.speed = ShotSpeed;

    // ショットを撃ったキャラのタグを代入
    initializeShotData.shooterTag = CollisionManager::BossAreaAttack;

    //作成したShotDataを返す
    return initializeShotData;
}

/// <summary>
/// アニメーションの再生率によってショットを作成
/// </summary>
/// <param name="position">自身の座標</param>
void BossAreaAttack::CreateShotByAnimationTime(const VECTOR position)
{
    // アニメーションの再生率が規定値を超えたら
    if (animationNowTime / animationLimitTime >= ShotCreateAnimationRatio &&
        attackState == NoAttack)
    {
        // 弾を作成
        CreateAttackShot(position);
        attackState = StartAttack;
    }
}
