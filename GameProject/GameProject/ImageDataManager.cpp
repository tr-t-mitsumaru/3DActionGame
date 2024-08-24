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
    imageHandle.insert(make_pair(ClearSceneBack, LoadGraph("UI/board.png")));
    imageHandle.insert(make_pair(ClearLogo, LoadGraph("UI/ClearLogo.png")));
    imageHandle.insert(make_pair(ReturnTitleText, LoadGraph("UI/ReturnTitleText.png")));



    // 分割画像のロード
    divisionImageHandle[ClearSceneScroll] = new int[ScrollImageAllDivision];
    LoadDivGraph("UI/ScrollResize2_.png", ScrollImageAllDivision, ScrollImageXDivision, ScrollImageYDivision, ScrollImageWidth, ScrollImageHeight, divisionImageHandle[ClearSceneScroll]);

   


}

/// <summary>
/// デストラクタ
/// </summary>
ImageDataManager::~ImageDataManager()
{
    // 画像の初期化
    InitGraph();

    // divisionImageHandle の解放
    for (auto& pair : divisionImageHandle) {
        delete[] pair.second;
        pair.second = nullptr;
    }
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

/// <summary>
/// 指定された分割画像のハンドルを渡す
/// </summary>
/// <param name="imageTag">imageタグ</param>
/// <returns>指定されたタグのハンドル</returns>
int* ImageDataManager::GetDibisionImageHandle(ImageTag imageTag)
{
    return divisionImageHandle.at(imageTag);
}




