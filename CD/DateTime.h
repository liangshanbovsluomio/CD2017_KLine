#pragma once
#include <string>
using namespace std;

class CDateTime
{
public:
	CDateTime(void);
	CDateTime(time_t seconds);
	CDateTime(int year,int month,int day);
	CDateTime(int year,int month,int day,int hour,int minute,int second);
	CDateTime(const std::string CDateTimeStr);
	CDateTime(const std::string CDateTimeStr,std::string formaterStr);
private:
	time_t seconds; //自1970年来的秒数
	tm date;
public:
	void AddYears(const time_t years);
	void AddMonths(const time_t months);
	void AddDays(const time_t days);
	void AddHours(const time_t hours);
	void AddMinutes(const time_t minutes);
	void AddSeconds(const time_t seconds);
	void AddWeeks(const time_t weeks);
	static int Compare(const CDateTime *value1,const CDateTime *value2);
	int CompareTo(const CDateTime *obj);
	int DaysInMonth(const int year,const int months);
	bool Equals(const CDateTime *CDateTime);
	static bool Equals(const CDateTime *value1,const CDateTime* value2);
	static CDateTime Parse(std::string CDateTimeStr);
	static CDateTime Parse(std::string CDateTimeStr,std::string formaterStr);
	std::string ToShortDateString();
	std::string ToString();
	std::string ToString(const std::string formaterStr);
public:
	int GetYear();
	int GetMonth();
	int GetDay();
	int GetHour();
	int GetMinute();
	int GetSecond();
	int DayOfWeek();
	int DayOfYear();
    static CDateTime GetNow();
public:
	bool operator ==(CDateTime &CDateTime);
	bool operator >(CDateTime &CDateTime);
	bool operator <(CDateTime &CDateTime);
	bool operator >=(CDateTime &CDateTime);
	bool operator <=(CDateTime &CDateTime);
	bool operator !=(CDateTime &CDateTime);
private:
	void InitByStr(std::string CDateTimeStr,std::string formaterStr);
public:
	~CDateTime(void);
};

