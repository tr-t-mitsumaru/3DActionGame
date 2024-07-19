#pragma once
#include<map>
#include<functional>
using namespace std;

struct ColisionData;

/// <summary>
/// �����蔻�������N���X
/// </summary>
class ColisionManager
{
public:
    /// <summary>
    /// �����蔻�������I�u�W�F�N�g�̎��
    /// </summary>
    enum HitObjectTag
    {

    };

    //�C���X�^���X���擾����Q�b�^�[
    static ColisionManager* GetInstance() { return colisionManager; }
    //�C���X�^���X���쐬����
    static void CreateInstance();

    /// <summary>
    /// �C���X�^���X�̍폜
    /// </summary>
    static void DeleteInstance();

    /// <summary>
    /// ���̂ƃJ�v�Z���Ƃ̓����蔻��
    /// </summary>
    /// <param name="sphere">���̂̓����蔻��ɕK�v�ȏ��</param>
    /// <param name="capsule">�J�v�Z���̓����蔻��ɕK�v�ȏ��</param>
    /// <returns>�������Ă��邩</returns>
    static bool IsHitSphereAndCapsule(ColisionData sphere, ColisionData capsule);

    /// <summary>
    /// �����������Ƃ̏����������Ă���
    /// </summary>
    /// <param name="colisionData">�����蔻��ɕK�v�ȏ��</param>
    /// <param name="hitObjectTag">�I�u�W�F�N�g�̎��</param>
    /// <param name="onHit">����������ɍs���֐��̃|�C���^</param>
    static void Resister(ColisionData colisionData, HitObjectTag hitObjectTag
        , function<void(ColisionData, HitObjectTag)> onHit);

    /// <summary>
    /// �X�V����
    /// </summary>
    void Update();

private:

    //���g�̃|�C���^
    static ColisionManager* colisionManager;

    //�����蔻��������������X�g
    map<HitObjectTag, ColisionData> hitObjectList;

    //�R���X�g���N�^
    ColisionManager();

    //�f�X�g���N�^
    ~ColisionManager();

};

