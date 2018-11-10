#include "StdAfx.h"
#include "QuoteRef.h"
#include "DateTimeRef.h"

QuoteRef::QuoteRef(void)
{

}

QuoteRef::QuoteRef(double _op,double _ep,double _hp,double _lp,double _rate,DateTimeRef^ _date)
{
	op=_op;
	ep=_ep;
	hp=_hp;
	lp=_lp;
	rate=_rate;
	date=_date;
}
