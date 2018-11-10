#pragma once
#include "DateTime.h"
#include "DateTimeRef.h"

class Quote
{
public:
	double op;
	double ep;
	double hp;
	double lp;
	double rate;
	CDateTime date;
public:
	Quote(double _op,double _ep,double _hp,double _lp,double _rate,CDateTime _date);
};
