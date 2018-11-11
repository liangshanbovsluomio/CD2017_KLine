#pragma once
using namespace System;
using namespace System::Runtime::InteropServices;
#include "QuoteRef.h"
#include "IndexRef.h"
#include "SecondMapRef.h"

namespace CD{
public ref class StkGDI
{
public:
	System::String^ StockName;
	int kLineSpace,kLineWidth,pointX,pointY; 
	bool ShowCur;
public:
	StkGDI(void);
	System::String^ DrawKLine(IntPtr ptr,cli::array<QuoteRef^>^ quotes,cli::array<IndexRef^>^ mainIndexs,cli::array<SecondMapRef^>^ secondMapRefs,String^ savefile);
};
}

