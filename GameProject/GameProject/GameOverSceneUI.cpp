#include"DxLib.h"
#include"ImageDataManager.h"
#include"GameOverSceneUI.h"


/// <summary>
/// コンストラクタ
/// </summary>
GameOverSceneUI::GameOverSceneUI()
    :currentSerectTextState(ContinueText)
    ,textScalingRate(1.0f)
    ,currentTextScaleState(Expanding)
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
    // 現在のテキストのサイズの状態
    if (currentTextScaleState == Riset)
    {
        textScalingRate = 1.0f;
        currentTextScaleState = Expanding;
    }
    else if (currentTextScaleState == Expanding)
    {
        textScalingRate += TextScalingSpeed;
        if (textScalingRate >= TextMaxScale)
        {
            currentTextScaleState = Expanded;
        }
    }
}


/// <summary>
/// 描画
/// </summary>
void GameOverSceneUI::Draw()
{
    DrawGraph(0, 0, frameImage, TRUE);

    // コンティニューが選択されている際の描画
    if (currentSerectTextState == ContinueText)
    {
        // タイトルに戻るテキストの描画
        DrawGraph(ReturnTitleTextPositionX, ReturnTitleTextPositionY, returnTitleTextSmallImage,TRUE);

        // コンティニューテキストの描画(選択された際に拡大しながら描画する)
        DrawRotaGraph(ContinueSmallTextPositionX, ContinueSmallTextPositionY, textScalingRate, 0, continueTextImage, TRUE);
    }
    // タイトルに戻るが選択されている際の描画
    else
    {
        // コンティニューテキストの描画
        DrawGraph(ContinueTextPositionX, ContinueTextPositionY, continueTextSmallImage, TRUE);

        // タイトルに戻るテキストの描画(選択された際に拡大しながら描画する)
        DrawRotaGraph(ReturnTitleSmallTextPositionX, ReturnTitleSmallTextPositionY, textScalingRate, 0, returnTitleTextImage, TRUE);
       
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

    currentTextScaleState = Riset;
}
