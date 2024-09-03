#include"BossDead.h"
#include"CollisionUtility.h"
#include"BossIdle.h"
#include"BossRunAttack.h"


const VECTOR BossRunAttack::OffsetCollisionPosition = VGet(0.0f, 20.0f, 0.0f);

///<summary>
///コンストラクタ
///</summary>
BossRunAttack::BossRunAttack(int& InitializeModelHandle, const int beforeAnimationIndex)
    :StateBase(InitializeModelHandle, Boss::Run, beforeAnimationIndex)
    ,currentRunState(RunStart)
    ,isAttackParameterInitialize(false)
{
    //アニメーション速度の初期化
    animationSpeed = InitializeAnimationSpeed;

    // コリジョンマネージャーのインスタンスをもってくる
    collisionManager = CollisionManager::GetInstance();

    //当たり判定がまだ生成されていない状態
    collisionData.collisionState = CollisionData::NoCollision;

}

/// <summary>
/// デストラクタ
/// </summary>
BossRunAttack::~BossRunAttack()
{
    //処理なし
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="position">プレイヤーモデルの向き</param>
void BossRunAttack::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition, VECTOR cameraPosition)
{

    // 体力によって変わる攻撃時のパラメーターを変更
    InitializeAttackPrameters();

    if (currentPlayAnimationState == BlendEnd)
    {
        // 移動する時に必要な情報を初期化
        InitializeRunPrameters(bossTargetPosition, position);
    }


    // 現在どれだけ進んだかを計算
    float currentDistance = VSize(VSub(position, startPosition));



    //ステートの切り替え処理を呼ぶ
    ChangeState();

    // 最初のターゲットの座標から少し進んだ位置まで到達したらステートを切り替える
    if (currentDistance >= targetLength + TargetOffsetDistance && currentRunState == Run)
    {
        currentRunState = RunEnd;
        //当たり判定を消す
        collisionData.collisionState = CollisionData::CollisionEnded;
    }

    //アニメーションの再生時間のセット
    UpdateAnimation(animationBlendSpeed);

    //当たり判定に必要な情報の更新
    UpdateCollisionData(modelDirection, position);

    // 当たり判定が有効になった入ればCollisionManagerに送信
    if (collisionData.collisionState == CollisionData::NoCollision)
    {
        //アニメーションの再生割合を調べて当たり判定情報をCollisionManagerに送信する
        collisionData.collisionState = CollisionUtility::SendCollisionDataByAnimationTime(GetAnimationNowTime(), GetAnimationLimitTime(),
            collisionData.collisionState, CollisionStratAnimationRatio);

        if (collisionData.collisionState == CollisionData::CollisionActive)
        {
            collisionManager->RegisterCollisionData(&collisionData);
        }
    }

    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();

}


/// <summary>
/// ステートの切り替え処理をまとめたもの
/// </summary>
void BossRunAttack::ChangeState()
{

    if (currentRunState == RunEnd)
    {
        nextState = new BossIdle(modelhandle, this->GetAnimationIndex(),Boss::RunAttack,isChangingMove);
    }
    else
    {
        nextState = this;
    }
}

/// <summary>
/// 座標などを当たり判定に必要なデータに変換
/// </summary>
/// <param name="modelDirection">モデルの向いている方向</param>
void BossRunAttack::UpdateCollisionData(const VECTOR& modelDirection, const VECTOR characterPosition)
{
    //当たり判定の座標を移動させる
    position = CollisionUtility::TransrateCollisionCapsulePosition(characterPosition, modelDirection,OffsetCollisionPosition,OffsetCollisionPositonScale);

    //中央座標の代入
    collisionData.centerPosition = position;

    //カプセルの下側の座標
    collisionData.bottomPosition = position;

    //カプセルの上側の座標
    collisionData.upPosition = position;

    //カプセルの球部分の半径
    collisionData.radius = CollisionRadius;

    //オブジェクトの種類
    collisionData.hitObjectTag = CollisionManager::BossDefaultAttack;

    //当たった際の関数
    collisionData.onHit = std::bind(&BossRunAttack::OnHit, this, std::placeholders::_1);

    //当たった際のダメージ量
    collisionData.damageAmount = damageAmount;
}


/// <summary>
/// 当たった時の処理
/// </summary>
/// <param name="targetCollisionData">当たった相手の当たり判定情報</param>
void BossRunAttack::OnHit(CollisionData targetCollisionData)
{
    //当たり判定を消す
    collisionData.collisionState = CollisionData::CollisionEnded;
}

/// <summary>
/// 走る時に必要な情報の初期化
/// </summary>
/// <param name="targetPosition">目標の座標</param>
/// <param name="position">自身の座標</param>
void BossRunAttack::InitializeRunPrameters(const VECTOR targetPosition, const VECTOR position)
{
    // 最初に方向を決める
    if (currentRunState == RunStart)
    {
        // 進む方向を決める
        direction = CalculateTargetDirection(targetPosition, position);

        // 初期座標を設定
        startPosition = position;

        // 目標との距離を決める
        targetLength = VSize(VSub(targetPosition, position));

        // 移動量の設定
        velocity = VScale(direction, moveSpeed);

        // 走っている状態を切り替える
        currentRunState = Run;
    }
}

/// <summary>
/// 体力に合わせて攻撃する強さのパラメーターを変更
/// </summary>
void BossRunAttack::InitializeAttackPrameters()
{
    if (!isAttackParameterInitialize)
    {
        switch (currentHpState)
        {
            // 体力が多い状態での初期化
        case Boss::High:

            animationBlendSpeed = EasyAnimationBlendSpeed;
            damageAmount = EasyDamageAmount;
            moveSpeed = EasyMoveSpeed;
            break;
            // 体力が通常状態での初期化
        case Boss::Middle:

            animationBlendSpeed = NormalAnimationBlendSpeed;
            damageAmount = NormalDamageAmount;
            moveSpeed = NormalMoveSpeed;

            break;
            // 体力が少ない状態での初期化
        case Boss::Low:

            animationBlendSpeed = HardAnimationBlendSpeed;
            damageAmount = HardDamageAmount;
            moveSpeed = HardMoveSpeed;

            break;
        default:


            break;
        }

        isAttackParameterInitialize = true;
    }
}


#ifdef _DEBUG

/// <summary>
/// 当たり判定を描画する(デバッグ用)
/// </summary>
void BossRunAttack::DrawCollision()
{
    if (collisionData.collisionState == CollisionData::CollisionActive)
    {
        //当たり判定が正しいかの確認用の描画
        DrawSphere3D(collisionData.centerPosition, collisionData.radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
    }
}
#endif // _DEBUG
