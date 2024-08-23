#include"DxLib.h"
#include"ImageDataManager.h"

ImageDataManager* ImageDataManager::imageDataManager = nullptr;


/// <summary>
/// コンストラクタ
/// </summary>
ImageDataManager::ImageDataManager()
{
    // 画像のロード
    imageHandle.insert(make_pair(HpGage, LoadGraph("UI/HpGage.png")));
    imageHandle.insert(make_pair(HpFrame, LoadGraph("UI/HPFrame.png")));
    imageHandle.insert(make_pair(Frame, LoadGraph("UI/Frame.png")));
    imageHandle.insert(make_pair(HpBack, LoadGraph("UI/HpBack.png")));
    imageHandle.insert(make_pair(HpGageSub, LoadGraph("UI/HpGageSub.png")));
    imageHandle.insert(make_pair(BossHpGage, LoadGraph("UI/BossHpGage.png")));
    imageHandle.insert(make_pair(BossHpFrame, LoadGraph("UI/BossHpFrame.png")));
    imageHandle.insert(make_pair(BossHpBack, LoadGraph("UI/BossHpBack.png")));
    imageHandle.insert(make_pair(BossHpGageSub, LoadGraph("UI/BossHpGageSub.png")));
    imageHandle.insert(make_pair(BossSecondTexture, LoadGraph("Model/GolemBlackLight.jpg")));
    imageHandle.insert(make_pair(BossThirdTexture, LoadGraph("Model/bossThirdTexture.jpg")));
    imageHandle.insert(make_pair(TitleStartString, LoadGraph("UI/startString.png")));



}

/// <summary>
/// デストラクタ
/// </summary>
ImageDataManager::~ImageDataManager()
{
    // 画像の初期化
    InitGraph();
}

/// <summary>
/// インスタスの作成
/// </summary>
void ImageDataManager::CreateInstance()
{
    //インスタンスが作られていなければ作成
    if (imageDataManager == nullptr)
    {
        imageDataManager = new ImageDataManager();
    }
}

/// <summary>
/// インスタンスの削除
/// </summary>
void ImageDataManager::DeleteInstance()
{
    if (imageDataManager != nullptr)
    {
        delete imageDataManager;
    }
}

/// <summary>
/// 指定されたタグのイメージハンドルを渡す
/// </summary>
/// <param name="imageTag">イメージタグ</param>
/// <returns>イメージハンドル</returns>
const int ImageDataManager::GetImageHandle(ImageTag imageTag)
{
    return imageHandle.at(imageTag);
}


