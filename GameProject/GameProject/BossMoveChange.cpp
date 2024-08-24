#include"BossMoveChange.h"
#include"EffectManager.h"
#include"CollisionManager.h"
#include"CollisionUtility.h"
#include"ImageDataManager.h"
#include"BossIdle.h"


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
/// <param name="beforeAnimationIndex">前のステートのアニメーションの種類</param>
BossMoveChange::BossMoveChange(int& modelHandle, const int beforeAnimationIndex)
    :StateBase(modelHandle,Boss::Intimidation,beforeAnimationIndex)
    ,isPlaedEffect(false)
    ,isChangedTexture(false)
{
    // アニメーションスピードの初期化
    animationSpeed = InitializeAnimationSpeed;

    // コリジョンマネージャーのインスタンスをもってくる
    collisionManager = CollisionManager::GetInstance();

    // エフェクトマネージャーのインスタンスをもってくる
    effectManager = EffectManager::GetInstance();

    // 画像管理クラスのインスタンスをもってくる
    ImageDataManager* imageDataManager = ImageDataManager::GetInstance();

    secondModelTexture = imageDataManager->GetImageHandle(ImageDataManager::BossSecondTexture);

    thirdModelTexture = imageDataManager->GetImageHandle(ImageDataManager::BossThirdTexture);

    //当たり判定がまだ生成されていない状態
    collisionData.collisionState = CollisionData::NoCollision;
}

/// <summary>
/// デストラクタ
/// </summary>
BossMoveChange::~BossMoveChange()
{
    // 処理なし
}

/// 更新処理
/// </summary>
/// <param name="position">自身のモデルの向き</param>
/// <param name="position">自身のキャラクターの座標</param>
/// <param name="targetPosition">敵対しているキャラの座標</param>
void BossMoveChange::Update(VECTOR& modelDirection, VECTOR& position, const VECTOR targetPosition, VECTOR cameraPosition)
{
    // ステートの切り替え処理を呼ぶ
    ChangeState();

    // アニメーションの再生時間のセット
    UpdateAnimation();

    // アニメーションの再生が終わったら当たり判定を消す
    if (currentPlayAnimationState == FirstRoopEnd)
    {
        collisionData.collisionState = CollisionData::CollisionEnded;
    }

    //当たり判定に必要な情報の更新
    UpdateCollisionData(position);

    // エフェクトデータの更新
    UpdateEffectData(position);

    // モデルのテクスチャを変更する
    ChangeModelTexture();

    // アニメーションに合わせてエフェクトを再生する
    PlayEffectByAnimationTime();

    // アニメーションの再生率に合わせて当たり判定を作成する
    CreateCollisionByAnimationTime();

    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();
}

/// <summary>
/// ステートの切り替え処理をまとめたもの
/// </summary>
void BossMoveChange::ChangeState()
{
    // アニメーションの最初のループが終了したら
    if (currentPlayAnimationState == FirstRoopEnd)
    {
        //ボスの静止ステートに移行
        nextState = new BossIdle(modelhandle, this->GetAnimationIndex(), BossIdle::MoveChange,isChangingMove);
    }
    else
    {
        nextState = this;
    }
}

/// <summary>
/// エフェクトの再生に必要なデータの更新
/// </summary>
/// <param name="characterPosition">キャラクターの座標</param>
void BossMoveChange::UpdateEffectData(const VECTOR characterPosition)
{
    // エフェクトの座標を代入
    effectData.position = characterPosition;

    // エフェクトの回転率はいらないので0で初期化
    effectData.rotationRate = VGet(0, 0, 0);

    // ボスの威嚇時のエフェクトにする
    effectData.effectTag = EffectManager::BossIntimidation;

    // ボスのエフェクトのサイズを代入する
    effectData.scalingRate =  EffectDefaultScale;

    // エフェクトの再生速度を設定する
    effectData.playSpeed = EffectPlaySpeed;

}

/// <summary>
/// 座標などを当たり判定に必要なデータに更新
/// </summary>
/// <param name="characterPositon">自身のポジション</param>
void BossMoveChange::UpdateCollisionData(const VECTOR characterPositon)
{
    // 当たり判定の座標を移動させる
    collisionData.centerPosition = characterPositon;

    collisionData.upPosition = characterPositon;

    collisionData.bottomPosition = characterPositon;

    // 当たり判定の半径サイズ
    collisionData.radius = CollisionRadius;

    collisionData.hitObjectTag = CollisionManager::BossIntimidation;

    //当たった際の関数
    collisionData.onHit = std::bind(&BossMoveChange::OnHit, this, std::placeholders::_1);

    //当たった際のダメージ量
    collisionData.damageAmount = DamageAmount;
}

/// <summary>
/// アニメーションの再生率に合わせてエフェクトを再生する
/// </summary>
void BossMoveChange::PlayEffectByAnimationTime()
{
    if (animationNowTime / animationLimitTime >= EffectPlayAnimationRatio && !isPlaedEffect)
    {
        effectManager->PlayEffect(&effectData);
        isPlaedEffect = true;
    }
}

/// <summary>
/// アニメーションの再生率に合わせて当たり判定を作成する
/// </summary>
void BossMoveChange::CreateCollisionByAnimationTime()
{
    // 当たり判定が有効になった入ればCollisionManagerに送信
    if (collisionData.collisionState == CollisionData::NoCollision)
    {
        //アニメーションの再生割合を調べて当たり判定情報をCollisionManagerに送信する
        collisionData.collisionState = CollisionUtility::SendCollisionDataByAnimationTime(GetAnimationNowTime(), GetAnimationLimitTime(),
            collisionData.collisionState, InitializeCollisionStartAnimationRatio);

        if (collisionData.collisionState == CollisionData::CollisionActive)
        {
            collisionManager->RegisterCollisionData(&collisionData);
            effectManager->PlayEffect(&effectData);
        }
    }
}

/// <summary>
/// 当たった時の処理
/// </summary>
void BossMoveChange::OnHit(CollisionData collisionData)
{
    //当たり判定を消す
    this->collisionData.collisionState = CollisionData::CollisionEnded;
}

/// <summary>
/// モデルのテクスチャを変更する
/// </summary>
void BossMoveChange::ChangeModelTexture()
{
    // テクスチャが変更されていなければ体力に合わせてテクスチャを変えう
    if (! isChangedTexture && animationNowTime / animationLimitTime >= ChangeTexturePlayAnimationRatio)
    {
        if (currentHpState == Boss::Middle)
        {
            MV1SetTextureGraphHandle(modelhandle, 0, secondModelTexture, FALSE);
        }
        else if (currentHpState == Boss::Low)
        {
            MV1SetTextureGraphHandle(modelhandle, 0, thirdModelTexture, FALSE);
        }

        isChangedTexture = true;
    }
}

#ifdef _DEBUG

/// <summary>
/// 当たり判定を描画する
/// </summary>
void BossMoveChange::DrawCollision()
{
    if (collisionData.collisionState == CollisionData::CollisionActive)
    {
        //当たり判定が正しいかの確認用の描画
        DrawSphere3D(collisionData.centerPosition, collisionData.radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
    }
}
#endif // _DEBUG
