#pragma once
#include"StateBase.h"
#include"EffectData.h"
#include"CollisionData.h"

class EffectManager;
class CollisionManager;
class ImageDataManager;
class SoundManager;

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
    /// <summary>
    /// テクスチャの状態
    /// </summary>
    enum TextureState
    {
        Easy   = 0,   // 弱い状態のボスのテクスチャ
        Normal = 1,   // 普通くらいの強さのボスのテクスチャ
        Hard   = 2,   // 強い状態のボスのテクスチャ
    };

    ///////   定数       ////////

    static constexpr float InitializeAnimationSpeed               = 0.6f;                    // 初期のアニメーションスピード
    static constexpr float AnimationBlendSpeed                    = 0.03f;                   // アニメーションのブレンドスピード
    static constexpr VECTOR EffectDefaultScale                    = { 4.0f,4.0f,4.0f };      // エフェクトの初期サイズ
    static constexpr float EffectPlaySpeed                        = 1.0f;                    // エフェクトの再生速度
    static constexpr float EffectPlayAnimationRatio               = 0.45f;                   // エフェクトを再生させるアニメーションの再生率
    static constexpr float ChangeTexturePlayAnimationRatio        = 0.45f;                   // テクスチャを変更するアニメーションの再生率
    static constexpr float CollisionRadius                        = 10.0f;                    // 当たり判定の半径サイズ
    static constexpr int   DamageAmount                           = 70;                      // ダメージ量
    static constexpr float InitializeCollisionStartAnimationRatio = 0.45f;                   // 当たり判定を作成するアニメーションの再生率
    static constexpr float CollisionDeleteAnimationRatio          = 0.8f;                    // 当たり判定を消すアニメーションの再生率

    ///////  メンバ変数  ////////

    EffectManager*    effectManager;        // エフェクトの管理クラス
    EffectData        effectData;           // エフェクトの再生に必要なデータをまとめたもの
    CollisionManager* collisionManager;     // 当たり判定の管理クラス
    int               secondModelTexture;   // 2回目のモデルのテクスチャ
    int               thirdModelTexture;    // 3回目のモデルテクスチャ
    CollisionData     collisionData;        // 当たり判定の更新に必要な情報
    bool              isPlaedEffect;        // エフェクトを再生したかどうか
    bool              isChangedTexture;     // テクスチャを変更したか
    SoundManager*     soundManager;         // 音管理クラスのポインタ 


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
    /// 座標などを当たり判定に必要なデータに更新
    /// </summary>
    /// <param name="characterPositon">自身のポジション</param>
    void UpdateCollisionData(const VECTOR characterPositon);

    /// <summary>
    /// アニメーションの再生率に合わせて当たり判定を作成する
    /// </summary>
    void CreateCollisionByAnimationTime();

    /// <summary>
    /// アニメーションの再生率に合わせてエフェクトを再生する
    /// </summary>
    void PlayEffectByAnimationTime();

    /// <summary>
    /// 当たった時の処理
    /// </summary>
    void OnHit(CollisionData collisionData);

    /// <summary>
    /// モデルのテクスチャを変更する
    /// </summary>
    void ChangeModelTexture();

#ifdef _DEBUG
    /// <summary>
    /// 当たり判定を描画する(デバッグ用)
    /// </summary>
    void DrawCollision() override;
#endif 

};

