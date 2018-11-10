// CD.h

#pragma once

using namespace System;

namespace CD {

	public ref class CCD
	{
		// TODO: 在此处添加此类的方法。
	    public:
		  CCD(void);
		  double test(double max,double min,IntPtr ptr);
		  static double ToFix(double source,int point);
		  
	};
}
