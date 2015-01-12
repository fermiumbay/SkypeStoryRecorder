#pragma once
#include "Date.h"
#include <string>
using namespace std;

// �Z���e���X�\����
struct Sentence{
	enum State{
		normal,	// �ʏ�
		edited,	// �ҏW�ς�
		deleted,	// �폜�ς�
	};

	Date makeDate,	// ��������
		updateDate;	// �ҏW�E�폜����
	string speaker,	// ������
		text,	// �e�L�X�g�{��
		note;	// ���ߕ�
	State state;	// �Z���e���X�̏��

	Sentence() :
		makeDate(Date()),
		updateDate(Date()),
		speaker(""),
		text(""),
		note(""),
		state(State::normal)
		{}

	// �Z���e���X�̍ŏI�I�ȍX�V�������擾
	Date GetFinalUpdate(){
		// �ҏW�E�폜����Ă���΂��̓����A�����łȂ���Δ���������Ԃ�
		return state == State::normal ? makeDate : updateDate;
	}

	bool operator==(Sentence s){
		return makeDate == s.makeDate
			&& updateDate == s.updateDate
			&& speaker == s.speaker
			&& text == s.text
			&& note == s.note
			&& state == s.state;
	}
};
