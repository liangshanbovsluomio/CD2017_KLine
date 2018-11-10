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

void CD::StkGDI::DrawKLine(IntPtr ptr,cli::array<QuoteRef^>^ quotes, cli::array<IndexRef^>^ mainIndexs, cli::array<SecondMapRef^>^ secondMapRefs,String^ savefile)
{
	if(quotes->Length==0)
		return;

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
		SecondMap secMap;
		for(int j=0;j<secondMapRefs[i]->IndexRefs->Length;j++)
		{
			CIndex cindex;
			cindex.color=RGB(secondMapRefs[i]->IndexRefs[j]->color->R,secondMapRefs[i]->IndexRefs[j]->color->G,secondMapRefs[i]->IndexRefs[j]->color->B);
			cindex.IndexName=(char*)Marshal::StringToCoTaskMemAnsi(secondMapRefs[i]->IndexRefs[j]->Indexname).ToPointer();
			cindex.DrawType=secondMapRefs[i]->IndexRefs[j]->DrawType;
			bool colors=secondMapRefs[i]->IndexRefs[j]->colors->Length==secondMapRefs[i]->IndexRefs[j]->values->Length;

			for(int z=0;z<secondMapRefs[i]->IndexRefs[j]->values->Length;z++)
			{
				if(colors)
				{
					ColorRef^ colRef= secondMapRefs[i]->IndexRefs[j]->colors[z];
					cindex.colors.push_back(RGB(colRef->R,colRef->G,colRef->B));
				}
				double db=secondMapRefs[i]->IndexRefs[j]->values[z];
				cindex.values.push_back(db);
			}
			secMap.indexs.push_back(cindex);
		}
		kl.SecIndexs.push_back(secMap);
	}

	kl.Quotes=klQuotes;
	kl.DrawKLine(reinterpret_cast<HWND>(ptr.ToPointer()),(char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(savefile).ToPointer());
}
