#pragma once
#include <afx.h>
class NumberHelper
{
public:
	NumberHelper(void);
 	static double ToFix(double source,int point);
	static CString ToString(double source);
};

