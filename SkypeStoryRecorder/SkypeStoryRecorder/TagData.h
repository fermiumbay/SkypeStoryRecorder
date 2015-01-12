#pragma once
#include "common.h"

// �^�O�Ǘ��N���X
class TagData{
	string text;	// �e�L�X�g�f�[�^
	vector<pair<string, vector<TagData>>> data;	// �������Ă���^�O���X�g

	string TagData::GetTag(string buf);	// �擪���u<�v�̕�����̐擪�^�O�����o��
	void AddData(string tag, TagData addData);	// �f�[�^�̒ǉ�
	void AddData(string tag, string text);	// �e�L�X�g���܂񂾃f�[�^�̒ǉ�
	void AddText(string text);	// �e�L�X�g�̒ǉ�

	TagData(string s = "");
public:
	vector<pair<string, vector<TagData>>> GetData();	// �f�[�^�̎擾
	string GetText();	// �e�L�X�g�̎擾

	TagData(ifstream &ifs);	// ���̓X�g���[������e�L�X�g����ǂ��ă^�O�f�[�^�Ƃ��ďo��
	TagData(const TagData&);
	TagData& operator=(const TagData&);
	~TagData(){}
};

