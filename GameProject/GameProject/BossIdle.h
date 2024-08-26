#pragma once
#include"StateBase.h"
#include"InputManager.h"

/// <summary>
/// ボスの静止ステート
/// </summary>
class BossIdle :public StateBase
{
public:

    enum  NextStateList
    {
        DefaultAttack = 0,  // 通常攻撃
        Move          = 1,  // 移動
        AreaAttack    = 2,  // 範囲攻撃
        ShotAttack    = 3,  // 弾を撃つ攻撃
        RunAttack     = 4,  // 突進攻撃
        MoveChange    = 5,  // 行動を変更させるステート
        Idle          = 6,  // 静止状態
        None          = 7,  // ステートが存在しない場合
    };

    ///////  メンバ関数  ///////

    //コンストラクタ
    BossIdle(int& modelHandle, const int beforeAnimationIndex, const NextStateList initializeBeforeStateName, const bool beforeIsChangintMove);
    //デストラクタ
    ~BossIdle();

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="position">プレイヤーモデルの向き</param>
    void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition, VECTOR cameraPosition)override;

private:



    /////////  定数  //////

    static constexpr float InitializeAnimationSpeed                = 1.0f;     // アニメーションの初期速度
    static constexpr float ShortRange                              = 9.0f;    // 近距離の範囲
    static constexpr float MiddleRange                             = 15.0f;   // 中距離の範囲
    static constexpr int   RandRange                               = 99;       // ランダムの範囲
    static constexpr float AnimationBlendSpeed                     = 0.03f;     // アニメーションのブレンドスピード
    static constexpr int   ShortRangeDefaultAttackProbability      = 60;       // 近距離での通常攻撃が出る確率
    static constexpr int   ShortRangeAreaAttackProbability         = 40;       // 近距離での通常攻撃が出る確率
    static constexpr int   MiddleRangeAreaAttackProbability        = 40;       // 中距離での範囲攻撃が出る確率
    static constexpr int   MiddleRangeShotAttackProbability        = 40;       // 中距離での弾を撃つ攻撃が出る確率
    static constexpr int   LongRangeRunAttackProbability           = 60;       // 長距離での突進攻撃が出る確率
    static constexpr int   LongRangeShotAttackProbability          = 20;       // 長距離での突進攻撃が出る確率
    static constexpr int   EasyIdleWaitCountLimit                  = 70.0f;    // 弱い時の静止状態の総カウント数
    static constexpr int   NormalIdleWaitCountLimit                = 50.0f;    // 通常時の静止状態の総カウント数
    static constexpr int   HardIdleWaitCountLimit                  = 30.0f;    // 強い時の静止状態の総カウント数


    ///////  メンバ変数  ///////

    NextStateList nextStateName;              // 次のステート
    NextStateList previousStateName;          // 前のステート
    int           idleWaitCountLimit;         // アイドル状態で止めておくカウントの総合時間
    int           idleWaitCount;              // アイドル状態でどれだけ止めているかのカウント
    bool          isIdleWaitCountInitialzie;  // 静止状態の待ち時間を初期化したか
    

    ///////  メンバ関数  ///////

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;

    /// <summary>
    /// 行動パターンを選択する
    /// </summary>
    void SelectActionPattern(const float targetDistance);

    /// <summary>
    /// 体力に合わせて静止状態にしておくカウントを変えて初期化
    /// </summary>
    void InitializeIdleWaitCount();
};

