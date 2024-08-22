#include<list>
#include"Shot.h"
#include"ShotObjectPool.h"


/// <summary>
/// コンストラクタ
/// </summary>
ShotObjectPool::ShotObjectPool()
{
    // 弾丸のインスタンスを作成
    for (int i = 0; i < ShotInstanceNumber; i++)
    {
        inactiveShot.push_back(new Shot());
    }
}

/// <summary>
/// デストラクタ
/// </summary>
ShotObjectPool::~ShotObjectPool()
{
    //メモリの解放
    for (Shot* shot : inactiveShot)
    {
        delete shot;
    }
}


/// <summary>
/// 未使用のインスタンスが残っているかの確認
/// </summary>
/// <returns> 未使用のものがあるか </returns>
bool ShotObjectPool::HasInactiveShotInstance()
{
    // 未使用があればture
    return inactiveShot.size();
}


/// <summary>
/// 未使用の弾丸のインスタンスを返す
/// </summary>
/// <returns>未使用弾丸があればそのアドレス   無ければnullptr</returns>
Shot* ShotObjectPool::GetInactiveShot()
{
    // プールから未使用が存在するかチェック
    if (HasInactiveShotInstance())
    {
        // 未使用のプールの先頭から要素を取得
        Shot* shot = inactiveShot.front();

        // 移動させるので先頭要素は削除
        inactiveShot.pop_front();

        return shot;
    }
    else
    {
        return nullptr;
    }
}

/// <summary>
/// 使用中リストから未使用リストにインスタンスを移動する
/// </summary>
/// <param name="activeShot">移動したい使用中のリストのアドレス</param>
void ShotObjectPool::ReturnActiveShotInstance(list<Shot*>& activeShot)
{
    // 使用中弾丸を調べる
    for (auto it = activeShot.begin(); it != activeShot.end();)
    {
        Shot* shot = *it;

        // 弾丸が使用中でない場合、未使用リストに移動する
        if (!shot->GetIsActive())
        {
            // 弾丸を未使用リストに移動
            inactiveShot.splice(inactiveShot.end(), activeShot, it++);
        }
        else
        {
            // イテレータを次に進める
            ++it;
        }
    }
}
