#include "StdAfx.h"
#include "DateTimeRef.h"
#include "DateTime.h"

DateTimeRef::DateTimeRef()
{
	CDateTime dt;
	CDateTime now= dt.GetNow();

	this->year=now.GetYear();
	this->mon=now.GetMonth()+1;
	this->day=now.GetDay();
	this->hour=now.GetHour();
	this->min=now.GetMinute();
	this->sec=now.GetSecond();
}

DateTimeRef::DateTimeRef(int year,int month,int day)
{
	this->year=year;
	this->mon=month;
	this->day=day;
}

DateTimeRef::DateTimeRef(int year,int month,int day,int hour,int min,int sec)
{
	this->year=year;
	this->mon=month;
	this->day=day;
	this->hour=hour;
	this->min=min;
	this->day=day;
}


