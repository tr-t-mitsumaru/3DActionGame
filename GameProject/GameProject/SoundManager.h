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
        OpenDoor            = 0,   // ドアを開ける音
        BossRoar            = 1,   // ボスの咆哮
        PlayerFootSteps     = 2,   // プレイヤーの足音
        BossStandUp         = 3,   // ボスが立ち上がる音
        PlayerAttackHit     = 4,   // プレイヤーの攻撃が当たった音
        AttackVoice1        = 5,   // プレイヤーの攻撃ボイス１
        AttackVoice2        = 6,   // プレイヤーの攻撃ボイス2
        AttackVoice3        = 7,   // プレイヤーの攻撃ボイス3
        RollingVoice        = 8,   // プレイヤーの回避ボイス
        DamageVoice         = 9,   // プレイヤーのダメージボイス
        DeadVoice           = 10,  // プレイヤーが死んだときのボイス
        BossDefaultAttack   = 11,  // ボスの通常攻撃の音
        BossAreaAttack      = 12,  // ボスの範囲攻撃時の音
        BossWalk            = 13,  // ボスが歩く音
        BossRun             = 14,  // ボスが走る音
        BossShotAttack      = 15,  // ボスの弾を撃つ攻撃の音
        StrongAttackVoice   = 16,  // 強攻撃のボイス
        ShotMagicVoice      = 17,  // 弾を撃つ攻撃の声
        ShotMagicAttack     = 18,  // 魔法攻撃の音
        BossDead            = 19,  // ボスが死んだときの音
        ShotHit             = 20,  // 弾が当たった時の音
        ScrollOpen          = 21,  // クリア時の巻物の音
        PlayerDiffence      = 22,  // プレイヤーのガード中のヒット音
        PlayerFootStepsFist = 23,  // プレイヤーの足音早め
        Cursor              = 24,  // 選択カーソルを切り替えた時の音
        Decision            = 25,  // 決定音
        GameOverSE          = 26,  // ゲームオーバーになったさいの音
    };

    /// <summary>
    /// BGMの種類
    /// </summary>
    enum BGMTag
    {
        Start    = 0,      // ゲームが開始した時の音
        Title    = 1,      // タイトル画面
        Game     = 2,      // ゲーム中
        GameOver = 3,      // ゲームオーバー
        ClearBgm = 4,      // クリア
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
    /// サウンドの名前を持ってきて停止させる
    /// </summary>
    void StopSoundEffect(SoundEffectTag soundEffectTag);

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

    ///////       定数

    static constexpr int OpenDoorVolume            = 200;      // ドアを開ける音のボリューム
    static constexpr int BossRoarVolume            = 200;      // ボスの咆哮のボリューム
    static constexpr int PlayerFootStepsVolume     = 100;      // プレイヤーの足音
    static constexpr int BossStandUpVolume         = 150;      // ボスが立ち上がる音のボリューム
    static constexpr int PlayerAttackHitVolume     = 100;      // プレイヤーの攻撃が当たった音のボリューム
    static constexpr int AttackVoice1Volume        = 100;      // プレイヤーの攻撃ボイス１
    static constexpr int AttackVoice2Volume        = 120;      // プレイヤーの攻撃ボイス2
    static constexpr int AttackVoice3Volume        = 120;      // プレイヤーの攻撃ボイス3
    static constexpr int RollingVoiceVolume        = 160;      // プレイヤーの回避音のボリューム
    static constexpr int DamageVoiceVolume         = 80;      // プレイヤーのダメージを受けた時のボイスボリューム
    static constexpr int DeadVoiceVolume           = 200;      // 死んだときのボイス音
    static constexpr int BossDefaultAttackVolume   = 150;      // ボス通常攻撃の音
    static constexpr int BossAreaAttackVolume      = 150;      // ボス範囲攻撃の音
    static constexpr int BossWalkVolume            = 200;      // ボスの歩き音
    static constexpr int BossRunVolume             = 200;      // ボスの走り音
    static constexpr int BossShotAttackVolume      = 210;      // ボスの射撃攻撃音
    static constexpr int StrongAttackVoiceVolume   = 150;      // 強攻撃の音
    static constexpr int ShotMagicVoiceVolume      = 150;      // ショット攻撃の声
    static constexpr int ShotMagicAttackVolume     = 150;      // ショット攻撃の音
    static constexpr int BossDeadVolume            = 150;      // ボスが死んだときの音
    static constexpr int ShotHitVolume             = 100;      // 弾が当たった時の音
    static constexpr int ScrollOpenVolume          = 150;      // クリア時の巻物を開ける音
    static constexpr int PlayerDiffenceVolume      = 150;      // プレイヤーのガード中のヒット音
    static constexpr int PlayerFootStepsFistVolume = 80;       // プレイヤーの足音
    static constexpr int CursorVolume              = 120;      // プレイヤーの足音
    static constexpr int DecisionVolume            = 120;      // プレイヤーの足音
    static constexpr int GameOverSEVolume          = 100;      // プレイヤーの足音


    // BGMの音量
    static constexpr int StartBGNVolume    = 100;       // ゲームが開始する時
    static constexpr int TitleBGMVolume    = 190;       // タイトルのBGM
    static constexpr int GameBGMVolume     = 90;       // ゲーム中のBGM
    static constexpr int GameOverBGMVolume = 140;       // ゲームオーバー
    static constexpr int ClearBGMVolume    = 140;       // クリア時








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


