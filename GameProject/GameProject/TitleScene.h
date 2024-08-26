#pragma once
#include"SceneBase.h"

class Camera;
class TitleSceneUI;
class EffectManager;
class SoundManager;


/// <summary>
/// タイトル用のシーン
/// </summary>
class TitleScene:public SceneBase
{
public:
    //コンストラクタ
    TitleScene();
    //デストラクタo
    ~TitleScene();
    /// <summary>
    /// 更新処理
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;
private:

    ///////       変数       ///////

    Camera*        camera;            // カメラ
    TitleSceneUI*  titleSceneUI;      // UI
    EffectManager* effectManager;     // エフェクト管理クラス
    SoundManager* soundManager;      // 音管理クラスのポインタ
    bool           pushedStartButton; // ゲーム開始のボタンが既に押されたか
};
