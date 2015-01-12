#include "DataManager.h"

DataManager::DataManager(){
}

DataManager::~DataManager(){
}

void DataManager::CreateStory(Story story){
	this->story = story;
	cout << story.title << "��V�K�쐬���܂����B" << endl;
}

bool DataManager::CreateDirectory(string path){
	// �t�H���_���쐬���Ȃ���X�g�[���[��ۑ�
	vector<string> fullpath = common::Split(path, '/');
	string folder = "";
	for (int i = 0; i < static_cast<signed int>(fullpath.size()) - 1; i++){
		folder += (i != 0 ? "/" : "");
		folder += fullpath[i];
		_mkdir(folder.c_str());
	}
	return true;
}

bool DataManager::SaveStory(string path){
	return SaveStory(path, this->story);
}

bool DataManager::SaveStory(string path, Story story){
	CreateDirectory(path);
	ofstream ofs(path);
	if (ofs.fail()){
		cout << path << "�̕ۑ��Ɏ��s���܂����B" << endl;
		return 1;
	}

	cout << path << "�̕ۑ����ł�..." << endl;

	ofs << "<title>" << story.title << "</title>" << endl;
	ofs << "<summary>" << story.summary << "</summary>" << endl;

	ofs << "<chara>" << endl;
	for (auto p : story.mainChara){
		ofs << "<main>" << p << "</main>" << endl;
	}
	for (auto p : story.subChara){
		ofs << "<sub>" << p << "</sub>" << endl;
	}
	ofs << "</chara>" << endl;

	ofs << "<story>" << endl;
	for (auto p : story.sentence){
		ofs << "<sentence>" << endl;
		ofs << "<makedate>" << p.makeDate.GetStr() << "</makedate>" << endl;
		if (p.state == Sentence::State::edited){
			ofs << "<editdate>" << p.updateDate.GetStr() << "</editdate>" << endl;
		}
		else if (p.state == Sentence::State::deleted){
			ofs << "<deletedate>" << p.updateDate.GetStr() << "</deletedate>" << endl;
		}
		ofs << "<speaker>" << p.speaker << "</speaker>" << endl;
		ofs << "<text>" << p.text << "</text>" << endl;
		ofs << "<note>" << p.note << "</note>" << endl;
		ofs << "</sentence>" << endl;
	}
	ofs << "</story>" << endl;

	cout << path << "��ۑ����܂����B" << endl;
	return 0;
}

bool DataManager::LoadStory(string path){
	bool ret;
	story = Story();
	ifstream ifs(path);
	if (ifs.fail()){
		cout << path << "�̓Ǎ��Ɏ��s���܂����B" << endl;
		ret = 1;
	}
	else{
		cout << path << "�̓ǂݍ��ݒ��ł�..." << endl;

		// ���̓e�L�X�g����^�O�f�[�^�ւ̕ϊ�
		TagData data(ifs);

		// ����ꂽ�^�O�f�[�^�����ƂɃX�g�[���[�f�[�^�i�[
		for (auto p : data.GetData()){
			if (p.first == "title"){
				story.title = p.second[0].GetText();
			}
			if (p.first == "summary"){
				story.summary = p.second[0].GetText();
			}
			if (p.first == "chara"){
				for (auto q : p.second[0].GetData()){
					if (q.first == "main"){
						for (auto r : q.second){
							story.mainChara.push_back(r.GetText());
						}
					}
					if (q.first == "sub"){
						for (auto r : q.second){
							story.subChara.push_back(r.GetText());
						}
					}
				}
			}

			if (p.first == "story"){
				int a = 0;
				for (auto q : p.second[0].GetData()){
					if (q.first == "sentence"){
						for (auto r : q.second){
							Sentence sentence;
							for (auto s : r.GetData()){
								if (s.first == "makedate"){
									sentence.makeDate = Date::ConvertDate(s.second[0].GetText());
								}
								if (s.first == "editdate"){
									sentence.updateDate = Date::ConvertDate(s.second[0].GetText());
									sentence.state = Sentence::State::edited;
								}
								if (s.first == "deletedate"){
									sentence.updateDate = Date::ConvertDate(s.second[0].GetText());
									sentence.state = Sentence::State::deleted;
								}
								if (s.first == "speaker"){
									sentence.speaker = s.second[0].GetText();
								}
								if (s.first == "text"){
									sentence.text = s.second[0].GetText();
								}
								if (s.first == "note"){
									sentence.note = s.second[0].GetText();
								}
							}
							story.sentence.push_back(sentence);
						}
					}
				}
			}
		}

		cout << path << "��ǂݍ��݂܂����B" << endl;
		ret = 0;
	}

	return ret;
}

