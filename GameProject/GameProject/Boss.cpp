#include"DxLib.h"
#include"Boss.h"
#include"ModelDataManager.h"
#include"CollisionManager.h"
#include"CollisionData.h"
#include"StateBase.h"
#include"BossStart.h"
#include"EffectManager.h"

//初期座標の入力
const VECTOR Boss::InitialPosition = VGet(0, 0, 6);
const VECTOR Boss::OffsetModelPosition = VGet(0,10, 0);

/// <summary>
/// コンストラクタ
/// </summary>
Boss::Boss()
    :position(InitialPosition)
    ,angle(DX_PI_F)
    ,nowState(NULL)
    ,modelDirection(VGet(0,0,-1))
    ,hp(10)
{
    //モデルマネージャーにアクセスるポインタの代入
    ModelDataManager* modelDataManager = ModelDataManager::GetInstance();

    // エフェクトマネージャーのインスタンスのポインタをもってくる
    effectManager = EffectManager::GetInstance();

    //モデルハンドルをもってくる
    modelHandle = MV1DuplicateModel(modelDataManager->GetModelHandle(ModelDataManager::Boss));

    //最初のステートを待機状態にする
    nowState = new BossStart(modelHandle, -1);

    //コリジョンマネージャーのインスタンスのアドレスを取得
    collisionManager = collisionManager->GetInstance();

    //当たり判定が生きている状態にする
    collisionData.collisionState = CollisionData::CollisionActive;

    //当たり判定用の変数の初期化
    UpdateCollisionData();

    //当たり判定データのポインタを渡す
    collisionManager->RegisterCollisionData(&collisionData);

    //スケールの初期化
    MV1SetScale(modelHandle, VGet(DefaultScale, 0.14, DefaultScale));

    //座標の設定
    MV1SetPosition(modelHandle, VAdd(InitialPosition,OffsetModelPosition));
}

/// <summary>
/// デストラクタ
/// </summary>
Boss::~Boss()
{
    //モデルの削除
    MV1DeleteModel(modelHandle);

}

/// <summary>
/// 更新処理
/// </summary>
void Boss::Update(const VECTOR bossTargetPosition,const VECTOR cameraPosition)
{
    //ステート毎のアップデートを行う
    nowState->Update(modelDirection, position,bossTargetPosition,cameraPosition);

    // 移動処理
    position = VAdd(position, nowState->GetVelocity());

    // モデルの向きの反映
    UpdateAngle();

    //当たり判定に必要なデータの更新
    UpdateCollisionData();

    //モデルを描画する座標の調整
    MV1SetPosition(modelHandle, VAdd(position,OffsetModelPosition));

    //更新処理の後次のループでのステートを代入する
    nextState = nowState->GetNextState();
    //次のループのシーンと現在のシーンが違う場合は移行処理を行う
    if (nowState != nextState)
    {
        ChangeState();
    }
}



void Boss::Draw()
{
    //モデルの描画
    MV1DrawModel(modelHandle);

#ifdef _DEBUG
    //当たり判定が正しいかの確認用の描画
    DrawCapsule3D(collisionData.bottomPosition, collisionData.upPosition, collisionData.radius, 64, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);

    //ステートの当たり判定を描画する
    nowState->DrawCollision();

    DrawFormatString(50, 250, GetColor(255, 255, 255), "HP : %d", hp);
#endif


}


/// <summary>
/// プレイヤーの情報から当たり判定に必要な情報を出して代入
/// </summary>
void Boss::UpdateCollisionData()
{
    //中央座標の代入
    collisionData.centerPosition = VAdd(position, VGet(0.0f, CollisionCapsuleLineLength * HalfLength, 0.0f));
    //カプセルの下側の座標
    collisionData.bottomPosition = position;
    //カプセルの上側の座標
    collisionData.upPosition = VAdd(position, VGet(0.0f, CollisionCapsuleLineLength, 0.0f));
    //カプセルの球部分の半径
    collisionData.radius = CollisionRadius;
    //オブジェクトの種類
    collisionData.hitObjectTag = CollisionManager::Boss;
    //当たった際の関数
    collisionData.onHit = std::bind(&Boss::OnHit, this, std::placeholders::_1);
}

