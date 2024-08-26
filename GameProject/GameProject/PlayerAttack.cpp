#include"PlayerAttack.h"
#include"StateBase.h"
#include"Player.h"
#include"PlayerHit.h"
#include"PlayerIdle.h"
#include"InputManager.h"
#include"CollisionUtility.h"
#include"InputManager.h"
#include"Utility.h"



const VECTOR PlayerAttack::NormalAttackOffsetPositionY = VGet(-1.0f,1.0f, 0.0f);
const VECTOR PlayerAttack::StrongAttackOffsetPositionY = VGet(0.0f, 1.0f, 0.0f);


/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="InitalModelHandle">モデルハンドル</param>
/// <param name="beforeAnimationIndex">前のステートでのアニメーション情報</param>
PlayerAttack::PlayerAttack(int InitalModelHandle, int beforeAnimationIndex, Player::AnimationState initializeAnimationState)
    :StateBase(InitalModelHandle,initializeAnimationState,beforeAnimationIndex)
    ,currentComboState(First)
    ,currentComboCollisionState(FirstStart)
{
    // インプットマネージャーのインスタンスをもってくる
    inputManager = InputManager::GetInstance();

    // 現在のステートを入れる
    nowStateTag = Player::AttackState;

    // 攻撃の種類を代入
    currentAttackState = initializeAnimationState;

    // 通常攻撃と強攻撃それぞれの当たり判定のサイズを代入
    if (initializeAnimationState == Player::ComboAttack)
    {
        // カプセルの長さ
        collisionCapsuleLineLength   = NormalAttackCollisionCapsuleLineLength;

        // 当たり判定の幅
        collisionRadius              = NormalAttackCollisionRadius;

        // 当たり判定を開始させるアニメーションの再生率
        collisionStratAnimationRatio = NormalAttackCollisionStartAnimationRatio;

        // 当たり判定カプセルの回転角度
        collisionCapsuleAngle        = NormalAttackCollisionCapsuleAngle;

        // 当たり判定座標のずらす量
        offsetPosition               = NormalAttackOffsetPositionY;

        // 当たり判定をモデルの向いている方向にどれだけすすめるか
        offsetPositionScale          = NormalAttackOffsetPositionScale;

        // ダメージ量の代入
        damageAmount = NormalAttackDamageAmount;

    }
    else
    {
        // カプセルの長さ
        collisionCapsuleLineLength   = StrongAttackCollisionCapsuleLineLength;

        // 当たり判定の幅
        collisionRadius              = StrongAttackCollisionRadius;

        // 当たり判定を開始させるアニメーションの再生率
        collisionStratAnimationRatio = StrongAttackCollisionStartAnimationRatio;

        // 当たり判定カプセルの回転角度
        collisionCapsuleAngle        = StrongAttackCollisionCapsuleAngle;

        // 当たり判定座標のずらす量
        offsetPosition               = StrongAttackOffsetPositionY;

        // 当たり判定をモデルの向いている方向にどれだけすすめるか
        offsetPositionScale          = StrongAttackOffsetPositionScale;

        // ダメージ量の代入
        damageAmount                 = StrongAttackDamageAmount;
    }

    //アニメーション速度の初期化
    animationSpeed = 1.0f;

    // コリジョンマネージャーのインスタンスをもってくる
    collisionManager = CollisionManager::GetInstance();

    //当たり判定がまだ生成されていない状態
    collisionData.collisionState = CollisionData::NoCollision;
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerAttack::~PlayerAttack()
{
    //当たり判定情報を削除
    this->collisionData.collisionState = CollisionData::CollisionEnded;
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="modelDirection">モデルの向き</param>
/// <param name="characterPosition">キャラクターの座標</param>
void PlayerAttack::Update(VECTOR& modelDirection, VECTOR& position,const VECTOR playerTargetPosition, VECTOR cameraPosition)
{
    //アニメーションの再生時間のセット
    UpdateAnimation();
 
    //ステートの切り替え処理を呼ぶ
    ChangeState();

    // コンボ攻撃を続けるか入力を見てチェック
    IsComboAttackActive();

    //アニメーションが終了していたら当たり判定を消す
    if (currentPlayAnimationState == FirstRoopEnd || currentComboState == End)
    {
        collisionData.collisionState = CollisionData::CollisionEnded;
    }

    // 移動量の計算
    CalculateVelocity(modelDirection);

    //当たり判定に必要な情報の更新
    UpdateCollisionData(modelDirection,position);

    // アニメーションの再生率に合わせて攻撃の当たり判定の調整
    UpdateCollisionStateByAnimationRatio();

    //シーンが切り替わっていればアニメーションをデタッチ
    DetachAnimation();

}

/// <summary>
/// アニメーションの再生率に合わせて当たり判定の状態を更新する
/// </summary>
void PlayerAttack::UpdateCollisionStateByAnimationRatio()
{
    // 現在のアニメーションの再生率を出す
    float currentAnimationRatio = animationNowTime / animationLimitTime;

    if (currentAttackState == Player::ComboAttack)
    {

        // アニメーションの再生率が１撃目の開始ラインを超えていたら当たり判定を作成
        if (currentAnimationRatio > FirstAttackCollisionStartTime && collisionData.collisionState == CollisionData::NoCollision)
        {
            collisionData.collisionState = CollisionData::CollisionActive;
            collisionManager->RegisterCollisionData(&collisionData);
        }

        // アニメーションの再生率が１撃目の終了ラインを超えていたら当たり判定を削除
        else if (currentAnimationRatio > FirstAttackCollisionEndTime && currentComboCollisionState == FirstStart)
        {
            collisionData.collisionState = CollisionData::CollisionEnded;
            currentComboCollisionState = FirstEnd;
        }

        // アニメーションの再生率が２撃目の開始ラインを超えていたら当たり判定を作成
        else if (currentAnimationRatio > SecondAttackCollisionStartTime && currentComboCollisionState == FirstEnd)
        {
            collisionData.collisionState = CollisionData::CollisionActive;
            collisionManager->RegisterCollisionData(&collisionData);
            currentComboCollisionState = SecondStart;
        }

        // アニメーションの再生率が２撃目の終了ラインを超えていたら当たり判定を削除
        else if (currentAnimationRatio > SecondAttackCollisionEndTime && currentComboCollisionState == SecondStart)
        {
            collisionData.collisionState = CollisionData::CollisionEnded;
            currentComboCollisionState = SecondEnd;
        }

        // アニメーションの再生率が３撃目の開始ラインを超えていたら当たり判定を作成
        else if (currentAnimationRatio > ThirdAttackCollisionStartTime && currentComboCollisionState == SecondEnd)
        {
            collisionData.collisionState = CollisionData::CollisionActive;
            collisionManager->RegisterCollisionData(&collisionData);
            currentComboCollisionState = ThirdStart;
        }

        // アニメーションの再生率が３撃目の終了ラインを超えていたら当たり判定を削除
        else if (currentAnimationRatio > ThirdAttackCollisionEndTime && currentComboCollisionState == ThirdStart)
        {
            collisionData.collisionState = CollisionData::CollisionEnded;
            currentComboCollisionState = ThirdEnd;
        }
    }
    else
    {
        if (currentAnimationRatio > StrongAttackCollisionStartAnimationRatio && collisionData.collisionState == CollisionData::NoCollision)
        {
            collisionData.collisionState = CollisionData::CollisionActive;
            collisionManager->RegisterCollisionData(&collisionData);
        }
    }


}

/// <summary>
/// ステートの切り替え処理をまとめたもの
/// </summary>
void PlayerAttack::ChangeState()
{
    // ダメージを受けていたらヒットステートに移行
    if (lifeState == Player::Damaged)
    {
        nextState = new PlayerHit(modelhandle, animationIndex, Player::Impact);
    }
    //アニメーションの再生が終了したらステートを切り替える
    else if (currentComboState == End || currentPlayAnimationState == FirstRoopEnd)
    {
        nextState = new PlayerIdle(modelhandle, this->GetAnimationIndex());

    }
    else
    {
        nextState = this;
    }
}


/// <summary>
/// プレイヤーの情報から当たり判定に必要な情報を出して代入
/// </summary>
void PlayerAttack::UpdateCollisionData(const VECTOR& modelDirection,const VECTOR characterPosition)
{
    //当たり判定の座標を移動させる
    position = CollisionUtility::TransrateCollisionCapsulePosition(characterPosition, modelDirection,offsetPosition,offsetPositionScale);

    //角度からラジアンに変換する
    float radianAngle = Utility::ConvertRadian(collisionCapsuleAngle);

    //カプセル回転用のベクトルを用意する
    VECTOR capsuleLineVector = CollisionUtility::RotationCollisionCapsule(radianAngle, modelDirection, position, collisionCapsuleLineLength);

    //中央座標の代入
    collisionData.centerPosition = position;
    //カプセルの下側の座標
    collisionData.bottomPosition = position;
    //カプセルの上側の座標
    collisionData.upPosition = VAdd(position, capsuleLineVector);
    //カプセルの球部分の半径
    collisionData.radius = collisionRadius;
    //オブジェクトの種類
    collisionData.hitObjectTag = CollisionManager::PlayerAttack;
    //当たった際の関数
    collisionData.onHit = std::bind(&PlayerAttack::OnHit, this, std::placeholders::_1);
    //当たった際のダメージ量
    collisionData.damageAmount = damageAmount;
}


/// <summary>
/// 当たった時の処理
/// </summary>
void PlayerAttack::OnHit(CollisionData collisionData)
{
    //当たったフラグをオンにする
    this->collisionData.collisionState = CollisionData::CollisionEnded;

    // 連続攻撃の状態に合わせてステートを切り替える
    switch (currentComboCollisionState)
    {
    // １撃目
    case FirstStart:
        currentComboCollisionState = FirstEnd;
        break;

    // ２撃目
    case SecondStart :
        currentComboCollisionState = SecondEnd;
        break;

    // ３撃目
    case ThirdEnd:
        currentComboCollisionState = ThirdEnd;
        break;

    default:
        break;
    }
}

/// <summary>
/// コンボ攻撃を続けるか調べて実行する
/// </summary>
void PlayerAttack::IsComboAttackActive()
{
    // 現在のアニメーションの再生率を出す
    float currentAnimationRatio = animationNowTime / animationLimitTime;

    if (currentAttackState == Player::ComboAttack)
    {
        switch (currentComboState)
        {
        case PlayerAttack::First:

            // 移動速度を代入
            moveSpeed = FirstComboMoveSpeed;

            // 規定時間の間で追加入力があればコンボを続行する
            if (currentAnimationRatio >= SecondAttackInputStartTime  && currentAnimationRatio < currentAnimationRatio < SecondAttackInputStartTime + InputTimeLimit &&
                inputManager->GetKeyPushState(InputManager::X) == InputManager::Push)
            {
                currentComboState = Second;
                damageAmount = SecondAttackDamageAmount;

            }
            else if (currentAnimationRatio > SecondAttackInputStartTime + InputTimeLimit)
            {
                currentComboState = End;
            }
            break;
        case PlayerAttack::Second:

            // 移動速度を代入
            moveSpeed = SecondComboMoveSpeed;

            // 規定時間の間で追加入力があればコンボを続行する
            if (currentAnimationRatio >= ThirdAttackInputStartTime && currentAnimationRatio < currentAnimationRatio < ThirdAttackInputStartTime + InputTimeLimit &&
                inputManager->GetKeyPushState(InputManager::X) == InputManager::Push)
            {
                currentComboState = Third;
                damageAmount = ThirdAttackDamageAmount;
            }
            else if (currentAnimationRatio > ThirdAttackInputStartTime + InputTimeLimit)
            {
                currentComboState = End;
            }

            break;
        case PlayerAttack::Third:

            // 移動速度を代入
            moveSpeed = ThirdComboMoveSpeed;

            // 攻撃アニメーションが終了したらコンボステートを切り替える
            if (currentPlayAnimationState == StateBase::FirstRoopEnd)
            {
                currentComboState = End;
            }

            break;
        case PlayerAttack::End:
            break;
        default:
            break;
        }
    }

}


/// <summary>
/// 移動する量の計算
/// </summary>
/// <param name="modelDirection">モデルの向いている方向</param>
void PlayerAttack::CalculateVelocity(const VECTOR modelDirection)
{
    // 移動量を計算する
    velocity = VScale(VNorm(modelDirection), moveSpeed);
}


#ifdef _DEBUG

void PlayerAttack::DrawCollision()
{
    if (collisionData.collisionState == CollisionData::CollisionActive)
    {
        //当たり判定が正しいかの確認用の描画
        DrawCapsule3D(collisionData.upPosition, collisionData.bottomPosition, collisionData.radius, 16, GetColor(255, 0, 0), GetColor(255, 0, 0), FALSE);
    }
}
#endif // _DEBUG

