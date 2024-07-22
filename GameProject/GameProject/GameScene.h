﻿#pragma once
#include"SceneBase.h"

class Stage;


/// <summary>
/// ゲーム中のシーン
/// </summary>
class GameScene:public SceneBase
{
public:
    //コンストラクタ
    GameScene();
    //デストラクタ
    ~GameScene();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update()override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

private:
    Stage* stage;   //ステージ
};

