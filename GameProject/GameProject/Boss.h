#pragma once
#include"CollisionData.h"
#include"EffectData.h"

class ModelDataManager;
class CollisionManager;
class StateBase;
class EffectManager;


/// <summary>
/// ボスのクラス
/// </summary>
class Boss
{
public:

    /// <summary>
    /// アニメーションの状態
    /// </summary>
    enum AnimationState
    {
        Attack       = 0,    //攻撃
        Dead         = 1,    //死亡
        FireBleth    = 2,    //ブレス攻撃
        GroundAttack = 3,    //地面に攻撃
        Idle         = 4,    //静止
        Intimidation = 5,    //登場時の威嚇
        JumpAttack   = 6,    //ジャンプして攻撃
        LeftShot     = 7,    //左手で撃つショット
        LeftTrun     = 8,    //左に向きを切り替える
        RightShot    = 9,    //右手で撃つショット
        RightTurn    = 10,   //右向き切り替え
        Run          = 11,   //走る
        SitDown      = 12,   //座っている状態
        Standing     = 13,   //立つ動作
        Walk         = 14    //歩き
    };

    ///////  ゲッター  ///////

    // 座標を渡す
    const VECTOR GetPosition() const { return position; }

    //コンストラクタ
    Boss();
    //デストラクタ
    ~Boss();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update(const VECTOR bossTargetPosition,const VECTOR cameraPosition);

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

private:
    ////定数////

    static const VECTOR InitialPosition;             //初期座標
    static const VECTOR OffsetModelPosition;         //モデルの位置調整用の値

    static constexpr float CollisionCapsuleLineLength = 60.0f;   // 当たり判定に必要なカプセルの線分の長さ
    static constexpr float CollisionRadius            = 35.0f;   // 当たり判定に必要なカプセルの半径の大きさ
    static constexpr float HalfLength                 = 0.5f;    // 中央座標を出す際の長さを半分にするための定数
    static constexpr int   NotRegisterTag             = -1;      // レジスタの識別番号が代入されていないことをしめす
    static constexpr float DefaultScale               = 0.2f;    // 基本的なスケール
    static constexpr float AngleSpeed                 = 0.15f;   // モデルの向きを変えるスピード
    static constexpr float ShotHitEffectScale         = 10.0f;   // 弾が当たった時のエフェクトのサイズ
    static constexpr float PlayerAttackHitEffectScale = 10.0f;   // プレイヤーの攻撃が当たった際のエフェクトのサイズ

    ////         メンバ変数           ////

    //モデル
    int               modelHandle;      //モデルハンドル
    VECTOR            modelDirection;   //モデルの向くべき方向
    float             angle;            //モデルの向きを変更させる際の値
    VECTOR            position;         //座標
    CollisionData     collisionData;    //当たり判定に必要な情報
    CollisionManager* collisionManager; //当たり判定管理クラスにアクセスするポインタ
    int               hp;

    //ステート
    StateBase* nowState;                //現在のステートを保存するポインタ
    StateBase* nextState;               //次のループでのステートをホゾンスルポインタ

    // エフェクト
    EffectManager* effectManager;         // エフェクトの管理クラスにアクセスするポインタ
    EffectData shotHitEffectData;         // エフェクトの更新に必要なデータ
    EffectData playerAttackHitEffectData; // プレイヤーの攻撃が当たった際のエフェクトのデータ
    


    ////メンバ関数////

    /// <summary>
    /// ボスの情報から当たり判定に必要な情報を出して代入
    /// </summary>
    void UpdateCollisionData();

    /// <summary>
    /// オブジェクトに当たった際の処理を書いたもの
    /// </summary>
    /// <param name="">当たり判定に必要な情報をまとめたデータ</param>
    void OnHit(const CollisionData collisionData);

    /// <summary>
    /// 弾と当たった際のエフェクトの初期化
    /// </summary>
    /// <param name="shotPosition">当たった弾の座標</param>
    void InitializeShotHitEffectData(const VECTOR shotPosition);

    /// <summary>
    /// プレイヤーの攻撃に当たった際のエフェクトの初期化
    /// </summary>
    /// <param name="attackPosition">攻撃の座標</param>
    void InitializePlayerAttackHitEffectData(const VECTOR attackPosition);


    /// <summary>
    /// ステートの移行処理
    /// </summary>
    void ChangeState();

    /// <summary>
    /// プレイヤーの回転制御
    /// </summary>
    void UpdateAngle();

};
