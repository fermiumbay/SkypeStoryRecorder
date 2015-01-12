#pragma once
#include "common.h"
#include "Date.h"
#include "Story.h"
#include "TagData.h"
#include <direct.h>

// データのセーブ・ロードを担う
class DataManager{
	Story story;	// ストーリーデータ
	map<string, common::CharaType> charaList;	// 登録されているキャラリスト
	map<string, map<common::UpdateDateType, Date>> updateList;	// 登録されている最終更新日リスト

	bool CreateDirectory(string path);	// 与えられたフルパスをもとに必要であればディレクトリを作成する

	DataManager(const DataManager&);
	DataManager& operator=(const DataManager&);
public:
	void CreateStory(Story story);	// ストーリーの作成
	bool SaveStory(string path);	// セーブファイルパスを入力すると物語ファイルを保存
	bool SaveStory(string path, Story story);	// セーブファイルパスとストーリーデータを入力すると物語ファイルを保存
	bool LoadStory(string path);	// セーブファイルパスを入力すると物語ファイルをロード
	bool SaveChara();	// キャラクター一覧ファイルのセーブ
	bool SaveUpdate();	// 最終更新一覧ファイルのセーブ
	bool LoadChara();	// キャラクター一覧ファイルのロード
	bool LoadUpdate();	// 最終更新一覧ファイルのロード
	string SearchStory(Story story);	// ストーリーを検索、見つからなければ空文字を返す
	bool MergeStory(Story dest);	// 二つのストーリーをマージする（一つ以上の更新があったらtrue、更新がなければfalse）
	bool ConfirmUnknownStory(Date unknownUpdate);	// 発見できなかった未知のストーリーをどうするか？（無視ならtrue、中断するならfalse）
	map<string, common::CharaType> GetCharaList();
	map<string, map<common::UpdateDateType, Date>> GetUpdateList();
	void SetCharaList(map<string, common::CharaType> charaList);
	void SetUpdateList(map<string, map<common::UpdateDateType, Date>> updateList);
	void SetStory(Story story);
	bool SaveTxtStory(string path);	// 現在のストーリー本文をtxtに変換して出力
	bool SaveTxtDetail(string path);	// 現在のストーリーの詳細情報をtxtに変換して出力

	DataManager();
	~DataManager();
};

