#pragma once
#include "common.h"
#include "Date.h"
#include "DataConverter.h"
#include "DataManager.h"
#include "SentenceDecoder.h"
#include <Windows.h>

// �X�J�C�v���ꃌ�R�[�_�[
class SkypeStoryRecorder{
	bool txtStoryOutputFlg;	// �{��txt�o�͂𓯎��ɍs���t���O
	bool txtDetailOutputFlg;	// �ڍ�txt�o�͂𓯎��ɍs���t���O
	int successNum, failNum, unMergeNum;	// �����̐������A���s���A�X�V�Ȃ���
	function<string()> GetClipText();	// �N���b�v�{�[�h�̃e�L�X�g���擾����

	bool SaveStory(string title, DataManager *dataManager, bool failMergeFlg);	// �X�g�[���[�̕ۑ�

	SkypeStoryRecorder();
	SkypeStoryRecorder(const SkypeStoryRecorder&);
	SkypeStoryRecorder& operator=(const SkypeStoryRecorder&);
public:
	SkypeStoryRecorder(bool txtStoryOutputFlg, bool txtDetailOutputFlg);
	~SkypeStoryRecorder();
	bool Run();	// ���s�itrue�^false�Ő���I���^�ُ�I���j
};
