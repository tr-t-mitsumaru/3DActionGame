#pragma once
#include"StateBase.h"
#include"InputManager.h"
#include"ShotManager.h"
#include"EffectData.h"

struct InitializeShotData;
class  EffectManager;

/// <summary>
/// ボスの静止ステート
/// </summary>
class BossShotAttack :public StateBase
{
public:

    ///////  メンバ関数  //////

    //コンストラクタ
    BossShotAttack(int& modelHandle, const int beforeAnimationIndex);
    //デストラクタ
    ~BossShotAttack();

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="position">プレイヤーモデルの向き</param>
    void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition, VECTOR cameraPosition)override;

private:

    ///////  enum  ///////

    /// <summary>
    /// 弾を撃っている状態
    /// </summary>
    enum ShotState
    {
        WaitLeftShot  = 0,   // レフトショット待機
        LeftShot      = 1,   // レフトショットを撃った後
        WaitRightShot = 2,   // ライトショット待機
        RightSHot     = 3,   // ライトショットを撃った後
    };

    ///////  定数  ///////

    static constexpr float InitializeAnimationSpeed = 0.5f;    //アニメーションの初期速度
    static constexpr float ShotCreateAnimationRatio = 0.35f;   // ショットを作成するアニメーションの再生率
    static constexpr float EffectPlaySpeed          = 1.0f;    // エフェクトの再生速度

    static constexpr float EasyAnimationBlendSpeed   = 0.008f;  // 弱い時のアニメーションのブレンドスピード
    static constexpr float NormalAnimationBlendSpeed = 0.01f;   // 通常時のアニメーションのブレンドスピード
    static constexpr float HardAnimationBlendSpeed   = 0.03f;   // 強い時のアニメーションのブレンドスピード
    static constexpr float EasyShotSpeed             = 0.6f;    // 弱い時の弾のスピード
    static constexpr float NormalShotSpeed           = 0.7f;    // 通常時の弾のスピード
    static constexpr float HardShotSpeed             = 0.9f;    // 強い時の弾のスピード
    static constexpr float EasyShotRadius            = 1.5f;   // 弱い時の弾の大きさ
    static constexpr float NormalShotRadius          = 2.5f;   // 通常時の弾の大きさ
    static constexpr float HardShotRadius            = 4.0f;   // 強い時の弾の大きさ
    static constexpr int   EasyShotDamageAmount      = 40;      // 弱い時の弾のダメージ
    static constexpr int   NormalShotDamageAmount    = 60;      // 通常時の弾のダメージ
    static constexpr int   HardShotDamageAmount      = 80;      // 強い時の弾のダメージ
    static constexpr float EasyEffectDefaultScale    = 1.5f;   // 弱い時のエフェクトの大きさ
    static constexpr float NormalEffectDefaultScale  = 2.5f;   // 通常時のエフェクトの大きさ
    static constexpr float HardEffectDefaultScale    = 4.0f;   // 強い時のエフェクトの大きさ






    

    ///////  メンバ変数  ////////


    ShotManager*   shotManager;                   // ショット管理クラスのポインタ
    bool           isAnimationSwitch;             // アニメーションを切り替えたかのフラグ
    ShotState      shotState;                     // 弾の撃っている状態
    bool           isAttackParameterInitialize;   // 攻撃に使うパラメーターを初期化したか
    float          animationBlendSpeed;           // 前のアニメーションとブレンドする時間
    float          shotSpeed;                     // 弾のスピード
    float          shotRadius;                    // 弾の半径
    int            shotDamageAmount;              // 弾のダメージ
    VECTOR         effectScale;                   // エフェクトのサイズ
    SoundManager*  soundManager;                  // 音管理クラスのポインタ        

    ///////  メンバ関数  //////

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;

    /// <summary>
    /// アニメーションの切り替え
    /// </summary>
    void SwitchAnimation();

    /// <summary>
    /// アニメーションの再生率によってショットを作成
    /// </summary>
    /// <param name="position">自身の座標</param>
    void SpawnShotByAnimationTime(const VECTOR position, const VECTOR bossTargetPosition, VECTOR& modelDirection);

    /// <summary>
    /// 弾の作成に必要な情報を
    /// </summary>
    InitializeShotData AssignInitializeShotData(const VECTOR position,const VECTOR bossTargetPosition);

    /// <summary>
    /// 攻撃に必要なパラメータの初期化
    /// </summary>
    void InitializeAttackParameter();
};
