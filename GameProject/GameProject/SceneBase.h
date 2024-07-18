#pragma once
#include"FadeInOut.h"
#include"SoundManager.h"


class SceneBase abstract
{
public:
    //�R���X�g���N�^
    SceneBase();
    //�f�X�g���N�^
    virtual ~SceneBase();

    /// <summary>
    /// ���̃V�[���x�[�X��Ԃ�
    /// </summary>
    /// <returns></returns>
    SceneBase* GetNextScene() { return inputScene; }

    virtual void Update()abstract;

private:
    SceneBase* inputScene;       //���̃��[�v�ł̃V�[��������ϐ�
    FadeInOut* fadeInOut;        //�t�F�[�h�A�E�g�ƃt�F�[�h�C���p�̃N���X
    SoundManager* soundManager;  //�T�E���h��炷���߂̃N���X
};

