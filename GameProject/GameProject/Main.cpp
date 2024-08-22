#include"DxLib.h"
#include"Game.h"
#include"InputManager.h"
#include"ModelDataManager.h"
#include"CollisionManager.h"
#include"ShotManager.h"

//----------------------------//
// WinMain関数.
//----------------------------//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpCmdLine, int nCmdShow)
{
    // 画面モードの設定
    SetGraphMode(Game::ScreenWidth, Game::ScreenHeight, 16);        //解像度をSCREEN_W*SCREEN_H、colorを16bitに設定.




    /////////////////////////////////
    // ウインドウモードに(コメントアウトしたらフルスクリーン).
     //ChangeWindowMode(TRUE);
    ////////////////////////////



        // ＤＸライブラリ初期化処理
    if (DxLib_Init() == -1)
    {
        return -1;
    }
    SetDrawScreen(DX_SCREEN_BACK);

    //シングルトンクラスのインスタンス確保
    InputManager::CreateInstance();
    ModelDataManager::CreateInstance();
    CollisionManager::CreateInstance();
    ShotManager::CreateInstance();



    Game* game = new Game();


    // 画面を初期化(真っ黒にする)
    SetCameraNearFar(10.0f, 500.0f);     //nearとfarの設定


    //ゲームループ
    game->Update();



    //シーンマネージャーの解放
    delete game;

    //シングルトンクラスの削除
    InputManager::DeleteInstance();
    ModelDataManager::DeleteInstance();
    CollisionManager::DeleteInstance();
    ShotManager::DeleteInstance();


    DxLib_End();                // ＤＸライブラリ使用の終了処理

    return 0;                   //ソフトの終了
}
