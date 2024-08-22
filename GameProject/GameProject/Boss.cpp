#include"DxLib.h"
#include"Boss.h"
#include"BossIdle.h"
#include"ModelDataManager.h"
#include"CollisionManager.h"
#include"CollisionData.h"
#include"StateBase.h"
#include"BossStart.h"
#include"EffectManager.h"
#include"GameScene.h"

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
    ,currentPlayAnimationState(Stop)
    ,currentStartMoveState(StopMove)
    ,animationSpeed(FirstAnimationSpeed)
    ,isStartUpdateStartScene(false)
    ,isEndAnimationFirstRoop(false)
    ,isPlaiedIntimidationEffect(false)
{
    //モデルマネージャーにアクセスるポインタの代入
    ModelDataManager* modelDataManager = ModelDataManager::GetInstance();

    // エフェクトマネージャーのインスタンスのポインタをもってくる
    effectManager = EffectManager::GetInstance();

    //モデルハンドルをもってくる
    modelHandle = MV1DuplicateModel(modelDataManager->GetModelHandle(ModelDataManager::Boss));

    //最初に座り状態のアニメーションをアタッチしておく
    animationIndex = MV1AttachAnim(modelHandle, Standing, -1, FALSE);

    //アニメーションの総再生時間を取得
    animationLimitTime = MV1GetAttachAnimTotalTime(modelHandle, animationIndex);

    //アニメーションの再生時間の初期化
    animationNowTime = 0.0f;

    //最初のステートを待機状態にする
    nowState = new BossIdle(modelHandle, -1,BossIdle::None);

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

// 登場シーンでの更新処理
void Boss::UpdateStartScene()
{
    // 開始フラグがたっていなければ早期リターン
    if (! isStartUpdateStartScene)
    {
        return;
    }
    else
    {
        // アニメーションの状態変更させる
        ChangeStartMoveState();

        // アニメーションの切り替え
        SwtchAnimation();

        //モデルを描画する座標の調整
        MV1SetPosition(modelHandle, VAdd(position, OffsetModelPosition));

        // アニメーションの更新
        UpdateAnimation();

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
/// 登場シーンのボスのアップデートを開始させる
/// </summary>
void Boss::StartUpdateStartScene()
{
    // フラグを切り替える
    isStartUpdateStartScene = true;
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

/// <summary>
/// 行動開始前のアニメーションの状態変更させるための関数
/// </summary>
void Boss::ChangeStartMoveState()
{
    //カウントが一定のラインに達するまで動きを止めておく
    if (currentStartMoveState == StopMove)
    {
        moveStartCount++;
        //カウントが超えたら立ち上がらせる
        if (moveStartCount >= MoveStartCountLimit)
        {
            currentStartMoveState = StandingNow;
        }
    }
    //立ち上がっている状態の処理
    else if (currentStartMoveState == StandingNow)
    {
        //アニメーションが開始されていなければ開始させる
        if (currentPlayAnimationState == Stop)
        {
            currentPlayAnimationState = Play;
        }
        //立ち上がり終わっていたら状態を変更させる
        else if (animationNowTime / animationLimitTime >= SwitchAnimationRatio)
        {
            currentStartMoveState = Stand;
            animationSpeed = SecondAnimationSpeed;
        }
    }
    // 威嚇状態のアニメーション中
    else if (currentStartMoveState == IntimidationStart)
    {
        if (animationNowTime / animationLimitTime >= GameScene::ShakeStartBossAnimationRatio && ! isPlaiedIntimidationEffect)
        {
            // エフェクトの初期化
            InitializeIntimidationEffectData();

            // エフェクトを流す
            effectManager->PlayEffect(&intimidationEffectData);

            // フラグを下げる
            isPlaiedIntimidationEffect = true;
        }
        // 一回目のループが終わったら再生を止める
        if (isEndAnimationFirstRoop)
        {
            currentPlayAnimationState = Stop;
            currentStartMoveState = EndMove;
        }
    }
}

/// <summary>
/// アニメーションの更新処理
/// </summary>
void Boss::UpdateAnimation()
{
    // ブレンドする
    if (currentPlayAnimationState == BlendNow)
    {
        //前回とのアニメーションをブレンドして表示
        animationBlendRate += AnimationBlendSpeed;
        //ブレンドが終わったら
        if (animationBlendRate >= 1.0f)
        {
            //前のアニメーションをでタッチ
            MV1DetachAnim(modelHandle, beforeAnimationIndex);
            beforeAnimationIndex = -1;
            currentPlayAnimationState = Play;
        }
        MV1SetAttachAnimBlendRate(modelHandle, beforeAnimationIndex, 1.0f - animationBlendRate);
        MV1SetAttachAnimBlendRate(modelHandle, animationIndex, animationBlendRate);
    }
    else if (currentPlayAnimationState == Play)
    {
        // 再生時間を進める
        animationNowTime += animationSpeed;

        // 再生時間をセットする
        MV1SetAttachAnimTime(modelHandle, animationIndex, animationNowTime);

        // 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
        if (animationNowTime >= animationLimitTime)
        {
            animationNowTime = 0.0f;
            // 最初のアニメーションのループが終了したフラグをたてる
            isEndAnimationFirstRoop = true;
        }
    }
}

void Boss::SwtchAnimation()
{
    // アニメーションの1ループが終了したら
    if (animationNowTime / animationLimitTime >= SwitchAnimationRatio && currentStartMoveState == Stand)
    {
        // 現在のアニメーションインデックスを前のインデックスに入れる
        beforeAnimationIndex = animationIndex;

        //アニメーションをアタッチ
        animationIndex = MV1AttachAnim(modelHandle, Intimidation, -1, FALSE);

        // アニメーションの総再生時間を取得
        animationLimitTime = MV1GetAttachAnimTotalTime(modelHandle, animationIndex);

        //アニメーションの再生時間の初期化
        animationNowTime = 0.0f;

        // アニメーションを切り替えたのでブレンドを開始させる
        currentPlayAnimationState = BlendNow;

        // ボスの動きの状態を変更する
        currentStartMoveState = IntimidationStart;

        // アニメーションも変更したのでフラグを戻す
        isEndAnimationFirstRoop = false;
    }
}

void Boss::InitializeIntimidationEffectData()
{
    // エフェクトの回転率
    intimidationEffectData.rotationRate = VGet(0.0f, 0.0f, 0.0f);

    // エフェクトの座標の初期化
    intimidationEffectData.position = position;

    // エフェクトの種類
    intimidationEffectData.effectTag = EffectManager::BossIntimidation;

    // エフェクトの再生速度
    intimidationEffectData.playSpeed = 1.0f;

    // エフェクトのサイズ
    intimidationEffectData.scalingRate = VGet(IntimidationEffectScale, IntimidationEffectScale, IntimidationEffectScale);
}

