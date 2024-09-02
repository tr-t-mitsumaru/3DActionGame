#pragma once
#include"DxLib.h"
#include"CollisionData.h"

//クラスのプロトタイプ宣言
class ModelDataManager;
class StateBase;

class Player
{
public:

    //アニメーションの種類
    enum AnimationState
    {
        Turn180        = 0,    //180度回転
        BackWalk       = 1,    //後ろ歩き
        Block          = 2,    //ガード
        BlockingImpact = 3,    //ガード中にダメージを受ける
        Clash          = 4,    //強攻撃
        Death          = 5,    //死亡
        Impact         = 6,    //ダメージヒット
        Idle           = 7,    //静止
        Jump           = 8,    //ジャンプ
        LeftTurn       = 9,    //左回転
        LeftWalk       = 10,   //左歩き
        RightTurn      = 11,   //右回転
        RightWalk      = 12,   //右歩き
        Rolling        = 13,   //回避
        Run            = 14,   //走り
        Slash          = 15,   //通常攻撃
        StartBlocking  = 16,   //防御開始
        Walk           = 17,   //歩き
        Spell          = 18    //魔法攻撃
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

    //コンストラクタ
    Player();
    //デストラクタ
    virtual ~Player();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update(const VECTOR playerTargetPosition, const VECTOR cameraPosition);

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();


private:
    ///////  定数  //////

    static const VECTOR ModelOffsetPosition;                  // モデルの位置を調整するための値
    static constexpr float AngleSpeed = 0.15f;                // モデルの向きを変えるスピード
    static constexpr float CollisionCapsuleLineLength = 24.0f;// 当たり判定に必要なカプセルの線分の長さ
    static constexpr float CollisionRadius            = 6.0f; // 当たり判定に必要なカプセルの半径の大きさ
    static constexpr float HalfLength                 = 0.5f; // 中央座標を出す際の長さを半分にするための定数
    static constexpr float InvincibilityStartRatio    = 0.1f; // 無敵時間の開始割合
    static constexpr float InvincibilityEndRatio      = 0.7f; // 無敵時間の終了割合

    ///////  メンバ変数  ///////

    int           modelHandle;      //モデルハンドル
    VECTOR        position;         //座標
    CollisionData collisionData;    //当たり判定に必要な情報をまとめたもの
    float         angle;            //モデルの向いている角度
    VECTOR        modelDirection;   //モデルの向くべき方向
    int           hp;               //プレイヤーの体力
    bool          isBossHited;      //ボスと当たったかのフラグ

    ///////  メンバクラス  ///////

    StateBase* nowState;                            //現在のステートを保存するポインタ
    StateBase* nextState;                           //次のループでのステートを保存するポインタ
    CollisionManager* collisionManager;             //当たり判定の管理クラスのポインタ


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
    /// プレイヤーの回転制御
    /// </summary>
    /// <param name="direction">プレイヤーの移動方向</param>
    void UpdateAngle();

    /// <summary>
    /// 無敵状態の切り替えを行う
    /// </summary>
    void SwitchInvincibility();

};
