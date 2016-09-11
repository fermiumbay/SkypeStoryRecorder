#include "SentenceDecoder.h"

SentenceDecoder::SentenceDecoder(vector<Sentence> sentence){
	unregisterFlg = false;
	this->sentence = sentence;
}

SentenceDecoder::~SentenceDecoder(){
}

vector<Story> SentenceDecoder::CreateStoryList(){
	vector<Story> ret;
	Story story;

	set<string> mainChara;	// ���C���L�����W��
	set<string> subChara;	// �T�u�L�����W��
	set<string> reserveChara;	// �ۗ�

	Date finalDate, updateDate;	// �ŏI�Z���e���X�X�V�A�ŏI�X�V

	// �X�g�[���[���̎擾
	auto GetStoryTitleString = [](string title, bool unknownStoryFlg){
		string ret;
		if (unknownStoryFlg){
			ret = "���m�̃X�g�[���[";
		}
		else{
			ret = "�X�g�[���[�w" + title + "�x";
		}
		return ret;
	};

	// �����𖞂����Ă���΂����̑���A�����łȂ���Α傫��������
	auto Substitute = [](bool flg, Date &source, Date dest){
		if (flg){
			common::Substitute(source, dest);
		}
		else{
			common::SubstituteMax(source, dest);
		}
	};

	cout << "���̓Z���e���X��������擾���Ă��܂�..." << endl;

	bool unknownStoryFlg = false;	// ���m�̃X�g�[���[�����݂���

	int maxsentence = this->sentence.size();
	cout << "�Z���e���X���� : " << maxsentence << endl;

	int counter = 0;
	int countRate = 0;
	for (auto p : this->sentence){
		stringstream ss(p.text);
		p.text = "";
		bool speakingFlg = false;	// �b�҂����݂��Ă���t���O
		bool summaryFlg = false;	// �v��ҏW���t���O

		counter++;
		for (; counter * 100 / maxsentence > countRate; countRate++){
			if ((countRate + 1) % 10 == 0){
				cout << " : " << (countRate + 1) << "% ����" << endl;
			}
		}

		while (!ss.eof()){
			string buf;	// �Z���e���X�̈�s
			getline(ss, buf);

			bool readTextFlg = false;	// �e�L�X�g�{����ǂݎn�߂��t���O
			while (buf != ""){
				if (summaryFlg){
					// �v��ҏW��
					if (buf == common::GetEndSummaryCommand()){
						// �v��I��
						summaryFlg = false;
					}
					else{
						// �v��ǉ�
						story.summary += buf + "\n";
					}
					buf = "";
				}
				else{
					// �v��ȊO
					if (regex_match(buf, regex(common::GetNewStoryCommand() + ".*"))){
						// �V�K�X�g�[���[
						if (story.title != "" || unknownStoryFlg){
							// ����܂Ŏ擾���Ă����͉̂����̃X�g�[���[�������̂�ret��push����
							for (auto q : mainChara){
								story.mainChara.push_back(q);
							}
							for (auto q : subChara){
								story.subChara.push_back(q);
							}
							ret.push_back(story);
							cout << GetStoryTitleString(story.title, unknownStoryFlg) << "�̍X�V���������܂����B" << endl;
							mainChara.clear();
							subChara.clear();

							// ��^�C�g���Ȃ炻�̂܂ܑ���A�����łȂ���΍ŐV�̓����̕���I��
							Substitute(story.title == "", updateList[story.title][common::UpdateDateType::finalDate], finalDate);
							Substitute(story.title == "", updateList[story.title][common::UpdateDateType::updateDate], updateDate);

							finalDate = Date();
							updateDate = Date();

							story = Story();
						}
						int index = common::GetNewStoryCommand().size();
						story.title = buf.substr(index);
						unknownStoryFlg = false;
						cout << GetStoryTitleString(story.title, unknownStoryFlg) << "�𔭌����܂����B" << endl;
						buf = "";
					}
					else if (buf == common::GetSummaryCommand()){
						// �v��J�n
						story.summary = "";	// �v�񂪑�����̂�h�����߁A������
						summaryFlg = true;
						buf = "";
					}
					else if (buf == common::GetIgnoreDateCommand()){
						// �����X�V���Ȃ��Z���e���X
						p.ignoreDateFlg = true;
						buf = "";
					}
					else{
						// ���̑� �F �L�����N�^�[�����������āA���X�g�ɒǉ�����L��������I��
						if (!readTextFlg){
							p.text += buf + "\n";
							readTextFlg = true;
						}
						map<common::CharaType, string> insertChara = InsertChara(buf, speakingFlg, reserveChara);
						for (auto q : insertChara){
							switch (q.first){
							case common::CharaType::none:
								reserveChara.insert(q.second);
								break;
							case common::CharaType::main:
								mainChara.insert(q.second);
								break;
							case common::CharaType::sub:
								subChara.insert(q.second);
								break;
							case common::CharaType::ignore:
								// �����L�����͖�������
								break;
							}
						}
					}
				}
			}
		}
		story.sentence.push_back(p);	// �Z���e���X���X�g�[���[�ɒǉ�
		if (story.title == "" && !unknownStoryFlg){
			unknownStoryFlg = true;	// �X�g�[���[�����s���̂܂܂������ꍇ�A�s���t���O�𗧂Ă�
		}

		if (!p.ignoreDateFlg){
			finalDate = p.makeDate;	// �ŏI�Z���e���X�����́u�쐬�����v�������X�V
			common::SubstituteMax(updateDate, p.GetFinalUpdate());	// �ŏI�X�V���͍ŐV�̂��̂������o����悤��
		}
	}

	// �Ō�Ɏc�����X�g�[���[��push
	for (auto q : mainChara){
		story.mainChara.push_back(q);
	}
	for (auto q : subChara){
		story.subChara.push_back(q);
	}
	ret.push_back(story);
	cout << GetStoryTitleString(story.title, unknownStoryFlg) << "�̍X�V���������܂����B" << endl;

	Substitute(story.title == "", updateList[story.title][common::UpdateDateType::finalDate], finalDate);
	Substitute(story.title == "", updateList[story.title][common::UpdateDateType::updateDate], updateDate);

	finalDate = Date();
	updateDate = Date();

	cout << "�擾�������܂����B" << endl;

	return ret;
}

