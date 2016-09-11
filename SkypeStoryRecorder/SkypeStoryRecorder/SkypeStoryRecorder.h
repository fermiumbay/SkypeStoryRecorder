#pragma once
#include "common.h"
#include "Date.h"
#include "DataConverter.h"
#include "DataManager.h"
#include "SentenceDecoder.h"
#include <Windows.h>

// スカイプ物語レコーダー
class SkypeStoryRecorder{
	bool txtStoryOutputFlg;	// 本文txt出力を同時に行うフラグ
	bool txtDetailOutputFlg;	// 詳細txt出力を同時に行うフラグ
	int successNum, failNum, unMergeNum;	// 処理の成功数、失敗数、更新なし数
	function<string()> GetClipText();	// クリップボードのテキストを取得する

	bool SaveStory(string title, DataManager *dataManager, bool failMergeFlg);	// ストーリーの保存

	SkypeStoryRecorder();
	SkypeStoryRecorder(const SkypeStoryRecorder&);
	SkypeStoryRecorder& operator=(const SkypeStoryRecorder&);
public:
	SkypeStoryRecorder(bool txtStoryOutputFlg, bool txtDetailOutputFlg);
	~SkypeStoryRecorder();
	bool Run();	// 実行（true／falseで正常終了／異常終了）
};
