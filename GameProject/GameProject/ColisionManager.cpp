#include "ColisionManager.h"
#include"ColisionData.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ColisionManager::ColisionManager()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ColisionManager::~ColisionManager()
{

}

/// <summary>
/// �C���X�^���X���쐬����
/// </summary>
void ColisionManager::CreateInstance()
{

}

/// <summary>
/// �C���X�^���X�̍폜
/// </summary>
void ColisionManager::DeleteInstance()
{

}



/// <summary>
/// ���̂ƃJ�v�Z���Ƃ̓����蔻��
/// </summary>
/// <param name="sphere">���̂̓����蔻��ɕK�v�ȏ��</param>
/// <param name="capsule">�J�v�Z���̓����蔻��ɕK�v�ȏ��</param>
/// <returns>�������Ă��邩</returns>
bool ColisionManager::IsHitSphereAndCapsule(ColisionData sphere, ColisionData capsule)
{

}

/// <summary>
/// �����������Ƃ̏����������Ă���
/// </summary>
/// <param name="colisionData">�����蔻��ɕK�v�ȏ��</param>
/// <param name="hitObjectTag">�I�u�W�F�N�g�̎��</param>
/// <param name="onHit">����������ɍs���֐��̃|�C���^</param>
void ColisionManager::Resister(ColisionData colisionData, HitObjectTag hitObjectTag
    , function<void(ColisionData, HitObjectTag)> onHit)
{

}


/// <summary>
/// �X�V����
/// </summary>
void ColisionManager::Update()
{

}