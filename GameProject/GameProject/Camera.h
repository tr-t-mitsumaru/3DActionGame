#pragma once
#include"InputManager.h"

class Camera
{
public:
    //コンストラクタ
    Camera();
    //デストラクタ
    ~Camera();

    ///////  ゲッター  ///////

    const VECTOR GetPosition()const { return position; }

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update(VECTOR playerPosition);

    /// <summary>
    /// カメラ座標を補正する
    /// </summary>
    void FixCameraPosition();

private:
    static constexpr float CameraTargetPlayerHeight = 30.0f;   // プレイヤーからカメラの注視点への高さ
    static constexpr float ToPlayerLength           = 70.0f;   // プレイヤーとの距離
    static constexpr float AngleSpeed               = 0.05f;   // 旋回速度

    VECTOR        position;          // 座標
    VECTOR        targetPosition;    // 注視店の座標
    float         angleVertical;     // カメラの垂直角度
    float         angleHorizontal;   // カメラの水平角度
    InputManager* inputManager;      // インプットマネージャにアクセスするポインタ
    
};
