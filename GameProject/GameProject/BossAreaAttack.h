#pragma once
#include"StateBase.h"
#include"InputManager.h"

class ShotManager;
struct InitializeShotData;

/// <summary>
/// ボスの静止ステート
/// </summary>
class BossAreaAttack :public StateBase
{
public:
    ///////  メンバ関数  /////////

    //コンストラクタ
    BossAreaAttack(int& modelHandle, const int beforeAnimationIndex);
    //デストラクタ
    ~BossAreaAttack();

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="position">自身のモデルの向き</param>
    /// <param name="position">自身のキャラクターの座標</param>
    /// <param name="bossTargetPosition">敵対しているキャラの座標</param>
    void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition, VECTOR cameraPosition)override;

    /// <summary>
    /// 攻撃用のショットを作成する
    /// </summary>
    void CreateAttackShot(VECTOR characterPosition);

private:

    enum AttackState
    {
        NoAttack    = 0,
        StartAttack = 1,
        EndAttack   = 2
    };

    ///////  定数  ///////

    static constexpr float InitializeAnimationSpeed = 0.6f;         // アニメーションの初期速度
    static constexpr float AnimationBlendSpeed      = 0.03f;        // アニメーションのブレンドスピード
    static constexpr int   SpawnShotCount           = 50;           // 範囲攻撃で生成する弾の数

    static constexpr float FullCircleRadian         = 2 * DX_PI_F;  // １回転分のラジアン
    static constexpr float ShotRadius               = 20.0f;        // 生成する弾の半径
    static constexpr int   ShotDamageAmount         = 15;           // 弾のダメージ量
    static constexpr float ShotSpeed                = 5.0f;         // 弾のスピード
    static constexpr float ShotCreateAnimationRatio = 0.5f;         // ショットを撃つアニメーションの再生率
    static constexpr float EffectDefaultScale       = 25.0f;        // エフェクトの初期スケール
    static constexpr float EffectPlaySpeed          = 1.0f;

    ///////  メンバ変数  ////////

    //攻撃した時の当たり判定に必要な情報をまとめたもの
    CollisionData collisionData;           //当たり判定に必要な情報をまとめたもの
    CollisionManager* collisionManager;    //当たり判定の管理クラスのポインタ
    //ショット関連
    ShotManager* shotManager;              // ショットの管理クラス
    AttackState  attackState;              // 現在の攻撃の状態

    ///////  メンバ関数  //////

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;

    /// <summary>
    /// 弾のデータを初期化
    /// </summary>
    /// <param name="index">何番目の弾かの添え字</param>
    /// <param name="characterPosition">基準となる座標</param>
    /// <returns>初期化用データ</returns>
    InitializeShotData InitializeShot(const int index, const VECTOR characterPosition);


    /// <summary>
    /// アニメーションの再生率によってショットを作成
    /// </summary>
    /// <param name="position">自身の座標</param>
    void CreateShotByAnimationTime(const VECTOR position);

};
