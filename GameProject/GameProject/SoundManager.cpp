#include"DxLib.h"
#include"SoundManager.h"

SoundManager* SoundManager::soundManager = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
SoundManager::SoundManager()
{
    // 効果音のロード
    soundEffect.insert(make_pair(OpenDoor, LoadSoundMem("Sound/OpenDoor.mp3")));

}

/// <summary>
/// デストラクタ
/// </summary>
SoundManager::~SoundManager()
{
    // 読み込んだサウンドの削除
    InitSoundMem();
}

/// <summary>
/// インスタンスを作成する
/// </summary>
void SoundManager::CreateInstance()
{
    // 既にインスタンスが確保されているかのチェック
    if (soundManager != nullptr)
    {
        return;
    }

    // インスタンスの作成
    soundManager = new SoundManager();
}

/// <summary>
/// インスタンスの削除
/// </summary>
void SoundManager::DeleteInstance()
{
    // インスタンスが存在しているかのチェック
    if (soundManager)
    {
        // インスタンスの削除
        delete soundManager;
    }
    soundManager = nullptr;
}

/// <summary>
/// サウンドの名前を持ってきて再生する
/// </summary>
/// <param name="soundEffectTag">再生する音の名前</param>
void SoundManager::PlaySoundEffect(SoundEffectTag soundEffectTag,bool overRide)
{
    //
    if (!overRide)
    {
        // 既に再生されていなければ再生する
        if (!CheckSoundMem(soundEffect.at(soundEffectTag)))
        {
            PlaySoundMem(soundEffect.at(soundEffectTag), DX_PLAYTYPE_BACK);
        }
    }
    // 上書きして再生するのでチェックは行わない
    else
    {
        PlaySoundMem(soundEffect.at(soundEffectTag), DX_PLAYTYPE_BACK);

    }

}

/// <summary>
/// BGMの名前を持ってきて再生する
/// </summary>
/// <param name="bgmTag">再生するBGMの名前</param>
void SoundManager::PlayBGM(BGMTag bgmTag)
{

}

/// <summary>
/// 指定のBGMを止める処理
/// </summary>
/// <param name="bgmTag">止めるBGMの名前</param>
void SoundManager::StopBGM(BGMTag bgmTag)
{

}
