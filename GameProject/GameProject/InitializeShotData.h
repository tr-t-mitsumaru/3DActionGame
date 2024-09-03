#pragma once
#include"DxLib.h"
#include"CollisionManager.h"
#include"EffectManager.h"

/// <summary>
/// 弾の初期化用のデータ構造
/// </summary>
struct InitializeShotData
{
    VECTOR                         position;           // 座標
    VECTOR                         direction;          // 移動する向き
    float                          radius;             // 弾の半径
    float                          speed;              // 移動スピード
    int                            damageAmount;       // 与えるダメージ
    CollisionManager::HitObjectTag shooterTag;         // ショットを撃ったキャラの種類
    EffectManager::EffectTag       effectTag;          // どのエフェクトを表示するかのタグ
    VECTOR                         effectRotationRate; // エフェクトの回転率
    VECTOR                         effectScalingRate;  // エフェクトのサイズ
    float                          effectPlaySpeed;    // エフェクトの再生速度
    
};
