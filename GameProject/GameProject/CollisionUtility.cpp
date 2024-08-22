#include"CollisionUtility.h"


/// <summary>
/// アニメーションの再生時間に合わせて当たり判定情報を渡す
/// </summary>
/// <param name="animationNowTime">現在のアニメーションの再生時間</param>
/// <param name="animationLimitTime">アニメーションの総再生時間</param>
CollisionData::CollisionState CollisionUtility::SendCollisionDataByAnimationTime(float animationNowTime, float animationLimitTime,
    CollisionData::CollisionState currentCollisionState,float collisionStartAnimationRatio)
{
    // 返り値用の変数を用意
    CollisionData::CollisionState nextCollisionState;

    //アニメーションの再生時間が指定の割合を超えていたらCollisionManagerに情報を渡す
    if (animationNowTime / animationLimitTime >= collisionStartAnimationRatio &&
        currentCollisionState == CollisionData::NoCollision)
    {
        //当たり判定の状態を変更する
        nextCollisionState = CollisionData::CollisionActive;
    }
    else
    {
        nextCollisionState = currentCollisionState;
    }

    return nextCollisionState;
}

/// <summary>
/// キャラクターの座標から当たり判定を生成する場所を設定する
/// </summary>
/// <param name="characterPosition">キャラの座標</param>
/// <param name="direction">生成する方向</param>
VECTOR CollisionUtility::TransrateCollisionCapsulePosition(VECTOR characterPosition, VECTOR direction, VECTOR offsetPosition, float offsetPositionScale)
{

    // 任意のベクトル (0, 1, 0) を使用して新しいX軸を定義
    VECTOR xAxis = VNorm(VCross(VGet(0.0f, 1.0f, 0.0f), direction));

    // 新しいY軸を計算
    VECTOR yAxis = VNorm(VCross(direction, xAxis));

    // 元となる座標からどの方向にどれだけ動くかのベロシティを計算する
    VECTOR velocity = VScale(direction, offsetPositionScale);

    //移送させた座標を出す
    VECTOR newPosition = VAdd(characterPosition, velocity);

    //移動させた方向軸を基準として動かせる行列を出す
    MATRIX changeAxisMatrix = MGetAxis1(xAxis, yAxis, direction, newPosition);

    //新しい軸を使って座標を移動させる
    VECTOR axisChangeOffsetPosition = VTransform(offsetPosition, changeAxisMatrix);


    // 平行移動させた位置を返す
    return  axisChangeOffsetPosition;

}


/// <summary>
/// カプセルの回転させるベクトルの計算
/// </summary>
/// <param name="radian">ラジアン</param>
/// <returns>計算したベクトル</returns>
VECTOR CollisionUtility::RotationCollisionCapsule(float radian, VECTOR modelDirection, VECTOR position, float capsuleLineLength)
{
    //回転用の行列を作成
    MATRIX capsuleAngle = MGetRotAxis(modelDirection, radian);

    //回転させる前のカプセルの終点座標（positionに垂直)
    VECTOR vertexCapsuleEndPosition = VAdd(position, VGet(0.0f, capsuleLineLength, 0.0f));

    //回転させる前のカプセルのラインベクトル
    VECTOR capsuleLineVector = VSub(vertexCapsuleEndPosition, position);

    //回転させた後のカプセルのラインベクトルを返す
    return VTransform(capsuleLineVector, capsuleAngle);
}
