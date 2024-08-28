#pragma once
#include"FadeInOut.h"
#include"SoundManager.h"
#include"InputManager.h"


class SceneBase abstract
{
public:
    //コンストラクタ
    SceneBase()
    {
        inputManager = InputManager::GetInstance();

        // フェードインアウトのメモリを確保
        fadeInOut = new FadeInOut();
    }

    //デストラクタ
    virtual ~SceneBase()
    {
        inputManager = nullptr;
        delete fadeInOut;
    }

    /// <summary>
    /// 次のシーンベースを返す
    /// </summary>
    /// <returns></returns>
    SceneBase* GetNextScene() { return nextScene; }

    /// <summary>
    /// 更新処理
    /// </summary>
    virtual void Update()abstract;

    /// <summary>
    /// 描画
    /// </summary>
    virtual void Draw()abstract;


protected:
    SceneBase* nextScene;        //次のループでのシーンを入れる変数
    FadeInOut* fadeInOut;        //フェードアウトとフェードイン用のクラス
    SoundManager* soundManager;  //サウンドを鳴らすためのクラス
    InputManager* inputManager;  //入力の処理を行うクラス
};

