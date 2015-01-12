#pragma once
#include "common.h"
#include "Date.h"
#include "Story.h"
#include "TagData.h"

// �Z���e���X����ǂ��ăX�g�[���[���Ƃɕ�������@�B
class SentenceDecoder{
	vector<Sentence> sentence;	// �Z���e���X�z��
	map<string, common::CharaType> charaList;	// �o�^����Ă���L�������X�g
	map<string, map<common::UpdateDateType, Date>> updateList;	// �o�^����Ă���ŏI�X�V�����X�g
	bool unregisterFlg;	// �L�����N�^�[�o�^��S�ۗ��ɂ���t���O

	map<common::CharaType, string> InsertChara(string &buf, bool &speakingFlg, set<string> reserveChara);	// �L�����N�^�[���̑}��
	inline common::CharaType CheckCharaType(string chara);	// ���������łɓo�^����Ă���L�����Ȃ�L�����^�C�v���A�����łȂ����none��Ԃ�
	common::CharaType RegisterChara(string chara);	// �L�����N�^�[�̐V�K�o�^

	SentenceDecoder();
	SentenceDecoder(const SentenceDecoder&);
	SentenceDecoder& operator=(const SentenceDecoder&);
public:
	vector<Story> CreateStoryList();	// �X�g�[���[�z������o���Ď擾����
	void EmptyUpdateToUpdateList(string path);	// ��^�C�g���̍X�V���X�g�𐄑��ł����^�C�g�����X�g�ɕύX����
	map<string, common::CharaType> GetCharaList();
	map<string, map<common::UpdateDateType, Date>> GetUpdateList();
	void SetCharaList(map<string, common::CharaType> charaList);
	void SetUpdateList(map<string, map<common::UpdateDateType, Date>> updateList);

	SentenceDecoder(vector<Sentence> sentence);
	~SentenceDecoder();
};

