#pragma once

//�N���X�̃v���g�^�C�v�錾
class SceneBase;
class FrameRateController;

/// <summary>
/// �Q�[���S�̂̃V�[���Ǘ��Ȃǂ��s���N���X
/// </summary>
class Game
{
public:
    //�R���X�g���N�g
    Game();
    //�f�X�g���N�^
    ~Game();

    /// <summary>
    /// �Q�[�����[�v���܂킷����
    /// </summary>
    void Update();

    /// <summary>
    /// �Q�[�����p�����邩�̔��f
    /// </summary>
    /// <returns>�p�����邩�̃t���O</returns>
    bool IsContinueGame();

private:
    /// <summary>
    /// ���݂̃V�[�����폜���Ď��̃V�[���̃|�C���^������
    /// </summary>
    void ChangeScene();

    SceneBase* nowScene;                            //���݂̃��[�v�ł̃V�[����ۑ�����|�C���^
    SceneBase* nextScene;                           //���̃��[�v�ł̃V�[����ۑ�����|�C���^
    FrameRateController* frameRateController;       //�t���[�����[�g�v�Z�p�N���X
};
