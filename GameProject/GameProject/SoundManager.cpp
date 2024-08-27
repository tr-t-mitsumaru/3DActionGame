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
    soundEffect.insert(make_pair(BossRoar, LoadSoundMem("Sound/BossRoar.mp3")));
    soundEffect.insert(make_pair(PlayerFootSteps, LoadSoundMem("Sound/PlayerFootSteps.mp3")));
    soundEffect.insert(make_pair(BossStandUp, LoadSoundMem("Sound/StandUp.mp3")));
    soundEffect.insert(make_pair(PlayerAttackHit, LoadSoundMem("Sound/PlayerAttackHit.mp3")));
    soundEffect.insert(make_pair(AttackVoice1, LoadSoundMem("Sound/AttackVoice1.wav")));
    soundEffect.insert(make_pair(AttackVoice2, LoadSoundMem("Sound/AttackVoice2.wav")));
    soundEffect.insert(make_pair(AttackVoice3, LoadSoundMem("Sound/AttackVoice3.wav")));
    soundEffect.insert(make_pair(RollingVoice, LoadSoundMem("Sound/RollingVoice.wav")));
    soundEffect.insert(make_pair(DamageVoice, LoadSoundMem("Sound/DamageVoice.wav")));
    soundEffect.insert(make_pair(DeadVoice, LoadSoundMem("Sound/DeadVoice.wav")));
    soundEffect.insert(make_pair(BossDefaultAttack, LoadSoundMem("Sound/BossDefaultAttack.mp3")));
    soundEffect.insert(make_pair(BossAreaAttack, LoadSoundMem("Sound/BossAreaAttack.mp3")));
    soundEffect.insert(make_pair(BossWalk, LoadSoundMem("Sound/BossWalk2.wav")));
    soundEffect.insert(make_pair(BossRun, LoadSoundMem("Sound/BossRun2.wav")));
    soundEffect.insert(make_pair(BossShotAttack, LoadSoundMem("Sound/BossShotAttack2.mp3")));
    soundEffect.insert(make_pair(StrongAttackVoice, LoadSoundMem("Sound/StrongAttackVoice.wav")));
    soundEffect.insert(make_pair(ShotMagicVoice, LoadSoundMem("Sound/ShotMagicVoice.wav")));
    soundEffect.insert(make_pair(ShotMagicAttack, LoadSoundMem("Sound/ShotMagic.mp3")));
    soundEffect.insert(make_pair(BossDead, LoadSoundMem("Sound/BossDead2.mp3")));
    soundEffect.insert(make_pair(ShotHit, LoadSoundMem("Sound/ShotHit.mp3")));
    soundEffect.insert(make_pair(ScrollOpen, LoadSoundMem("Sound/ScrollOpen.mp3")));
    soundEffect.insert(make_pair(PlayerDiffence, LoadSoundMem("Sound/PlayerDiffence.mp3")));
    soundEffect.insert(make_pair(PlayerFootStepsFist, LoadSoundMem("Sound/PlayerFootStepsFast.mp3")));
    soundEffect.insert(make_pair(Cursor, LoadSoundMem("Sound/Cursor.wav")));
    soundEffect.insert(make_pair(Decision, LoadSoundMem("Sound/Decision.wav")));
    soundEffect.insert(make_pair(GameOverSE, LoadSoundMem("Sound/GameOver.mp3")));


    // BGMのロード
    bgm.insert(make_pair(Start, LoadSoundMem("Sound/StartBGM.wav")));
    bgm.insert(make_pair(Title, LoadSoundMem("Sound/TitleBgm.wav")));
    bgm.insert(make_pair(Game, LoadSoundMem("Sound/GameBgm.wav")));
    bgm.insert(make_pair(GameOver, LoadSoundMem("Sound/GameOverBgm.mp3")));
    bgm.insert(make_pair(ClearBgm, LoadSoundMem("Sound/ClearBGM.wav")));









    // ボリュームの調整
    ChangeVolumeSoundMem(OpenDoorVolume, soundEffect.at(OpenDoor));
    ChangeVolumeSoundMem(BossRoarVolume, soundEffect.at(BossRoar));
    ChangeVolumeSoundMem(PlayerFootStepsVolume, soundEffect.at(PlayerFootSteps));
    ChangeVolumeSoundMem(BossStandUpVolume, soundEffect.at(BossStandUp));
    ChangeVolumeSoundMem(PlayerAttackHitVolume, soundEffect.at(PlayerAttackHit));
    ChangeVolumeSoundMem(AttackVoice1Volume, soundEffect.at(AttackVoice1));
    ChangeVolumeSoundMem(AttackVoice2Volume, soundEffect.at(AttackVoice2));
    ChangeVolumeSoundMem(AttackVoice3Volume, soundEffect.at(AttackVoice3));
    ChangeVolumeSoundMem(RollingVoiceVolume, soundEffect.at(RollingVoice));
    ChangeVolumeSoundMem(DeadVoiceVolume, soundEffect.at(DeadVoice));
    ChangeVolumeSoundMem(BossDefaultAttackVolume, soundEffect.at(BossDefaultAttack));
    ChangeVolumeSoundMem(BossAreaAttackVolume, soundEffect.at(BossAreaAttack));
    ChangeVolumeSoundMem(BossWalkVolume, soundEffect.at(BossWalk));
    ChangeVolumeSoundMem(BossRunVolume, soundEffect.at(BossRun));
    ChangeVolumeSoundMem(BossShotAttackVolume, soundEffect.at(BossShotAttack));
    ChangeVolumeSoundMem(StrongAttackVoiceVolume, soundEffect.at(StrongAttackVoice));
    ChangeVolumeSoundMem(ShotMagicVoiceVolume, soundEffect.at(ShotMagicVoice));
    ChangeVolumeSoundMem(ShotMagicAttackVolume, soundEffect.at(ShotMagicAttack));
    ChangeVolumeSoundMem(BossDeadVolume, soundEffect.at(BossDead));
    ChangeVolumeSoundMem(ShotHitVolume, soundEffect.at(ShotHit));
    ChangeVolumeSoundMem(ScrollOpenVolume, soundEffect.at(ScrollOpen));
    ChangeVolumeSoundMem(PlayerDiffenceVolume, soundEffect.at(PlayerDiffence));
    ChangeVolumeSoundMem(PlayerFootStepsFistVolume, soundEffect.at(PlayerFootStepsFist));
    ChangeVolumeSoundMem(CursorVolume, soundEffect.at(Cursor));
    ChangeVolumeSoundMem(DecisionVolume, soundEffect.at(Decision));
    ChangeVolumeSoundMem(GameOverSEVolume, soundEffect.at(GameOverSE));


    // BGMの音量
    ChangeVolumeSoundMem(StartBGNVolume, bgm.at(Start));
    ChangeVolumeSoundMem(TitleBGMVolume, bgm.at(Title));
    ChangeVolumeSoundMem(GameBGMVolume, bgm.at(Game));
    ChangeVolumeSoundMem(GameOverBGMVolume, bgm.at(GameOver));
    ChangeVolumeSoundMem(ClearBGMVolume, bgm.at(ClearBgm));


    







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
/// サウンドの名前を持ってきて停止させる
/// </summary>
void SoundManager::StopSoundEffect(SoundEffectTag soundEffectTag)
{
    // 指定のサウンドを停止する
    StopSoundMem(soundEffect.at(soundEffectTag));
}

/// <summary>
/// BGMの名前を持ってきて再生する
/// </summary>
/// <param name="bgmTag">再生するBGMの名前</param>
void SoundManager::PlayBGM(BGMTag bgmTag)
{
    // 既に再生されていなければ再生する
    if (!CheckSoundMem(bgm.at(bgmTag)))
    {
        PlaySoundMem(bgm.at(bgmTag), DX_PLAYTYPE_LOOP);
    }
}

/// <summary>
/// 指定のBGMを止める処理
/// </summary>
/// <param name="bgmTag">止めるBGMの名前</param>
void SoundManager::StopBGM(BGMTag bgmTag)
{
    // 指定のサウンドを停止する
    StopSoundMem(bgm.at(bgmTag));
}
