#pragma once
#include<map>

using namespace std;

class ImageCollection
{
public:

    /// <summary>
    /// ���[�h�����摜�̃^�O
    /// </summary>
    enum ImageTag
    {

    };

    /// <summary>
    /// ���g�̃C���X�^���X�ɃA�N�Z�X����|�C���^��n��
    /// </summary>
    /// <returns></returns>
    static ImageCollection* GetInstance() { return imageCollection; }

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
    static const int GetImageHandle(ImageTag imageTag);


private:

    //�R���X�g���N�^
    ImageCollection();
    //�f�X�g���N�^
    ~ImageCollection();

    /// <summary>
    /// �K�v�ȉ摜�����[�h����
    /// </summary>
    static void LoadImage();

    /// <summary>
    /// �ǂݍ��񂾉摜�̍폜
    /// </summary>
    static void DeleteImage();

    //���g�̃|�C���^
    static ImageCollection* imageCollection;

    //�����o�ϐ�
    map<ImageTag, int> imageHandle;           //�C���[�W�n���h�����܂Ƃ߂Ă���}�b�v�z��

};