/// <summary>
/// オブジェクトに当たった際の処理を書いたもの
/// </summary>
/// <param name="">当たり判定に必要な情報をまとめたデータ</param>
void Boss::OnHit(const CollisionData collisionData)
{
    switch (collisionData.hitObjectTag)
    {
    case CollisionManager::PlayerAttack:
        //HPを減らす
        hp -= collisionData.damageAmount;

        // プレイヤーの攻撃に当たった際のエフェクトの初期化
        InitializePlayerAttackHitEffectData(collisionData.centerPosition);

        // エフェクトの再生
        effectManager->PlayEffect(&playerAttackHitEffectData);

        break;
    case CollisionManager::PlayerShot:

        // 弾にあった際のエフェクトの初期化
        InitializeShotHitEffectData(collisionData.centerPosition);

        // エフェクトの再生
        effectManager->PlayEffect(&shotHitEffectData);

        //HPを減らす
        hp -= collisionData.damageAmount;

        break;
    default:
        break;
    }
}

/// <summary>
/// 弾と当たった際のエフェクトの初期化
/// </summary>
/// <param name="shotPosition">当たった弾の座標</param>
void Boss::InitializeShotHitEffectData(const VECTOR shotPosition)
{
    // ボスと弾のベクトルを出す
    VECTOR direction = VSub(position, shotPosition);

    // ボスと弾のベクトルからエフェクトの回転率を出す
    float angle = atan2(direction.x, direction.z);

    // エフェクトの回転率
    shotHitEffectData.rotationRate = VGet(0.0f, angle, 0.0f);

    // エフェクトの座標の初期化
    shotHitEffectData.position = shotPosition;

    // エフェクトの種類
    shotHitEffectData.effectTag = EffectManager::PlayerShotHit;

    // エフェクトの再生速度
    shotHitEffectData.playSpeed = 1.0f;

    // エフェクトのサイズ
    shotHitEffectData.scalingRate = VGet(ShotHitEffectScale, ShotHitEffectScale, ShotHitEffectScale);


}

/// <summary>
/// プレイヤーの攻撃に当たった際のエフェクトの初期化
/// </summary>
/// <param name="attackPosition">攻撃の座標</param>
void Boss::InitializePlayerAttackHitEffectData(const VECTOR attackPosition)
{
    // ボスと弾のベクトルを出す
    VECTOR direction = VSub(position, attackPosition);

    // ボスと弾のベクトルからエフェクトの回転率を出す
    float angle = atan2(direction.x, direction.z);

    // エフェクトの回転率
    playerAttackHitEffectData.rotationRate = VGet(0.0f, angle, 0.0f);

    // エフェクトの座標の初期化
    playerAttackHitEffectData.position = attackPosition;

    // エフェクトの種類
    playerAttackHitEffectData.effectTag = EffectManager::PlayerAttackHit;

    // エフェクトの再生速度
    playerAttackHitEffectData.playSpeed = 1.0f;

    // エフェクトのサイズ
    playerAttackHitEffectData.scalingRate = VGet(PlayerAttackHitEffectScale, PlayerAttackHitEffectScale, PlayerAttackHitEffectScale);
}

/// <summary>
/// ステートの移行処理
/// </summary>
void Boss::ChangeState()
{
    delete nowState;
    nowState = nextState;
    nextState = NULL;
}

/// <summary>
/// プレイヤーの回転制御
/// </summary>
void Boss::UpdateAngle()
{
    // プレイヤーの移動方向にモデルの方向を近づける
    float targetAngle;			// 目標角度
    float difference;			// 目標角度と現在の角度との差

    // 目標の方向ベクトルから角度値を算出する
    targetAngle = static_cast<float>(atan2(modelDirection.x, modelDirection.z));

    // 目標の角度と現在の角度との差を割り出す
    // 最初は単純に引き算
    difference = targetAngle - angle;

    // ある方向からある方向の差が１８０度以上になることは無いので
    // 差の値が１８０度以上になっていたら修正する
    if (difference < -DX_PI_F)
    {
        difference += DX_TWO_PI_F;
    }
    else if (difference > DX_PI_F)
    {
        difference -= DX_TWO_PI_F;
    }

    // 角度の差が０に近づける
    if (difference > 0.0f)
    {
        // 差がプラスの場合は引く
        difference -= AngleSpeed;
        if (difference < 0.0f)
        {
            difference = 0.0f;
        }
    }
    else
    {
        // 差がマイナスの場合は足す
        difference += AngleSpeed;
        if (difference > 0.0f)
        {
            difference = 0.0f;
        }
    }

    // モデルの角度を更新
    angle = targetAngle - difference;
    MV1SetRotationXYZ(modelHandle, VGet(0.0f, angle + DX_PI_F, 0.0f));
}

