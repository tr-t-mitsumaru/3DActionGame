﻿#include"ImageDataManager.h"


/// <summary>
/// コンストラクタ
/// </summary>
ImageDataManager::ImageDataManager()
{
    
}

/// <summary>
/// デストラクタ
/// </summary>
ImageDataManager::~ImageDataManager()
{

}

/// <summary>
/// インスタスの作成
/// </summary>
void ImageDataManager::CreateInstance()
{

}

/// <summary>
/// インスタンスの削除
/// </summary>
void ImageDataManager::DeleteInstance()
{

}

/// <summary>
/// 指定されたタグのイメージハンドルを渡す
/// </summary>
/// <param name="imageTag">イメージタグ</param>
/// <returns>イメージハンドル</returns>
const int ImageDataManager::GetImageHandle(ImageTag imageTag)
{
    //ToDo
    //現在使用する画像が無いためエラーが起きないように
    //返り値を返しています
    int image = 0;
    return image;
}


/// <summary>
/// 必要な画像をロードする
/// </summary>
void ImageDataManager::LoadImage()
{

}

/// <summary>
/// 読み込んだ画像の削除
/// </summary>
void ImageDataManager::DeleteImage()
{

}
