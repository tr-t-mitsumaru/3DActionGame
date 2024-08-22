#pragma once
#include"SceneBase.h"

class Stage;
class Camera;
class Player;
class Boss;
class CollisionManager;
class ShotManager;


/// <summary>
/// ゲーム中のシーン
/// </summary>
class GameScene:public SceneBase
{
public:
    
    //コンストラクタ
    GameScene();
    //デストラクタ
    ~GameScene();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update()override;

    /// <summary>
    /// 描画
    /// </summary>
    void Draw() override;

private:
    Stage*  stage;  //ステージ
    Camera* camera; //カメラ
    Player* player; //プレイヤー
    Boss*   boss;   //ボス
    CollisionManager* collisionManager; //コリジョンマネージャーのインスタンスにアクセスるポインタ
    ShotManager*      shotManager;      // ショットマネージャーにアクセスするポインタ
    
};

