﻿#pragma once
#include"StateBase.h"
#include"InputManager.h"

/// <summary>
/// ボスの静止ステート
/// </summary>
class BossIdle :public StateBase
{
public:

    ///////  メンバ関数  ///////

    //コンストラクタ
    BossIdle(int& modelHandle, const int beforeAnimationIndex, const Boss::BossActionState initializeBeforeStateName);
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
    static constexpr float ShortRange                              = 90.0f;   // 近距離の範囲
    static constexpr float MiddleRange                             = 150.0f;   // 中距離の範囲
    static constexpr int   RandRange                               = 99;       // ランダムの範囲
    static constexpr float AnimationBlendSpeed                     = 0.03f;    // アニメーションのブレンドスピード
    static constexpr int   ShortRangeDefaultAttackProbability      = 60;       // 近距離での通常攻撃が出る確率
    static constexpr int   ShortRangeAreaAttackProbability         = 40;       // 近距離での通常攻撃が出る確率
    static constexpr int   MiddleRangeAreaAttackProbability        = 40;       // 中距離での範囲攻撃が出る確率
    static constexpr int   MiddleRangeShotAttackProbability        = 40;       // 中距離での弾を撃つ攻撃が出る確率
    static constexpr int   LongRangeRunAttackProbability           = 60;       // 長距離での突進攻撃が出る確率
    static constexpr int   LongRangeShotAttackProbability          = 20;       // 長距離での突進攻撃が出る確率

    ///////  メンバ変数  ///////

    Boss::BossActionState nextStateName;          // 次のステート
    Boss::BossActionState previousStateName;      // 前のステート

    ///////  メンバ関数  ///////

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;

    /// <summary>
    /// 行動パターンが決定するまでループを続ける
    /// </summary>
    void SelectActionPatternLoop(const float bossTargetDistance);

    /// <summary>
    /// 距離に合わせて行動パターンを選択
    /// </summary>
    /// <param name="targetDistance">ボスが攻撃する対象との距離</param>
    bool SelectActionOnRange(const float bossTargetDistance);
};

