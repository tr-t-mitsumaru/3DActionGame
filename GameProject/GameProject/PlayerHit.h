#pragma once
#include"StateBase.h"

class SoundManager;

/// <summary>
/// プレイヤーが被弾した際のステート
/// </summary>
class PlayerHit:public StateBase
{
public:

    ///////  コンストラクタ・デストラクタ  ////////

    /// <summary>
    /// コンストラクタ
    /// </summary>
    PlayerHit(int InitalModelHandle, int beforeAnimationIndex, Player::AnimationState animationState);

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~PlayerHit();

    ///////         メンバ関数           //////

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="modelDirection">モデルの向き</param>
    /// <param name="characterPosition">キャラクターの座標</param>
    void Update(VECTOR& modelDirection, VECTOR& position, const VECTOR targetPosition, VECTOR cameraPosition)override;


private:

    ///////           定数             ///////

    static constexpr float AnimationSpeed = 0.5f;   // アニメーションの速度

    ///////           変数            ///////

    SoundManager* soundManager;     // 音管理クラスのポインタ
    bool          playedHitVoice;   // ヒットした時の音を流したか


    ///////         メンバ関数           //////

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;

};

