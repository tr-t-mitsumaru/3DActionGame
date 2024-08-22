#include<list>
#include"Shot.h"
#include"ShotObjectPool.h"
#include"InitializeShotData.h"
#include"ShotManager.h"

ShotManager* ShotManager::shotManager = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
ShotManager::ShotManager()
{
    // ショットオブジェクトプールをインスタンス化
    shotObjectPool = new ShotObjectPool();
}

/// <summary>
/// デストラクタ
/// </summary>
ShotManager::~ShotManager()
{
    delete shotObjectPool;
}

/// <summary>
/// インスタンスを作成する
/// </summary>
void ShotManager::CreateInstance()
{
    //既にインスタンスが確保されているかのチェック
    if (shotManager != nullptr)
    {
        return;
    }

    //シングルトンの作成
    shotManager = new ShotManager();
}

/// <summary>
/// ショットの更新
/// </summary>
void ShotManager::Update()
{
    //弾の移動処理
    for (auto it = activeShot.begin(); it != activeShot.end(); ++it)
    {
        (*it)->Update();
    }

    //弾の削除
    DeleteShot();

}

void ShotManager::Draw()
{
    //弾の移動処理
    for (auto it = activeShot.begin(); it != activeShot.end(); ++it)
    {
        (*it)->Draw();
    }
}

/// <summary>
/// インスタンスの削除
/// </summary>
void ShotManager::DeleteInstance()
{
    if (shotManager)
    {
        delete shotManager;
    }
    //シングルトンの作成
    shotManager = nullptr;
}

/// <summary>
/// 弾の生成
/// </summary>
/// <param name="initializeShotData">弾の初期化用のデータ</param>
void ShotManager::SpawnShot(InitializeShotData initializeShotData)
{
    // 未使用の弾をオブジェクトプールから取得
    Shot* shot = shotObjectPool->GetInactiveShot();

    // 未使用の弾があれば追加する
    if (shot != nullptr)
    {
        shot->Initialize(initializeShotData);
        activeShot.emplace_back(shot);
    }

}

/// <summary>
/// 弾の削除
/// </summary>
void ShotManager::DeleteShot()
{
    // 使用しなくなった弾を未使用に移動
    shotObjectPool->ReturnActiveShotInstance(activeShot);
}
