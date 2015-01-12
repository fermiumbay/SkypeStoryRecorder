#pragma once
#include "common.h"
#include "Date.h"
#include "Sentence.h"

// ���̓f�[�^�̃Z���e���X�ϊ��@�\
class DataConverter{
	string data;	// ���̓e�L�X�g�f�[�^
	bool successFlg;	// ���͓ǂݍ��ݐ����t���O

	Date GetDate(string text, Date beforeDate = Date());	// ���t�����񂩂���t���擾����i�X�V��̓����̏ꍇ�͑������ɍX�V�O���������j

	DataConverter();
	DataConverter(const DataConverter&);
	DataConverter& operator=(const DataConverter&);
public:
	DataConverter(function<string()> GetText);	// ���͂̎擾���@�������Ƃ���
	bool IsSuccess();	// �ǂݍ��ݐ���������
	vector<Sentence> ExtractSentences();	// �Z���e���X�����o���ďo��
	~DataConverter();
};
