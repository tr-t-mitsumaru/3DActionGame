#pragma once
#include"StateBase.h"
#include"ShotManager.h"


struct InitializeShotData;

/// <summary>
/// プレイヤーの攻撃に関するステート
/// </summary>
class PlayerShotMagic :public StateBase
{
public:
    //コンストラクタ
    PlayerShotMagic(int modelHandle, int beforeAnimationIndex);
    //デストラクタ
    ~PlayerShotMagic();

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="position">自身のモデルの向き</param>
    /// <param name="position">自身のキャラクターの座標</param>
    /// <param name="playerTargetPosition">敵対しているキャラの座標</param>
    /// <param name="targetPosition">敵対しているキャラの座標</param>
    void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR playerTargetPosition, VECTOR cameraPosition)override;

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;

private:
    ///////      定数      ///////

    static constexpr float ShotCreateAnimationRatio  = 0.2f;     // ショットを作成するアニメーションの再生率
    static constexpr float ShotSpeed                 = 7.0f;     // ショットのスピード
    static constexpr float ShotRadius                = 5.0f;     // 弾の半径
    static constexpr int   ShotDamageAmount          = 1;        // ショットが与えるダメージ


    ///////      変数      ////////

    ShotManager* shotManager;    // ショット管理クラスのポインタ
    bool         isShotFired;    // 弾が撃たれたかのフラグ


    ///////      関数     ////////


    /// <summary>
    /// アニメーションの再生率によってショットを作成する
    /// </summary>
    /// <param name="position">座標</param>
    /// <param name="targetPosition">目標の座標</param>
    /// <param name="modelDirection">モデルの向き</param>
    void CreateShotByAnimationRatio(const VECTOR position, const VECTOR targetPosition, VECTOR& modelDirection);


    /// <summary>
    /// 弾の作成に必要な情報を代入する
    /// </summary>
    /// <param name="position">自身の座標</param>
    /// <param name="targetPosition">弾を撃つ目標の座標</param>
    /// <returns>初期化した弾のデータ</returns>
    InitializeShotData AssignInitializeShotData(const VECTOR position, const VECTOR targetPosition);



};
