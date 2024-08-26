#pragma once
#include"EffectData.h"

class ModelDataManager;
class EffectManager;

class Stage
{
public:
    //コンストラクタ
    Stage();
    //デストラクタ
    ~Stage();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

    /// <summary>
    /// エフェクトの再生を開始させる
    /// </summary>
    void StartEffectPaly();

private:
    //定数
    static const VECTOR InitialPosition;          //初期座標
    static constexpr VECTOR SafeZoneEffectScale            = { 9.5f,9.5f,9.5f };    // エフェクトのサイズ
    static constexpr int    StartEffectPlayCount           = 1000;                    // エフェクトの再生を開始させるカウント
    static constexpr int    StageModelRotationRate         = 180;


    ///////        変数        ///////

    int            modelHandle;             //モデルハンドル
    EffectManager* effectManager;           // エフェクト管理クラスのポインタ
    EffectData     safeZoneEffectData;      // 移動可能な場所表すエフェクトのデータ
    bool           startedEffectPlay;       // 既にエフェクトの再生が開始されているか
    int            effectPlayCount;         // エフェクトがどれだけ再生されているかのカウント

    ///////       関数       ////////

    /// <summary>
    /// 移動可能な場所を表すエフェクトの初期化
    /// </summary>
    void InitializeSafeZoneEffectData();

};


