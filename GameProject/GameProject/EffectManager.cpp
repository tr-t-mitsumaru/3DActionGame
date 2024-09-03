#include"DxLib.h"
#include"EffekseerForDXLib.h"
#include"EffectManager.h"
#include"EffectData.h"




EffectManager* EffectManager::effectManager = nullptr;

/// <summary>
/// コンストラクタ
/// </summary>
EffectManager::EffectManager()
{
    // エフェクトのロード
    effectHandle.insert(make_pair(BossShot, LoadEffekseerEffect("Effect/FireBall3.efkefc", 1.0f)));
}

/// <summary>
/// デストラクタ
/// </summary>
EffectManager::~EffectManager()
{
    // 読み込んだエフェクトの削除
    for (int i = 0; i < effectHandle.size(); i++)
    {
        DeleteEffekseerEffect(effectHandle.at((EffectTag)(i)));
    }

}

/// <summary>
/// インスタンスの作成
/// </summary>
void EffectManager::CreateInstance()
{
    // 既にインスタンスが確保されているかのチェック
    if (effectManager != nullptr)
    {
        return;
    }

    // インスタンスの作成
    effectManager = new EffectManager();
}

/// <summary>
/// インスタンスの削除
/// </summary>
void EffectManager::DeleteInstance()
{
    // インスタンスが存在しているかのチェック
    if (effectManager)
    {
        // インスタンスの削除
        delete effectManager;
    }
    effectManager = nullptr;

}

///// <summary>
///// エフェクシアの設定を行う
///// </summary>
int EffectManager::Setting()
{
    //Effekseerをしようする際にDirectX11を使えるようにする必要がある
    SetUseDirect3DVersion(DX_DIRECT3D_11);

    //Effekseerの初期化
    if (Effekseer_Init(8000) == -1)
    {
        return -1;
    }

    // フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
    // Effekseerを使用する場合は必ず設定する。
    SetChangeScreenModeGraphicsSystemResetFlag(FALSE);

    // Zバッファを有効にする。
    // Effekseerを使用する場合、2DゲームでもZバッファを使用する。
    SetUseZBuffer3D(TRUE);
    // Zバッファへの書き込みを有効にする。
    // Effekseerを使用する場合、2DゲームでもZバッファを使用する。
    SetWriteZBuffer3D(TRUE);

    return 0;
}

/// <summary>
/// 指定されたエフェクトを再生する
/// </summary>
/// <param name="effectData">再生するエフェクトの情報が格納されている場所のポインタ</param>
void EffectManager::PlayEffect(EffectData* effectData)
{
    // 再生するエフェクトの番号を作成する
    effectData->effectNumber = PlayEffekseer3DEffect(effectHandle.at(effectData->effectTag));

    // エフェクトデータリストに追加
    effectDataList.emplace_back(effectData);

}

/// <summary>
/// 指定されたエフェクトを停止する
/// </summary>
/// <param name="effectData">止めるエフェクトの情報</param>
void EffectManager::StopEffect(EffectData effectData)
{
    // エフェクトデータのサイズ分回す
    for (int i = 0; i <  effectDataList.size(); i++)
    {
        // 同じエフェクトデータを削除する
        if (effectDataList[i]->effectNumber == effectData.effectNumber)
        {
            StopEffekseer3DEffect(effectDataList[i]->effectNumber);
            effectDataList.erase(effectDataList.begin() + i);
        }
    }
}

/// <summary>
///  エフェクト全体の更新処理
/// </summary>
void EffectManager::Update()
{
    //DXライブラリのカメラとEffekseerのカメラを同期
    Effekseer_Sync3DSetting();

    for (int i = 0; i < effectDataList.size(); i++)
    {
        // 座標の設定
        SetPosPlayingEffekseer3DEffect(effectDataList[i]->effectNumber,
            effectDataList[i]->position.x, effectDataList[i]->position.y, effectDataList[i]->position.z);

        // 大きさの設定
        SetScalePlayingEffekseer3DEffect(effectDataList[i]->effectNumber,
            effectDataList[i]->scalingRate.x, effectDataList[i]->scalingRate.y, effectDataList[i]->scalingRate.z);

        // 回転率の設定
        SetRotationPlayingEffekseer3DEffect(effectDataList[i]->effectNumber,
            effectDataList[i]->rotationRate.x, effectDataList[i]->rotationRate.y, effectDataList[i]->rotationRate.z);

        // 再生速度の設定
        SetSpeedPlayingEffekseer3DEffect(effectDataList[i]->effectNumber, effectDataList[i]->playSpeed);
    }


    // Effekseerで再生中のエフェクトを更新する
    UpdateEffekseer3D();
}


/// <summary>
///  エフェクトの描画
/// </summary>
void EffectManager::Draw()
{
    // 描画
    DrawEffekseer3D();
}
