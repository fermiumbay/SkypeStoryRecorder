#pragma once
#include "common.h"

// タグ管理クラス
class TagData{
	string text;	// テキストデータ
	vector<pair<string, vector<TagData>>> data;	// 所持しているタグリスト

	string TagData::GetTag(string buf);	// 先頭が「<」の文字列の先頭タグを取り出す
	void AddData(string tag, TagData addData);	// データの追加
	void AddData(string tag, string text);	// テキストを含んだデータの追加
	void AddText(string text);	// テキストの追加

	TagData(string s = "");
public:
	vector<pair<string, vector<TagData>>> GetData();	// データの取得
	string GetText();	// テキストの取得

	TagData(ifstream &ifs);	// 入力ストリームからテキストを解読してタグデータとして出力
	TagData(const TagData&);
	TagData& operator=(const TagData&);
	~TagData(){}
};

