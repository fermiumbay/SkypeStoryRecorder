#include "DataConverter.h"

DataConverter::DataConverter(function<string()> GetText){
	data = GetText();
	successFlg = (data != "");
}

DataConverter::~DataConverter(){
}

Date DataConverter::GetDate(string text, Date beforeDate){
	// �X�V�O���t�̏ȗ��F�����̓��t
	// �X�V����t�̏ȗ��F�X�V�O���t�̓���or����
	smatch match;
	Date ret;
	int border = text.find(" ");
	vector<string> split;
	if (border != string::npos){
		split = common::Split(text.substr(0, border), '/');
		ret.year = common::StoI(split[0]);
		ret.month = common::StoI(split[1]);
		ret.day = common::StoI(split[2]);
		split.clear();
		split = common::Split(text.substr(border + 1, text.size() - (border + 1)), ':');
	}
	else{
		if (beforeDate == Date()){
			auto now = Date::GetNowTime();
			ret.year = now.year;
			ret.month = now.month;
			ret.day = now.day;
		}
		split = common::Split(text, ':');
	}
	ret.hour = common::StoI(split[0]);
	ret.minute = common::StoI(split[1]);
	ret.second = common::StoI(split[2]);

	if (border == string::npos && beforeDate != Date()){
		ret.year = beforeDate.year;
		ret.month = beforeDate.month;
		ret.day = beforeDate.day;
		if (ret < beforeDate){
			ret.day++;
		}
	}
	return ret;
}

