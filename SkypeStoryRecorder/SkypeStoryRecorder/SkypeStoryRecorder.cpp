#include "SkypeStoryRecorder.h"

bool SkypeStoryRecorder::Run(){
	DataConverter dataConverter(GetClipText());	// �N���b�v�{�[�h�̕������ΏۂɃf�[�^�R���o�[�g
	if (!dataConverter.IsSuccess()){
		cout << "�N���b�v�{�[�h�Ƀe�L�X�g���R�s�[���Ă��������B" << endl;
		return false;
	}
	vector<Sentence> sentence = dataConverter.ExtractSentences();	// �R���o�[�g����ďo���Z���e���X���擾

	DataManager dataManager;	// �f�[�^�̕ۑ��E�Ǎ���S���f�[�^�}�l�[�W���[
	dataManager.LoadChara();	// �L�����ꗗ���擾
	dataManager.LoadUpdate();	// �X�V�����ꗗ���擾

	// �Z���e���X�f�R�[�_�[�Ƀf�[�^�}�l�[�W���[����L�����ꗗ�ƍX�V�����ꗗ��n������
	SentenceDecoder sentenceDecoder(sentence);
	sentenceDecoder.SetCharaList(dataManager.GetCharaList());
	sentenceDecoder.SetUpdateList(dataManager.GetUpdateList());

	vector<Story> storyList = sentenceDecoder.CreateStoryList();	// �X�g�[���[�z����擾����

	// ����ꂽ�X�g�[���[�����ɕۑ�
	successNum = 0, failNum = 0, unMergeNum = 0;
	bool breakFlg = false;	// �����𒆒f�����t���O
	for (auto p : storyList){
		dataManager.SetStory(p);

		if (p.title == ""){
			// �^�C�g���s���i�X�V���������ƂɒT���j
			cout << "���m�̃X�g�[���[�̒T�����J�n���܂�..." << endl;
			string path = dataManager.SearchStory(p);
			if (path == ""){
				// ������Ȃ�����
				failNum++;
				if (!dataManager.ConfirmUnknownStory(sentenceDecoder.GetUpdateList()[""][common::UpdateDateType::updateDate])){
					breakFlg = true;
					break;
				}
			}
			else{
				// ��������
				cout << "���m�̃X�g�[���[�́w" << path << "�x�ł���ƒf�肵�܂����B" << endl;
				p.title = path;
				sentenceDecoder.EmptyUpdateToUpdateList(path);	// ��^�C�g�������������̂ŁA�X�V�����𐄑������X�g�[���[�ɒu������
				bool failMergeFlg = false;	// �}�[�W�ň����v�����ւ񂩂���
				if (dataManager.LoadStory(common::DatRootPath() + "/" + path + common::StoryFileExtension()) == 0){
					failMergeFlg = !dataManager.MergeStory(p);
				}
				else{
					dataManager.CreateStory(p);
				}
				SaveStory(path, &dataManager, failMergeFlg);
			}
		}
		else{
			// ���m�^�C�g��
			bool failMergeFlg = false;	// �}�[�W�ň����v�����ւ񂩂���
			if (dataManager.LoadStory(common::DatRootPath() + "/" + p.title + common::StoryFileExtension()) == 0){
				// ���݂����F�X�V
				failMergeFlg = !dataManager.MergeStory(p);
			}
			else{
				// ���݂��Ȃ������F�V�K�쐬
				dataManager.CreateStory(p);
			}
			SaveStory(p.title, &dataManager, failMergeFlg);
		}
	}

	// �f�[�^�}�l�[�W���[�Ɉꗗ��Ԃ��ĕۑ�
	dataManager.SetCharaList(sentenceDecoder.GetCharaList());
	dataManager.SaveChara();	// �L�����ꗗ��ۑ�
	if (!breakFlg){	// �����𒆒f������X�V���������ۑ������Ȃ�
		dataManager.SetUpdateList(sentenceDecoder.GetUpdateList());
		dataManager.SaveUpdate();	// �X�V�����ꗗ��ۑ�
	}

	cout << "* ";
	cout <<"����I��: " << successNum;
	cout << ", �ύX�Ȃ� : " << unMergeNum;
	cout << ", ���s : " << failNum;
	cout << " *" << endl;
	bool ret = true;
	if (failNum > 0){
		cout << "�@[���s�̌�����Ɖ�����]" << endl;
		cout << "�@�E�N���b�v�{�[�h�ɐ���ɃR�s�[����ĂȂ��H �� �R�s�[���m�F���čăg���C" << endl;
		cout << "�@�E�������ŏI�X�V���痣��Ă���H �� ����������ɒ������΂��čăg���C" << endl;
		cout << "�@�E�X�g�[���[�̕ۑ��Ɏ��s�H �� �G�N�X�v���[���ُ̈킩 ���Ԃ��󂯂čăg���C" << endl;
		cout << "�@[���s�ƌ�F���ꂽ�ꍇ�̗�]" << endl;
		cout << "�@�E���łɍX�V���I���Ă���A�Â���������͂���" << endl;
		cout << "�@�E�X�g�[���[�ł͂Ȃ��Ӑ}�����e�L�X�g���擪�ɂ�����" << endl;
		ret = false;
	}
	else if (successNum == 0){
		cout << "�@�X�V�̌��ʁA�ύX������܂���ł����B" << endl;
		ret = false;
	}
	return ret;
}

bool SkypeStoryRecorder::SaveStory(string title, DataManager *dataManager, bool failMergeFlg){
	if (dataManager->SaveStory(common::DatRootPath() + "/" + title + common::StoryFileExtension()) == 0){
		failMergeFlg ? unMergeNum++ : successNum++;
		if (txtStoryOutputFlg){
			dataManager->SaveTxtStory(common::StoryRootPath() + "/" + title + ".txt");
		}
		if (txtDetailOutputFlg){
			dataManager->SaveTxtDetail(common::StoryRootPath() + "/" + title + "_detail.txt");
		}
	}
	else{
		failNum++;
	}
	return true;
}

SkypeStoryRecorder::SkypeStoryRecorder(bool txtStoryOutputFlg, bool txtDetailOutputFlg){
	cout << "[SkypeStoryRecorder ver1.01]" << endl;
	this->txtStoryOutputFlg = txtStoryOutputFlg;
	this->txtDetailOutputFlg = txtDetailOutputFlg;
}

SkypeStoryRecorder::~SkypeStoryRecorder(){
}

function<string()> SkypeStoryRecorder::GetClipText(){
	return[](){
		string ret = "";

		OpenClipboard(NULL);

		HANDLE hText = GetClipboardData(CF_TEXT);
		if (hText != NULL) {
			ret = static_cast<string>((char *)GlobalLock(hText));
			GlobalUnlock(hText);
		}

		CloseClipboard();
		return ret;
	};
}


