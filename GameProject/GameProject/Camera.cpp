#include"DxLib.h"
#include"Camera.h"
#include"GameScene.h"


const VECTOR Camera::StartCameraPosition = VGet(0.0f, 100.0f, -150.0f);

const VECTOR Camera::StartTargetPosition = VGet(0.0f, 0.0f, -280.0f);

const VECTOR Camera::SecondCameraPosition = VGet(120.0f, 50.0f, -230.0f);

const VECTOR Camera::SecondTargetPosition = VGet(0.0f, 10.0f, -40.0f);

const VECTOR Camera::ThirdCameraPosition = VGet(40.0f, 30.0f, -180.0f);

const VECTOR Camera::ForceCameraPosition = VGet(0.0f, 80.0f, -100.0f);

const VECTOR Camera::SecondToThirdVelocity = VScale(VNorm(VSub(Camera::ThirdCameraPosition, Camera::SecondCameraPosition)), 0.7f);


/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera()
    :angleHorizontal(0)
    ,angleVertical(0)
    ,position(StartCameraPosition)
    ,targetPosition(StartTargetPosition)
    ,currentStartCameraState(StartView)
    ,shakeOffset(0.0f)
    ,isShaked(false)
    ,shakeOffsetAdjustment(ShakeSpeed)
    ,movingForward(false)
{
    // インプットマネージャーのポインタを取得
    inputManager = InputManager::GetInstance();

    //カメラの初期水平角度は180度
    angleHorizontal = DX_PI_F;

    //垂直角度は0度
    angleVertical = 0.0f;

    //奥行0.1～1000までをカメラの描画範囲とする
    SetCameraNearFar(0.1f, 1000.0f);

    // カメラに位置を反映.
    SetCameraPositionAndTarget_UpVecY(position, targetPosition);

    // 座標を設定
    titlePosition = InitializeTitlePosition;
    titleTargetPosition = InitializeTitleTargetPosition;

}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{
    //処理なし
}

/// <summary>
/// タイトルシーンでの更新処理
/// </summary>
void Camera::UpdateTitleScene()
{
    // 前に進むフラグがたっていれば移動を始める
    if (movingForward)
    {
        titlePosition.z -= MovingForwardSpeed;
        titleTargetPosition.z -= MovingForwardSpeed;
    }

    // 座標を反映させる
    SetCameraPositionAndTarget_UpVecY(titlePosition, titleTargetPosition);
}


/// <summary>
/// 更新処理
/// </summary>
void Camera::UpdateStartScene(const float playerBossDistance,const VECTOR bossPosition,const VECTOR playerPosition)
{
    // フラグがたっていればカメラを揺らす
    ShakeCamera();

    // 最初のカメラの移動
    if (playerBossDistance <= SwitchPositionDistance && currentStartCameraState == StartView)
    {
        // カメラの座標を変更
        position = SecondCameraPosition;

        // 注視点の変更
        targetPosition = SecondTargetPosition;

        // 状態の切り替え
        currentStartCameraState = SecondView;
    }
    // ２つ目のカメラの移動
    else if (currentStartCameraState == SecondView)
    {
        float cameraDistanceToTarget = VSize(VSub(ThirdCameraPosition, position));

        position = VAdd(position, SecondToThirdVelocity);

        if (cameraDistanceToTarget <= 1.0f)
        {
            position = ThirdCameraPosition;

            currentStartCameraState = FoucusOnBoss;
        }
    }
    // ボスに注目するカメラの移動
    else if (currentStartCameraState == FoucusOnBoss)
    {
        // カメラ座標の切り替え
        position = ForceCameraPosition;

        // 注視点をボスにする
        targetPosition = VAdd(bossPosition,VGet(0.0f,OffsetBossPositionY,0.0f));
    }
    // プレイヤーに注目するカメラの移動
    else if (currentStartCameraState == FoucusOnPlayer)
    {
        // 目標の座標を計算
        VECTOR movePosition = VAdd(playerPosition, VGet(0.0f, CameraTargetPlayerHeight, -ToPlayerLength - shakeOffset));

        // 目標の座標との現在の距離との距離を計算
        float cameraDistanceToTarget = VSize(VSub(movePosition, position));

        // ４番目の座標からターゲットに移動するベクトルを計算
        VECTOR velocityToTaget = VSub(movePosition, ForceCameraPosition);

        // ベクトルからベロシティを計算
        velocityToTaget = VScale(VNorm(velocityToTaget), CameraMoveSpeed);

        // カメラの座標を移動させる
        position = VAdd(position, velocityToTaget);

        // 1.0＋ずらした量より小さくなったら移動をやめる
        if (cameraDistanceToTarget <= 1.0f + shakeOffset)
        {
            // 目標座標に変更する
            position = movePosition;

            currentStartCameraState = Stop;
        }
    }



    // カメラに位置を反映.
    SetCameraPositionAndTarget_UpVecY(position, targetPosition);
}

