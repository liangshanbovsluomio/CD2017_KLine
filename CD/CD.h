// CD.h

#pragma once

using namespace System;

namespace CD {

	public ref class CCD
	{
		// TODO: �ڴ˴���Ӵ���ķ�����
	    public:
		  CCD(void);
		  double test(double max,double min,IntPtr ptr);
		  static double ToFix(double source,int point);
		  
	};
}
