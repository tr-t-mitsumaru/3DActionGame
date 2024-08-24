#pragma once



class GameClearSceneUI
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    GameClearSceneUI();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~GameClearSceneUI();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

private:

    /// <summary>
    /// UIの状態
    /// </summary>
    enum UIState
    {
        OpeningScroll   = 0,
        NowLogoUpdate   = 1,
        EndedLogoUpdate = 2,
    };


    ////////        定数       ///////
    static constexpr int   ScrollAnimationCountLimit = 125;       // 巻物のアニメーションカウントの上限
    static constexpr int   LogoPositionX             = 650;       // ロゴのX座標
    static constexpr int   LogoPositionY             = 300;       // ロゴのY座標
    static constexpr int   LogoBlendSpeed            = 1;         // ロゴのブレンドスピード
    static constexpr int   BlendMax                  = 255;       // ブレンド率の最大
    static constexpr int   ReturnTitleTextPositionX  = 680;       // タイトルに戻る説明テキストのX座標
    static constexpr int   ReturnTitleTextPositionY  = 750;       // タイトルに戻る説明テキストのY座標  
    static constexpr int   DisplayTime               = 25;        // 表示時間
    static constexpr int   InbisibleTime             = 50;        // 非表示時間



    ////////       変数       ///////

    int*    scrollImage;                   // 巻物の画像ハンドル
    int     backGroundImage;               // 背景画像
    int     logoImage;                     // ロゴのイメージ
    int     returnTitleTextImage;          // タイトルへ戻る説明のテキスト画像
    int     scrollAnimationCount;          // 巻物のアニメーションのカウント
    UIState currentUIState;                // 現在のUIの状態
    int     logoBlendRate;                 // ロゴのブレンド率
    int     blinkingCount;                 // 点滅表示
    bool    display;                       // 描画しているか


    ///////       関数       ///////

    /// <summary>
    /// ロゴの大きさなどの更新処理
    /// </summary>
    void UpdateLogo();

    /// <summary>
    /// 巻物の更新を行う
    /// </summary>
    void UpdateScroll();

    /// <summary>
    /// 点滅表示
    /// </summary>
    void Blinking();

};

