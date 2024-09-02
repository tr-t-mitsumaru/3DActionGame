#pragma once
#include"DxLib.h"
#include"Player.h"
#include"Boss.h"



/// <summary>
/// ステート管理に使用する基底クラス
/// </summary>
class StateBase
{
public:

    //コンストラクタ
    StateBase(int& modelHandle,const int animationState, const int beforeAnimationIndex);
    //デストラクタ
    virtual ~StateBase();

    /////          ゲッター           ////

    /// 移動処理などが終わった後のベロシティを渡す
    const VECTOR GetVelocity() const { return velocity; }

    //前のステートのアニメーションのインデックスを返す
    const int GetBeforeAnimationIndex() const { return beforeAnimationIndex; }

    // アニメーションの現在の再生率を返す
    const float GetAnimationNowTime() const { return animationNowTime; }

    // アニメーションの総再生時間を返す
    const float GetAnimationLimitTime() const { return animationLimitTime; }

    // 体力の状態を返す
    const Player::LifeState GetLifeState() const { return lifeState; }

    ///////  メンバ関数  //////

    /// <summary>
    /// 次のステートを返す
    /// </summary>
    /// <returns>次のステート</returns>
    StateBase* GetNextState() { return nextState; }

    /// <summary>
    /// 現在のステートを渡す
    /// </summary>
    /// <returns>現在のステート</returns>
    const Player::PlayerState GetNowStateTag() const { return nowStateTag; }

#ifdef _DEBUG
    /// <summary>
    /// 当たり判定を描画する(デバッグ用)
    /// </summary>
    virtual void DrawCollision() {}
#endif 

    /// <summary>
    /// 更新処理
    /// </summary>
    virtual void Update(VECTOR& modelDirection, VECTOR& position,const VECTOR targetPosition,VECTOR cameraPosition)abstract;

    /// <summary>
    /// ダメージを受けた際に呼ばれる関数
    /// </summary>
    void OnDamage();

    /// <summary>
    /// ライフが0になった状態に設定する
    /// </summary>
    void SetNoLifeState();

protected:

    ///////  enum  //////

    //アニメーションの再生状態
    enum AnimationPlayState
    {
        BlendStart   = 0,    //前のアニメーションとのブレンドを開始
        BlendEnd     = 1,    //前のアニメーションとのブレンドの終了
        FirstLoop    = 2,    //ファーストループの再生中
        FirstLoopEnd = 3,    //ファーストループの再生終了
        Stop         = 4,    //停止中
    };

    ///////  メンバ変数  ////////

    int                 modelhandle;               // ステートで使用するモデルハンドル
    StateBase*          nextState;                 // 次のループでのステートを代入する用の変数
    VECTOR              velocity;                  // 速度やベクトルを含んだ値
    AnimationPlayState  currentPlayAnimationState; // アニメーションの再生状態
    Player::PlayerState nowStateTag;               // 現在のステートの識別用
    Player::LifeState   lifeState;                 // プレイヤーの体力の状態
    float               animationSpeed;            // 派生クラスで代入する
    float               animationLimitTime;        // 再生中のアニメーションの総再生時間
    float               animationNowTime;          // 再生中のアニメーションの現在の経過時間
    int                 animationIndex;            // 現在のアニメーションのインデックス
    int                 beforeAnimationIndex;      // 前回のアニメーションのインデックス
    float               animationBlendRate;        // 前回のアニメーションと現在のアニメーションでのブレンド率



    ///////  ゲッター  ///////

    //アニメーションインデックスを返す
    const int GetAnimationIndex() const { return animationIndex; }


    ///////  メンバ関数  //////

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    virtual void ChangeState()abstract;

    /// <summary>
    /// アニメーションの更新処理
    /// </summary>
    void UpdateAnimation(const float blendSpeed = 0.2f);

    /// <summary>
    /// アニメーションを止める処理
    /// </summary>
    void StopAnimation();

    /// <summary>
    /// アニメーションを開始させる処理
    /// </summary>
    void StartAnimation();

    /// シーンが切り替わってた際にアニメーションをデタッチする
    /// </summary>
    void DetachAnimation();

    /// <summary>
    /// 移動方向の設定
    /// </summary>
    /// <param name="targetPosition">移動したい目標の座標</param>
    /// <param name="position">自身の座標</param>
    /// <returns>設定した方向</returns>
    VECTOR CalculateTargetDirection(const VECTOR targetPosition, const VECTOR position);

private:

    ///////  メンバ変数  ////////





};
