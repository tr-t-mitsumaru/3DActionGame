#include"DxLib.h"
#pragma once

class ImageDataManager;
class Font;

/// <summary>
/// ゲーム中のUI
/// </summary>
class GameSceneUI
{
public:

    ////////        enum           ////////

    /// <summary>
    /// ブレンドの状態
    /// </summary>
    enum BlendState
    {
        Transparent,      // 完全に透明な状態
        Opaque,           // 完全に不透明な状態
        FadingIn,         // 透明率を下げている状態（徐々に不透明になる）
        FadingOut         // 透明率を上げている状態（徐々に透明になる）
    };


    /// <summary>
    /// 現在のブレンドの状態を返す
    /// </summary>
    /// <returns>現在のブレンドの状態</returns>
    const BlendState GetCurrentBlendState()const { return currentBlendState; }

    /// <summary>
    /// ゲームオーバー時の処理が終わったかのフラグを返す
    /// </summary>
    /// <returns></returns>
    const bool GetEndedGameOvetUpdate()const { return endedGameOvetUpdate; }

    /// <summary>
    /// コンストラクタ
    /// </summary>
    GameSceneUI();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~GameSceneUI();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update(const int playerHp, const int bossHp);

    /// <summary>
    /// フェードインを開始する
    /// </summary>
    void StartFeadIn();

    /// <summary>
    /// フェードアウトを開始する
    /// </summary>
    void StartFeadOut();

    /// <summary>
    /// ゲームオーバーのテキストを描画を開始する
    /// </summary>
    void StartGameOverTextDraw();

    /// <summary>
    /// チュートリアル画像の描画を開始させる
    /// </summary>
    void StartTutorialImageDraw();

    /// <summary>
    /// チュートリアル画像の描画を終了させる
    /// </summary>
    void EndTutorialImageDraw();

    /// <summary>
    /// 点滅表示
    /// </summary>
    void Blinking();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

private:


    ////////         定数          ////////

    static constexpr VECTOR StartHpGagePosition           = { 600.0f,-100.0f,0.0f };      // HPゲージの最初の座標
    static constexpr VECTOR StartHpFramePosition          = { 550.0f,-100.0f,0.0f };      // HPゲージフレームの最初の座標
    static constexpr float  FeadInMax                     =  255;                         // フェードインの最大値
    static constexpr int    FeadInSpeed                   =  4;                           // フェードインのスピード
    static constexpr int    FeadOutSpeed                  =  4;                           // フェードアウトのスピード
    static constexpr float  LerpSpeed                     = 0.01f;                        // Hpバーを減らす線形補間の速さ
    static constexpr int    PlayerHpGageXPosition         = 700;                          // プレイヤーのHPバーのX座標
    static constexpr int    PlayerHpGageYPosition         = 90;                           // プレイヤーのHPバーのY座標
    static constexpr int    BossHpGageXPosition           = 600;                          // ボスのHPバーのX座標
    static constexpr int    BossHpGageYPosition           = 950;                          // ボスのHPバーのY座標
    static constexpr int    BossNameStringXPosition       = 800;                          // ボスの名前を表示するX座標
    static constexpr int    BossNameStringYPosition       = 900;                          // ボスの名前を表示するY座標
    static constexpr int    GameOvetTextDisplayCountLimit = 100;                          // ゲームオーバーテキストを描画する時間
    static constexpr int    StartTextX1Position           = 700;                          // 開始説明のテキストのX1座標
    static constexpr int    StartTextY1Position           = 950;                          // 開始説明のテキストのY1座標
    static constexpr int    StartTextX2Position           = 1200;                         // 開始説明のテキストのX2座標
    static constexpr int    StartTextY2Position           = 1050;                         // 開始説明のテキストのY2座標
    static constexpr int    DisplayTime                   = 30;                           // 表示する時間
    static constexpr int    InbisibleTime                 = 60;                           // 表示しない時間





    ////////         変数          ////////

    ImageDataManager* imageDataManager;           // 画像管理クラスにアクセスするポインタ
    Font*             font;                       // フォントをまとめたクラス

    // ハンドル
    int               frameHandle;                   // 画面のフレーム
    int               hpGageHadle;                   // HPゲージ
    int               hpFrameHandle;                 // HPゲージのフレーム
    int               hpBackGageHandle;              // HPゲージの背景
    int               subHpGageHandle;               // サブHPゲージ
    int               bossHpBackHandle;              // ボスのHPゲージの下地
    int               bossHpFrameHandle;             // ボスのHPゲージの枠
    int               bossHpGageHandle;              // ボスのHPゲージ
    int               bossSubHpGageHandle;           // ボスのサブHPゲージ
    int               gameOvetTextImage;             // ゲームオーバーのテキスト
    int               tutorialImage;                 // チュートリアル画像
    int               startTextImage;                // ゲームを開始する時の説明テキストの画像
    int               uiBlendOpacity;                // UIのブレンド率
    int               playerHpGageMaxWidth;          // プレイヤーのHPゲージの幅
    int               playerHpGageMaxHeight;         // プレイヤーのHPゲージの高さ
    int               bossHpGageMaxWidth;            // プレイヤーのHPゲージの幅
    int               bossHpGageMaxHeight;           // プレイヤーのHPゲージの高さ
    int               currentPlayerHpGageWidth;      // 現在のプレイヤーのHPゲージの幅
    int               currentPlayerSubHpGageWidth;   // 現在のプレイヤーのサブHPゲージの幅
    int               currentBossHpGageWidth;        // 現在のボスのHPゲージの幅
    int               currentBossSubHpGageWidth;     // 現在のボスのサブHPゲージの幅
    float             playerDisplayHp;               // 描画するプレイヤーHPの量
    float             bossDisplayHp;                 // 描画するボスのHPの量
    BlendState        currentBlendState;             // 現在のブレンドの状態
    int               gameOverTextBlendRate;         // ゲームオーバーテキストのブレンド率
    BlendState        currentGameOvetTextBlendState; // ゲームオーバーテキストのブレンド状態
    int               gameOvetTextDisplayCount;      // ゲームオーバーテキストを描画するカウント
    bool              endedGameOvetUpdate;           // ゲームオーバー時の更新が終わったか
    bool              drawTutorialImage;             // チュートリアル画像を描画しているか
    int               blinkingCount;                 // 点滅表示に使うカウント
    bool              display;                       // 表示しているか


    ///////         関数          ////////

    /// <summary>
    /// ブレンド率の更新処理
    /// </summary>
    void UpdateBlendRate();

    /// <summary>
    /// ボスに関するUIの描画
    /// </summary>
    void DrawBossUI();

    /// <summary>
    /// プレイヤーに関するUIの描画
    /// </summary>
    void DrawPlayerUI();


    /// <summary>
    /// ゲームオーバーテキストのブレンド率を更新
    /// </summary>
    void UpdateGameOvetTextBlendRate();

    /// <summary>
    /// ゲームオーバテキストを描画するカウントの更新
    /// </summary>
    void UpdateGameOverTextDisplayCount();

    /// <summary>
    /// チュートリアルの描画
    /// </summary>
    void DrawTutorial();


};

