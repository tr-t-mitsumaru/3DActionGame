#pragma once
#include"StateBase.h"
#include"InputManager.h"

/// <summary>
/// ボスの登場時の行動開始
/// </summary>
class BossStart:public StateBase
{
public:

    ///////  メンバ関数  //////

    //コンストラクタ
    BossStart(int& modelHandle, const int beforeAnimationIndex);
    //デストラクタ
    ~BossStart();

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="position">自身のモデルの向き</param>
    /// <param name="position">自身のキャラクターの座標</param>
    /// <param name="bossTargetPosition">敵対しているキャラの座標</param>
    void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR bossTargetPosition)override;

private:

    ///////  enum  ///////

    /// <summary>
    /// 行動しているかの状態
    /// </summary>
    enum StartMoveState
    {
        SitDown  = 0,  //座っている
        Standing = 1,  //立ち上がっている途中
        Stand    = 2,  //立ち上がった
    };

    ///////  定数  ///////

    static constexpr float InitializeAnimationSpeed = 1.0f;    //最初のアニメーションのスピード
    static constexpr int   StartCountLimit               = 40; //ボスが行動を開始するまでのカウントの上限               

    ///////  メンバ変数  ////////

    int startCount;             //ボスが行動を開始するまでのカウント
    StartMoveState currentStartMoveState; //現在のボスの行動状態

    //ToDo
    //AIを作成したら入力は必要ないので削除
    InputManager* inputManager;      //入力管理クラス

    ///////  メンバ関数  //////

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;

    /// <summary>
    /// 行動開始前のアニメーションの状態変更させるための関数
    /// </summary>
    void ChangeStartMoveState();
};

