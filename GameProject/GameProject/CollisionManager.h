#pragma once
#include"DxLib.h"
#include<functional>
#include<vector>
#include<list>
using namespace std;

struct CollisionData;

/// <summary>
/// 当たり判定を見るクラス
/// </summary>
class CollisionManager
{
public:
    /// <summary>
    /// 当たり判定を見るオブジェクトの種類
    /// </summary>
    enum HitObjectTag
    {
        Player            = 0,    // プレイヤー
        Boss              = 1,    // ボス
        PlayerAttack      = 2,    // プレイヤーの攻撃
        BossDefaultAttack = 3,    // ボスの通常攻撃
        BossAreaAttack    = 4,    // ボスの範囲攻撃
    };

    //定数
    static constexpr int   NotRegisterTag = -1;   //レジスタの識別番号が代入されていないことをしめす

    //インスタンスを取得するゲッター
    static CollisionManager* GetInstance() { return collisionManager; }
    //インスタンスを作成する
    static void CreateInstance();

    /// <summary>
    /// インスタンスの削除
    /// </summary>
    static void DeleteInstance();

    /// <summary>
    ///　コリジョンデータのポインタをもらってコリジョンリストに追加
    /// </summary>
    /// <param name="data">追加するコリジョンデータ</param>
    void RegisterCollisionData(CollisionData* data);

    /// <summary>
    /// 地面に向けたベクトルの調整
    /// </summary>
    /// <param name="velocity">プレイヤーのベロシティ</param>
    /// <param name="beforePosition">反映させる前の座標</param>
    /// <returns>調整したベロシティ</returns>
    static VECTOR AdjustGroundToWardVelocity(VECTOR velocity, VECTOR beforePosition);

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update();

private:
    ///////  定数  ///////

    static constexpr float AdjustVelocityY = 0.01f;//y方向のvelocityを調整する際の大きさ

    ///////  メンバ変数  //////

    //自身のポインタ
    static CollisionManager* collisionManager;
    //当たり判定情報をもったリスト
    vector<CollisionData*> hitObjectList;

    ///////  メンバ関数  ///////

    //コンストラクタ
    CollisionManager();

    //デストラクタ
    ~CollisionManager();

    /// <summary>
    /// 物体に当たったかを判定してその際の関数を呼ぶ
    /// </summary>
    /// <param name="collider">衝突を検出するオブジェクト</param>
    /// <param name="target">衝突の対象になるオブジェクト</param>
    void ResponseColisionIfDetected(CollisionData* const& collider, CollisionData* const& target);


    /// <summary>
    /// 2つのベクトル間の距離を求める
    /// </summary>
    /// <param name="vector1">ベクトル１</param>
    /// <param name="vector2">ベクトル２</param>
    /// <returns>計算した距離</returns>
    float CalculationDistance(const VECTOR vector1,const VECTOR vector2);

    /// <summary>
    /// 球体とカプセルとの当たり判定
    /// </summary>
    /// <param name="sphere">球体の当たり判定に必要な情報</param>
    /// <param name="capsule">カプセルの当たり判定に必要な情報</param>
    /// <returns>当たっているか</returns>
    bool IsHitSphereAndCapsule( CollisionData sphere, CollisionData capsule);

    /// <summary>
    /// カプセル同士の当たり判定
    /// </summary>
    /// <param name="collider">衝突を検知する側の情報</param>
    /// <param name="target">目標の情報</param>
    /// <returns>当たったか</returns>
    bool IsHitCapsuleAndCapsule(const CollisionData  collider,const CollisionData target);

};

