#include "StdAfx.h"
#include "DateTime.h"
#include <time.h>
#include <string>
using namespace std;

CDateTime::CDateTime(void)
{
}

CDateTime::CDateTime(const time_t seconds)
{
	this->seconds=seconds;
	this->date=*localtime(&this->seconds);
}

CDateTime::CDateTime(int year,int month,int day)
{
	tm t;
	t.tm_year=year-1900;
    t.tm_mon=month-1;
	t.tm_mday=day;
	t.tm_hour=0;
	t.tm_min=0;
	t.tm_sec=0;
	this->seconds=mktime(&t);
	this->date=*localtime(&this->seconds);
}

CDateTime::CDateTime(std::string CDateTimeStr)
{
	this->InitByStr(CDateTimeStr,"%d%d%d %d:%d:%d");
}

CDateTime::CDateTime(std::string CDateTimeStr,std::string formaterStr)
{
	this->InitByStr(CDateTimeStr,formaterStr);
}

CDateTime::~CDateTime(void)
{

}

void CDateTime::InitByStr(std::string CDateTimeStr,std::string formeratStr)
{
	int year,month,day,hour,minutes,seconds;

}

int CDateTime::DayOfWeek()
{
	return this->date.tm_wday;
}

int CDateTime::DayOfYear()
{
	return this->date.tm_yday;
}

int CDateTime::DaysInMonth(const int year,const int month)
{
	return 0;
}

bool CDateTime::Equals(const CDateTime* CDateTime)
{
	if(this->seconds==CDateTime->seconds)
		return true;
	return false;
}

bool CDateTime::Equals(const CDateTime *value1,const CDateTime* value2)
{
	if(value1->seconds==value2->seconds)
		return true;

	return false;
}

int CDateTime::GetDay()
{
	return this->date.tm_mday;
}

int CDateTime::GetHour()
{
	return this->date.tm_hour;
}

int CDateTime::GetMinute()
{
	return this->date.tm_min;
}

int CDateTime::GetMonth()
{
	return this->date.tm_mon;
}

CDateTime CDateTime::GetNow()
{
	CDateTime CDateTime(time(0));
	return CDateTime;
}

int CDateTime::GetSecond()
{
	return this->date.tm_sec;
}

int CDateTime::GetYear()
{
	return this->date.tm_year+1900;
}

void CDateTime::AddYears(const time_t years)
{
	this->date.tm_year=this->date.tm_year+years;
	this->seconds=mktime(&this->date);
}

void CDateTime::AddMonths(const time_t months)
{
	int a=(int)((this->date.tm_mon+months)/12);
	this->date.tm_year=this->date.tm_year+a;
	this->date.tm_mon=this->date.tm_mon+(int)((this->date.tm_mon+months)%12)-1;
	this->seconds=mktime(&this->date);
}

void CDateTime::AddDays(const time_t days)
{
	this->AddHours(days*24);
}

void CDateTime::AddHours(const time_t hours)
{
	this->AddMinutes(hours*60);
}

void CDateTime::AddMinutes(const time_t minutes)
{
	this->AddSeconds(minutes*60);
}

void CDateTime::AddSeconds(const time_t seconds)
{
	this->seconds=this->seconds+seconds;
	this->date=*localtime(&this->seconds);
}

void CDateTime::AddWeeks(const time_t weeks)
{
	this->AddDays(weeks*7);
}

int CDateTime::Compare(const CDateTime *t1,const CDateTime *t2)
{
	if(t1->seconds==t2->seconds)
		return 0;
	if(t1->seconds<t2->seconds)
		return -1;
	return 1;
}

int CDateTime::CompareTo(const CDateTime *value)
{
	if(this->seconds==value->seconds)
		return 0;
	if(this->seconds<value->seconds)
		return -1;
	return 1;
}

std::string CDateTime::ToShortDateString()
{
	return this->ToString("%Y-%m-%d");
}

std::string CDateTime::ToString()
{
	return this->ToString("%Y-%m-%d %H:%M:%S");
}

std::string CDateTime::ToString(const std::string formaterStr)
{
	char s[256];
	strftime(s,sizeof(s),formaterStr.c_str(),&this->date);
	std::string str(s);
	return str;
}

bool CDateTime::operator ==(CDateTime &CDateTime)
{
	return this->Equals(&CDateTime);
}

bool CDateTime::operator !=(CDateTime &CDateTime)
{
	if(this->seconds!=CDateTime.seconds)
		return true;

	return false;
}

bool CDateTime::operator>(CDateTime &CDateTime)
{
	if(this->seconds>CDateTime.seconds)
		return true;
	return false;
}

bool CDateTime::operator<(CDateTime &CDateTime)
{
	if(this->seconds<CDateTime.seconds)
		return true;

	return false;
}

bool CDateTime::operator >=(CDateTime &CDateTime)
{
	if(this->seconds>=CDateTime.seconds)
		return true;
	return false;
}

bool CDateTime::operator <=(CDateTime &CDateTime)
{
	if(this->seconds<=CDateTime.seconds)
		return true;
	return false;
}