void SentenceDecoder::EmptyUpdateToUpdateList(string path){
	// ��^�C�g���������
	if (updateList.count("") > 0){
		// ��^�C�g���������ς݃^�C�g�����傫�������X�V�����Ƃ��ċL�^
		// ��^�C�g���̕����傫���F��ɐ����^�C�g�����o�ꂵ�Ȃ������̂ŁA��^�C�g���̂��̂����ł���
		// ��^�C�g���̕����������F��ɐ����^�C�g�����o�ꂵ�Ă���̂ŁA�ŐV�Ȃ̂͐����^�C�g���̕�
		common::SubstituteMax(updateList[path][common::UpdateDateType::finalDate], updateList[""][common::UpdateDateType::finalDate]);
		common::SubstituteMax(updateList[path][common::UpdateDateType::updateDate], updateList[""][common::UpdateDateType::updateDate]);
	}
}

map<common::CharaType, string> SentenceDecoder::InsertChara(string &buf, bool &speakingFlg, set<string> reserveChara){
	map<common::CharaType, string> ret;	// �}������L�����N�^�[�����i�[����

	if (!speakingFlg){
		// �b�҂��܂��������ĂȂ�
		if (regex_match(buf, regex(".*(�u|�i).*"))){
			int index;

			int index1 = buf.find("�u");
			int index2 = buf.find("�i");
			if (index1 == string::npos){
				index = index2;
			}
			else if (index2 == string::npos){
				index = index1;
			}
			else{
				if (index1 < index2){
					index = index1;
				}
				else{
					index = index2;
				}
			}

			string charaName = buf.substr(0, index);
			common::CharaType charaType = CheckCharaType(charaName);
			if (charaType == common::CharaType::none){
				// �ۗ��ɂȂ����L�����ȊO��o�^�ΏۂƂ���
				if (reserveChara.find(charaName) == reserveChara.end()){
					charaType = RegisterChara(charaName);
				}
			}
			ret[charaType] = charaName;

			buf = buf.substr(index + 2);
			speakingFlg = true;
		}
		else{
			buf = "";
		}
	}
	else{
		// �b�҂��������Ă���
		if (regex_match(buf, regex(".*(�v|�j).*"))){
			int index;

			int index1 = buf.find("�v");
			int index2 = buf.find("�j");
			if (index1 == string::npos){
				index = index2;
			}
			else if (index2 == string::npos){
				index = index1;
			}
			else{
				if (index1 < index2){
					index = index1;
				}
				else{
					index = index2;
				}
			}

			buf = buf.substr(index + 2);
			speakingFlg = false;
		}
		else{
			buf = "";
		}
	}

	return ret;
}

common::CharaType SentenceDecoder::CheckCharaType(string chara){
	if (charaList.count(chara) > 0){
		return charaList[chara];
	}
	else{
		return common::CharaType::none;
	}
}

common::CharaType SentenceDecoder::RegisterChara(string chara){
	int ret = 0;
	if (!unregisterFlg){
		cout << "* ���o�^ : " << chara << endl;
		cout << "�i-1:�S���ۗ��@0:�ۗ��@1:���C���@2:�T�u�@3:�����j" << endl;
		cout << " : ";
		string stype;
		cin >> stype;
		ret = common::StoI(stype);
	}

	switch (ret){
	case 1:
		charaList[chara] = common::CharaType::main;
		cout << "�u" << chara << "�v�����C���L�����N�^�[�Ƃ��ēo�^���܂����B" << endl;
		break;
	case 2:
		charaList[chara] = common::CharaType::sub;
		cout << "�u" << chara << "�v���T�u�L�����N�^�[�Ƃ��ēo�^���܂����B" << endl;
		break;
	case 3:
		charaList[chara] = common::CharaType::ignore;
		cout << "�u" << chara << "�v�����㖳�����邱�Ƃɂ��܂����B" << endl;
		break;
	default:
		if (ret == -1){
			unregisterFlg = true;
		}
		ret = 0;
		cout << "�u" << chara << "�v�̓o�^��ۗ����܂����B" << endl;
		break;
	}
	return static_cast<common::CharaType>(ret);
}

map<string, common::CharaType> SentenceDecoder::GetCharaList(){
	return charaList;
}

map<string, map<common::UpdateDateType, Date>> SentenceDecoder::GetUpdateList(){
	return updateList;
}

void SentenceDecoder::SetCharaList(map<string, common::CharaType> charaList){
	this->charaList = charaList;
}

void SentenceDecoder::SetUpdateList(map<string, map<common::UpdateDateType, Date>> updateList){
	this->updateList = updateList;
}
