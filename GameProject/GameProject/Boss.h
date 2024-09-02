#pragma once
#include"CollisionData.h"

class ModelDataManager;
class CollisionManager;
class StateBase;


/// <summary>
/// ボスのクラス
/// </summary>
class Boss
{
public:

    /// <summary>
    /// アニメーションの種類
    /// </summary>
    enum AnimationType
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
        Sitting      = 12,   //座っている状態
        Standing     = 13,   //立つ動作
        Walk         = 14    //歩き
    };

    /// <summary>
    /// ボスの行動の状態
    /// </summary>
    enum  BossActionState
    {
        DefaultAttack = 0,  // 通常攻撃
        Move          = 1,  // 移動
        AreaAttack    = 2,  // 範囲攻撃
        ShotAttack    = 3,  // 弾を撃つ攻撃
        RunAttack     = 4,  // 突進攻撃
        None          = 5,  // ステートが存在しない場合
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

    static constexpr float CollisionCapsuleLineLength = 60.0f;   //当たり判定に必要なカプセルの線分の長さ
    static constexpr float CollisionRadius            = 23.0f;   //当たり判定に必要なカプセルの半径の大きさ
    static constexpr float HalfLength                 = 0.5f;    //中央座標を出す際の長さを半分にするための定数
    static constexpr int   NotRegisterTag             = -1;      //レジスタの識別番号が代入されていないことをしめす
    static constexpr float DefaultScale               = 0.2f;    //基本的なスケール
    static constexpr float AngleSpeed                 = 0.15f;   //モデルの向きを変えるスピード

    ////メンバ変数////

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
    /// ステートの移行処理
    /// </summary>
    void ChangeState();

    /// <summary>
    /// プレイヤーの回転制御
    /// </summary>
    void UpdateAngle();

};
