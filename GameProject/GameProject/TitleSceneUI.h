#pragma once
#include"EffectData.h"

class EffectManager;

class TitleSceneUI
{
public:

    /// <summary>
    /// ドアの動きの状態
    /// </summary>
    enum DoorOpenState
    {
        Close = 0,      // しまっている
        Opening = 1,      // 開いている途中
        Opened = 2,      // 開き終わった
    };

    ///////       ゲッター       ///////

    /// <summary>
    /// 現在のドアの状態を返す
    /// </summary>
    /// <returns>ドアの開いている状態</returns>
    const DoorOpenState GetCurrentDoorOpenState()const { return currentDoorOpenState; }

    /// <summary>
    /// コンストラクタ
    /// </summary>
    TitleSceneUI();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~TitleSceneUI();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

    /// <summary>
    /// ドアを開け始める
    /// </summary>
    void StartDoorOpen();

private:



    ///////       定数       ///////

    static constexpr VECTOR DoorModelScale            = { 0.03f,0.03f,0.03f };    // ドアのモデルのサイズ
    static constexpr VECTOR TitleStageScale           = { 0.03f,0.03f,0.03f };    // ステージのサイズ
    static constexpr VECTOR BoardScale                = { 0.17f,0.17f,0.17f };    // 看板のスケール
    static constexpr VECTOR BoardPosition             = { -0.2f,8.6f,1.0f };      // 看板の座標
    static constexpr VECTOR RightFireEffectPosition   = { -5.9f,5.9f,1.8f };      // 炎のエフェクトの最初の座標
    static constexpr VECTOR LeftFireEffectPosition    = { 5.9f,5.9f,1.8f };       // 炎のエフェクトの最初の座標
    static constexpr VECTOR OffsetEffectBaseVector    = { 0.0f,1.0f,0.0f };       // エフェクトをずらすための基準となるベクトル
    static constexpr VECTOR StartStringPosition       = { 0.0f,2.f,1.0f };        // 始め方の説明文字を描画する座標
    static constexpr VECTOR DoorPosition              = { -2.2f, 0, 0 };          // ドアを描画する座標
    static constexpr int    CreateEffectCount         = 4;                        // 再生させるエフェクトの数
    static constexpr int    PlayEffectCountLimit    = 200;                        // エフェクトを作るカウントの上限
    static constexpr int    StopEffectCountLimit    = 220;                        // エフェクトを消すカウントの上限
    static constexpr float  Harf                    = 0.5f;                       // 値を半分にするための数字
    static constexpr int    DoorOpenSpeed           = 1;                          // ドアを空けるスピード
    static constexpr int    DoorRotationLimit       = 140;                        // ドアを回転させるリミット
    static constexpr int    DisplayTime             = 25;                         // 表示する時間
    static constexpr int    InbisibleTime           = 50;                         // 表示しない時間
    static constexpr int    EvenNumber              = 2;                          // 偶数と奇数の計算にしようする
    static constexpr float  BillboardSize           = 6.0f;                       // ビルボードとして描画する座標
    static constexpr int    FullCircle              = 360;                        // １週分の定数
    static constexpr float  FireEffectZRange        = 0.7f;                       // 炎のエフェクトがZ軸で動ける範囲
   


    ///////       変数       ///////

    int                doorModelHandle;                 // ドアのモデルハンドル
    int                stageModelHandle;                // ステージのモデルハンドル
    int                boardModelHandle;                // タイトルロゴの看板のハンドル
    int                startStringImageHandle;          // ゲームを始める時の説明の画像
    EffectManager*     effectManager;                   // エフェクトの管理クラス
    vector<EffectData> fireEffectData;                  // エフェクトのデータ
    vector<bool>       isEffectPlaying;                 // エフェクトを再生しているか 
    bool               hasSentEffectData;               // エフェクトデータを送信したかのフラグ
    int                effectVectorRotationRate;        // エフェクトの移動ベクトルの回転率
    int                effectCreateCount;               // エフェクトを作り出すカウント
    bool               hasEffectChanged;                // エフェクトを切り替えたかのフラグ
    float              effectOffsetZPosition;           // エフェクトのZ軸をどれだけずらすか
    float              effectVectorZ;                   // エフェクトのZ軸の移動方向
    int                effectDeleteCount;               // エフェクトを消す際のカウント
    DoorOpenState      currentDoorOpenState;            // 現在のドアの開いている状態
    float              doorRotation;                    // ドアの回転率
    int                blinkingCount;                   // 点滅表示のカウント
    bool               display;                         // 表示しているか

    ///////       関数      ///////

    /// <summary>
    /// エフェクトデータの更新を行う
    /// </summary>
    void UpdateEffectData();

    /// <summary>
    /// 再生しているエフェクトを切り替える
    /// </summary>
    void SwitchEffectPlay();

    /// <summary>
    /// ドアを空ける処理
    /// </summary>
    void OpenDoor();

    /// <summary>
    /// 点滅させるための値の更新
    /// </summary>
    void Blinking();


};

