#pragma once
#include<map>

using namespace std;

class SoundManager
{
public:
    /// <summary>
    /// ���ʉ��̎��
    /// </summary>
    enum SoundEffectTag
    {

    };

    /// <summary>
    /// BGM�̎��
    /// </summary>
    enum BGMTag
    {

    };

    //�C���X�^���X���擾����Q�b�^�[
    static SoundManager* GetInstance() { return soundManager; }
    //�C���X�^���X���쐬����
    static void CreateInstance();

    /// <summary>
    /// �C���X�^���X�̍폜
    /// </summary>
    static void DeleteInstance();

    /// <summary>
    /// �T�E���h�̖��O�������Ă��čĐ�����
    /// </summary>
    /// <param name="soundEffectTag">�Đ����鉹�̖��O</param>
    void PlaySoundEffect(SoundEffectTag soundEffectTag);

    /// <summary>
    /// BGM�̖��O�������Ă��čĐ�����
    /// </summary>
    /// <param name="bgmTag">�Đ�����BGM�̖��O</param>
    void PlayBGM(BGMTag bgmTag);

    /// <summary>
    /// �w���BGM���~�߂鏈��
    /// </summary>
    /// <param name="bgmTag">�~�߂�BGM�̖��O</param>
    void StopBGM(BGMTag bgmTag);

private:

    //�R���X�g���N�^
    SoundManager();
    //�f�X�g���N�^
    ~SoundManager();

    //���g�̃|�C���^
    static SoundManager* soundManager;

    //�T�E���h�n���h�����i�[����Ă���}�b�v�R���e�i
    map<SoundEffectTag, int> soundEffect;     //���ʉ��Ɋւ���}�b�v
    map<BGMTag,         int> bgm;             //bgm�Ɋւ���}�b�v
};


