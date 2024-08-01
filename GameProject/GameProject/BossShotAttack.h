﻿#pragma once
#include"StateBase.h"
#include"InputManager.h"

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
    void Update(VECTOR& modelDirection, VECTOR& position)override;

private:

    ///////  定数  ///////

    static constexpr float InitializeAnimationSpeed = 0.5f;    //アニメーションの初期速度

    ///////  メンバ変数  ////////

    //ToDo
    //AIを作成したら入力は必要ないので削除
    InputManager* inputManager;      //入力管理クラス

    ///////  メンバ関数  //////

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;
};
