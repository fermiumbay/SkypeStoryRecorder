#pragma once
#include "common.h"
#include "Date.h"
#include "Story.h"
#include "TagData.h"

// センテンスを解読してストーリーごとに分離する機械
class SentenceDecoder{
	vector<Sentence> sentence;	// センテンス配列
	map<string, common::CharaType> charaList;	// 登録されているキャラリスト
	map<string, map<common::UpdateDateType, Date>> updateList;	// 登録されている最終更新日リスト
	bool unregisterFlg;	// キャラクター登録を全保留にするフラグ

	map<common::CharaType, string> InsertChara(string &buf, bool &speakingFlg, set<string> reserveChara);	// キャラクター名の挿入
	inline common::CharaType CheckCharaType(string chara);	// 引数がすでに登録されているキャラならキャラタイプを、そうでなければnoneを返す
	common::CharaType RegisterChara(string chara);	// キャラクターの新規登録

	SentenceDecoder();
	SentenceDecoder(const SentenceDecoder&);
	SentenceDecoder& operator=(const SentenceDecoder&);
public:
	vector<Story> CreateStoryList();	// ストーリー配列を取り出して取得する
	void EmptyUpdateToUpdateList(string path);	// 空タイトルの更新リストを推測できたタイトルリストに変更する
	map<string, common::CharaType> GetCharaList();
	map<string, map<common::UpdateDateType, Date>> GetUpdateList();
	void SetCharaList(map<string, common::CharaType> charaList);
	void SetUpdateList(map<string, map<common::UpdateDateType, Date>> updateList);

	SentenceDecoder(vector<Sentence> sentence);
	~SentenceDecoder();
};

