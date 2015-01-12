#pragma once
#include "common.h"
#include <chrono>
#include <ctime>

class Date{
	int GetDayValue();	// ���t��\����̒l���擾�i2014�N12��21����20141221�j
	int GetTimeValue();	// ������\����̒l���擾�i17:44:20��174420�j
public:
	int year, month, day, hour, minute, second;
	Date(int y, int m, int d, int h, int min, int sec) :
		year(y), month(m), day(d), hour(h), minute(min), second(sec){}
	Date(){ *this = Date(0, 0, 0, 0, 0, 0); }
	Date(const Date&);
	Date& operator=(const Date&);

	static Date GetNowTime();	// ���ݎ����̎擾
	static Date ConvertDate(string str);	// �u2014/12/28 20:31:30�v�̌`���̕������Date�ɕϊ�

	// ���t�̑召�́A�i��ł��鎞���̕����傫�����̂ł���Ƃ���i2014�N < 2015�N�j
	bool operator>(Date& d);
	bool operator<(Date& d);
	bool operator==(Date& d);
	bool operator!=(Date& d);
	bool operator>=(Date& d);
	bool operator<=(Date& d);
	string GetStr();	// �����̕\���e�L�X�g��Ԃ�
	void Print();	// GetStr�̕W���o�͔�
};
