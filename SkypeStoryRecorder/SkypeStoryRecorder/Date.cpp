#include "Date.h"

int Date::GetDayValue(){
	int ret = year;
	ret = 100 * ret + month;
	ret = 100 * ret + day;
	return ret;
}

int Date::GetTimeValue(){
	int ret = hour;
	ret = 100 * ret + minute;
	ret = 100 * ret + second;
	return ret;
}

Date Date::GetNowTime(){
	auto p = std::chrono::system_clock::now();
	time_t t = std::chrono::system_clock::to_time_t(p);
	tm lt;
	localtime_s(&lt, &t);

	Date ret;
	ret.year = lt.tm_year + 1900;
	ret.month = lt.tm_mon + 1;
	ret.day = lt.tm_mday;
	ret.hour = lt.tm_hour;
	ret.minute = lt.tm_min;
	ret.second = lt.tm_sec;
	return ret;
}

Date Date::ConvertDate(string str){
	Date ret;
	if (regex_match(str, regex("[0-9]+/[0-9]+/[0-9]+ [0-9]+:[0-9]+:[0-9]+"))){
		int space = str.find(" ");
		vector<string> day = common::Split(str.substr(0, space), '/');
		vector<string> time = common::Split(str.substr(space + 1), ':');
		ret.year = common::StoI(day[0]);
		ret.month = common::StoI(day[1]);
		ret.day = common::StoI(day[2]);
		ret.hour = common::StoI(time[0]);
		ret.minute = common::StoI(time[1]);
		ret.second = common::StoI(time[2]);
	}
	return ret;
}

string Date::GetStr(){
	string ret = "";
	ret += common::ItoS(year, 4);	ret += "/";
	ret += common::ItoS(month, 2);	ret += "/";
	ret += common::ItoS(day, 2);	ret += " ";
	ret += common::ItoS(hour, 2);	ret += ":";
	ret += common::ItoS(minute, 2);	ret += ":";
	ret += common::ItoS(second, 2);
	return ret;
}

void Date::Print(){
	cout << GetStr() << endl;
}

Date::Date(const Date& d){
	*this = d;
}

Date& Date::operator=(const Date& d){
	this->year = d.year;
	this->month = d.month;
	this->day = d.day;
	this->hour = d.hour;
	this->minute = d.minute;
	this->second = d.second;
	return *this;
}

bool Date::operator>(Date& d){
	bool ret;
	int dayValue1 = this->GetDayValue();
	int dayValue2 = d.GetDayValue();
	if (dayValue1 > dayValue2){
		ret = true;
	}
	else if (dayValue1 < dayValue2){
		ret = false;
	}
	else{
		ret = this->GetTimeValue() > d.GetTimeValue();
	}
	return ret;
}

bool Date::operator<(Date& d){
	return d > *this;
}

bool Date::operator==(Date& d){
	bool ret;
	if (this->GetDayValue() == d.GetDayValue()
		&& this->GetTimeValue() == d.GetTimeValue()){
		ret = true;
	}
	else{
		ret = false;
	}
	return ret;
}

bool Date::operator!=(Date& d){
	return !(*this == d);
}

bool Date::operator>=(Date& d){
	return (*this > d) || (*this == d);
}

bool Date::operator<=(Date& d){
	return (*this < d) || (*this == d);
}
