#include"DxLib.h"
#include"ModelDataManager.h"
#include"ImageDataManager.h"
#include"EffectManager.h"
#include"Utility.h"
#include"TitleSceneUI.h"


/// <summary>
/// コンストラクタ
/// </summary>
TitleSceneUI::TitleSceneUI()
    :hasSentEffectData(false)
    ,effectVectorRotationRate(0.0f)
    ,hasEffectChanged(false)
    ,effectCreateCount(0)
    ,effectVectorZ(0.003)
    ,effectOffsetZPosition(0.0f)
    ,currentDoorOpenState(Close)
    ,doorRotation(0)
    ,blinkingCount(0)
    ,display(true)
{
    // エフェクト管理クラスのインスタンスをもってくる
    effectManager = EffectManager::GetInstance();

    // モデル管理クラスのインスタンスをもってくる
    ModelDataManager* modelDataManager = ModelDataManager::GetInstance();
    ImageDataManager* imageDataManager = ImageDataManager::GetInstance();

    // モデルハンドルを代入
    doorModelHandle = MV1DuplicateModel(modelDataManager->GetModelHandle(ModelDataManager::Door));
    stageModelHandle = MV1DuplicateModel(modelDataManager->GetModelHandle(ModelDataManager::TitleStage));
    boardModelHandle = MV1DuplicateModel(modelDataManager->GetModelHandle(ModelDataManager::Board));

    // 画像のハンドルの代入
    startStringImageHandle = imageDataManager->GetImageHandle(ImageDataManager::TitleStartString);

    MV1SetScale(doorModelHandle, DoorModelScale);
    MV1SetScale(stageModelHandle, TitleStageScale);
    MV1SetScale(boardModelHandle, BoardScale);

    float angleX = Utility::ConvertRadian(90);
    float angleZ = Utility::ConvertRadian(90);

    MV1SetRotationXYZ(boardModelHandle, VGet(angleX, 0.0f, angleZ));




    // 座標を設定
    MV1SetPosition(doorModelHandle, DoorPosition);
    MV1SetPosition(stageModelHandle, VGet(0, 0, 0));
    MV1SetPosition(boardModelHandle, BoardPosition);


    // エフェクトデータを用意
    for (int i = 0; i < CreateEffectCount; i++)
    {
        // エフェクトデータの作成
        fireEffectData.emplace_back();
        isEffectPlaying.emplace_back(false);
        // 奇数番号と偶数番号は交互に再生するので交互にtrueとfalseにする
        if (!(i % EvenNumber))
        {
            isEffectPlaying[i] = true;
        }
    }

    // エフェクトデータの座標などの更新
    UpdateEffectData();

    // フラグがたっているエフェクトを再生する
    for (int i = 0; i < CreateEffectCount; i++)
    {
        if (isEffectPlaying[i])
        {
            effectManager->PlayEffect(&fireEffectData[i]);
            
        }
    }

}

/// <summary>
/// デストラクタ
/// </summary>
TitleSceneUI::~TitleSceneUI()
{
    // 処理なし
}

/// <summary>
/// 更新処理
/// </summary>
void TitleSceneUI::Update()
{
    // エフェクトの再生と停止を切り替える
    SwitchEffectPlay();

    // エフェクトデータの更新を行う
    UpdateEffectData();

    // ドアを空ける処理
    OpenDoor();

    // 点滅表示の更新
    Blinking();


}


/// <summary>
/// 描画
/// </summary>
void TitleSceneUI::Draw()
{
    MV1DrawModel(doorModelHandle);
    MV1DrawModel(stageModelHandle);
    MV1DrawModel(boardModelHandle);
    if (display)
    {
        DrawBillboard3D(StartStringPosition, Harf, Harf, BillboardSize, 0.0f, startStringImageHandle, TRUE);
    }
}

/// <summary>
/// エフェクトデータの更新を行う
/// </summary>
void TitleSceneUI::UpdateEffectData()
{
    // エフェクトのベクトルを回転させる
    effectVectorRotationRate += 1;

    if (effectVectorRotationRate >= FullCircle)
    {
        effectVectorRotationRate = 0;
    }

    for (int i = 0; i < fireEffectData.size(); i++)
    {
        // 角度をラジアンに変更
        float radian = Utility::ConvertRadian(effectVectorRotationRate);

        // 回転用の行列を計算
        MATRIX rotationZ = MGetRotZ(radian);

        // エフェクトの移動ベクトルを行列を使って回転させる
        VECTOR raotationVector = VTransform(OffsetEffectBaseVector, rotationZ);
   
        // 計算したベクトルから座標を出す
        if (i < 2)
        {
            fireEffectData[i].position = VAdd(RightFireEffectPosition, raotationVector);
        }
        else
        {
            fireEffectData[i].position = VAdd(LeftFireEffectPosition, raotationVector);
        }

        effectOffsetZPosition += effectVectorZ;

        fireEffectData[i].position.z += effectOffsetZPosition;

        if (effectOffsetZPosition >= FireEffectZRange|| effectOffsetZPosition <= -FireEffectZRange)
        {
            effectVectorZ = effectVectorZ * -1;
        }

        // 使うエフェクトの種類
        fireEffectData[i].effectTag = EffectManager::BossShot;

        // エフェクトの再生スピード
        fireEffectData[i].playSpeed = 1.0f;

        //エフェクトの回転率
        fireEffectData[i].rotationRate = VGet(0, 0, 0);

        // エフェクトのサイズ
        fireEffectData[i].scalingRate = VGet(1.0f, 1.0f, 1.0f);
    }


}





