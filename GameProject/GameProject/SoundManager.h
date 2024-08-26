#pragma once
#include<map>

using namespace std;

class SoundManager
{
public:
    /// <summary>
    /// 効果音の種類
    /// </summary>
    enum SoundEffectTag
    {
        OpenDoor = 0,   // ドアを開ける音
    };

    /// <summary>
    /// BGMの種類
    /// </summary>
    enum BGMTag
    {

    };

    //インスタンスを取得するゲッター
    static SoundManager* GetInstance() { return soundManager; }
    //インスタンスを作成する
    static void CreateInstance();

    /// <summary>
    /// インスタンスの削除
    /// </summary>
    static void DeleteInstance();

    /// <summary>
    /// サウンドの名前を持ってきて再生する
    /// </summary>
    /// <param name="soundEffectTag">再生する音の名前</param>
    /// <param name="overRide">上書きして再生するか</param>
    void PlaySoundEffect(SoundEffectTag soundEffectTag,bool overRide = false);

    /// <summary>
    /// BGMの名前を持ってきて再生する
    /// </summary>
    /// <param name="bgmTag">再生するBGMの名前</param>
    void PlayBGM(BGMTag bgmTag);

    /// <summary>
    /// 指定のBGMを止める処理
    /// </summary>
    /// <param name="bgmTag">止めるBGMの名前</param>
    void StopBGM(BGMTag bgmTag);

private:

    //コンストラクタ
    SoundManager();
    //デストラクタ
    ~SoundManager();

    //自身のポインタ
    static SoundManager* soundManager;

    //サウンドハンドルが格納されているマップコンテナ
    map<SoundEffectTag, int> soundEffect;     //効果音に関するマップ
    map<BGMTag,         int> bgm;             //bgmに関するマップ
};


