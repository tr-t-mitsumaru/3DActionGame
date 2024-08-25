#pragma once


class GameOverSceneUI
{
public:

    ///////       enum       ///////

    /// <summary>
    /// どちらのテキストを選択しているかの状態
    /// </summary>
    enum SerectTextState
    {
        ContinueText    = 0,      // 「コンティニュー」
        ReturnTitleText = 1,      // 「タイトルに戻る」
    };

    /// <summary>
    /// テキストの大きさの状態
    /// </summary>
    enum TextScaleState
    {
        Riset     = 0,
        Expanding = 1,
        Expanded  = 2,
    };

    /// <summary>
    /// 現在のテキストを選択している状態を返す
    /// </summary>
    /// <returns>どのテキストを選択しているか</returns>
    const SerectTextState GetCurrentSerectTextState()const { return currentSerectTextState; }

    /// <summary>
    /// コンストラクタ
    /// </summary>
    GameOverSceneUI();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~GameOverSceneUI();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

    /// <summary>
    /// 選んでいるテキストの入れ替え
    /// </summary>
    void SwitchSerectText();

private:

    ///////       定数       ///////

    static constexpr float TextScalingSpeed         = 0.02f;    // テキストを拡大させるスピード
    static constexpr int   ContinueTextPositionX         = 790;      // 「コンティニュー」のテキストのX座標
    static constexpr int   ContinueTextPositionY         = 400;      // 「コンティニュー」のテキストのY座標
    static constexpr int   ContinueSmallTextPositionX    = 950;      // 「コンティニュー」の小さいテキストのX座標
    static constexpr int   ContinueSmallTextPositionY    = 430;      // 「コンティニュー」の小さいテキストのY座標
    static constexpr int   ReturnTitleTextPositionX      = 790;      // 「タイトルに戻る」のテキストのX座標
    static constexpr int   ReturnTitleTextPositionY      = 610;      // 「タイトルに戻る」のテキストのY座標
    static constexpr int   ReturnTitleSmallTextPositionX = 950;      // 「タイトルに戻る」の小さいテキストのX座標
    static constexpr int   ReturnTitleSmallTextPositionY = 630;      // 「タイトルに戻る」の小さいテキストのX座標
    static constexpr float TextMaxScale                  = 1.3f;     // テキストの最大拡大率


    ///////       変数       ///////

    int             frameImage;                  // 枠の画像
    int             returnTitleTextImage;        // 「タイトルに戻る」
    int             returnTitleTextSmallImage;   // 「タイトルに戻る」小さく描画する用
    int             continueTextImage;           // 「コンティニュー」
    int             continueTextSmallImage;      // 「コンティニュー」小さく描画する用
    float           textScalingRate;             // テキストの大きさ
    SerectTextState currentSerectTextState;      // 現在どちらのテキストが選択されているか
    TextScaleState  currentTextScaleState;       // 現在のテキストの状態
    bool            switchText;                  // テキストを切り替えたかのフラグ

};