/// <summary>
/// 再生しているエフェクトを切り替える
/// </summary>
void TitleSceneUI::SwitchEffectPlay()
{
    effectCreateCount++;
    effectDeleteCount++;
    for (int i = 0; i < CreateEffectCount * Harf; i++)
    {
        int evenNumber = 0 + i * CreateEffectCount * Harf;     // 偶数の添え字
        int oddNumber  = 1 + i * CreateEffectCount * Harf;     // 奇数の添え字

        // 奇数番号か偶数番号のどちらか再生されていない方を再生させる
        if (effectCreateCount >= PlayEffectCountLimit && !isEffectPlaying[oddNumber])
        {
            // エフェクトを再生させる
            effectManager->PlayEffect(&fireEffectData[oddNumber]);

            // 最後のループの時にカウントを0にする
            if (i == CreateEffectCount * Harf -1)
            {
                effectCreateCount = 0;
            }
        }
        else if (effectCreateCount >= PlayEffectCountLimit && !isEffectPlaying[evenNumber])
        {
            
            effectManager->PlayEffect(&fireEffectData[evenNumber]);

            // 最後のループの時にカウントを0にする
            if (i == CreateEffectCount * Harf - 1)
            {
                effectCreateCount = 0;
            }
        }

        // 奇数番号か偶数番号のどちらか再生されている方を停止させる
        if (effectDeleteCount >= StopEffectCountLimit && isEffectPlaying[evenNumber])
        {

            // それぞれ再生と停止をさせたのでフラグを切り替える
            isEffectPlaying[oddNumber] = true;
            isEffectPlaying[evenNumber] = false;

            // エフェクトを停止させる
            effectManager->StopEffect(fireEffectData[evenNumber]);

            // 最後のループの時に再生中のカウントに合わせる
            if (i == CreateEffectCount * Harf - 1)
            {
                effectDeleteCount = effectCreateCount;
            }
        }
        else if (effectDeleteCount >= StopEffectCountLimit && isEffectPlaying[oddNumber])
        {

            // それぞれ再生と停止をさせたのでフラグを切り替える
            isEffectPlaying[evenNumber] = true;
            isEffectPlaying[oddNumber] = false;

            // エフェクトを停止させる
            effectManager->StopEffect(fireEffectData[oddNumber]);

            // 最後のループの時に再生中のカウントに合わせる
            if (i == CreateEffectCount * Harf - 1)
            {
                effectDeleteCount = effectCreateCount;
            }
        }
    }
}

/// <summary>
/// ドアを開け始める
/// </summary>
void TitleSceneUI::StartDoorOpen()
{
    // ステートを切り替える
    currentDoorOpenState = Opening;
}

/// <summary>
/// ドアを空ける処理
/// </summary>
void TitleSceneUI::OpenDoor()
{
    // ドアを開けている状態なら
    if (currentDoorOpenState == Opening)
    {
        // ドアの回転量を上げていく
        doorRotation += DoorOpenSpeed;

        // ドアが開ききったら
        if (doorRotation >= DoorRotationLimit)
        {
            doorRotation = DoorRotationLimit;
            currentDoorOpenState = Opened;
        }

        // 角度をラジアンに変更
        float radian = Utility::ConvertRadian(doorRotation);

        // 回転率を反映させる
        MV1SetRotationXYZ(doorModelHandle,VGet(0.0f, radian, 0.0f));
    }
}

/// <summary>
/// 点滅させるための値の更新
/// </summary>
void TitleSceneUI::Blinking()
{
    blinkingCount++;
    //文字を表示する時間のカウント
    if (blinkingCount <= DisplayTime)
    {
        display = true;
    }
    //文字を消す時間のカウント
    else if (blinkingCount > DisplayTime && blinkingCount <= InbisibleTime)
    {
        display = false;
        if (blinkingCount == InbisibleTime)
        {
            blinkingCount = 0;
        }
    }

}
