#pragma once
#include "common.h"
#include "Date.h"
#include "Story.h"
#include "TagData.h"
#include <direct.h>

// �f�[�^�̃Z�[�u�E���[�h��S��
class DataManager{
	Story story;	// �X�g�[���[�f�[�^
	map<string, common::CharaType> charaList;	// �o�^����Ă���L�������X�g
	map<string, map<common::UpdateDateType, Date>> updateList;	// �o�^����Ă���ŏI�X�V�����X�g

	bool CreateDirectory(string path);	// �^����ꂽ�t���p�X�����ƂɕK�v�ł���΃f�B���N�g�����쐬����

	DataManager(const DataManager&);
	DataManager& operator=(const DataManager&);
public:
	void CreateStory(Story story);	// �X�g�[���[�̍쐬
	bool SaveStory(string path);	// �Z�[�u�t�@�C���p�X����͂���ƕ���t�@�C����ۑ�
	bool SaveStory(string path, Story story);	// �Z�[�u�t�@�C���p�X�ƃX�g�[���[�f�[�^����͂���ƕ���t�@�C����ۑ�
	bool LoadStory(string path);	// �Z�[�u�t�@�C���p�X����͂���ƕ���t�@�C�������[�h
	bool SaveChara();	// �L�����N�^�[�ꗗ�t�@�C���̃Z�[�u
	bool SaveUpdate();	// �ŏI�X�V�ꗗ�t�@�C���̃Z�[�u
	bool LoadChara();	// �L�����N�^�[�ꗗ�t�@�C���̃��[�h
	bool LoadUpdate();	// �ŏI�X�V�ꗗ�t�@�C���̃��[�h
	string SearchStory(Story story);	// �X�g�[���[�������A������Ȃ���΋󕶎���Ԃ�
	bool MergeStory(Story dest);	// ��̃X�g�[���[���}�[�W����i��ȏ�̍X�V����������true�A�X�V���Ȃ����false�j
	bool ConfirmUnknownStory(Date unknownUpdate);	// �����ł��Ȃ��������m�̃X�g�[���[���ǂ����邩�H�i�����Ȃ�true�A���f����Ȃ�false�j
	map<string, common::CharaType> GetCharaList();
	map<string, map<common::UpdateDateType, Date>> GetUpdateList();
	void SetCharaList(map<string, common::CharaType> charaList);
	void SetUpdateList(map<string, map<common::UpdateDateType, Date>> updateList);
	void SetStory(Story story);
	bool SaveTxtStory(string path);	// ���݂̃X�g�[���[�{����txt�ɕϊ����ďo��
	bool SaveTxtDetail(string path);	// ���݂̃X�g�[���[�̏ڍ׏���txt�ɕϊ����ďo��

	DataManager();
	~DataManager();
};

