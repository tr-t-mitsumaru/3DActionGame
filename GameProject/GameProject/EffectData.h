#pragma once
#include"EffectManager.h"
#include"DxLib.h"

/// <summary>
/// エフェクトの再生に必要なデータ
/// </summary>
struct EffectData
{
    EffectManager::EffectTag effectTag;       // エフェクトの種類
    int                      effectNumber;    // エフェクトの番号
    VECTOR                   position;        // 座標
    VECTOR                   rotationRate;    // 回転率
    VECTOR                   scalingRate;     // サイズ
    float                    playSpeed;       // 再生速度
};
