#pragma once
#include "Sentence.h"

// ストーリー構造体
struct Story{
	string title,	// ストーリータイトル
		summary;	// 概要
	vector<string> mainChara,	// メインキャラ名
		subChara;	// サブキャラ名
	vector<Sentence> sentence;	// センテンス配列
	Story() :
		title(""),
		summary("")
	{
		mainChara.clear();
		subChara.clear();
		sentence.clear();
	}

	bool operator==(Story s){
		return title == s.title
			&& summary == s.summary
			&& mainChara == s.mainChara
			&& subChara == s.subChara
			&& common::EqualVector(sentence, s.sentence);
	}
};