// 这是主 DLL 文件。

#include "stdafx.h"
#include <string>
#include "DateTime.h"
#include "KLine.h"
#include "CD.h"
#include "NumberHelper.h"
using namespace std;

CD::CCD::CCD(void){

}

double CD::CCD::test(double _max,double _min,IntPtr ptr)
{
    CDateTime dateTime(2013,12,12);
	KLine kl;
	//kl.DrawPriceScral(reinterpret_cast<HWND>(ptr.ToPointer()),123.0);
	//return kl.GetScr(_max,_min);
	return 0;
}

double CD::CCD::ToFix(double source,int point)
{
   return NumberHelper::ToFix(source,point);
}

