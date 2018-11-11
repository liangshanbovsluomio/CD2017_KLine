#include "StdAfx.h"
#include "KLine.h"
#include "StkGDI.h"
#include "Index.h"
#include "SecondMap.h"


CD::StkGDI::StkGDI(void)
{
	kLineSpace=2;
	kLineWidth=5;
	pointX=0;
	pointY=0;
	ShowCur=false;
}

System::String^ CD::StkGDI::DrawKLine(IntPtr ptr,cli::array<QuoteRef^>^ quotes, cli::array<IndexRef^>^ mainIndexs,cli::array<SecondMapRef^>^ secondMapRefs,String^ savefile)
{
	if(quotes->Length==0)
		return System::String::Empty;

	KLine kl;
	kl.kLineWidth=kLineWidth;
	kl.kLineSpace=kLineSpace;
	kl.PointX=pointX;
	kl.PointY=pointY;
	kl.StockName=(char*)Marshal::StringToCoTaskMemAnsi(StockName).ToPointer();
	kl.ShowCur=ShowCur;
	std::vector<Quote> klQuotes;
	for(int i=0;i<quotes->Length;i++)
	{
		Quote qt(quotes[i]->op,quotes[i]->ep,quotes[i]->hp,quotes[i]->lp,quotes[i]->rate,CDateTime(quotes[i]->date->year,quotes[i]->date->mon,quotes[i]->date->day));
		klQuotes.push_back(qt);
	}

	for(int i=0;i<mainIndexs->Length;i++)
	{
		CIndex cindex;
		cindex.color=RGB(mainIndexs[i]->color->R,mainIndexs[i]->color->G,mainIndexs[i]->color->B);
		cindex.IndexName=(char*)Marshal::StringToCoTaskMemAnsi(mainIndexs[i]->Indexname).ToPointer();
		for(int j=0;j<mainIndexs[i]->values->Length;j++)
		{
			double db=mainIndexs[i]->values[j];
			cindex.values.push_back(db);
		}
		kl.MainIndexs.push_back(cindex);
	}

	for(int i=0;i<secondMapRefs->Length;i++)
	{
		SecondMapRef^ arr0 = secondMapRefs[i];
		SecondMap secMap;
		for (int j = 0; j < arr0->IndexRefs->Length; j++)
		{
			cli::array<IndexRef^>^ arr1= arr0->IndexRefs[j];
			vector<CIndex> vcindex;
			for (int m = 0; m < arr1->Length; m++) {
				IndexRef^ arr2 = arr1[m];
				CIndex cindex;
				cindex.color = RGB(arr2->color->R, arr2->color->G, arr2->color->B);
				cindex.IndexName = (char*)Marshal::StringToCoTaskMemAnsi(arr2->Indexname).ToPointer();
				cindex.DrawType = arr2->DrawType;
				bool colors = arr2->colors->Length == arr2->values->Length;

				for (int z = 0; z < arr2->values->Length; z++)
				{
					if (colors)
					{
						ColorRef^ colRef = arr2->colors[z];
						cindex.colors.push_back(RGB(colRef->R, colRef->G, colRef->B));
					}
					double db = arr2->values[z];
					cindex.values.push_back(db);
				}
				vcindex.push_back(cindex);
			}
			secMap.indexs.push_back(vcindex);
		}
		kl.SecIndexs.push_back(secMap);
	}

	kl.Quotes=klQuotes;
	return kl.DrawKLine(reinterpret_cast<HWND>(ptr.ToPointer()),(char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(savefile).ToPointer());
}
