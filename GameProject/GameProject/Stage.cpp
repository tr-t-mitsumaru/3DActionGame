#include"DxLib.h"
#include"Stage.h"
#include"ModelDataManager.h"
#include"EffectManager.h"

//定数の初期化
const VECTOR Stage::InitialPosition = VGet(0, 0, 0);

/// <summary>
/// コンストラクタ
/// </summary>
Stage::Stage()
    :startedEffectPlay(false)
    ,effectPlayCount(StartEffectPlayCount)
{
    ModelDataManager* modelDataManager = ModelDataManager::GetInstance();
    modelHandle = modelDataManager->GetModelHandle(ModelDataManager::Stage);

    // エフェクト管理クラスのインスタンスをもってくる
    effectManager = EffectManager::GetInstance();

    //ステージの座標の初期化
    MV1SetPosition(modelHandle, InitialPosition);

    //　エフェクトデータの初期化
    InitializeSafeZoneEffectData();
}

/// <summary>
/// デストラクタ
/// </summary>
Stage::~Stage()
{
    //処理なし
}

/// <summary>
/// 更新処理
/// </summary>
void Stage::Update()
{
    effectPlayCount++;
    // エフェクトの再生を開始させる
    if (effectPlayCount >= StartEffectPlayCount && startedEffectPlay)
    {
        effectManager->PlayEffect(&safeZoneEffectData);
        effectPlayCount = 0;
    }
}

/// <summary>
/// 描画
/// </summary>
void Stage::Draw()
{
    MV1DrawModel(modelHandle);
}

/// <summary>
/// 移動可能な場所を表すエフェクトの初期化
/// </summary>
void Stage::InitializeSafeZoneEffectData()
{
    // 座標
    safeZoneEffectData.position = VGet(0, 0, 0);

    // 回転率
    safeZoneEffectData.rotationRate = VGet(0, 0, 0);

    // サイズ
    safeZoneEffectData.scalingRate = SafeZoneEffectScale;

    // 再生速度
    safeZoneEffectData.playSpeed = 1.0f;

    // エフェクトの種類
    safeZoneEffectData.effectTag = EffectManager::SafeArea;

}

/// <summary>
/// エフェクトの再生を開始させる
/// </summary>
void Stage::StartEffectPaly()
{
    startedEffectPlay = true;
}
