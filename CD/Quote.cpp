#include "StdAfx.h"
#include "Quote.h"
#include "DateTime.h"

Quote::Quote(double _op,double _ep,double _hp,double _lp,double _rate,CDateTime _date)
{
	this->op=_op;
	this->ep=_ep;
	this->hp=_hp;
	this->lp=_lp;
	this->rate=_rate;
	this->date=_date;
}
