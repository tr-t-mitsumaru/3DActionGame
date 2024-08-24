#include"DxLib.h"
#include"ImageDataManager.h"
#include"GameOverSceneUI.h"


/// <summary>
/// コンストラクタ
/// </summary>
GameOverSceneUI::GameOverSceneUI()
    :currentSerectTextState(ContinueText)
{
    //　画像管理クラスのインスタンスをもってくる
    ImageDataManager* imageDataManager = ImageDataManager::GetInstance();

    // 画像を代入
    frameImage = imageDataManager->GetImageHandle(ImageDataManager::GameOverFrame);
    returnTitleTextImage = imageDataManager->GetImageHandle(ImageDataManager::GameOverReturnTitleText);
    returnTitleTextSmallImage = imageDataManager->GetImageHandle(ImageDataManager::GameOverReturnTitleTextSmall);
    continueTextImage = imageDataManager->GetImageHandle(ImageDataManager::ContinueText);
    continueTextSmallImage = imageDataManager->GetImageHandle(ImageDataManager::ContinueTextSmall);

}

/// <summary>
/// デストラクタ
/// </summary>
GameOverSceneUI::~GameOverSceneUI()
{

}

/// <summary>
/// 更新処理
/// </summary>
void GameOverSceneUI::Update()
{
}


/// <summary>
/// 描画
/// </summary>
void GameOverSceneUI::Draw()
{
    if (currentSerectTextState == ContinueText)
    {
        DrawGraph(0, 0, returnTitleTextSmallImage,TRUE);
        DrawExtendGraph(0, 0, 0, 0, continueTextImage, TRUE);   
    }
    else
    {
        DrawGraph(0, 0, continueTextSmallImage, TRUE);
        DrawExtendGraph(0, 0, 0, 0, returnTitleTextImage, TRUE);
       
    }
}

/// <summary>
/// 選んでいるテキストの入れ替え
/// </summary>
void GameOverSceneUI::SwitchSerectText()
{
    // 現在のテキストでは無い方に切り替える
    if (currentSerectTextState == ContinueText)
    {
        currentSerectTextState = ReturnTitleText;
    }
    else
    {
        currentSerectTextState = ContinueText;
    }
}
