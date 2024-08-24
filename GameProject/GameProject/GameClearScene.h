#pragma once
#include"SceneBase.h"

class GameClearSceneUI;

/// <summary>
/// クリアシーン
/// </summary>
class GameClearScene:public SceneBase
{
public:
    //コンストラクタ
    GameClearScene();
    //デストラクタ
    ~GameClearScene();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update()override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw()override;
private:

    ///////       変数        ////////

    GameClearSceneUI* gameClearSceneUI;     // UI

};

