#pragma once


class FadeInOut
{
    //フェードインアウトの状態
    enum FadeInOutState
    {
        NowFadeIn    = 0,
        EndFadeIn    = 1,
        NowFadeOut   = 2,
        EndFadeOut   = 3
    };
    //コンストラクタ
    FadeInOut();
    //デストラクタ
    ~FadeInOut();

    //getter,setter
    const FadeInOutState GetCurrentFadeInOutState()const { return currentFadeInOutState; }
    /// <summary>
    /// フェードアウトを始めるためにフラグを切り替える関数
    /// </summary>
    void StartFadeOut();
    /// <summary>
    /// フェードインに関する処理
    /// </summary>
    void FadeIn();
    /// <summary>
    /// フェードアウトに関する処理
    /// </summary>
    void FadeOut();
    /// <summary>
    /// フェードインフェードアウトで使う黒い画像の描画
    /// </summary>
    void Draw();

private:
    //定数
    static constexpr int BlendMax     = 255; //透過率の最大値
    static constexpr int FadeInSpeed  = 6;   //フェードインのスピード
    static constexpr int FadeOutSpeed = 10;  //フェードアウトのスピード

    //変数
    int blendCount;                          //画面の透過の値
    int blackImage;                          //フェードインアウトに使用する黒い画像
    FadeInOutState currentFadeInOutState;    //現在のフェードインアウトの状態
};

