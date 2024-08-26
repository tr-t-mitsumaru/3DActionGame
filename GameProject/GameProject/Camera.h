#pragma once
#include"InputManager.h"

class Camera
{
public:


    ///////       enum       ///////

    /// <summary>
    /// 登場シーンでのカメラの状態
    /// </summary>
    enum StartCameraState
    {
        StartView      = 0,     // 最初の位置
        SecondView     = 1,     // ２つ目の位置
        FoucusOnBoss   = 2,     // ボスに注目する状態
        FoucusOnPlayer = 3,     // プレイヤーに注目する状態
        Stop           = 4,     // 視点の移動をしない状態
    };

    //コンストラクタ
    Camera();
    //デストラクタ
    ~Camera();

    ///////  ゲッター  ///////

    const VECTOR GetPosition()const { return position; }

    const StartCameraState GetStartCameraState() const { return currentStartCameraState; }

    /// <summary>
    /// タイトルシーンでの更新処理
    /// </summary>
    void UpdateTitleScene();

    /// <summary>
    /// ゲームが開始した直後の更新処理
    /// </summary>
    void UpdateStartScene(const float playerBossDistance, const VECTOR bossPosition, const VECTOR playerPosition);

    /// <summary>
    /// プレイヤーに追従するカメラの更新処理
    /// </summary>
    /// <param name="playerPosition">プレイヤーの座標</param>
    void UpdatePlayerCamera(const VECTOR playerPosition);

    /// <summary>
    /// カメラ座標を補正する
    /// </summary>
    void FixCameraPosition();

    /// <summary>
    /// カメラのフォーカスをプレイヤーに変更する
    /// </summary>
    void ChangeForcusPlayer();

    /// <summary>
    /// カメラの揺らしを開始する
    /// </summary>
    void StartCameraShake();

    /// <summary>
    /// カメラの揺らしを停止する
    /// </summary>
    void StopCameraShake();

    /// <summary>
    /// 前に動き始める
    /// </summary>
    void StartMovingForward();
private:


    static constexpr float   CameraTargetPlayerHeight = 3.0f;    // プレイヤーからカメラの注視点への高さ
    static constexpr float   ToPlayerLength           = 7.0f;    // プレイヤーとの距離
    static constexpr float   AngleSpeed               = 0.006f;  // 旋回速度
    static constexpr float   SwitchPositionDistance   = 20.0f;   // カメラの座標を切り替える際に使用する距離
    static const     VECTOR  StartCameraPosition;                // ゲーム開始時のカメラの座標
    static const     VECTOR  StartTargetPosition;                // ゲーム開始時のカメラの注視点
    static const     VECTOR  SecondCameraPosition;               // ゲーム開始時の二つ目のカメラの座標
    static const     VECTOR  SecondTargetPosition;               // ゲーム開始時の二つ目のカメラの座標
    static const     VECTOR  ThirdCameraPosition;                // 三番目のカメラの座標
    static const     VECTOR  SecondToThirdVelocity;              // ２番目の座標から三番目の座標に向かうベロシティ
    static const     VECTOR  ForceCameraPosition;                // ４番目のカメラ座標
    static constexpr float   CameraMoveSpeed          = 0.05f;   // カメラの移動スピード
    static constexpr float   OffsetBossPositionY      = 5.0f;    // ボスのY座標をどれだけずらした場所を注視点にするか
    static constexpr float   ShakeOffsetLimit         = 0.4f;    // 画面を揺らす量の上限
    static constexpr float   ShakeSpeed               = 0.1f;    // 画面を揺らすスピード

    // タイトルシーン
    static constexpr VECTOR  InitializeTitlePosition       = { 0,5.4f,11.0f };    // タイトルシーンでの座標
    static constexpr VECTOR  InitializeTitleTargetPosition = { 0.0f,5.4f,0.0f };  // タイトルシーンでの注視点
    static constexpr float   MovingForwardSpeed            = 0.08f;               // カメラが前に進むスピード


    VECTOR           position;                // 座標
    VECTOR           targetPosition;          // 注視店の座標
    float            angleVertical;           // カメラの垂直角度
    float            angleHorizontal;         // カメラの水平角度
    InputManager*    inputManager;            // インプットマネージャにアクセスするポインタ
    StartCameraState currentStartCameraState; // 登場シーンでのカメラの状態
    float            shakeOffset;             // カメラを揺らしたときのずらした値の保存
    float            shakeOffsetAdjustment;   // 画面のずらす量を調整するための値
    bool             isShaked;                // 画面を揺らしているか
    bool             movingForward;           // 前に進んでいるか
    VECTOR           titlePosition;           // タイトル画面で使用する座標
    VECTOR           titleTargetPosition;     // タイトル画面でのカメラの注視点

    /// <summary>
    /// カメラを揺らす
    /// </summary>
    void ShakeCamera();
    
};
