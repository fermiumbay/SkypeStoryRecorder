#pragma once
#include "Date.h"
#include <string>
using namespace std;

// センテンス構造体
struct Sentence{
	enum State{
		normal,	// 通常
		edited,	// 編集済み
		deleted,	// 削除済み
	};

	Date makeDate,	// 発言日時
		updateDate;	// 編集・削除日時
	string speaker,	// 発言者
		text,	// テキスト本文
		note;	// 注釈文
	State state;	// センテンスの状態
	bool ignoreDateFlg;	// 日付更新しないフラグ

	Sentence() :
		makeDate(Date()),
		updateDate(Date()),
		speaker(""),
		text(""),
		note(""),
		state(State::normal),
		ignoreDateFlg(false)
		{}

	// センテンスの最終的な更新日時を取得
	Date GetFinalUpdate(){
		// 編集・削除されていればその日時、そうでなければ発言日時を返す
		return state == State::normal ? makeDate : updateDate;
	}

	bool operator==(Sentence s){
		return makeDate == s.makeDate
			&& updateDate == s.updateDate
			&& speaker == s.speaker
			&& text == s.text
			&& note == s.note
			&& state == s.state
			&& ignoreDateFlg == s.ignoreDateFlg;
	}
};
