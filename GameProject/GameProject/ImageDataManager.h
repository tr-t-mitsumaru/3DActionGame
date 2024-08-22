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
        HpGage            = 0,
        HpFrame           = 1,
        Frame             = 2,
        HpBack            = 3,
        HpGageSub         = 4,
        BossHpGage        = 5,
        BossHpFrame       = 6,
        BossHpBack        = 7,
        BossHpGageSub     = 8,
        BossSecondTexture = 9,
        BossThirdTexture  = 10,
    };

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


private:

    //コンストラクタ
    ImageDataManager();
    //デストラクタ
    ~ImageDataManager();

    //自身のポインタ
    static ImageDataManager* imageDataManager;

    //メンバ変数
    map<ImageTag, int> imageHandle;           //イメージハンドルをまとめているマップ配列

};


