#pragma once
#include "Sentence.h"

// �X�g�[���[�\����
struct Story{
	string title,	// �X�g�[���[�^�C�g��
		summary;	// �T�v
	vector<string> mainChara,	// ���C���L������
		subChara;	// �T�u�L������
	vector<Sentence> sentence;	// �Z���e���X�z��
	Story() :
		title(""),
		summary("")
	{
		mainChara.clear();
		subChara.clear();
		sentence.clear();
	}

	bool operator==(Story s){
		return title == s.title
			&& summary == s.summary
			&& mainChara == s.mainChara
			&& subChara == s.subChara
			&& common::EqualVector(sentence, s.sentence);
	}
};