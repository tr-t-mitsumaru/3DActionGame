#pragma once
#include<map>

using namespace std;

class ImageDataManager
{
public:

    /// <summary>
    /// ロードした画像のタグ
    /// </summary>
    enum ImageTag
    {
        HpGage                       = 0,
        HpFrame                      = 1,
        Frame                        = 2,
        HpBack                       = 3,
        HpGageSub                    = 4,
        BossHpGage                   = 5,
        BossHpFrame                  = 6,
        BossHpBack                   = 7,
        BossHpGageSub                = 8,
        BossSecondTexture            = 9,
        BossThirdTexture             = 10,
        TitleStartString             = 11,
        ClearSceneScroll             = 12,
        ClearSceneBack               = 13,
        ClearLogo                    = 14,
        ReturnTitleText              = 15,
        GameOverText                 = 16,
        GameOverFrame                = 17,
        GameOverReturnTitleText      = 18,
        GameOverReturnTitleTextSmall = 19,
        ContinueText                 = 20,
        ContinueTextSmall            = 21,
        Tutorial                     = 22,
    };

    ///////       定数       ///////

    static constexpr int ScrollImageAllDivision = 25;       // 巻物画像の全分割数
    static constexpr int ScrollImageXDivision   = 5;        // 巻物画像のX方向の分割数
    static constexpr int ScrollImageYDivision   = 5;        // 巻物画像のY方向の分割数
    static constexpr int ScrollImageWidth       = 1650;     // 巻物画像の幅
    static constexpr int ScrollImageHeight      = 1080;     // 巻物画像の高さ

    /// <summary>
    /// 自身のインスタンスにアクセスするポインタを渡す
    /// </summary>
    /// <returns></returns>
    static ImageDataManager* GetInstance() { return imageDataManager; }

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
    const int GetImageHandle(ImageTag imageTag);

    /// <summary>
    /// 指定された分割画像のハンドルを渡す
    /// </summary>
    /// <param name="imageTag">imageタグ</param>
    /// <returns>指定されたタグのハンドル</returns>
    int* GetDibisionImageHandle(ImageTag imageTag);


private:

    //コンストラクタ
    ImageDataManager();
    //デストラクタ
    ~ImageDataManager();

    //自身のポインタ
    static ImageDataManager* imageDataManager;

    //メンバ変数
    map<ImageTag, int> imageHandle;           //イメージハンドルをまとめているマップ配列
    map<ImageTag, int*> divisionImageHandle;  // 分割画像のハンドル

};


