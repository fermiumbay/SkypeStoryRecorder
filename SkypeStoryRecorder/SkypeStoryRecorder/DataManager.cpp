#include "DataManager.h"

DataManager::DataManager(){
}

DataManager::~DataManager(){
}

void DataManager::CreateStory(Story story){
	this->story = story;
	cout << story.title << "を新規作成しました。" << endl;
}

bool DataManager::CreateDirectory(string path){
	// フォルダを作成しながらストーリーを保存
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
		cout << path << "の保存に失敗しました。" << endl;
		return 1;
	}

	cout << path << "の保存中です..." << endl;

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

	cout << path << "を保存しました。" << endl;
	return 0;
}

bool DataManager::LoadStory(string path){
	bool ret;
	story = Story();
	ifstream ifs(path);
	if (ifs.fail()){
		cout << path << "の読込に失敗しました。" << endl;
		ret = 1;
	}
	else{
		cout << path << "の読み込み中です..." << endl;

		// 入力テキストからタグデータへの変換
		TagData data(ifs);

		// 得られたタグデータをもとにストーリーデータ格納
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

		cout << path << "を読み込みました。" << endl;
		ret = 0;
	}

	return ret;
}

bool DataManager::SaveChara(){
	static const string path = common::DatRootPath() + "/" + common::CharaListFile();

	CreateDirectory(path);
	ofstream ofs(path);
	if (ofs.fail()){
		cout << path << "の保存に失敗しました。" << endl;
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

	cout << path << "を保存しました。" << endl;
	return 0;
}

bool DataManager::SaveUpdate(){
	static const string path = common::DatRootPath() + "/" + common::UpdateListFile();

	CreateDirectory(path);
	ofstream ofs(path);
	if (ofs.fail()){
		cout << path << "の保存に失敗しました。" << endl;
		return 1;
	}

	for (auto p : updateList){
		ofs << "<file>" << endl;
		ofs << "<path>" << p.first << "</path>" << endl;
		ofs << "<update>" << p.second[common::UpdateDateType::updateDate].GetStr() << "</update>" << endl;
		ofs << "<final>" << p.second[common::UpdateDateType::finalDate].GetStr() << "</final>" << endl;
		ofs << "</file>" << endl;
	}

	cout << path << "を保存しました。" << endl;
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
	string ret = "";	// 見つからなければ空文字

	// 最終センテンス日時が一致するセンテンスを検索し、存在すればそのストーリーだと断定する
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
	vector<string> storyTitle;	// 未知のストーリーより後の更新のストーリー名
	for (auto p : updateList){
		if (p.first == ""){
			continue;
		}
		if (unknownUpdate < p.second[common::UpdateDateType::updateDate]){
			storyTitle.push_back(p.first);
		}
	}
	cout << "未知のストーリーを断定できませんでした。" << endl;
	if (storyTitle.size() > 0){
		cout << "* 以下の既存ストーリーより、最終更新が古いです。" << endl;
		cout << "  この中に未知のストーリーと思われるタイトルがあれば、" << endl;
		cout << "  すでに更新を終えた古いストーリーである可能性があります。" << endl;
		cout << "  該当ストーリーがない場合は、最終更新から更新が飛んでいる可能性があります。" << endl;
		cout << "  履歴をさらに長く延ばして再トライすることを推奨します。" << endl;
		for (auto p : storyTitle){
			cout << "『" << p << "』";
		}
		cout << endl;
		cout << "* 該当ストーリーがなければ中断を、あれば無視を推奨します。" << endl;
	}
	else{
		cout << "* 既存ストーリーのどれよりも最終更新が後なので、" << endl;
		cout << "  最終更新から更新が飛んでいる可能性があります。" << endl;
		cout << "  処理を中断し、履歴をさらに長く延ばして再トライすることを推奨します。" << endl;
	}
	cout << "どうしますか？　1…処理を中断　2…無視して再開" << endl;
	cout << " : ";
	string buf;
	cin >> buf;
	ret = (common::StoI(buf) == 2);
	return ret;
}

bool DataManager::MergeStory(Story dest){
	cout << dest.title << "のマージを開始します..." << endl;

	story.title = dest.title;	// タイトルはdestのものを代入
	if (dest.summary != ""){
		story.summary = dest.summary;	// 要約は空でなければ代入（更新を意味する）
	}

	// キャラクターは和集合とする
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

	bool updateFlg = false;	// 更新が一つでもあったフラグ
	// センテンスは更新日時をもとにマージする
	for (auto destSentence : dest.sentence){
		// destのうち最終更新日より後の更新のみを処理対象とする
		if (updateList[story.title][common::UpdateDateType::updateDate] < destSentence.GetFinalUpdate()){
			// destと一致する更新日時がないか検索
			bool existFlg = false;	// 既存センテンスと一致したフラグ
			for (auto &sourceSentence  : story.sentence){
				if (sourceSentence.makeDate == destSentence.makeDate){
					sourceSentence = destSentence;
					existFlg = true;
				}
			}
			// 一致するのがなかった
			if (!existFlg){
				story.sentence.push_back(destSentence);
			}
			updateFlg = true;	// 最終更新より後のんが一個もなかったら通らへん
		}
	}

	if (updateFlg){
		cout << dest.title << "のマージが完了しました。" << endl;
	}
	else{
		cout << dest.title << "をマージしましたが、変更がありませんでした。" << endl;
	}
	return updateFlg;
}

bool DataManager::SaveTxtDetail(string path){
	CreateDirectory(path);
	ofstream ofs(path);
	if (ofs.fail()){
		cout << path << "の保存に失敗しました。" << endl;
		return false;
	}

	cout << path << "を作成中..." << endl;

	ofs << "【タイトル】" << endl;
	ofs << story.title << endl;
	ofs << endl;

	if (story.summary != ""){
		ofs << "【概要】" << endl;
		ofs << story.summary << endl;
		ofs << endl;
	}

	if (story.mainChara.size() > 0 || story.subChara.size() > 0){
		ofs << "【登場メインキャラ】" << endl;
		bool firstFlg = false;
		for (auto p : story.mainChara){
			ofs << (!firstFlg ? "" : ", ") << p;
			firstFlg = true;
		}
		ofs << endl << endl;
		ofs << "【登場サブキャラ】" << endl;
		firstFlg = false;
		for (auto p : story.subChara){
			ofs << (!firstFlg ? "" : ", ") << p;
			firstFlg = true;
		}
		ofs << endl << endl;
	}

	const string border = "------------------------------------------------\n";
	ofs << border;
	for (auto p : story.sentence){
		ofs << "発言者 : " << p.speaker << endl;
		ofs << "発言日時 : " << p.makeDate.GetStr() << endl;
		ofs << (p.state == Sentence::State::edited ? "編集日時 : " : "");
		ofs << (p.state == Sentence::State::deleted ? "削除日時 : " : "");
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
			ofs << "【";
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
			ofs << "】" << endl;
			ofs << endl;
		}
		ofs << border;
	}

	cout << path << "を保存しました。" << endl;
	return true;
}

bool DataManager::SaveTxtStory(string path){
	CreateDirectory(path);
	ofstream ofs(path);
	if (ofs.fail()){
		cout << path << "の保存に失敗しました。" << endl;
		return false;
	}

	cout << path << "を作成中..." << endl;
	ofs << "【" << story.title << "】" << endl;
	if (story.summary != ""){
		ofs << story.summary << endl;
	}
	ofs << endl;

	ofs << "【本文】" << endl;
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

	cout << path << "を保存しました。" << endl;
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