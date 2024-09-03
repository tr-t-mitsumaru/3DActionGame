#include"DxLib.h"
#include"CollisionManager.h"
#include"CollisionData.h"
#include"StateBase.h"
#include"Player.h"
#include"ModelDataManager.h"
#include"PlayerIdle.h"
#include"EffectManager.h"
#include"GameScene.h"
#include"SoundManager.h"
#include"Utility.h"


const VECTOR Player::ModelOffsetPosition = VGet(0, 0, -0.6);

/// <summary>
/// コンストラクタ
/// </summary>
Player::Player()
    : position(VGet(0, 0, -37.0f))
    , angle(0.0f)
    , nowState(NULL)
    , modelDirection(VGet(0, 0, 1))
    , hp(MaxHp)
    , isBossHited(false)
    , isEndMove(false)
    , isBlendingAnimation(false)
    , endedDeadMove(false)
{
    //インスタンスを持ってくる
    ModelDataManager* modelDataManager = ModelDataManager::GetInstance();

    // エフェクトマネージャーのインスタンスのポインタをもってくる
    effectManager = EffectManager::GetInstance();

    // サウンドマネージャーのインスタンスをもってくる
    soundManager = SoundManager::GetInstance();

    //モデルハンドルをもらう
    modelHandle = MV1DuplicateModel(modelDataManager->GetModelHandle(ModelDataManager::Player));

    //最初にIdle状態のアニメーションをアタッチしておく
    animationIndex = MV1AttachAnim(modelHandle, Walk, -1,FALSE);

    //アニメーションの総再生時間を取得
    animationLimitTime = MV1GetAttachAnimTotalTime(modelHandle, animationIndex);

    //アニメーションの再生時間の初期化
    animationNowTime = 0.0f;

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

    // 大きさを変更
    MV1SetScale(modelHandle, VGet(DefaultScale, DefaultScale, DefaultScale));

    //座標の設定
    MV1SetPosition(modelHandle, VGet(0, 0, 0));

    // モデルの回転
    MV1SetRotationXYZ(modelHandle, VGet(0.0f, DX_PI_F, 0.0f));


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
/// ボスとの戦いが始まった瞬間の初期化
/// </summary>
void Player::InitializeBattleStart(const VECTOR playerTargetPosition)
{
    // ボスからプレイヤーの向きを計算
    VECTOR playerBossDirection = VNorm(VSub(position, playerTargetPosition));

    // ボスから一定の位置にプレイヤーを配置
    position = VAdd(playerTargetPosition, VScale(playerBossDirection, MoveDistance));

    // 移動をやめる
    isEndMove = true;

    // 音を止める
    soundManager->StopSoundEffect(SoundManager::PlayerFootSteps);

    // デタッチを行う
    MV1DetachAnim(modelHandle, animationIndex);

    // インデックスの読み込み
    animationIndex = MV1AttachAnim(modelHandle, Idle, -1, FALSE);

    //アニメーションの総再生時間を取得
    animationLimitTime = MV1GetAttachAnimTotalTime(modelHandle, animationIndex);


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

    // 体力が0かつダメージを受けるステートの再生が終了していれば
    if (hp <= 0)
    {
        hp = 0;
        if (nowState->GetNowStateTag() == HitState && nowState->GetCurrentAnimationPlayState() == StateBase::FirstLoopEnd)
        {
            // ライフが0になったことをステートに伝える
            WaitTimer(50);

            nowState->SetPlayerNoLifeState();
        }
    }


    // ステートが死亡かつ死亡時のアニメーションが終了していたら
    if (nowState->GetNowStateTag() == DeadState && nowState->GetCurrentAnimationPlayState() == StateBase::Stop)
    {
        // 死んだフラグをたてる
        endedDeadMove = true;
    }

    
    //更新処理の後次のループでのステートを代入する
    nextState = nowState->GetNextState();

    MV1SetPosition(modelHandle, VAdd(position,ModelOffsetPosition));



    // 無敵時間の作成
    SwitchInvincibility();

    //次のループのシーンと現在のシーンが違う場合は移行処理を行う
    if (nowState != nextState)
    {
        ChangeState();
    }

    //当たり判定を行う前に当たっているかをfalseにしておく
    isBossHited = false;


}

/// <summary>
/// 登場シーンでの更新処理
/// </summary>
/// <param name="distanceToBoss">ボスとの距離</param>
void Player::UpdateStartScene(const float distanceToBoss)
{
    // 移動が終わってなければ
    if (!isEndMove)
    {
        // ボスに向かって移動させる
        position.z += MoveSpeed;

        // 足音を立てる
        soundManager->PlaySoundEffect(SoundManager::PlayerFootSteps);

        // ボスとの距離が特定の距離まで近づいたら
        if (distanceToBoss <= MoveDistance)
        {
            // 音を止める
            soundManager->StopSoundEffect(SoundManager::PlayerFootSteps);
            // 移動をやめる
            isEndMove = true;

            // 静止状態のアニメーションに切り替える
            isBlendingAnimation = true;

            // インデックスの切り替え
            beforeAnimationIndex = animationIndex;

            // インデックスの読み込み
            animationIndex = MV1AttachAnim(modelHandle, Idle, -1, FALSE);

            //アニメーションの総再生時間を取得
            animationLimitTime = MV1GetAttachAnimTotalTime(modelHandle, animationIndex);
        }
    }

    // モデルの移動
    MV1SetPosition(modelHandle, position);

    // アニメーションの更新を行う
    UpdateAnimation();
}

/// <summary>
/// アニメーションの更新処理
/// </summary>
void Player::UpdateAnimation()
{
    if (isBlendingAnimation)
    {
        //前回とのアニメーションをブレンドして表示
        animationBlendRate += AnimationBlendSpeed;
        //ブレンドが終わったら
        if (animationBlendRate >= 1.0f)
        {
            //前のアニメーションをでタッチ
            MV1DetachAnim(modelHandle, beforeAnimationIndex);
            beforeAnimationIndex = -1;
            isBlendingAnimation = false;
        }
        MV1SetAttachAnimBlendRate(modelHandle, beforeAnimationIndex, 1.0f - animationBlendRate);
        MV1SetAttachAnimBlendRate(modelHandle, animationIndex, animationBlendRate);
    }
    else
    {
        // 再生時間を進める
        animationNowTime += AnimationSpeed;

        // 再生時間をセットする
        MV1SetAttachAnimTime(modelHandle, animationIndex, animationNowTime);

        // 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
        if (animationNowTime >= animationLimitTime)
        {
            animationNowTime = 0.0f;
        }
    }

}


/// <summary>
/// 描画
/// </summary>
void Player::Draw()
{
    //プレイヤーの描画
    MV1DrawModel(modelHandle);

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
    float damageRate = 1.0f;

    // 1フレームで複数のダメージを受けないようにする
    if (nowState->GetNowStateTag() != HitState && nowState->GetNowStateTag() != DeadState && !nowState->GetChangedState())
    {
        if (nowState->GetNowStateTag() == DefenseState)
        {
            // ガード中のヒット音を流す
            soundManager->PlaySoundEffect(SoundManager::PlayerDiffence);

            damageRate = 0.5f;
        }
    }

    switch (collisionData.hitObjectTag)
    {
        case CollisionManager::Boss:
        {
            // ボスと当たった際に押し戻し処理を行う
            PushBack(collisionData.bottomPosition, collisionData.radius);

            break;
        }
        case CollisionManager::BossIntimidation:
        {
        case CollisionManager::BossDefaultAttack:
        case CollisionManager::BossRunAttack:
            // 1フレームで複数のダメージを受けないようにする
            if (nowState->GetNowStateTag() != HitState && nowState->GetNowStateTag() != DeadState && !nowState->GetChangedState())
            {
                //敵の攻撃に当たったのでHPを減らす
                hp -= collisionData.damageAmount * damageRate;

                // ステートにダメージを受けた事を伝える
                nowState->OnDamage();
            }

            break;
        }
        case CollisionManager::BossShot:
        {
        case CollisionManager::BossAreaAttack:
            // 1フレームで複数のダメージを受けないようにする
            if (nowState->GetNowStateTag() != HitState && nowState->GetNowStateTag() != DeadState && !nowState->GetChangedState())
            {
                //敵の攻撃に当たったのでHPを減らす
                hp -= collisionData.damageAmount * damageRate;

                // ステートにダメージを受けた事を伝える
                nowState->OnDamage();
                // ショットが当たった際のエフェクトの初期化
                InitializeShotHitEffectData(collisionData.centerPosition);

                // エフェクトの再生を行う
                effectManager->PlayEffect(&shotHitEffectData);
            }
        }
        default:
        {
            break;
        }
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


/// <summary>
/// ボスに当たった際の押し戻し処理
/// </summary>
/// <param name="targetPosition">相手の座標</param>
void Player::PushBack(const VECTOR targetPosition, const float targetRadius)
{
    float radiusSum = targetRadius + collisionData.radius;

    // y座標は変更しなくていいので０に修正する
    VECTOR correctedTargetPosition = VGet(targetPosition.x, 0.0f, targetPosition.z);

    // プレイヤーも同じように修正
    VECTOR correctedPlayerPosition = VGet(position.x, 0.0f, position.z);

    // 修正した座標からボスからプレイヤーの向きのベクトルを計算
    VECTOR vectorToPlayer = VSub(correctedPlayerPosition, correctedTargetPosition);

    // ベクトルのサイズを計算
    float distance = VSize(vectorToPlayer);

    // 押し戻す距離の計算
    distance = radiusSum - distance;


    // ベクトルを正規化する
    vectorToPlayer = VNorm(vectorToPlayer);

    VECTOR pushBackVector = VScale(vectorToPlayer, distance);

    // 計算したベクトルからプレイヤーの位置を変更
    position = VAdd(position, pushBackVector);

    // モデルの位置も合わせて修正
    MV1SetPosition(modelHandle, position);
}

/// <summary>
/// 弾と当たった際のエフェクトの初期化
/// </summary>
/// <param name="shotPosition">当たった弾の座標</param>
void Player::InitializeShotHitEffectData(const VECTOR shotPosition)
{
    // 自身のポジションと弾の座標からエフェクトの向きを算出
    float angle = Utility::CalculateAngleBetweenPositions(position, shotPosition);

    // エフェクトの回転率
    shotHitEffectData.rotationRate = VGet(0.0f, angle, 0.0f);

    // エフェクトの座標の初期化
    shotHitEffectData.position = shotPosition;

    // エフェクトの種類
    shotHitEffectData.effectTag = EffectManager::BossShotHit;

    // エフェクトの再生速度
    shotHitEffectData.playSpeed = 1.0f;

    // エフェクトのサイズ
    shotHitEffectData.scalingRate = VGet(ShotHitEffectScale, ShotHitEffectScale, ShotHitEffectScale);


}

/// <summary>
/// 中央からの最大距離に達した際の押し戻し処理
/// </summary>
void Player::ApplyBoundaryPushback()
{
    // 中央からの現在の座標へのベクトルを出す
    VECTOR direction = VSub(position, VGet(0, 0, 0));

    // ベクトルのサイズから距離を出す
    float distance = VSize(direction);

    // 中央からの距離が規定値を超えていたら
    if (distance > GameScene::MaxDistanceFromCenter)
    {
        // ベクトルを正規化
        direction = VNorm(direction);

        // 修正したベクトルを出す
        VECTOR correctedVector = VScale(direction, GameScene::MaxDistanceFromCenter);

        // 修正したベクトルを足し押し戻した座標に変更する
        position = VAdd(VGet(0, 0, 0), correctedVector);
    }
}













