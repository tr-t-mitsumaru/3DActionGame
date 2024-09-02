#include"DxLib.h"
#include"InputManager.h"
#include"Utility.h"


//nullポインターを入れる
InputManager* InputManager::inputManager = nullptr;



/// <summary>
/// コンストラクタ
/// </summary>
InputManager::InputManager()
{
    //それぞれのキー名前とDxライブラリ上での識別番号をセットにする
    keyTag.insert(make_pair(Move,       MoveKeyIndex));
    keyTag.insert(make_pair(Space,      PAD_INPUT_10));
    keyTag.insert(make_pair(Left,       PAD_INPUT_LEFT));
    keyTag.insert(make_pair(Right,      PAD_INPUT_RIGHT));
    keyTag.insert(make_pair(Up,         PAD_INPUT_UP));
    keyTag.insert(make_pair(Down,       PAD_INPUT_DOWN));
    keyTag.insert(make_pair(X,          PAD_INPUT_1));
    keyTag.insert(make_pair(Y,          PAD_INPUT_2));
    keyTag.insert(make_pair(A,          PAD_INPUT_3));
    keyTag.insert(make_pair(B,          PAD_INPUT_4));
    keyTag.insert(make_pair(LB,         PAD_INPUT_5));
    keyTag.insert(make_pair(RB,         PAD_INPUT_6));
    keyTag.insert(make_pair(LT,         PAD_INPUT_7));
    keyTag.insert(make_pair(RT,         PAD_INPUT_8));
    keyTag.insert(make_pair(LeftStick,  PAD_INPUT_9));

    //キータグを反転させたものを代入する
    for (int i = 0; i < keyTag.size(); i++)
    {
        reverseKeyTag.insert(make_pair(keyTag.at((KeyKinds)(i)), (KeyKinds)(i)));
    }

    //それぞれのキーの状態をfalseに
    for (int i = 0; i < keyTag.size(); i++)
    {
        keyPushState.insert(make_pair(keyTag.at((KeyKinds)(i)), NotPush));
    }
}

/// <summary>
/// デストラクタ
/// </summary>
InputManager::~InputManager()
{
    //処理なし
}

/// <summary>
/// インスタンスを作成する
/// </summary>
void InputManager::CreateInstance()
{
    //既にインスタンスが確保されているかのチェック
    if (inputManager != nullptr)
    {
        return;
    }

    //シングルトンの作成
    inputManager = new InputManager();
}


/// <summary>
/// インスタンスの削除
/// </summary>
void InputManager::DeleteInstance()
{
    if (inputManager)
    {
        delete inputManager;
    }
    inputManager = nullptr;
}

/// <summary>
/// キーの入力状態を取得
/// </summary>
/// <param name="keyTag.at(keyKinds)">チェックしたい入力キー</param>
/// <returns>キーを離したか</returns>
InputManager::KeyPushState InputManager::GetKeyPushState(const KeyKinds keyKinds)
{
    //キー入力
    auto input = GetJoypadInputState(DX_INPUT_KEY_PAD1);

  

    //キーが離されている状態
     if (keyPushState.at(keyTag.at(keyKinds)) == JustRelease)
    {
        //続けて押されていないのでNotPushに変える
        if (!(input & keyTag.at(keyKinds)))
        {
            keyPushState.at(keyTag.at(keyKinds)) = NotPush;
        }
    }
    // キー離した瞬間を取る.
    if (keyPushState.at(keyTag.at(keyKinds)) == Push)
    {
        //指定のキーが押されていない場合はフラグを切り替える
        if (!(input & keyTag.at(keyKinds)))
        {
            keyPushState.at(keyTag.at(keyKinds)) = JustRelease;
        }
    }
    //キーが最初に押されたタイミングをとる
    else if (keyPushState.at(keyTag.at(keyKinds)) !=  Push && (input & keyTag.at(keyKinds)))
    {
        keyPushState.at(keyTag.at(keyKinds)) = Push;
    }

    return keyPushState.at(keyTag.at(keyKinds));
}

/// <summary>
/// アナログキーの入力状態を見る
/// </summary>
/// <param name="analogKeyState">アナログキーの種類</param>
/// <returns>倒されているか</returns>
bool InputManager::IsInputAnalogKey(const AnalogKeyState analogKeyState)
{
    // アナログキーの縦横の入力値を用意
    int inputX = 0;
    int inputY = 0;

    // 入力されているか
    bool isInput = false;

    // それぞれの入力をチェック
    GetJoypadAnalogInputRight(&inputX, &inputY, DX_INPUT_PAD1);

    // どのアナログキーをチェックしたいのかをみてそれが倒されているかを見る
    switch (analogKeyState)
    {
    case Right:
        if (inputX > AnalogKeyDeadZone)
        {
            isInput = true;
        }
        break;
    case Left:
        if (inputX < -AnalogKeyDeadZone)
        {
            isInput = true;
        }
        break;
    case Up:
        if (inputY < -AnalogKeyDeadZone)
        {
            isInput = true;
        }
        break;
    case Down:
        if (inputY > AnalogKeyDeadZone)
        {
            isInput = true;
        }
        break;
    default:
        break;
    }

    return isInput;
}


