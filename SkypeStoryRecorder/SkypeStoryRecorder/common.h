#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <functional>
#include <vector>
#include <map>
#include <set>
#include <regex>
using namespace std;

// ���ʃw�b�_
class common{
	common();
	~common();
	common(const common&);
	common& operator=(const common&);
public:
	// �ŏI�X�V���̃^�C�v
	enum UpdateDateType{
		updateDate,	// �t�@�C���S�̂̍ŏI�X�V��
		finalDate,	// �ŏI�Z���e���X�̍ŏI�X�V��
	};

	// �L�����N�^�[�̃^�C�v
	enum CharaType{
		none = 0,	// ���݂��Ȃ�
		main,	// ���C��
		sub,	// �T�u
		ignore,	// ����
	};

	static const string DatRootPath(){ return "dat"; }	// �X�g�[���[�f�[�^�t�@�C���̃��[�g�p�X��
	static const string StoryRootPath(){ return "story"; }	// �X�g�[���[�̃e�L�X�g�̃��[�g�p�X��
	static const string StoryFileExtension(){ return ".story"; }	// �X�g�[���[�t�@�C���̊g���q
	static const string CharaListFile(){ return "chara.dat"; }	// �L�����ꗗ�t�@�C����
	static const string UpdateListFile(){ return "update.dat"; }	// �X�V�ꗗ�t�@�C����
	static const string GetNewStoryCommand(){ return "�mSkypeStoryRecorder : Title�n"; }	// �V�K�X�g�[���[�쐬�R�}���h
	static const string GetSummaryCommand(){ return "�mSkypeStoryRecorder : Summary�n"; }	// �v��ҏW�R�}���h
	static const string GetEndSummaryCommand(){ return "�mSkypeStoryRecorder : EndSummary�n"; }	// �v��ҏW�I���R�}���h

	static string ItoS(int value);	// �����𕶎���ɕϊ�
	static string ItoS(int value, int num);	// �����𕶎���ɕϊ��i�������Ō����w��j
	static int StoI(string str);	// ������𐮐��ɕϊ�
	static vector<string> Split(string s, char d);	// ����d�ŋ�؂�Split�֐�

	// source��dest�����������̂�Ԃ������iSubstituteMax�Ƃ̑Ή��̂��߂ɗp�Ӂj
	template<class C>
	static void Substitute(C &source, C dest){
		source = dest;
	}

	// �񐔂̂����傫������source�ɑ�����ĕԂ��i�N���X��<��=�̒�`���K�v�j
	template<class C>
	static void SubstituteMax(C &source, C dest){
		if (source < dest){
			source = dest;
		}
	}

	// vector�̗v�f�����ׂē�����
	template<class C>
	static bool EqualVector(vector<C> source, vector<C> dest){
		bool ret;
		int size = source.size();
		ret = (size == dest.size());
		if (ret){
			for (int i = 0; i < size; i++){
				ret &= (source[i] == dest[i]);
			}
		}
		return ret;
	}
};
