#include"CollisionData.h"
#include"InitializeShotData.h"
#include"CollisionManager.h"
#include"EffectData.h"
#include"EffectManager.h"
#include"Shot.h"


/// <summary>
/// コンストラクタ
/// </summary>
Shot::Shot()
{
    //CollisionManagerのインスタンスをもってくる
    collisionManager = CollisionManager::GetInstance();

    // EffectManagerのインスタンスをもってくる
    effectManager = EffectManager::GetInstance();

}

/// <summary>
/// デストラクタ
/// </summary>
Shot::~Shot()
{
    //処理なし
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="initializeShotData">初期化用の情報</param>
void Shot::Initialize(InitializeShotData initializeShotData)
{
    //弾を有効な状態に
    isActive = true;

    // 座標の初期化
    position = initializeShotData.position;

    // 移動方向
    direction = initializeShotData.direction;

    // スピード
    speed = initializeShotData.speed;

    // 半径
    radius = initializeShotData.radius;

    // ダメージ
    damageAmount = initializeShotData.damageAmount;

    // 撃ったキャラの種類
    shooterTag = initializeShotData.shooterTag;

    // エフェクトの種類
    effectData.effectTag = initializeShotData.effectTag;

    // エフェクトのサイズ
    effectData.scalingRate = initializeShotData.effectScalingRate;

    // エフェクトの回転率
    effectData.rotationRate = initializeShotData.effectRotationRate;

    // エフェクトの再生率
    effectData.playSpeed = initializeShotData.effectPlaySpeed;

    effectData.position = position;

    // 弾が生成された時点で当たり判定を作成する
    collisionData.collisionState = CollisionData::CollisionActive;

    // 当たり判定情報を設定
    UpdateCollisionData();

    // 弾の当たり判定情報をわたす
    collisionManager->RegisterCollisionData(&collisionData);

    // エフェクトの再生を行う
    effectManager->PlayEffect(&effectData);
}

/// <summary>
/// 更新処理
/// </summary>
void Shot::Update()
{
    // 移動方向と量
    VECTOR velocity = VGet(0, 0, 0);

    // 方向から量を
    velocity = VScale(direction, speed);

    //座標を更新
    position = VAdd(position, velocity);


    DeleteShot();

    // 当たり判定情報を設定
    UpdateCollisionData();

    UpdataEffectData();
}

/// <summary>
/// 当たった時の処理
/// </summary>
void Shot::OnHit(CollisionData collisionData)
{
    // 当たり判定を削除する
    this->collisionData.collisionState = CollisionData::CollisionEnded;

    // エフェクトを停止
    effectManager->StopEffect(effectData);

    // 弾自体の生きているフラグもおろす
    isActive = false;
}

/// <summary>
/// 座標などを当たり判定に必要なデータに変換
/// </summary>
void Shot::UpdateCollisionData()
{
    // 中心座標
    collisionData.centerPosition = position;

    // カプセルの下側の座標
    collisionData.bottomPosition = position;

    // カプセルの上側の座標
    collisionData.upPosition = position;

    // カプセルの半径
    collisionData.radius = radius;

    // オブジェクトの種類
    collisionData.hitObjectTag = shooterTag;

    // 当たった際のダメージ量
    collisionData.damageAmount = damageAmount;

    //当たった際の関数
    collisionData.onHit = std::bind(&Shot::OnHit, this, std::placeholders::_1);
}

/// <summary>
/// エフェクトの再生に必要な情報を更新する
/// </summary>
void Shot::UpdataEffectData()
{
    // 座標の更新
    effectData.position = position;
}

/// <summary>
/// 中心からの距離をはかる
/// </summary>
/// <returns>中心からの距離</returns>
float Shot::CalculateDistanceFromCenter()
{
    // 中央から現在の位置へのベクトルを計算
    VECTOR vector = VSub(position, VGet(0.0f, 0.0f, 0.0f));

    // 中央からの距離を計算
    float distanceFromCenter = VSize(vector);

    return distanceFromCenter;
}

/// <summary>
/// 弾の削除
/// </summary>
void Shot::DeleteShot()
{
    // 中心からの距離を出す
    float distanceFromeCenter = CalculateDistanceFromCenter();

    if (distanceFromeCenter >= 41)
    {
        // 当たり判定を削除する
        this->collisionData.collisionState = CollisionData::CollisionEnded;

        // エフェクトの停止
        effectManager->StopEffect(effectData);

        // 弾自体の生きているフラグもおろす
        isActive = false;
    }
}

/// <summary>
/// 非アクティブ状態に切り替える
/// </summary>
void Shot::SetInactive()
{
    // アクティブフラグを切り替える
    isActive = false;
}


/// <summary>
/// 描画
/// </summary>
void Shot::Draw()
{
#ifdef _DEBUG
#endif 
}
