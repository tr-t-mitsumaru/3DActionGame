#include"DxLib.h"
#include"CollisionManager.h"
#include"CollisionData.h"
#include"StateBase.h"
#include"Player.h"
#include"ModelDataManager.h"
#include"PlayerIdle.h"


const VECTOR Player::ModelOffsetPosition = VGet(0, 0, -3);

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
    : position(VGet(0, 0, 0))
    , angle(0.0f)
    , nowState(NULL)
    , modelDirection(VGet(0, 0, 0))
    , hp(100)
    , isBossHited(false)
    , isDamage(false)
{
    //インスタンスを持ってくる
    ModelDataManager* modelDataManager = ModelDataManager::GetInstance();
    //モデルハンドルをもらう
    modelHandle = MV1DuplicateModel(modelDataManager->GetModelHandle(ModelDataManager::Player));

    //最初にIdle状態のアニメーションをアタッチしておく
    MV1AttachAnim,(modelHandle, Idle, -1);

     //最初のステートを待機状態にする
    nowState = new PlayerIdle(modelHandle,-1);

    //コリジョンマネージャーのインスタンスを代入
    collisionManager = CollisionManager::GetInstance();

    //当たり判定が生きている状態にする
    collisionData.collisionState = CollisionData::CollisionActive;

    //当たり判定用の変数の初期化
    UpdateCollisionData();

    //当たり判定データを渡す
    collisionManager->RegisterCollisionData(&collisionData);

    //座標の設定
    MV1SetPosition(modelHandle, VGet(0, 0, 0));


}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
    //コピーしたモデルの削除
    MV1DeleteModel(modelHandle);

    //メモリの解放
    delete nowState;
}

/// <summary>
/// 更新処理
/// </summary>
void Player::Update(const VECTOR playerTargetPosition, const VECTOR cameraPosition)
{

    //ステート毎のアップデートを行う
    nowState->Update(modelDirection,position,playerTargetPosition,cameraPosition);

    // 移動
    position = VAdd(position, nowState->GetVelocity());

    //モデルの向きを反映
    UpdateAngle();

    //当たり判定に必要なデータを更新
    UpdateCollisionData();


    MV1SetPosition(modelHandle, VAdd(position,ModelOffsetPosition));
    
    //更新処理の後次のループでのステートを代入する
    nextState = nowState->GetNextState();

    // 体力が0かつ
    if (hp <= 0 && nextState->GetNowStateTag() == HitState)
    {
        // ライフが0になったことをステートに伝える
        nextState->SetNoLifeState();

    }

    // 無敵時間の作成
    SwitchInvincibility();

    //次のループのシーンと現在のシーンが違う場合は移行処理を行う
    if (nowState != nextState)
    {
        ChangeState();
    }

    //当たり判定を行う前に当たっているかをfalseにしておく
    isBossHited = false;

    // 毎ループでダメージを受けていない状態にする
    isDamage = false;
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
    //プレイヤーの描画
    MV1DrawModel(modelHandle);

#ifdef _DEBUG
    //当たり判定が正しいかの確認用の描画

    if (collisionData.collisionState == CollisionData::CollisionActive)
    {
        DrawCapsule3D(collisionData.upPosition, collisionData.bottomPosition, collisionData.radius, 16, GetColor(255, 255, 255), GetColor(255, 255, 255), FALSE);
    }

    //ステートの当たり判定を描画する
    nowState->DrawCollision();

    //プレイヤーの座標の表示
    DrawFormatString(50, 150, GetColor(255, 255, 255), "x %f  y %f  z %f", position.x, position.y, position.z);

    //ボスに当たった際の表示
    if (isBossHited)
    {
        DrawString(50, 200, "BossHit", GetColor(255, 255, 255));
    }

    DrawFormatString(50, 300, GetColor(255, 255, 255), "HP : %d", hp);

#endif

}

/// <summary>
/// ステートの移行処理
/// </summary>
void Player::ChangeState()
{
    delete nowState;
    nowState = nextState;
    nextState = NULL;
}



/// <summary>
/// プレイヤーの回転制御
/// </summary>
void Player::UpdateAngle()
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

/// <summary>
/// プレイヤーの情報から当たり判定に必要な情報を出して代入
/// </summary>
void Player::UpdateCollisionData()
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
    collisionData.hitObjectTag = CollisionManager::Player;
    //当たった際の関数
    collisionData.onHit = std::bind(&Player::OnHit, this, std::placeholders::_1);
    //ダメージ
    collisionData.damageAmount = 0;
}

/// <summary>
/// オブジェクトに当たった際の処理を書いたもの
/// </summary>
/// <param name="">当たり判定に必要な情報をまとめたデータ</param>
void Player::OnHit(CollisionData collisionData)
{
    switch (collisionData.hitObjectTag)
    {
    case CollisionManager::Boss:

        //ボスと当たったフラグを立てる
        isBossHited = true;

        break;
    case CollisionManager::BossDefaultAttack:

    case CollisionManager::BossRunAttack:

    case CollisionManager::BossShot:

    case CollisionManager::BossAreaAttack:

        // 1フレームで複数のダメージを受けないようにする
        if (nowState->GetLifeState() == NoDamage)
        {
            if (nowState->GetNowStateTag() == DefenseState)
            {
                //敵の攻撃に当たったのでHPを減らす
                hp -= collisionData.damageAmount * 0.5f;
            }
            else
            {
                hp -= collisionData.damageAmount;
            }

            // ステートにダメージを受けた事を伝える
            nowState->OnDamage();
        }


        break;
    default:
        break;
    }


}

/// <summary>
/// 無敵状態の切り替えを行う
/// </summary>
void Player::SwitchInvincibility()
{
    // 回避状態なら
    if (nextState->GetNowStateTag() == RollingState)
    {
        // 無敵時間の範囲になったら当たり判定を消す
        if (nextState->GetAnimationNowTime() / nextState->GetAnimationLimitTime() >= InvincibilityStartRatio &&
            nextState->GetAnimationNowTime() / nextState->GetAnimationLimitTime() < InvincibilityEndRatio)
        {
            // 当たり判定の削除
            collisionData.collisionState = CollisionData::CollisionEnded;
        }
        // 無敵時間の範囲を超えたら当たり判定を戻す
        else if (nextState->GetAnimationNowTime() / nextState->GetAnimationLimitTime() > InvincibilityEndRatio &&
                 collisionData.collisionState == CollisionData::CollisionEnded)
        {
            // 当たり判定を有効化
            collisionData.collisionState = CollisionData::CollisionActive;

            // 当たり判定データを渡す
            collisionManager->RegisterCollisionData(&collisionData);
        }
    }
}













