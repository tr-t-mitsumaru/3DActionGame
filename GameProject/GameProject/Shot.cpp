#include"CollisionData.h"
#include"InitializeShotData.h"
#include"CollisionManager.h"
#include"Shot.h"


/// <summary>
/// コンストラクタ
/// </summary>
Shot::Shot()
{
    //CollisionManagerのインスタンスをもってくる
    collisionManager = CollisionManager::GetInstance();

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

    // 弾が生成された時点で当たり判定を作成する
    collisionData.collisionState = CollisionData::CollisionActive;

    // 当たり判定情報を設定
    UpdateCollisionData();

    // 弾の当たり判定情報を
    collisionManager->RegisterCollisionData(&collisionData);
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

    // 当たり判定情報を設定
    UpdateCollisionData();
}

/// <summary>
/// 当たった時の処理
/// </summary>
void Shot::OnHit(CollisionData collisionData)
{
    // 当たり判定を削除する
    this->collisionData.collisionState = CollisionData::CollisionEnded;

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
/// 描画
/// </summary>
void Shot::Draw()
{
#ifdef _DEBUG
    DrawSphere3D(collisionData.centerPosition, collisionData.radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
#endif 


}
