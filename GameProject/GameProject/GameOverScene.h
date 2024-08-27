﻿#pragma once
#include"SceneBase.h"

class GameOverSceneUI;
class SoundManager;

/// <summary>
/// ゲームオーバーシーン
/// </summary>
class GameOverScene:public SceneBase
{
public:
    //コンストラクタ
    GameOverScene();
    //デストラクタ
    ~GameOverScene();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update()override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw()override;
private:

    GameOverSceneUI* gameOverSceneUI;       // ゲームオーバーシーンUI
    SoundManager*    soundManager;          // 音管理クラスのポインタ
};

