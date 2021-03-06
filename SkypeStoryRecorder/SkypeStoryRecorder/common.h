#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <functional>
#include <vector>
#include <map>
#include <set>
#include <regex>
using namespace std;

// 共通ヘッダ
class common{
	common();
	~common();
	common(const common&);
	common& operator=(const common&);
public:
	// 最終更新日のタイプ
	enum UpdateDateType{
		updateDate,	// ファイル全体の最終更新日
		finalDate,	// 最終センテンスの最終更新日
	};

	// キャラクターのタイプ
	enum CharaType{
		none = 0,	// 存在しない
		main,	// メイン
		sub,	// サブ
		ignore,	// 無視
	};

	static const string DatRootPath(){ return "dat"; }	// ストーリーデータファイルのルートパス名
	static const string StoryRootPath(){ return "story"; }	// ストーリーのテキストのルートパス名
	static const string StoryFileExtension(){ return ".story"; }	// ストーリーファイルの拡張子
	static const string CharaListFile(){ return "chara.dat"; }	// キャラ一覧ファイル名
	static const string UpdateListFile(){ return "update.dat"; }	// 更新一覧ファイル名
	static const string GetNewStoryCommand(){ return "［SkypeStoryRecorder : Title］"; }	// 新規ストーリー作成コマンド
	static const string GetSummaryCommand(){ return "［SkypeStoryRecorder : Summary］"; }	// 要約編集コマンド
	static const string GetEndSummaryCommand(){ return "［SkypeStoryRecorder : EndSummary］"; }	// 要約編集終了コマンド
	static const string GetIgnoreDateCommand(){ return "［SkypeStoryRecorder : IgnoreDate］"; }	// このセンテンスは日付更新しないコマンド
	static const regex GetURLRegex(){ return regex("(https?|ftp)(:\\/\\/[-_.!~*\\'()a-zA-Z0-9;\\/?:\\@&=+\\$,%#]+)"); }	// URLと判定する正規表現（http://phpspot.net/php/pg%E6%AD%A3%E8%A6%8F%E8%A1%A8%E7%8F%BE%EF%BC%9A%E6%AD%A3%E3%81%97%E3%81%84%EF%BC%B5%EF%BC%B2%EF%BC%AC%E3%81%8B%E3%81%A9%E3%81%86%E3%81%8B%E8%AA%BF%E3%81%B9%E3%82%8B.html）

	static string ItoS(int value);	// 整数を文字列に変換
	static string ItoS(int value, int num);	// 整数を文字列に変換（第二引数で桁数指定）
	static int StoI(string str);	// 文字列を整数に変換
	static vector<string> Split(string s, char d);	// 文字dで区切るSplit関数

	// sourceにdestを代入したものを返すだけ（SubstituteMaxとの対応のために用意）
	template<class C>
	static void Substitute(C &source, C dest){
		source = dest;
	}

	// 二数のうち大きい方をsourceに代入して返す（クラスに<と=の定義が必要）
	template<class C>
	static void SubstituteMax(C &source, C dest){
		if (source < dest){
			source = dest;
		}
	}

	// vectorの要素がすべて等しい
	template<class C>
	static bool EqualVector(vector<C> source, vector<C> dest){
		bool ret;
		int size = source.size();
		ret = (size == dest.size());
		if (ret){
			for (int i = 0; i < size; i++){
				ret &= (source[i] == dest[i]);
			}
		}
		return ret;
	}
};
