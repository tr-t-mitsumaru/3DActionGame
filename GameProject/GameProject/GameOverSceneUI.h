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

    ///////       変数       ///////

    int             frameImage;                  // 枠の画像
    int             returnTitleTextImage;        // 「タイトルに戻る」
    int             returnTitleTextSmallImage;   // 「タイトルに戻る」小さく描画する用
    int             continueTextImage;           // 「コンティニュー」
    int             continueTextSmallImage;      // 「コンティニュー」小さく描画する用
    SerectTextState currentSerectTextState;      // 現在どちらのテキストが選択されているか

};

