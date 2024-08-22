#pragma once
#include"DxLib.h"
#include"CollisionManager.h"

/// <summary>
/// 弾の初期化用のデータ構造
/// </summary>
struct InitializeShotData
{
    VECTOR                         position;        // 座標
    VECTOR                         direction;       // 移動する向き
    float                          radius;          // 弾の半径
    float                          speed;           // 移動スピード
    int                            damageAmount;    // 与えるダメージ
    CollisionManager::HitObjectTag shooterTag;      // ショットを撃ったキャラの種類
};
