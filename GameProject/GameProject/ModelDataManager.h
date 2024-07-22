﻿#pragma once
#include<map>

using namespace std;

/// <summary>
/// モデルのハンドルをロードして渡す用のクラス
/// </summary>
class ModelCollection
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
    static ModelCollection* GetInstance() { return modelCollection; }

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
    ModelCollection();
    //デストラクタ
    ~ModelCollection();

    static ModelCollection* modelCollection;     //自身のインスタンスのポインタ

    map<ModelTag, int> modelHandle;              //モデルハンドルをまとめているマップ配列

    
};
