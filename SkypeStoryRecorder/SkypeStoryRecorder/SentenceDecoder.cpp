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

	set<string> mainChara;	// メインキャラ集合
	set<string> subChara;	// サブキャラ集合
	set<string> reserveChara;	// 保留

	Date finalDate, updateDate;	// 最終センテンス更新、最終更新

	// ストーリー名の取得
	auto GetStoryTitleString = [](string title, bool unknownStoryFlg){
		string ret;
		if (unknownStoryFlg){
			ret = "未知のストーリー";
		}
		else{
			ret = "ストーリー『" + title + "』";
		}
		return ret;
	};

	// 条件を満たしていればただの代入、そうでなければ大きい方を代入
	auto Substitute = [](bool flg, Date &source, Date dest){
		if (flg){
			common::Substitute(source, dest);
		}
		else{
			common::SubstituteMax(source, dest);
		}
	};

	cout << "入力センテンスから情報を取得しています..." << endl;

	bool unknownStoryFlg = false;	// 未知のストーリーが存在した

	int maxsentence = this->sentence.size();
	cout << "センテンス総数 : " << maxsentence << endl;

	int counter = 0;
	int countRate = 0;
	for (auto p : this->sentence){
		stringstream ss(p.text);
		p.text = "";
		bool speakingFlg = false;	// 話者が存在しているフラグ
		bool summaryFlg = false;	// 要約編集中フラグ

		counter++;
		for (; counter * 100 / maxsentence > countRate; countRate++){
			if ((countRate + 1) % 10 == 0){
				cout << " : " << (countRate + 1) << "% 完了" << endl;
			}
		}

		while (!ss.eof()){
			string buf;	// センテンスの一行
			getline(ss, buf);

			bool readTextFlg = false;	// テキスト本文を読み始めたフラグ
			while (buf != ""){
				if (summaryFlg){
					// 要約編集中
					if (buf == common::GetEndSummaryCommand()){
						// 要約終了
						summaryFlg = false;
					}
					else{
						// 要約追加
						story.summary += buf + "\n";
					}
					buf = "";
				}
				else{
					// 要約以外
					if (regex_match(buf, regex(common::GetNewStoryCommand() + ".*"))){
						// 新規ストーリー
						if (story.title != "" || unknownStoryFlg){
							// これまで取得してきたのは何かのストーリーだったのでretにpushする
							for (auto q : mainChara){
								story.mainChara.push_back(q);
							}
							for (auto q : subChara){
								story.subChara.push_back(q);
							}
							ret.push_back(story);
							cout << GetStoryTitleString(story.title, unknownStoryFlg) << "の更新を完了しました。" << endl;
							mainChara.clear();
							subChara.clear();

							// 空タイトルならそのまま代入、そうでなければ最新の日時の方を選択
							Substitute(story.title == "", updateList[story.title][common::UpdateDateType::finalDate], finalDate);
							Substitute(story.title == "", updateList[story.title][common::UpdateDateType::updateDate], updateDate);

							finalDate = Date();
							updateDate = Date();

							story = Story();
						}
						int index = common::GetNewStoryCommand().size();
						story.title = buf.substr(index);
						unknownStoryFlg = false;
						cout << GetStoryTitleString(story.title, unknownStoryFlg) << "を発見しました。" << endl;
						buf = "";
					}
					else if (buf == common::GetSummaryCommand()){
						// 要約開始
						story.summary = "";	// 要約が増えるのを防ぐため、初期化
						summaryFlg = true;
						buf = "";
					}
					else{
						// その他 ： キャラクター名を検索して、リストに追加するキャラ名を選定
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
								// 無視キャラは無視する
								break;
							}
						}
					}
				}
			}
		}
		story.sentence.push_back(p);	// センテンスをストーリーに追加
		if (story.title == "" && !unknownStoryFlg){
			unknownStoryFlg = true;	// ストーリー名が不明のままだった場合、不明フラグを立てる
		}

		finalDate = p.makeDate;	// 最終センテンス日時は「作成日時」を順次更新
		common::SubstituteMax(updateDate, p.GetFinalUpdate());	// 最終更新日は最新のものだけ取り出せるように
	}

	// 最後に残ったストーリーをpush
	for (auto q : mainChara){
		story.mainChara.push_back(q);
	}
	for (auto q : subChara){
		story.subChara.push_back(q);
	}
	ret.push_back(story);
	cout << GetStoryTitleString(story.title, unknownStoryFlg) << "の更新を完了しました。" << endl;

	Substitute(story.title == "", updateList[story.title][common::UpdateDateType::finalDate], finalDate);
	Substitute(story.title == "", updateList[story.title][common::UpdateDateType::updateDate], updateDate);

	finalDate = Date();
	updateDate = Date();

	cout << "取得完了しました。" << endl;

	return ret;
}

void SentenceDecoder::EmptyUpdateToUpdateList(string path){
	// 空タイトルがあれば
	if (updateList.count("") > 0){
		// 空タイトルか推測済みタイトルか大きい方を更新日時として記録
		// 空タイトルの方が大きい：後に推測タイトルが登場しなかったので、空タイトルのものを代入できる
		// 空タイトルの方が小さい：後に推測タイトルが登場しているので、最新なのは推測タイトルの方
		common::SubstituteMax(updateList[path][common::UpdateDateType::finalDate], updateList[""][common::UpdateDateType::finalDate]);
		common::SubstituteMax(updateList[path][common::UpdateDateType::updateDate], updateList[""][common::UpdateDateType::updateDate]);
	}
}

map<common::CharaType, string> SentenceDecoder::InsertChara(string &buf, bool &speakingFlg, set<string> reserveChara){
	map<common::CharaType, string> ret;	// 挿入するキャラクター名を格納する

	if (!speakingFlg){
		// 話者がまだ見つかってない
		if (regex_match(buf, regex(".*(「|（).*"))){
			int index;

			int index1 = buf.find("「");
			int index2 = buf.find("（");
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
				// 保留になったキャラ以外を登録対象とする
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
		// 話者が見つかっている
		if (regex_match(buf, regex(".*(」|）).*"))){
			int index;

			int index1 = buf.find("」");
			int index2 = buf.find("）");
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
		cout << "* 未登録 : " << chara << endl;
		cout << "（-1:全部保留　0:保留　1:メイン　2:サブ　3:無視）" << endl;
		cout << " : ";
		string stype;
		cin >> stype;
		ret = common::StoI(stype);
	}

	switch (ret){
	case 1:
		charaList[chara] = common::CharaType::main;
		cout << "「" << chara << "」をメインキャラクターとして登録しました。" << endl;
		break;
	case 2:
		charaList[chara] = common::CharaType::sub;
		cout << "「" << chara << "」をサブキャラクターとして登録しました。" << endl;
		break;
	case 3:
		charaList[chara] = common::CharaType::ignore;
		cout << "「" << chara << "」を今後無視することにしました。" << endl;
		break;
	default:
		if (ret == -1){
			unregisterFlg = true;
		}
		ret = 0;
		cout << "「" << chara << "」の登録を保留しました。" << endl;
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
