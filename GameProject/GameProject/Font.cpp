#include"DxLib.h"
#include "Font.h"


/// <summary>
/// コンストラクタ
/// </summary>
Font::Font()
{
    // フォントのロード
    int count = AddFontResourceEx("Font/Hyakki85.otf", FR_PRIVATE, NULL);


    LPCSTR bossNameFontName = "百鬼夜行 R";

    // フォントハンドルの設定
    bossNameHandle = CreateFontToHandle(bossNameFontName, 40, 7, DX_FONTTYPE_NORMAL);
}

/// <summary>
/// デストラクタ
/// </summary>
Font::~Font()
{
    // 処理なし
}


