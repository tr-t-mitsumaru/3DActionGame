#pragma once
#include<map>
#include<vector>

using namespace std;
struct EffectData;

class EffectManager
{
public:
    /// <summary>
    /// エフェクトの種類
    /// </summary>
    enum EffectTag
    {
        BossShot          = 0,   // Bossのショットのエフェクト
        BossClaw          = 1,   // ボスの通常攻撃のエフェクト
        PlayerShot        = 2,   // プレイヤーのショット攻撃のエフェクト
        PlayerMagicCircle = 3,   // プレイヤーがショットを撃つ際の魔法陣
        PlayerShotHit     = 4,   // プレイヤーのショットが当たった際のエフェクト
        BossShotHit       = 5,   // ボスのショットが当たった際のエフェクト
        PlayerAttackHit   = 6,   // プレイヤーの攻撃が当たった際のエフェクト
    };


    /// <summary>
    /// 自身のインスタンスにアクセスするポインタを渡す
    /// </summary>
    /// <returns></returns>
    static EffectManager* GetInstance() { return effectManager; }

    /// <summary>
    /// インスタンスの作成
    /// </summary>
    static void CreateInstance();

    /// <summary>
    /// インスタンスの削除
    /// </summary>
    static void DeleteInstance();

    /// <summary>
    /// エフェクシアの設定を行う
    /// </summary>
    static int Setting();

    /// <summary>
    /// 指定されたエフェクトを再生する
    /// </summary>
    /// <param name="effectData">再生するエフェクトの情報が格納されている場所のポインタ</param>
    void PlayEffect(EffectData* effectData);

    /// <summary>
    /// 指定されたエフェクトを停止する
    /// </summary>
    /// <param name="effectData">止めるエフェクトの情報</param>
    void StopEffect(const EffectData effectData);

    /// <summary>
    /// 全てのエフェクトの停止
    /// </summary>
    void StopAllEffect();

    /// <summary>
    ///  エフェクト全体の更新処理
    /// </summary>
    void Update();

    /// <summary>
    /// エフェクトの描画
    /// </summary>
    void Draw();

private:
    //コンストラクタ
    EffectManager();

    //デストラクタ
    ~EffectManager();


    //自身のポインタ
    static EffectManager* effectManager;

    //メンバ変数
    map<EffectTag, int> effectHandle;           //イメージハンドルをまとめているマップ配列
    vector<EffectData*> effectDataList;

};


