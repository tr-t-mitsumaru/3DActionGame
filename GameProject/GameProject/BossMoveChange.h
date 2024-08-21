#pragma once
#include"StateBase.h"
#include"EffectData.h"

class EffectManager;

class BossMoveChange :public StateBase
{
public:

    ////////       メンバ関数       ///////

    /// <summary>
    /// コンストラクタ
    /// </summary>
    /// <param name="modelHandle">モデルハンドル</param>
    /// <param name="beforeAnimationIndex">前のステートのアニメーションの種類</param>
    BossMoveChange(int& modelHandle, const int beforeAnimationIndex);

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~BossMoveChange();

    /// <summary>
    /// 更新処理
    /// </summary>
    /// <param name="position">自身のモデルの向き</param>
    /// <param name="position">自身のキャラクターの座標</param>
    /// <param name="targetPosition">敵対しているキャラの座標</param>
    void Update(VECTOR& modelDirection, VECTOR& position, const VECTOR targetPosition, VECTOR cameraPosition)override;

private:

    ///////   定数       ////////

    static constexpr float InitializeAnimationSpeed = 0.6f;                    // 初期のアニメーションスピード
    static constexpr float AnimationBlendSpeed      = 0.03f;                   // アニメーションのブレンドスピード
    static constexpr VECTOR EffectDefaultScale      = { 40.0f,40.0f,40.0f };   // エフェクトの初期サイズ
    static constexpr float EffectPlaySpeed          = 1.0f;                    // エフェクトの再生速度
    static constexpr float EffectPlayAnimationRatio = 0.45f;                   // エフェクトを再生させるアニメーションの再生率

    ///////  メンバ変数  ////////

    EffectManager* effectManager;   // エフェクトの管理クラス
    EffectData     effectData;      // エフェクトの再生に必要なデータをまとめたもの
    bool           isPlaedEffect;   // エフェクトを再生したかどうか

    ///////  メンバ関数  //////

    /// <summary>
    /// ステートの切り替え処理をまとめたもの
    /// </summary>
    void ChangeState()override;

    /// <summary>
    /// エフェクトの再生に必要なデータの更新
    /// </summary>
    /// <param name="characterPosition">キャラクターの座標</param>
    void UpdateEffectData(const VECTOR characterPosition);

    /// <summary>
    /// アニメーションの再生率に合わせてエフェクトを再生する
    /// </summary>
    void PlayEffectByAnimationTime();

};

