#include "StdAfx.h"
#include "NumberHelper.h"
#include <afx.h>

NumberHelper::NumberHelper(void)
{
}

double NumberHelper::ToFix(double source,int point)
{
	if(point<0)
		point=0;
	CString cs;
	CString csformat("%.");
	char ch[25];
	csformat.Append(itoa(point,ch,10));
	csformat.Append("f");
	cs.Format(csformat,source);
	return atof((LPSTR)(LPCSTR)cs);
}

CString NumberHelper::ToString(double source)
{
	CString cs;
	CString csformat("%.");
	char ch[25];
	csformat.Append(itoa(2,ch,10));
	csformat.Append("f");
	cs.Format(csformat,source);
	return cs;
}