bool DataManager::SaveChara(){
	static const string path = common::DatRootPath() + "/" + common::CharaListFile();

	CreateDirectory(path);
	ofstream ofs(path);
	if (ofs.fail()){
		cout << path << "�̕ۑ��Ɏ��s���܂����B" << endl;
		return 1;
	}

	for (auto p : charaList){
		switch (p.second){
		case common::CharaType::main:
			ofs << "<main>" << p.first << "</main>" << endl;
			break;
		case common::CharaType::sub:
			ofs << "<sub>" << p.first << "</sub>" << endl;
			break;
		case common::CharaType::ignore:
			ofs << "<ignore>" << p.first << "</ignore>" << endl;
			break;
		}
	}

	cout << path << "��ۑ����܂����B" << endl;
	return 0;
}

bool DataManager::SaveUpdate(){
	static const string path = common::DatRootPath() + "/" + common::UpdateListFile();

	CreateDirectory(path);
	ofstream ofs(path);
	if (ofs.fail()){
		cout << path << "�̕ۑ��Ɏ��s���܂����B" << endl;
		return 1;
	}

	for (auto p : updateList){
		ofs << "<file>" << endl;
		ofs << "<path>" << p.first << "</path>" << endl;
		ofs << "<update>" << p.second[common::UpdateDateType::updateDate].GetStr() << "</update>" << endl;
		ofs << "<final>" << p.second[common::UpdateDateType::finalDate].GetStr() << "</final>" << endl;
		ofs << "</file>" << endl;
	}

	cout << path << "��ۑ����܂����B" << endl;
	return 0;
}

bool DataManager::LoadChara(){
	charaList.clear();

	static const string path = common::DatRootPath() + "/" + common::CharaListFile();

	ifstream ifs(path);
	if (ifs.fail()){
		ofstream ofs(path);
	}
	else{
		TagData data(ifs);
		for (auto p : data.GetData()){
			if (p.first == "main"){
				for (auto q : p.second){
					charaList[q.GetText()] = common::CharaType::main;
				}
			}
			if (p.first == "sub"){
				for (auto q : p.second){
					charaList[q.GetText()] = common::CharaType::sub;
				}
			}
			if (p.first == "ignore"){
				for (auto q : p.second){
					charaList[q.GetText()] = common::CharaType::ignore;
				}
			}
		}
	}

	return true;
}

bool DataManager::LoadUpdate(){
	updateList.clear();

	static const string path = common::DatRootPath() + "/" + common::UpdateListFile();

	ifstream ifs(path);
	if (ifs.fail()){
		ofstream ofs(path);
	}
	else{
		TagData data(ifs);
		for (auto p : data.GetData()){
			if (p.first == "file"){
				for (auto q : p.second){
					string storyPath = "";
					map<common::UpdateDateType, Date> date;
					for (auto r : q.GetData()){
						if (r.first == "path"){
							storyPath = r.second[0].GetText();
						}
						if (r.first == "update"){
							date[common::UpdateDateType::updateDate] = Date::ConvertDate(r.second[0].GetText());
						}
						if (r.first == "final"){
							date[common::UpdateDateType::finalDate] = Date::ConvertDate(r.second[0].GetText());
						}

					}
					updateList[storyPath] = date;
				}
			}
		}
	}

	return true;
}

string DataManager::SearchStory(Story story){
	string ret = "";	// ������Ȃ���΋󕶎�

	// �ŏI�Z���e���X��������v����Z���e���X���������A���݂���΂��̃X�g�[���[���ƒf�肷��
	for (auto p : updateList){
		Date finalDate = p.second[common::UpdateDateType::finalDate];
		for (auto q : story.sentence){
			if (finalDate == q.makeDate){
				ret = p.first;
			}
		}
	}

	return ret;
}