/// <summary>
/// カメラのフォーカスをプレイヤーに変更する
/// </summary>
void Camera::ChangeForcusPlayer()
{
    currentStartCameraState = FoucusOnPlayer;
}

/// <summary>
/// プレイヤーに追従するカメラの更新処理
/// </summary>
/// <param name="playerPosition">プレイヤーの座標</param>
void Camera::UpdatePlayerCamera(const VECTOR playerPosition)
{
    // 「←」ボタンが押されていたら水平角度をマイナスする
    if (inputManager->IsInputAnalogKey(InputManager::AnalogLeft))
    {
        angleHorizontal += AngleSpeed;

        // －１８０度以下になったら角度値が大きくなりすぎないように３６０度を足す
        if (angleHorizontal > DX_PI_F)
        {
            angleHorizontal -= DX_TWO_PI_F;
        }
    }

    // 「→」ボタンが押されていたら水平角度をプラスする
    if (inputManager->IsInputAnalogKey(InputManager::AnalogRight))
    {
        angleHorizontal -= AngleSpeed;

        // １８０度以上になったら角度値が大きくなりすぎないように３６０度を引く
        if (angleHorizontal < -DX_PI_F)
        {
            angleHorizontal += DX_TWO_PI_F;
        }
    }

    // 「↑」ボタンが押されていたら垂直角度をマイナスする
    if (inputManager->IsInputAnalogKey(InputManager::AnalogUp))
    {
        angleVertical += AngleSpeed;

        //// ある一定角度以下にはならないようにする
        if (angleVertical > DX_PI_F * 0.5f - 0.6f)
        {
            angleVertical = DX_PI_F * 0.5f - 0.6f;
        }
    }

    // 「↓」ボタンが押されていたら垂直角度をプラスする
    if (inputManager->IsInputAnalogKey(InputManager::AnalogDown))
    {
        angleVertical -= AngleSpeed;

        //// ある一定角度以上にはならないようにする
        if (angleVertical < -DX_PI_F * 0.5f + 0.6f)
        {
            angleVertical = -DX_PI_F * 0.5f + 0.6f;
        }
    }
    // カメラの注視点はプレイヤー座標から規定値分高い座標
    targetPosition = VAdd(playerPosition, VGet(0.0f, CameraTargetPlayerHeight, 0.0f));

    // カメラの座標を補正する
    FixCameraPosition();

    // Ｘ軸にカメラとプレイヤーとの距離分だけ伸びたベクトルを
    // 垂直方向回転( Ｚ軸回転 )させたあと水平方向回転( Ｙ軸回転 )して更に
    // 注視点の座標を足したものがカメラの座標
    SetCameraPositionAndTarget_UpVecY(position, targetPosition);
}


/// <summary>
/// カメラの座標補正する
/// </summary>
void Camera::FixCameraPosition()
{
    // 水平方向の回転はＹ軸回転
    auto rotationY = MGetRotY(angleHorizontal);

    // 垂直方向の回転はＺ軸回転 )
    auto rotationX = MGetRotX(angleVertical);

    position = VAdd(VTransform(VTransform(VGet(0.0f, 0.0f,ToPlayerLength + shakeOffset), rotationX), rotationY), targetPosition);
}

void Camera::ShakeCamera()
{
    // 画面を揺らすフラグが立っていたら値を動かす
    if (isShaked)
    {
        // 加算する値の符号を判定させてゆらす向きを変える
        if (shakeOffset <  0.0f || shakeOffset > ShakeOffsetLimit)
        {
            shakeOffsetAdjustment = shakeOffsetAdjustment * -1;
        }
        // 揺らす量を増やす
        shakeOffset += shakeOffsetAdjustment;
    }
    // 画面を揺らさないように値を0にする
    else
    {
        shakeOffset = 0.0f;
        shakeOffsetAdjustment = ShakeSpeed;
    }


}

/// <summary>
/// カメラの揺らしを開始する
/// </summary>
void Camera::StartCameraShake()
{
    isShaked = true;
}

/// <summary>
/// カメラの揺らしを停止する
/// </summary>
void Camera::StopCameraShake()
{
    isShaked = false;
 }

/// <summary>
/// 前に動き始める
/// </summary>
void Camera::StartMovingForward()
{
    // フラグを切り替える
    movingForward = true;
}
