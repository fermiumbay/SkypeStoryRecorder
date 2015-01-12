#pragma once
#include "common.h"
#include <chrono>
#include <ctime>

class Date{
	int GetDayValue();	// 日付を表す一つの値を取得（2014年12月21日⇒20141221）
	int GetTimeValue();	// 時刻を表す一つの値を取得（17:44:20⇒174420）
public:
	int year, month, day, hour, minute, second;
	Date(int y, int m, int d, int h, int min, int sec) :
		year(y), month(m), day(d), hour(h), minute(min), second(sec){}
	Date(){ *this = Date(0, 0, 0, 0, 0, 0); }
	Date(const Date&);
	Date& operator=(const Date&);

	static Date GetNowTime();	// 現在時刻の取得
	static Date ConvertDate(string str);	// 「2014/12/28 20:31:30」の形式の文字列をDateに変換

	// 日付の大小は、進んでいる時刻の方が大きいものであるとする（2014年 < 2015年）
	bool operator>(Date& d);
	bool operator<(Date& d);
	bool operator==(Date& d);
	bool operator!=(Date& d);
	bool operator>=(Date& d);
	bool operator<=(Date& d);
	string GetStr();	// 日時の表示テキストを返す
	void Print();	// GetStrの標準出力版
};
