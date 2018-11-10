#pragma once
#include <string>
using namespace std;

public ref class DateTimeRef
{
public:
	int year,mon,day,hour,min,sec;
public:
	DateTimeRef();
	DateTimeRef(int year,int month,int day);
	DateTimeRef(int year,int month,int day,int hour,int min,int sec);
};

