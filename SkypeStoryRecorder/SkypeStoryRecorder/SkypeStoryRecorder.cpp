#include "SkypeStoryRecorder.h"

bool SkypeStoryRecorder::Run(){
	DataConverter dataConverter(GetClipText());	// クリップボードの文字列を対象にデータコンバート
	if (!dataConverter.IsSuccess()){
		cout << "クリップボードにテキストをコピーしてください。" << endl;
		return false;
	}
	vector<Sentence> sentence = dataConverter.ExtractSentences();	// コンバートされて出たセンテンスを取得

	DataManager dataManager;	// データの保存・読込を担うデータマネージャー
	dataManager.LoadChara();	// キャラ一覧を取得
	dataManager.LoadUpdate();	// 更新日時一覧を取得

	// センテンスデコーダーにデータマネージャーからキャラ一覧と更新日時一覧を渡させる
	SentenceDecoder sentenceDecoder(sentence);
	sentenceDecoder.SetCharaList(dataManager.GetCharaList());
	sentenceDecoder.SetUpdateList(dataManager.GetUpdateList());

	vector<Story> storyList = sentenceDecoder.CreateStoryList();	// ストーリー配列を取得する

	// 得られたストーリーを順に保存
	successNum = 0, failNum = 0, unMergeNum = 0;
	bool breakFlg = false;	// 処理を中断したフラグ
	for (auto p : storyList){
		dataManager.SetStory(p);

		if (p.title == ""){
			// タイトル不明（更新日時をもとに探索）
			cout << "未知のストーリーの探索を開始します..." << endl;
			string path = dataManager.SearchStory(p);
			if (path == ""){
				// 見つからなかった
				failNum++;
				if (!dataManager.ConfirmUnknownStory(sentenceDecoder.GetUpdateList()[""][common::UpdateDateType::updateDate])){
					breakFlg = true;
					break;
				}
			}
			else{
				// 見つかった
				cout << "未知のストーリーは『" << path << "』であると断定しました。" << endl;
				p.title = path;
				sentenceDecoder.EmptyUpdateToUpdateList(path);	// 空タイトルが判明したので、更新日時を推測したストーリーに置き換え
				bool failMergeFlg = false;	// マージで一個も一致せえへんかった
				if (dataManager.LoadStory(common::DatRootPath() + "/" + path + common::StoryFileExtension()) == 0){
					failMergeFlg = !dataManager.MergeStory(p);
				}
				else{
					dataManager.CreateStory(p);
				}
				SaveStory(path, &dataManager, failMergeFlg);
			}
		}
		else{
			// 既知タイトル
			bool failMergeFlg = false;	// マージで一個も一致せえへんかった
			if (dataManager.LoadStory(common::DatRootPath() + "/" + p.title + common::StoryFileExtension()) == 0){
				// 存在した：更新
				failMergeFlg = !dataManager.MergeStory(p);
			}
			else{
				// 存在しなかった：新規作成
				dataManager.CreateStory(p);
			}
			SaveStory(p.title, &dataManager, failMergeFlg);
		}
	}

	// データマネージャーに一覧を返して保存
	dataManager.SetCharaList(sentenceDecoder.GetCharaList());
	dataManager.SaveChara();	// キャラ一覧を保存
	if (!breakFlg){	// 処理を中断したら更新日時だけ保存をしない
		dataManager.SetUpdateList(sentenceDecoder.GetUpdateList());
		dataManager.SaveUpdate();	// 更新日時一覧を保存
	}

	cout << "* ";
	cout <<"正常終了: " << successNum;
	cout << ", 変更なし : " << unMergeNum;
	cout << ", 失敗 : " << failNum;
	cout << " *" << endl;
	bool ret = true;
	if (failNum > 0){
		cout << "　[失敗の原因例と解決策]" << endl;
		cout << "　・クリップボードに正常にコピーされてない？ ⇒ コピーを確認して再トライ" << endl;
		cout << "　・履歴が最終更新から離れている？ ⇒ 履歴をさらに長く延ばして再トライ" << endl;
		cout << "　・ストーリーの保存に失敗？ ⇒ エクスプローラの異常か 時間を空けて再トライ" << endl;
		cout << "　[失敗と誤認された場合の例]" << endl;
		cout << "　・すでに更新を終えており、古い履歴を入力した" << endl;
		cout << "　・ストーリーではない意図したテキストが先頭にあった" << endl;
		ret = false;
	}
	else if (successNum == 0){
		cout << "　更新の結果、変更がありませんでした。" << endl;
		ret = false;
	}
	return ret;
}

bool SkypeStoryRecorder::SaveStory(string title, DataManager *dataManager, bool failMergeFlg){
	if (dataManager->SaveStory(common::DatRootPath() + "/" + title + common::StoryFileExtension()) == 0){
		failMergeFlg ? unMergeNum++ : successNum++;
		if (txtStoryOutputFlg){
			dataManager->SaveTxtStory(common::StoryRootPath() + "/" + title + ".txt");
		}
		if (txtDetailOutputFlg){
			dataManager->SaveTxtDetail(common::StoryRootPath() + "/" + title + "_detail.txt");
		}
	}
	else{
		failNum++;
	}
	return true;
}

SkypeStoryRecorder::SkypeStoryRecorder(bool txtStoryOutputFlg, bool txtDetailOutputFlg){
	cout << "[SkypeStoryRecorder ver1.01]" << endl;
	this->txtStoryOutputFlg = txtStoryOutputFlg;
	this->txtDetailOutputFlg = txtDetailOutputFlg;
}

SkypeStoryRecorder::~SkypeStoryRecorder(){
}

function<string()> SkypeStoryRecorder::GetClipText(){
	return[](){
		string ret = "";

		OpenClipboard(NULL);

		HANDLE hText = GetClipboardData(CF_TEXT);
		if (hText != NULL) {
			ret = static_cast<string>((char *)GlobalLock(hText));
			GlobalUnlock(hText);
		}

		CloseClipboard();
		return ret;
	};
}


