#pragma once

using namespace std;

class Shot;

/// <summary>
/// ショットのオブジェクトプール
/// </summary>
class ShotObjectPool
{
public:

    /// <summary>
    /// コンストラクタ
    /// </summary>
    ShotObjectPool();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~ShotObjectPool();

    ///////  関数  ///////

    /// <summary>
    /// 未使用のインスタンスが残っているかの確認
    /// </summary>
    /// <returns> 未使用のものがあるか </returns>
    bool HasInactiveShotInstance();

    /// <summary>
    /// 未使用の弾のインスタンスを返す
    /// </summary>
    /// <returns>未使用の弾のインスタンス　無ければnullptr</returns>
    Shot* GetInactiveShot();

    /// <summary>
    /// 使用中リストから未使用リストにインスタンスを移動する
    /// </summary>
    /// <param name="activeBullet">移動したい使用中のリストのアドレス</param>
    void ReturnActiveShotInstance(list<Shot*>& activeShot);


private:
    ///////  定数  ///////

    static constexpr int ShotInstanceNumber = 100;     //　確保する弾のインスタンス数


    //////// 変数  ///////

    list<Shot*>  inactiveShot;    //未使用の弾


};
