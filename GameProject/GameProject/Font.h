#pragma once


class Font
{
public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    Font();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~Font();

    ///////        gettter・setter       ///////

    /// <summary>
    /// ボスの名前を表示するフォントのハンドルを返す
    /// </summary>
    /// <returns>フォントハンドル</returns>
    const int GetBossNameHandle()const { return bossNameHandle; }

private:

    ////////        変数        /////////

    int bossNameHandle;     // ボスの名前の表示を行うフォントハンドル
};

