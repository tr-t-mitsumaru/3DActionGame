#pragma once
#include<map>

using namespace std;

class EffectManager
{
public:
    /// <summary>
    /// �G�t�F�N�g�̎��
    /// </summary>
    enum EffectTag
    {

    };


    /// <summary>
    /// ���g�̃C���X�^���X�ɃA�N�Z�X����|�C���^��n��
    /// </summary>
    /// <returns></returns>
    static EffectManager* GetInstance() { return effectManager; }

    /// <summary>
    /// �C���X�^���X�̍쐬
    /// </summary>
    static void CreateInstance();

    /// <summary>
    /// �C���X�^���X�̍폜
    /// </summary>
    static void DeleteInstance();

    /// <summary>
    /// �w�肳�ꂽ�G�t�F�N�g���Đ�����
    /// </summary>
    /// <param name="imageTag">�G�t�F�N�g�̎��</param>
    static const void PlayEffect(EffectTag effectTag);

private:
    //�R���X�g���N�^
    EffectManager();

    //�f�X�g���N�^
    ~EffectManager();

    /// <summary>
    /// �K�v�ȃG�t�F�N�g�����[�h����
    /// </summary>
    static void LoadEffect();

    /// <summary>
    /// �ǂݍ��񂾃G�t�F�N�g�̍폜
    /// </summary>
    static void DeleteEffect();

    //���g�̃|�C���^
    static EffectManager* effectManager;

    //�����o�ϐ�
    map<EffectTag, int> effectHandle;           //�C���[�W�n���h�����܂Ƃ߂Ă���}�b�v�z��
};


