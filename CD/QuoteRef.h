#pragma once
#include "DateTimeRef.h"
public ref class QuoteRef
{
public:
	double op,ep,hp,lp,rate;
	DateTimeRef^ date;
public:
	QuoteRef(void);
	QuoteRef(double _op,double _ep,double _hp,double _lp,double _rate,DateTimeRef^ _date);
};

