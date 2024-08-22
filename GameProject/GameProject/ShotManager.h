#pragma once
#include<list>

using namespace std;

class ShotObjectPool;
struct InitializeShotData;
class Shot;

/// <summary>
/// ショットの管理クラス
/// </summary>
class ShotManager
{
public:
    //インスタンスを取得するゲッター
    static ShotManager* GetInstance() { return shotManager; }
    //インスタンスを作成する
    static void CreateInstance();

    /// <summary>
    /// インスタンスの削除
    /// </summary>
    static void DeleteInstance();

    /// <summary>
    /// 弾の生成
    /// </summary>
    /// <param name="initializeShotData">弾の初期化用のデータ</param>
    void SpawnShot(InitializeShotData initializeShotData);

    /// <summary>
    /// 弾の削除
    /// </summary>
    void DeleteShot();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update();

    /// <summary>
    /// 描画
    /// </summary>
    void Draw();

private:
    ///////  変数  //////

    static ShotManager* shotManager;    // 自身のポインタ
    ShotObjectPool*     shotObjectPool; // 弾のオブジェクトプール
    list<Shot*>         activeShot;     // 使用されている弾


    ///////  関数  ///////

    /// <summary>
    /// コンストラクタ
    /// </summary>
    ShotManager();

    /// <summary>
    /// デストラクタ
    /// </summary>
    ~ShotManager();


};
