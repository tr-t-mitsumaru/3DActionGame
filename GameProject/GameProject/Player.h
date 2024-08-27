#pragma once
#include"DxLib.h"
#include"CollisionData.h"
#include"EffectData.h"

//クラスのプロトタイプ宣言
class ModelDataManager;
class StateBase;
class SoundManager;

class Player
{
public:

    /////////        定数         ////////

    static constexpr int   MaxHp = 700;   // HPの最大値

    //アニメーションの種類
    enum AnimationState
    {
        BackWalk         = 0,    // 後ろ歩き
        StartBlocking    = 1,    // 防御開始
        BlockingImpact   = 2,    // ガード中にダメージを受ける
        ComboAttack = 3,    // 連続攻撃
        Death            = 4,    // 死亡
        Slash            = 5,   // 通常攻撃
        Idle             = 6,    // 静止
        Impact           = 7,    // ダメージヒット
        Jump             = 8,    // ジャンプ
        Rolling          = 9,    // 回避
        Run              = 10,   // 走り
        Spell            = 11,   // 魔法攻撃
        Clash            = 12,    // 強攻撃
        Walk             = 13,   // 歩き
    };

    /// <summary>
    /// プレイヤーのステートの種類
    /// </summary>
    enum PlayerState
    {
        IdleState    = 0,   // 静止
        MoveState    = 1,   // 移動
        JumpState    = 2,   // ジャンプ
        AttackState  = 3,   // 攻撃
        DefenseState = 4,   // 防御
        HitState     = 5,   // 被弾
        RollingState = 6,   // 回避
        ShotState    = 7,   // 弾を撃つ
        DeadState    = 8,   // 死亡
    };

    /// <summary>
    /// 体力の状態
    /// </summary>
    enum LifeState
    {
        NoDamage = 0,   // ダメージを受けていない
        Damaged  = 1,   // ダメージを受けた
        NoLife   = 2,   // 体力が0
    };

    //getter,setter
    const VECTOR GetPosition()const { return position; }

    /// <summary>
    /// HPを渡す
    /// </summary>
    /// <returns>HP</returns>
    const int GetHp()const { return hp; }

    /// <summary>
    /// 死亡時の動きが終わったフラグを返す
    /// </summary>
    /// <returns>死亡時の動きが終わったかのフラグ</returns>
    const bool GetEndedDeadMove() const { return endedDeadMove; }

    //コンストラクタ
    Player();
    //デストラクタ
    virtual ~Player();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update(const VECTOR playerTargetPosition, const VECTOR cameraPosition);

    /// <summary>
    /// 登場シーンでの更新処理
    /// </summary>
    void UpdateStartScene(const float distanceToBoss);

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

    /// <summary>
    /// 中央からの最大距離に達した際の押し戻し処理
    /// </summary>
    void ApplyBoundaryPushback();


private:

    ///////  定数  //////

    static const VECTOR ModelOffsetPosition;                    // モデルの位置を調整するための値
    static constexpr float  DefaultScale              = 0.02f;   // モデルの初期スケール
    static constexpr float AngleSpeed                 = 0.15f;  // モデルの向きを変えるスピード
    static constexpr float CollisionCapsuleLineLength = 2.4f;  // 当たり判定に必要なカプセルの線分の長さ
    static constexpr float CollisionRadius            = 0.6f;   // 当たり判定に必要なカプセルの半径の大きさ
    static constexpr float HalfLength                 = 0.5f;   // 中央座標を出す際の長さを半分にするための定数
    static constexpr float InvincibilityStartRatio    = 0.1f;   // 無敵時間の開始割合
    static constexpr float InvincibilityEndRatio      = 0.7f;   // 無敵時間の終了割合
    static constexpr float ShotHitEffectScale         = 1.0f;  // 弾が当たった時のエフェクトのサイズ

    // 登場シーンで使用する定数
    static constexpr float MoveSpeed           = 0.07f;  // 登場シーンでの移動スピード
    static constexpr float MoveDistance        = 15.0f;  // 移動する距離
    static constexpr float AnimationSpeed      = 0.8f;   // アニメーションの再生速度
    static constexpr float AnimationBlendSpeed = 0.1f;   // アニメーションのブレンドスピード

    ///////  メンバ変数  ///////

    int           modelHandle;      //モデルハンドル
    VECTOR        position;         //座標
    CollisionData collisionData;    //当たり判定に必要な情報をまとめたもの
    float         angle;            //モデルの向いている角度
    VECTOR        modelDirection;   //モデルの向くべき方向
    int           hp;               //プレイヤーの体力
    bool          isBossHited;      //ボスと当たったかのフラグ
    bool          isDamage;         // ダメージを受けたかのフラグ

    // 登場シーンで使用する変数
    float animationLimitTime;        // 再生中のアニメーションの総再生時間
    float animationNowTime;          // 再生中のアニメーションの現在の経過時間
    int   animationIndex;            // 現在のアニメーションのインデックス
    int   beforeAnimationIndex;      // 前のアニメーションのインデックス
    float animationBlendRate;        // アニメーションのブレンド率
    bool  isEndMove;                 // 移動が終了したかのフラグ
    bool  isBlendingAnimation;       // 今ブレンドを行っているかのフラグ
    bool  endedDeadMove;               // 死亡時の移動が終了したフラグ



    ///////  メンバクラス  ///////

    StateBase* nowState;                            //現在のステートを保存するポインタ
    StateBase* nextState;                           //次のループでのステートを保存するポインタ
    CollisionManager* collisionManager;             //当たり判定の管理クラスのポインタ

    // エフェクト
    EffectManager* effectManager;   // エフェクトの管理クラスにアクセスするためのポインタ
    EffectData shotHitEffectData;   // エフェクトの更新に必要なデータ

    SoundManager* soundManager;     // 音管理クラス


    ///////  メンバ関数  ///////

    /// <summary>
    /// ステートの移行処理
    /// </summary>
    void ChangeState();

    /// <summary>
    /// 座標などを当たり判定に必要なデータに変換
    /// </summary>
    void UpdateCollisionData();

    /// <summary>
    /// オブジェクトに当たった際の処理を書いたもの
    /// </summary>
    /// <param name="">当たり判定に必要な情報をまとめたデータ</param>
    void OnHit(CollisionData collitionData);

    /// <summary>
    /// 弾と当たった際のエフェクトの初期化
    /// </summary>
    /// <param name="shotPosition">当たった弾の座標</param>
    void InitializeShotHitEffectData(const VECTOR shotPosition);
    
    /// <summary>
    /// プレイヤーの回転制御
    /// </summary>
    /// <param name="direction">プレイヤーの移動方向</param>
    void UpdateAngle();

    /// <summary>
    /// 無敵状態の切り替えを行う
    /// </summary>
    void SwitchInvincibility();

    /// <summary>
    /// ボスに当たった際の押し戻し処理
    /// </summary>
    /// <param name="targetPosition">相手の座標</param>
    void PushBack(const VECTOR targetPosition, const float targetRadius);

    /// <summary>
    /// アニメーションの更新処理
    /// </summary>
    void UpdateAnimation();


 

};
