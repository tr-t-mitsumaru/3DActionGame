#pragma once
#include<map>

using namespace std;

/// <summary>
/// ���f���̃n���h�������[�h���ēn���p�̃N���X
/// </summary>
class ModelCollection
{
public:
    /// <summary>
    /// ���[�h�������f���̃^�O
    /// </summary>
    enum ModelTag
    {
        Player = 0,
        Boss   = 1,
        Stage  = 2
    };

    /// <summary>
    /// ���g�̃C���X�^���X�ɃA�N�Z�X����|�C���^��n��
    /// </summary>
    /// <returns></returns>
    static ModelCollection* GetInstance() { return modelCollection; }

    /// <summary>
    /// �C���X�^���X�̍쐬
    /// </summary>
    static void CreateInstance();

    /// <summary>
    /// �C���X�^���X�̍폜
    /// </summary>
    static void DeleteInstance();

    /// <summary>
    /// �w�肳�ꂽ�^�O�̃C���[�W�n���h����n��
    /// </summary>
    /// <param name="imageTag">�C���[�W�^�O</param>
    /// <returns>�C���[�W�n���h��</returns>
    static const int GetModelHandle(ModelTag modelTag);

private:

    //�R���X�g���N�^
    ModelCollection();
    //�f�X�g���N�^
    ~ModelCollection();

    /// <summary>
    /// �K�v�ȃ��f�������[�h����
    /// </summary>
    static void LoadModel();

    /// <summary>
    /// �ǂݍ��񂾃��f���̍폜
    /// </summary>
    static void DeleteModel();

    static ModelCollection* modelCollection;     //���g�̃C���X�^���X�̃|�C���^

    map<ModelTag, int> modelHandle;              //���f���n���h�����܂Ƃ߂Ă���}�b�v�z��

    
};