bool DataManager::ConfirmUnknownStory(Date unknownUpdate){
	bool ret;
	vector<string> storyTitle;	// ���m�̃X�g�[���[����̍X�V�̃X�g�[���[��
	for (auto p : updateList){
		if (p.first == ""){
			continue;
		}
		if (unknownUpdate < p.second[common::UpdateDateType::updateDate]){
			storyTitle.push_back(p.first);
		}
	}
	cout << "���m�̃X�g�[���[��f��ł��܂���ł����B" << endl;
	if (storyTitle.size() > 0){
		cout << "* �ȉ��̊����X�g�[���[���A�ŏI�X�V���Â��ł��B" << endl;
		cout << "  ���̒��ɖ��m�̃X�g�[���[�Ǝv����^�C�g��������΁A" << endl;
		cout << "  ���łɍX�V���I�����Â��X�g�[���[�ł���\��������܂��B" << endl;
		cout << "  �Y���X�g�[���[���Ȃ��ꍇ�́A�ŏI�X�V����X�V�����ł���\��������܂��B" << endl;
		cout << "  ����������ɒ������΂��čăg���C���邱�Ƃ𐄏����܂��B" << endl;
		for (auto p : storyTitle){
			cout << "�w" << p << "�x";
		}
		cout << endl;
		cout << "* �Y���X�g�[���[���Ȃ���Β��f���A����Ζ����𐄏����܂��B" << endl;
	}
	else{
		cout << "* �����X�g�[���[�̂ǂ�����ŏI�X�V����Ȃ̂ŁA" << endl;
		cout << "  �ŏI�X�V����X�V�����ł���\��������܂��B" << endl;
		cout << "  �����𒆒f���A����������ɒ������΂��čăg���C���邱�Ƃ𐄏����܂��B" << endl;
	}
	cout << "�ǂ����܂����H�@1�c�����𒆒f�@2�c�������čĊJ" << endl;
	cout << " : ";
	string buf;
	cin >> buf;
	ret = (common::StoI(buf) == 2);
	return ret;
}

bool DataManager::MergeStory(Story dest){
	cout << dest.title << "�̃}�[�W���J�n���܂�..." << endl;

	story.title = dest.title;	// �^�C�g����dest�̂��̂���
	if (dest.summary != ""){
		story.summary = dest.summary;	// �v��͋�łȂ���Α���i�X�V���Ӗ�����j
	}

	// �L�����N�^�[�͘a�W���Ƃ���
	map<common::CharaType, set<string>> charaList;
	for (auto p : story.mainChara){
		charaList[common::CharaType::main].insert(p);
	}
	for (auto p : story.subChara){
		charaList[common::CharaType::sub].insert(p);
	}
	for (auto p : dest.mainChara){
		charaList[common::CharaType::main].insert(p);
	}
	for (auto p : dest.subChara){
		charaList[common::CharaType::sub].insert(p);
	}
	story.mainChara.clear();
	story.subChara.clear();
	for (auto p : charaList[common::CharaType::main]){
		story.mainChara.push_back(p);
	}
	for (auto p : charaList[common::CharaType::sub]){
		story.subChara.push_back(p);
	}

	bool updateFlg = false;	// �X�V����ł��������t���O
	// �Z���e���X�͍X�V���������ƂɃ}�[�W����
	for (auto destSentence : dest.sentence){
		// dest�̂����ŏI�X�V������̍X�V�݂̂������ΏۂƂ���
		if (updateList[story.title][common::UpdateDateType::updateDate] < destSentence.GetFinalUpdate()){
			// dest�ƈ�v����X�V�������Ȃ�������
			bool existFlg = false;	// �����Z���e���X�ƈ�v�����t���O
			for (auto &sourceSentence  : story.sentence){
				if (sourceSentence.makeDate == destSentence.makeDate){
					sourceSentence = destSentence;
					existFlg = true;
				}
			}
			// ��v����̂��Ȃ�����
			if (!existFlg){
				story.sentence.push_back(destSentence);
			}
			updateFlg = true;	// �ŏI�X�V����̂񂪈���Ȃ�������ʂ�ւ�
		}
	}

	if (updateFlg){
		cout << dest.title << "�̃}�[�W���������܂����B" << endl;
	}
	else{
		cout << dest.title << "���}�[�W���܂������A�ύX������܂���ł����B" << endl;
	}
	return updateFlg;
}

