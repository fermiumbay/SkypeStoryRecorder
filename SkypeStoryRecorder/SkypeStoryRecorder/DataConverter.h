#pragma once
#include "common.h"
#include "Date.h"
#include "Sentence.h"

// 入力データのセンテンス変換機構
class DataConverter{
	string data;	// 入力テキストデータ
	bool successFlg;	// 入力読み込み成功フラグ

	Date GetDate(string text, Date beforeDate = Date());	// 日付文字列から日付を取得する（更新後の日時の場合は第二引数に更新前日時を代入）

	DataConverter();
	DataConverter(const DataConverter&);
	DataConverter& operator=(const DataConverter&);
public:
	DataConverter(function<string()> GetText);	// 入力の取得方法を引数とする
	bool IsSuccess();	// 読み込み成功したか
	vector<Sentence> ExtractSentences();	// センテンスを取り出して出力
	~DataConverter();
};
