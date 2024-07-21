﻿#pragma once
#include<map>

using namespace std;

/// <summary>
/// モデルのハンドルをロードして渡す用のクラス
/// </summary>
class ModelDataManager
{
public:
    /// <summary>
    /// ロードしたモデルのタグ
    /// </summary>
    enum ModelTag
    {
        Player = 0,
        Boss   = 1,
        Stage  = 2
    };

    /// <summary>
    /// 自身のインスタンスにアクセスするポインタを渡す
    /// </summary>
    /// <returns></returns>
    static ModelDataManager* GetInstance() { return modelDataManager; }

    /// <summary>
    /// インスタンスの作成
    /// </summary>
    static void CreateInstance();

    /// <summary>
    /// インスタンスの削除
    /// </summary>
    static void DeleteInstance();

    /// <summary>
    /// 指定されたタグのイメージハンドルを渡す
    /// </summary>
    /// <param name="imageTag">イメージタグ</param>
    /// <returns>イメージハンドル</returns>
    const int GetModelHandle(ModelTag modelTag);

private:

    //コンストラクタ
    ModelDataManager();
    //デストラクタ
    ~ModelDataManager();

<<<<<<< HEAD:GameProject/GameProject/ModelDataManager.h
    /// <summary>
    /// 必要なモデルをロードする
    /// </summary>
    static void LoadModel();

    /// <summary>
    /// 読み込んだモデルの削除
    /// </summary>
    static void DeleteModel();

    static ModelDataManager* modelDataManager;     //自身のインスタンスのポインタ
=======
    static ModelCollection* modelCollection;     //自身のインスタンスのポインタ
>>>>>>> 5e37c53 (不要な関数の削除):GameProject/GameProject/ModelCollection.h

    map<ModelTag, int> modelHandle;              //モデルハンドルをまとめているマップ配列

    
};
