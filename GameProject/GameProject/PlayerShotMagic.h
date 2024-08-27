#pragma once
#include"StateBase.h"
#include"ShotManager.h"
#include"EffectData.h"


struct InitializeShotData;
class  EffectManager;
class SoundManager;

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

    static constexpr float ShotCreateAnimationRatio  = 0.2f;        // ショットを作成するアニメーションの再生率
    static constexpr float ShotSpeed                 = 0.7f;        // ショットのスピード
    static constexpr float ShotRadius                = 0.5f;        // 弾の半径
    static constexpr int   ShotDamageAmount          = 30;           // ショットが与えるダメージ
    static constexpr float EffectDefaultScale        = 1.0f;        // エフェクトのサイズ
    static constexpr float EffectPlaySpeed           = 1.0f;        // エフェクトの再生速度
    static constexpr float OffsetEffectPositionScale = 1.5f;        // エフェクトを再生する位置をどれだけ前に進めるか 
    static const VECTOR    OffsetEffectPosition;                    // エフェクトを再生する位置をプレイヤーからみてどれだけずらすか


    ///////      変数      ////////

    ShotManager*   shotManager;        // ショット管理クラスのポインタ
    bool           isShotFired;        // 弾が撃たれたかのフラグ
    EffectManager* effectManager;      // エフェクトの管理クラスのポインタ
    EffectData     effectData;         // エフェクトの移動などに必要なデータ
    bool           isPlaiedEffect;     // エフェクトを再生したか
    SoundManager*  soundManager;       // 音管理クラスのポインタ
    bool           playedShotVoice;    // プレイヤーのボイスを再生したか


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

    /// <summary>
    /// エフェクトデータの初期化
    /// </summary>
    void UpdateEffectData(const VECTOR characterPosition, const VECTOR modelDirection);



};