vector<Sentence> DataConverter::ExtractSentences(){
	vector<Sentence> ret;
	stringstream ss;
	ss << data;

	Sentence sentence;	// ��荞�ރZ���e���X
	bool memoFlg = false;	// �u***�v����n�܂��āu***�v�ŏI��郁���������ꍇ�I��
	bool commentFlg = false;	// �u�y�v����n�܂��āu�z�v�ŏI��郁���������ꍇ�I��

	cout << "���̓f�[�^�̉�͒��ł�..." << endl;

	stringstream sstmp;
	sstmp << data;
	int maxline = 0;
	while (!sstmp.eof()){
		string buf;
		getline(sstmp, buf);
		maxline++;
	}

	cout << "�s�� : " << maxline << endl;

	cout << "  ";

	int counter = 0;
	int countRate = 0;
	bool convertStartFlg = false;	// �ŏ��̓��t������āA�R���o�[�g���J�n����t���O
	while(!ss.eof()){
		string buf, tmp;
		getline(ss, buf);

		// ��͒��̕\��
		counter++;
		for (; counter * 100 / maxline > countRate; countRate++){
			cout << "��";
			if ((countRate + 1) % 10 == 0){
				cout << " " << (countRate + 1) << "% ok" << endl;
				if (countRate + 1 != 100){
					cout << "  ";
				}
			}
		}

		// �擪�����t���ǂ����ŕ���
		if(regex_search(buf, regex("^\\[.*\\] .*"))){
			convertStartFlg = true;	// ���t�����Ă���A����t�̃e�L�X�g��ǂݎn�߂�

			if (sentence.makeDate != Date()){
				ret.push_back(sentence);
				sentence = Sentence();
			}

			int index1 = buf.find("[");
			int index2 = buf.find("]") + 1;
			string date = buf.substr(index1, index2 - index1);

			smatch match;

			if (regex_search(date, regex("\\[.* \\| .*��ҏW���܂���\\]"))){
				sentence.state = Sentence::State::edited;

				regex_search(date, match, regex("\\[.* \\|"));
				tmp = *match.begin();
				sentence.makeDate = GetDate(tmp.substr(1, tmp.size() - 3));
				
				regex_search(date, match, regex("\\| .*��"));
				tmp = *match.begin();
				sentence.updateDate = GetDate(tmp.substr(2, tmp.size() - 4), sentence.makeDate);
			}
			else if (regex_search(date, regex("\\[.* \\| .*���폜���܂���\\]"))){
				sentence.state = Sentence::State::deleted;

				regex_search(date, match, regex("\\[.* \\|"));
				tmp = *match.begin();
				sentence.makeDate = GetDate(tmp.substr(1, tmp.size() - 3));

				regex_search(date, match, regex("\\| .*��"));
				tmp = *match.begin();
				sentence.updateDate = GetDate(tmp.substr(2, tmp.size() - 4), sentence.makeDate);
			}
			else{
				sentence.state = Sentence::State::normal;

				regex_search(date, match, regex("\\[.*\\]"));
				tmp = *match.begin();
				sentence.makeDate = GetDate(tmp.substr(1, tmp.size() - 2));
			}

			regex_search(buf, match, regex("\\] "));
			buf = buf.substr(match.position() + 2, buf.size() - (match.position() + 2));

			memoFlg = (buf.substr(0, 4) == "*** ");
			if (memoFlg){
				buf = buf.substr(4, buf.size() - 4);
			}
			else{
				// �����ҋL�^
				int pos = buf.find(":");
				sentence.speaker = buf.substr(0, pos);
				buf = buf.substr(pos + 2, buf.size() - (pos + 2));
			}
			commentFlg = false;
		}

		// �Ȍ�A���t�֌W�Ȃ��̋��ʕ���
		if (!convertStartFlg){
			// ���t���܂�����������ĂȂ��̂ɈȌ���X�g�[���[�Ƃ͌��Ȃ��Ȃ�
			continue;
		}

		// \r��������Ȍ����
		int pos = buf.find("\r");
		if (pos != string::npos){
			buf = buf.substr(0, pos);
		}

		if (memoFlg){
			int pos = buf.find(" ***");
			if (pos != string::npos){
				if (sentence.note != ""){
					sentence.note += "\n";
				}
				sentence.note += buf.substr(0, pos);
				memoFlg = false;
			}
			else{
				if (sentence.note != ""){
					sentence.note += "\n";
				}
				sentence.note += buf;
			}
		}
		else{
			while (buf != ""){

				// ���ߕ��y�z�ł���
				if (commentFlg){
					int pos = buf.find("�z");
					if (pos != string::npos){
						if (pos != 0){
							if (sentence.note != ""){
								sentence.note += "\n";
							}
							sentence.note += buf.substr(0, pos);
						}
						commentFlg = false;
						buf = buf.substr(pos + 2, buf.size() - (pos + 2));
					}
					else{
						if (sentence.note != ""){
							sentence.note += "\n";
						}
						sentence.note += buf;
						buf = "";
					}
				}
				else{
					int pos = buf.find("�y");
					if (pos != string::npos){
						if (pos != 0){
							if (sentence.text != ""){
								sentence.text += "\n";
							}
							sentence.text += buf.substr(0, pos);
						}
						commentFlg = true;
						buf = buf.substr(pos + 2, buf.size() - 2);
					}
					else{
						if (sentence.text != ""){
							sentence.text += "\n";
						}
						sentence.text += buf;
						buf = "";
					}
				}
			}
		}
	}

	if (sentence.makeDate != Date()){
		ret.push_back(sentence);
		sentence = Sentence();
	}

	//// �m�F�p
	//for (auto p : ret){
	//	cout << "���������F" << p.makeDate.GetStr() << endl;
	//	if (p.state == Sentence::State::edited){
	//		cout << "�ҏW�����F" << p.updateDate.GetStr() << endl;
	//	}
	//	else if (p.state == Sentence::State::deleted){
	//		cout << "�폜�����F" << p.updateDate.GetStr() << endl;
	//	}
	//	cout << "�����ҁF" << p.speaker << endl;
	//	cout << "�{���F" << p.text << endl;
	//	cout << "���߁F" << p.note << endl;
	//	cout << "-------------------------------------------------------" << endl;
	//}

	return ret;
}

bool DataConverter::IsSuccess(){
	return successFlg;
}
