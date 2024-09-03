#include<cmath>
#include"DxLib.h"
#include"Utility.h"


/// <summary>
/// ビット同士の足し算
/// </summary>
/// <param name="bitAddend">足される側のビット</param>
/// <param name="bitAdder">足す側のビット</param>
/// <returns>ビットの足し算の結果</returns>
int Utility::AddBit(int bitAddend, int bitAdder)
{
    while (bitAdder != 0)
    {
        // キャリーを計算
        int carry = bitAddend & bitAdder;

        // XOR演算でキャリーなしの加算を行う
        bitAddend = bitAddend ^ bitAdder;

        // キャリーを左にシフトして次の桁に移動
        bitAdder = carry << 1;
    }
    return bitAddend;
}

/// <summary>
/// 任意の角度をラジアンに変換
/// </summary>
/// <param name="angle">任意の角度</param>
/// <returns>ラジアン</returns>
float Utility::ConvertRadian(const int angle)
{
    //計算した値を返す
    return DX_PI_F / 180 * angle;
}

/// <summary>
/// 二つの座標同士の向きから角度を算出する
/// </summary>
float Utility::CalculateAngleBetweenPositions(const VECTOR firstPosition, const VECTOR secondPosition)
{
    // ボスと弾のベクトルを出す
    VECTOR direction = VSub(firstPosition, secondPosition);

    // ボスと弾のベクトルからエフェクトの回転率を出す
    float angle = atan2(direction.x, direction.z);

    return angle;
}