bool DataManager::SaveTxtDetail(string path){
	CreateDirectory(path);
	ofstream ofs(path);
	if (ofs.fail()){
		cout << path << "�̕ۑ��Ɏ��s���܂����B" << endl;
		return false;
	}

	cout << path << "���쐬��..." << endl;

	ofs << "�y�^�C�g���z" << endl;
	ofs << story.title << endl;
	ofs << endl;

	if (story.summary != ""){
		ofs << "�y�T�v�z" << endl;
		ofs << story.summary << endl;
		ofs << endl;
	}

	if (story.mainChara.size() > 0){
		ofs << "�y�o�ꃁ�C���L�����z" << endl;
		bool firstFlg = false;
		for (auto p : story.mainChara){
			ofs << (!firstFlg ? "" : ", ") << p;
			firstFlg = true;
		}
		ofs << endl << endl;
	}

	if (story.subChara.size() > 0){
		ofs << "�y�o��T�u�L�����z" << endl;
		bool firstFlg = false;
		for (auto p : story.subChara){
			ofs << (!firstFlg ? "" : ", ") << p;
			firstFlg = true;
		}
		ofs << endl << endl;
	}

	const string border = "------------------------------------------------\n";
	ofs << border;
	for (auto p : story.sentence){
		if (p.text == "" && p.note == ""){
			continue;
		}
		ofs << "������ : " << p.speaker << endl;
		ofs << "�������� : " << p.makeDate.GetStr() << endl;
		ofs << (p.state == Sentence::State::edited ? "�ҏW���� : " : "");
		ofs << (p.state == Sentence::State::deleted ? "�폜���� : " : "");
		if (p.state != Sentence::State::normal){
			ofs << p.updateDate.GetStr() << endl;
		}
		ofs << endl;

		if (p.text != ""){
			stringstream ss(p.text);
			while (!ss.eof()){
				string buf;
				getline(ss, buf);
				if (buf != ""){
					ofs << buf << endl;
				}
			}
			ofs << endl;
		}

		if (p.note != ""){
			ofs << "�y";
			stringstream ss(p.note);
			bool headerFlg = false;
			while (!ss.eof()){
				string buf;
				getline(ss, buf);
				if (buf != ""){
					if (headerFlg){
						ofs << endl;
					}
					else{
						headerFlg = true;
					}
					ofs << buf;
				}
			}
			ofs << "�z" << endl;
			ofs << endl;
		}
		ofs << border;
	}

	cout << path << "��ۑ����܂����B" << endl;
	return true;
}

bool DataManager::SaveTxtStory(string path){
	CreateDirectory(path);
	ofstream ofs(path);
	if (ofs.fail()){
		cout << path << "�̕ۑ��Ɏ��s���܂����B" << endl;
		return false;
	}

	cout << path << "���쐬��..." << endl;
	ofs << "�y" << story.title << "�z" << endl;
	if (story.summary != ""){
		ofs << story.summary << endl;
	}
	ofs << endl;

	ofs << "�y�{���z" << endl;
	for (auto p : story.sentence){
		if (p.state != Sentence::State::deleted){
			stringstream ss(p.text);
			while (!ss.eof()){
				string buf;
				getline(ss, buf);
				if (buf != ""){
					ofs << buf << endl;
				}
			}
		}
	}

	cout << path << "��ۑ����܂����B" << endl;
	return true;
}

map<string, common::CharaType> DataManager::GetCharaList(){
	return charaList;
}

map<string, map<common::UpdateDateType, Date>> DataManager::GetUpdateList(){
	return updateList;
}

void DataManager::SetCharaList(map<string, common::CharaType> charaList){
	this->charaList = charaList;
}

void DataManager::SetUpdateList(map<string, map<common::UpdateDateType, Date>> updateList){
	this->updateList = updateList;
}

void DataManager::SetStory(Story story){
	this->story = story;
}