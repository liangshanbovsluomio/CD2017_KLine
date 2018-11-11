#include "StdAfx.h"
#include "KLine.h"
#include <vector>
#include "Quote.h"
#include <string>
#include <afx.h>
#include "math.h"
#include <afxwin.h>
#include "NumberHelper.h"
#include "Index.h"
#include <map>

using namespace std;

KLine::KLine(void)
{
	picLOX=0;
	picHOY=25;
	kLineSpace=1;
	kLineWidth=3;
	kLineUpColor=RGB(253,5,6);
	kLineDownColor=RGB(1,168,0);
	kLineMA5Color=RGB(0,124,200);
	kLineMA10Color=RGB(0,42,191);
	kLineMA20Color=RGB(192,0,0);
	BackgroundColor=RGB(0,0,0);

	kLineFrom=0;
	this->PointX=0;
	this->PointY=0;

	this->penUP.CreatePen(0,1,kLineUpColor);
	this->penDown.CreatePen(0,1,kLineDownColor);
	this->whitePen.CreatePen(0,1,RGB(255,255,255));

	this->penUPBrush=CreateSolidBrush(kLineUpColor);
	this->penDownBrush=CreateSolidBrush(kLineDownColor);
	this->backgroundBrush=CreateSolidBrush(this->BackgroundColor);

	this->pDC=NULL;

	this->picRightLeft=60;
	ShowCur=false;
}

double KLine::GetScr(double _max,double _min,int scrCount)
{
   if(_max<=_min)
	   return 0.01;
   
   double diff=_max-_min;
   CString cs;
   cs.Format("%.2f",diff);
   int num=0;
   int i=0;
   for(;i<cs.GetLength();i++)
   {
	   char ch=cs.GetAt(i);
	   if(ch!='0'&&ch!='.')
	   {
		   num=atoi(&ch);
           break;
	   }
   }
   if(diff<1)
	   i=i+1;

   int pointIndex=cs.Find('.');
   
   double bnum=pow(10.0,pointIndex==-1?0:(pointIndex-1));
   int numSize=(pointIndex==-1?cs.GetLength():pointIndex-i);
   double bb=pow(10.0,numSize>0?numSize-1:0);
   int leftNum=atoi(cs.Left(i).GetString());
   //::MessageBox(0,(LPCTSTR)cs,"",0);
   
   char chr[18];
   CString temp(itoa(leftNum,chr,10));
   temp.Append(gcvt(bb,10,chr));

   CString temp2(itoa(leftNum,chr,10));
   temp2.Append(gcvt(bb*2,10,chr));
   
   if(diff>atof(temp) && diff<=atof(temp2))
	   return NumberHelper::ToFix(atof(temp2)/scrCount,2);

   CString temp5(itoa(leftNum,chr,10));
   temp5.Append(gcvt(bb*5,10,chr));
   if(diff>atof(temp2) && (diff<=atof(temp5)))
       return NumberHelper::ToFix(atof(temp5)/scrCount,2);

    if(diff>atof(temp5))
       return NumberHelper::ToFix(atof(temp)*2,2);

   return NumberHelper::ToFix(diff*bnum/scrCount,2);
   
}

double KLine::GetBigerPrice(double _pr,double _scr)
{
	double pr;
	
	if(_scr>1)
	{
		pr=NumberHelper::ToFix(_pr,0);
	}
	else
	{
		CString cs;
		cs.Format("%f",_scr);
		for(int i=0;i<cs.GetLength();i++)
		{
			char ch=cs.GetAt(i);
			if(ch!='0' &&ch!='.')
			{
				pr=NumberHelper::ToFix(_pr,i-1);
				break;
			}
		}
	}

	if(pr<pr+_scr/2)
		return NumberHelper::ToFix(pr+_scr/2,2);

	return pr+_scr;
}

double KLine::GetSmallerPrice(double _pr,double _scr)
{
	double pr;
	if(_scr>=1)
	{
		pr=NumberHelper::ToFix(_pr,0);
	}
	else
	{
		CString cs;
		cs.Format("%f",_scr);
		for(int i=0;i<cs.GetLength();i++)
		{
			char ch=cs.GetAt(i);
			if(ch!='0' &&ch!='.')
			{
				pr=NumberHelper::ToFix(_pr,i-1);
				break;
			}
		}
	}

	if(pr>pr-_scr/2)
		return NumberHelper::ToFix(pr-_scr/2,2);

	return pr-_scr;
}

void KLine::InitDraw(const RECT& rect)
{
    this->stageH=rect.bottom-rect.top;
	this->picHt=(this->stageH-this->picHOY)*2/(2+this->SecIndexs.size());
	this->stageW=rect.right-rect.left;
	this->picWh=this->stageW-this->picRightLeft;
	maxKlineNumber=(this->picWh)/(kLineSpace+kLineWidth);
	kLineFrom=Quotes.size()<=maxKlineNumber?0:(Quotes.size()-maxKlineNumber);
	ShowCur=ShowCur&&(PointY<picHt||(PointY<stageH-picHOY &&(PointY-picHt)%(picHt/2)>picHOY));
}

System::String^ KLine::DrawKLine(HWND hwnd,const char* savefile){
    RECT rect;
	bool boo= GetClientRect(hwnd,&rect);
	
	if(!boo)
		return System::String::Empty;

	InitDraw(rect);
	if(strlen(savefile)>0)
	   PointX=this->picWh-1;

	if(this->picHt<5 ||this->picWh<5)
		return System::String::Empty;

	PAINTSTRUCT paintParm;
	paintParm.fErase=0;
	::BeginPaint(hwnd,&paintParm);

	if(pDC==NULL)
	{
	  hdc=GetDC(hwnd);
	  pDC=CDC::FromHandle(hdc);
	}

	CDC memDC;
	CBitmap memBitmap;
	memDC.CreateCompatibleDC(pDC);
	memBitmap.CreateCompatibleBitmap(pDC,this->stageW,this->stageH);
	CBitmap *pOldBit=memDC.SelectObject(&memBitmap);
	memDC.FillSolidRect(0,0,this->stageW,this->stageH,this->BackgroundColor);

	//不要在这里刷屏，否则界面会很闪
	/*HBRUSH hbrush= CreateSolidBrush(this->BackgroundColor);
	HBRUSH oldHbrush= (HBRUSH)SelectObject(hdc,hbrush);
	FillRect(hdc,&rect,hbrush);
	SelectObject(hdc,oldHbrush);
	DeleteObject(hbrush);*/

	/*::SetTextColor(hdc,RGB(255,255,255));
	::SetBkColor(hdc,RGB(0,0,0));
	::TextOut(hdc,1,1,this->StockName,strlen(this->StockName));*/
	int textPos=2;
	double maxPr=0,minPr=MAXDOUBLE;
	for(vector<Quote>::iterator it=Quotes.begin()+kLineFrom;it!=Quotes.end();it++)
	{
		if(it->hp>maxPr)
			maxPr=it->hp;

		if(it->lp<minPr)
			minPr=it->lp;
	}

	Quote qot= GetKLineQuoteByPoint(this->PointX);
	if(qot.op==0)
	{
		CString cs(StockName);
		cs.Append(" 收:");
		cs.Append(NumberHelper::ToString(qot.ep));
		cs.Append(" 幅:");
		cs.Append(NumberHelper::ToString(qot.rate));
		cs.Append("%");
		textPos+= DrawText(memDC,1,1,cs,RGB(255,255,255))+10;
	}else
	{
		CString cs(StockName);
		cs.Append(" 开:");
		cs.Append(NumberHelper::ToString(qot.op));
		cs.Append(" 收:");
		cs.Append(NumberHelper::ToString(qot.ep));
		cs.Append(" 高:");
		cs.Append(NumberHelper::ToString(qot.hp));
		cs.Append(" 低:");
		cs.Append(NumberHelper::ToString(qot.lp));
		cs.Append(" 幅:");
		cs.Append(NumberHelper::ToString(qot.rate));
		cs.Append("%");
		textPos+= DrawText(memDC,1,1,cs,RGB(255,255,255))+10;
	}
	

    scral=GetScr(maxPr,minPr,10);
	topPrice=GetBigerPrice(maxPr,scral);
	botPrice=GetSmallerPrice(minPr,scral);

	double pr=botPrice+scral;
	int ct=0;

	for(;pr<=topPrice;ct++)
	{
		this->DrawPriceScral(memDC,pr);
		pr+=scral;
	}

	//DrawPriceScral(memDC,topPrice);
	int index=0;
	System::String^ currdate=System::String::Empty;
	for(vector<Quote>::iterator it=Quotes.begin()+kLineFrom;it!=Quotes.end()&&index<maxKlineNumber;it++)
	{
		if (DrawKLineSharp(memDC, *it, index++))
		{
			
			currdate =gcnew System::String(it->date.ToString().c_str());
		}
	}

	
	//画主图指标
	for(vector<CIndex>::iterator it=MainIndexs.begin();it!=MainIndexs.end();it++)
	{
		int pointIndex=PointX/(kLineWidth+kLineSpace)+kLineFrom;
		pointIndex=pointIndex<0?0:pointIndex;
		pointIndex=pointIndex>=it->values.size()?it->values.size()-1:pointIndex;
	    textPos+=DrawText(memDC,textPos,1,it->IndexName+":"+NumberHelper::ToString(it->values.at(pointIndex)),it->color)+10;

		vector<CPoint> points;
		for(int i=kLineFrom;i<it->values.size();i++)
		{
			CPoint pt= GetPoint(it->values.at(i),i-kLineFrom);
			pt.x+=this->kLineWidth/2;
			points.push_back(pt);
		}
		DrawQX(memDC,points,it->color,picHt);
	}

	//画刻度价格
	this->DrawLine(memDC,this->picWh,this->picHOY,this->picWh,this->stageH,RGB(255,0,0));

	//画十字光标
	if(ShowCur)
	{
		DrawLine(memDC,0,PointY,picWh,PointY,RGB(255,255,255));
		DrawLine(memDC,PointX,picHOY,PointX,picHt,RGB(255,255,255));
		
	}

	if(PointY<picHt&&PointY>picHOY)
    {
	   double rightPrice=NumberHelper::ToFix(topPrice-(topPrice-botPrice)*(PointY-picHOY)/(picHt-picHOY),2);
	   char buff[18];
	   RECT rt;
	   rt.left=picWh;
	   rt.top=PointY-8;
	   rt.right=stageW;
	   rt.bottom=PointY+8;
	   CBrush bsh(RGB(0,0,255));
	   memDC.FillRect(&rt,&bsh);
	   DrawLine(memDC,picWh,PointY-8,stageW,PointY-8,RGB(255,0,0));
	   DrawText(memDC,picWh+10,PointY-6,NumberHelper::ToString(rightPrice),RGB(255,255,255),RGB(0,0,255));
	   DrawLine(memDC,picWh,PointY+8,stageW,PointY+8,RGB(255,0,0));
	   DeleteObject(bsh);
	}

	//画副图指标
	for(int i=0;i<this->SecIndexs.size();i++)
	{
		DrawSecondIndex(memDC,this->SecIndexs.at(i),i);
	}
	//end 画副图指标

	DrawLine(memDC,0,this->stageH-this->picHOY,this->stageW,this->stageH-this->picHOY,RGB(255,0,0));

    //画十字光标
	//if(PointX>0 &&PointY>picHOY &&PointX<this->picWh&&PointY<stageH-picHOY)
	//{
	//	CPen *oldPen= memDC.SelectObject(&whitePen);
	//	memDC.MoveTo(this->picLOX,PointY);
	//	memDC.LineTo(picWh,PointY);
	//	memDC.MoveTo(PointX,picHOY);
	//	memDC.LineTo(PointX,picHt);
	//	memDC.SelectObject(oldPen);

	//	//显示右边价格
	//	double rightPrice=NumberHelper::ToFix(topPrice-(topPrice-botPrice)*(PointY-picHOY)/(picHt-picHOY),2);
	//	char buff[18];
	//	DrawText(memDC,picWh+10,PointY-6,NumberHelper::ToString(rightPrice),RGB(0,0,255));
	//	//显示下边日期
	//}

	if(PointX>0 && PointX<picWh)
	{
		Quote qt= this->GetKLineQuoteByPoint(PointX);
		if(qt.ep>0)
		{
			
			int wh=DrawText(memDC,2,stageH-picHOY+5,qt.date.ToShortDateString().c_str(),RGB(0,0,0));
			int x=PointX+wh+1<=picWh?(PointX<wh/2?1:PointX-1-wh/2):picWh-wh-1;
			RECT rt;
			CBrush bsh(RGB(0,0,255));
			rt.left=x-1;
			rt.top=stageH-picHOY+1;
			rt.right=x+wh+1;
			rt.bottom=stageH;
			memDC.FillRect(&rt,&bsh);
			DrawLine(memDC,x-1,stageH-picHOY,x-1,stageH,RGB(255,0,0));
			DrawLine(memDC,x+wh+1,stageH-picHOY,x+wh+1,stageH,RGB(255,0,0));
			DrawText(memDC,x,stageH-picHOY+5,qt.date.ToShortDateString().c_str(),RGB(255,255,255),RGB(0,0,255));
			DeleteObject(bsh);
		}
	}

	pDC->BitBlt(0,0,this->stageW,this->stageH,&memDC,0,0,SRCCOPY);
	if(strlen(savefile)>0)
	  SaveToPic(memDC,memBitmap,savefile);
	memBitmap.DeleteObject();
	memDC.DeleteDC();
	ReleaseDC(hwnd,hdc);
	::EndPaint(hwnd,&paintParm);

	return currdate;
}

void KLine::DrawLine(CDC& cdc,int x,int y,int x2,int y2,COLORREF c,int ps_style)
{
	CPen newPen;
	newPen.CreatePen(ps_style,1,c);
	CPen *oldPen=cdc.SelectObject(&newPen);
	cdc.MoveTo(x,y);
	cdc.LineTo(x2,y2);
	cdc.SelectObject(oldPen);
	DeleteObject(newPen);
}

void KLine::DrawBar(CDC& cdc,vector<CPoint> values,COLORREF color,vector<COLORREF> colors,int y)
{
	if(values.size()==0)
		return;
	bool maxColor=colors.size()==values.size()+kLineFrom;

	CBrush pen;
	CBrush *oldPen=NULL;
	if(!maxColor)
	{
		pen.CreateSolidBrush(color);
		oldPen=cdc.SelectObject(&pen);
		
	}else
	{
		pen.CreateSolidBrush(colors.at(kLineFrom));
	}

	
	int width=max(this->kLineWidth/2,1);

	map<DWORD,CBrush*> pens;
	pens.insert(pair<DWORD,CBrush*>(color,&pen));

	for(int i=0;i<values.size();i++)
	{
		RECT rt;
	    rt.left=values.at(i).x-width;
	    rt.right=values.at(i).x+width;
		rt.top=values.at(i).y;
		rt.bottom=y;

		if(i>0 &&maxColor)
		{
			 /*  map<DWORD,CBrush*>::const_iterator it= pens.find(colors.at(i+kLineFrom));
			   if(it==pens.end())
			   {*/
				   //CBrush pen2;
				   //pen2.CreateSolidBrush(colors.at(i+kLineFrom));
				   cdc.FillSolidRect(&rt,colors.at(i+kLineFrom));
				   //pens.insert(pair<DWORD,CBrush*>(colors.at(i+kLineFrom),&pen2));
			  /* }
			   else
			   {
				   cdc.FillRect(&rt,it->second);
			   }  */
		}else
		{
			cdc.FillRect(&rt,&pen);
		}
	}

	if(oldPen!=NULL)
	  cdc.SelectObject(oldPen);
	for(map<DWORD,CBrush*>::iterator it=pens.begin();it!=pens.end();it++)
	{
		DeleteObject(*it->second);
	}
}

void KLine::DrawStike(CDC& cdc,vector<CPoint> values,COLORREF color,vector<COLORREF> colors,int y)
{
	if(values.size()==0)
		return;
	bool maxColor=colors.size()==values.size()+kLineFrom;

	CPen pen;
	CPen *oldPen=NULL;
	if(!maxColor)
	{
		pen.CreatePen(0,1,color);
		oldPen=cdc.SelectObject(&pen);
	}

	for(int i=0;i<values.size();i++)
	{
		   if(i>0 &&maxColor)
		   {
			  DrawLine(cdc,values.at(i).x,values.at(i).y,values.at(i).x,y,colors.at(kLineFrom+i));
		   }else
		   {
			   cdc.MoveTo(values.at(i));
			   cdc.LineTo(values.at(i).x,y);
		   }
	}

	if(oldPen!=NULL)
	   cdc.SelectObject(oldPen);	
}

void KLine::DrawQX(CDC& cdc,vector<CPoint> values,COLORREF color,int maxY)
{
	if(values.size()==0)
		return;

	CPen qxPen(0,1,color);
	CPen* oldPen= cdc.SelectObject(&qxPen);

	cdc.MoveTo(values.at(0));
	for(int i=1;i<values.size();i++)
	{
		if(values.at(i-1).y<=maxY)
		{
			cdc.LineTo(values.at(i));
		}
		
		cdc.MoveTo(values.at(i));
	}

	cdc.SelectObject(oldPen);
	DeleteObject(qxPen);
}

void KLine::DrawPriceScral(CDC& cdc,double _pr)
{
	CPoint pt= GetPoint(_pr,0);
	int span=2;
	for(int i=pt.x;i<picWh+8;)
	{
		cdc.SetPixel(i,pt.y,RGB(255,0,0));

		if(i>=picWh-2)
			span=1;
		i+=span;
	}

	if(pt.y<picHt)
	   DrawText(cdc,picWh+10,pt.y-6,NumberHelper::ToString(_pr),RGB(255,255,255));

	//1/4的价格
	CPoint pt1=GetPoint(_pr-scral/4,0);
	if(pt1.y<picHt)
	  DrawLine(cdc,picWh,pt1.y,picWh+2,pt1.y,RGB(255,0,0));

	//一半的价格
	CPoint pt2=GetPoint(_pr-scral/2,0);
	if(pt2.y<picHt)
	  DrawLine(cdc,picWh,pt2.y,picWh+4,pt2.y,RGB(255,0,0));

	//3/4的价格
	CPoint pt3=GetPoint(_pr-scral*3/4,0);
	if(pt3.y<picHt)
	  DrawLine(cdc,picWh,pt3.y,picWh+2,pt3.y,RGB(255,0,0));

	
}

void KLine::DrawPriceScral(HWND hwnd,double _pr)
{
	/*CPen *oldPen;
	CPen dotPen(PS_DOT,1,RGB(255,0,0));
	oldPen=pDC->SelectObject(&dotPen);*/

	CPoint pt= GetPoint(_pr,0);
	/*CPoint p1(picLOX,pt.y);
	CPoint p2(picLOX+picWh,pt.y);
	pDC->MoveTo(p1);
	pDC->LineTo(p2);*/
	int span=2;
	for(int i=pt.x;i<picWh+8;)
	{
		pDC->SetPixel(i,pt.y,RGB(255,0,0));
		if(i>=picWh-2)
			span=1;
		i+=span;
	}

	/*pDC->SelectObject(oldPen);
	DeleteObject(dotPen);*/
}

CPoint KLine::GetPoint(double _pr,int _index)
{
	int x,y;
	//x=this->picLOX+(this->kLineSpace+this->kLineWidth)*(_index-this->kLineFrom);
	x=this->picLOX+(this->kLineSpace+this->kLineWidth)*(_index);
	y=(this->topPrice-_pr)*(this->picHt-this->picHOY)/(this->topPrice-this->botPrice)+this->picHOY;
	return CPoint(x,y);
}

vector<CDateTime> KLine::GetTimeSpan(vector<Quote> quotes)
{
	vector<CDateTime> timeSpan;
	int lastMonth=0;

	for(vector<Quote>::iterator it=quotes.begin();it!=quotes.end();it++)
	{
		int nowMonth=it->date.GetMonth();
		int nowDay=it->date.GetDay();
	}

	return timeSpan;
}

bool KLine::DrawKLineSharp(CDC& cdc,Quote quote,int _index)
{
	if(quote.hp==0||quote.lp==0||quote.op==0)
		return false;
	CPen *oldPen=NULL;

	CPoint pt_open=GetPoint(quote.op,_index);
	CPoint pt_close=GetPoint(quote.ep,_index);
	
	if(quote.op>quote.ep)
	{
		oldPen=cdc.SelectObject(&penDown);
	}else
	{
		oldPen=cdc.SelectObject(&penUP);
	}

	
	RECT rect={pt_open.x,quote.ep>quote.op?pt_close.y:pt_open.y,pt_open.x+kLineWidth,::abs(pt_open.y-pt_close.y)+(quote.ep>quote.op?pt_close.y:pt_open.y)};
	if(rect.top==rect.bottom)
	{
		cdc.MoveTo(pt_open);
		cdc.LineTo(pt_open.x+kLineWidth,pt_open.y);
	}
	else
	{
	  
	 /* HBRUSH oldHbrush= (HBRUSH)SelectObject(hdc,quote.ep>quote.op?penUPBrush:penDownBrush);
	  FillRect(hdc,&rect,quote.ep>quote.op?penUPBrush:penDownBrush);
	  SelectObject(hdc,oldHbrush);*/
	  cdc.FillSolidRect(pt_open.x,quote.ep>quote.op?pt_close.y:pt_open.y,kLineWidth,::abs(pt_open.y-pt_close.y)+1,quote.ep>quote.op?kLineUpColor:kLineDownColor);
	}

	//上影线
	CPoint pt_h=GetPoint(quote.hp,_index);
	pt_h.x=pt_h.x+(int)floor(kLineWidth/2.0);
	cdc.MoveTo(pt_h);
	cdc.LineTo(pt_h.x,quote.ep>quote.op?pt_close.y:pt_open.y);

	//下影线
	CPoint pt_l=GetPoint(quote.lp,_index);
	cdc.MoveTo(pt_h.x,pt_l.y);
	cdc.LineTo(pt_h.x,quote.ep<quote.op?pt_close.y:pt_open.y);

	cdc.SelectObject(oldPen);

	return this->PointX >= rect.left&&this->PointX <= rect.right
		&&this->PointY >= pt_h.y&&this->PointY <= pt_l.y;
}

bool KLine::DrawKLineSharp(HWND hwnd,Quote quote,int _index)
{
	CPen *oldPen=NULL;

	CPoint pt_open=GetPoint(quote.op,_index);
	CPoint pt_close=GetPoint(quote.ep,_index);
	
	if(quote.op>quote.ep)
	{
		oldPen=pDC->SelectObject(&penDown);
	}else
	{
		oldPen=pDC->SelectObject(&penUP);
	}

	RECT rect={pt_open.x,quote.ep>quote.op?pt_close.y:pt_open.y,pt_open.x+kLineWidth,::abs(pt_open.y-pt_close.y)+(quote.ep>quote.op?pt_close.y:pt_open.y)};
	if(rect.top==rect.bottom)
	{
		pDC->MoveTo(pt_open);
		pDC->LineTo(pt_open.x+kLineWidth,pt_open.y);
	}
	else
	{
	  
	  HBRUSH oldHbrush= (HBRUSH)SelectObject(hdc,quote.ep>quote.op?penUPBrush:penDownBrush);
	  FillRect(hdc,&rect,quote.ep>quote.op?penUPBrush:penDownBrush);
	  SelectObject(hdc,oldHbrush);
	}

	//上影线
	CPoint pt_h=GetPoint(quote.hp,_index);
	pt_h.x=pt_h.x+(int)floor(kLineWidth/2.0);
	pDC->MoveTo(pt_h);
	pDC->LineTo(pt_h.x,quote.ep>quote.op?pt_close.y:pt_open.y);

	//下影线
	CPoint pt_l=GetPoint(quote.lp,_index);
	pDC->MoveTo(pt_h.x,pt_l.y);
	pDC->LineTo(pt_h.x,quote.ep<quote.op?pt_close.y-1:pt_open.y-1);

	pDC->SelectObject(oldPen);
	//ReleaseDC(hwnd,hdc);

	return this->PointX >= rect.left&&this->PointX <= rect.right
		&&this->PointY >= pt_h.y&&this->PointY <= pt_l.y;
}

Quote KLine::GetKLineQuoteByPoint(int pointX)
{
	if(Quotes.size()==0)
		return Quote(0,0,0,0,0,CDateTime());
	int index=(pointX-this->picLOX)/(this->kLineWidth+this->kLineSpace)+kLineFrom;
    if(index<0)
		return Quotes.at(0);
	if(index>=Quotes.size())
		return Quotes.at(Quotes.size()-1);
	return Quotes.at(index);
}

int KLine::DrawText(CDC& cdc,int x,int y,const CString cs,COLORREF c,COLORREF bgc)
{
	int txtLen=0;
	CFont myfont;
	int nheight= MulDiv(10,cdc.GetDeviceCaps(LOGPIXELSY),72);
	myfont.CreateFontA(nheight,0,0,0,400,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,"宋体");
	CFont* pOldFont=cdc.SelectObject(&myfont);
	COLORREF oldColor= cdc.SetTextColor(c);
	cdc.SetBkColor(bgc);
	txtLen=cdc.GetTextExtent(cs).cx;
	cdc.TextOutA(x,y,cs);
	cdc.SetTextColor(oldColor);
	cdc.SelectObject(pOldFont);
	DeleteObject(myfont);
	return txtLen;
}

void KLine::DrawSecondIndex(CDC& cdc,SecondMap&  secondMap,int index)
{
	DrawLine(cdc,0,picHt+index*picHt/2,this->stageW,picHt+picHt*index/2,RGB(255,0,0));
	int textPos=2;
	double max=0,min=MAXDOUBLE;
	for (vector<vector<CIndex>>::iterator it0 = secondMap.indexs.begin(); it0 != secondMap.indexs.end(); it0++) {
		for (vector<CIndex>::iterator it = it0->begin(); it != it0->end(); it++)
		{
			for (int i = kLineFrom; i < it->values.size(); i++)
			{
				if (it->values[i] > max)
					max = it->values[i];

				if (it->values[i] < min)
					min = it->values[i];
			}
		}
	}

	double scr= GetScr(max,min,5);
    double top= GetBigerPrice(max,scr);
    double bot=GetSmallerPrice(min,scr);
	if(top<=0)
		top=scr;

	int y0=picHt+picHt*index/2+picHOY+(picHt/2-picHOY)*(top)/(top-bot);
	if(top>0 &&bot<0)
	{
	  for(int i=0;i<picWh;i=i+2)
	  {
		cdc.SetPixel(i,y0,RGB(255,255,255));
	  }
	  DrawText(cdc,picWh+10,y0-6,"0.00",RGB(255,255,255));
	  for(double s=scr;s<top;s+=scr)
	  {
		int y=picHt+picHt*index/2+picHOY+(picHt/2-picHOY)*(top-s)/(top-bot);
		for(int i=0;i<picWh;i=i+2)
		{
			cdc.SetPixel(i,y,RGB(255,0,0));
		}
		DrawLine(cdc,picWh,y,picWh+8,y,RGB(255,0,0));
		DrawText(cdc,picWh+10,y-6,NumberHelper::ToString(NumberHelper::ToFix(s,2)),RGB(255,255,255));
	  }
	  for(double s=-scr;s>bot;s-=scr)
	  {
		int y=picHt+picHt*index/2+picHOY+(picHt/2-picHOY)*(top-s)/(top-bot);
		for(int i=0;i<picWh;i=i+2)
		{
			cdc.SetPixel(i,y,RGB(255,0,0));
		}
		DrawLine(cdc,picWh,y,picWh+8,y,RGB(255,0,0));
		DrawText(cdc,picWh+10,y-6,NumberHelper::ToString(NumberHelper::ToFix(s,2)),RGB(255,255,255));
	  }
	  
	}else
	{
		for(double s=bot+scr;s<top;s+=scr)
	    {
		  int y=picHt+picHt*index/2+picHOY+(picHt/2-picHOY)*(top-s)/(top-bot);
		  for(int i=0;i<picWh;i=i+2)
		  {
			 cdc.SetPixel(i,y,RGB(255,0,0));
		  }
		  DrawLine(cdc,picWh,y,picWh+8,y,RGB(255,0,0));
		  DrawText(cdc,picWh+10,y-6,NumberHelper::ToString(NumberHelper::ToFix(s,2)),RGB(255,255,255));
	   }
	}
	//DrawLine(cdc,picWh,picHt+picLOX+picHt*index/2,picWh,picHt+picHt*(index+1)/2,RGB(255,0,0));
	
	for (vector<vector<CIndex>>::iterator it0 = secondMap.indexs.begin(); it0 != secondMap.indexs.end(); it0++) {
		for (vector<CIndex>::iterator it = it0->begin(); it != it0->end(); it++)
		{
			if (it->values.size() < kLineFrom || it->values.size() != this->Quotes.size())
				continue;

			int pointIndex = PointX / (kLineWidth + kLineSpace) + kLineFrom;
			if (pointIndex < it->values.size() && pointIndex >= 0)
			{
				textPos += DrawText(cdc, textPos, picHt + picHt*index / 2 + 5, it->IndexName + ":" + NumberHelper::ToString(it->values.at(pointIndex)), it->color) + 10;
			}
			else {
				textPos += DrawText(cdc, textPos, picHt + picHt*index / 2 + 5, it->IndexName + ":" + NumberHelper::ToString(it->values.at(it->values.size() - 1)), it->color) + 10;
			}


			vector<CPoint> pts;
			for (int i = kLineFrom; i < it->values.size(); i++)
			{
				int y = picHt + picHt*index / 2 + picHOY + (picHt / 2 - picHOY)*(top - it->values.at(i)) / (top - bot);
				CPoint pt((i - kLineFrom)*(kLineWidth + kLineSpace) + kLineWidth / 2, y);
				pts.push_back(pt);
			}

			if (it->DrawType == 0)
			{
				DrawQX(cdc, pts, it->color, picHt + picHt*(index + 1) / 2);
			}
			else if (it->DrawType == 1)
			{
				DrawStike(cdc, pts, it->color, it->colors, min(y0, picHt + picHt*index / 2 + picHOY + (picHt / 2 - picHOY)));
			}
			else if (it->DrawType == 2)
			{
				DrawBar(cdc, pts, it->color, it->colors, min(y0, picHt + picHt*index / 2 + picHOY + (picHt / 2 - picHOY)));
			}

		}
	}

	//画十字光标
	if(ShowCur)
	{
		DrawLine(cdc,PointX,picHt+picHt*(index)/2+picHOY,PointX,picHt+picHt*(index+1)/2,RGB(255,255,255));
	}

	if(PointY>picHt+picHt*(index)/2+picHOY&&PointY<picHt+picHt*(index+1)/2)
	{
		double rightPrice=NumberHelper::ToFix(top-(top-bot)*(PointY-picHt-picHt*index/2-picHOY)/(picHt/2-picHOY),2);
		char buff[18];
		RECT rt;
		rt.left=picWh;
		rt.right=stageW;
		rt.top=PointY-8;
		rt.bottom=PointY+8;
		CBrush bsh(RGB(0,0,255));
		cdc.FillRect(&rt,&bsh);
		DrawLine(cdc,picWh,PointY-8,stageW,PointY-8,RGB(255,0,0));
		DrawText(cdc,picWh+10,PointY-6,NumberHelper::ToString(rightPrice),RGB(255,255,255),RGB(0,0,255));
		DrawLine(cdc,picWh,PointY+8,stageW,PointY+8,RGB(255,0,0));
		DeleteObject(bsh);
	}
}

void KLine::SaveToPic(CDC& memDC,CBitmap& memBitmap,const char* filename)
{
	 //以下代码保存memDC中的位图到文件
    BITMAP bmp;
    memBitmap.GetBitmap(&bmp);//获得位图信息
	FILE *fp = fopen(filename, "w+b");
	if(fp==NULL)
		return;
	BITMAPINFOHEADER bih = {0};//位图信息头
    bih.biBitCount = bmp.bmBitsPixel;//每个像素字节大小
    bih.biCompression = BI_RGB;
    bih.biHeight = bmp.bmHeight;//高度
    bih.biPlanes = 1;
    bih.biSize = sizeof(BITMAPINFOHEADER);
    bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;//图像数据大小
    bih.biWidth = bmp.bmWidth;//宽度
	BITMAPFILEHEADER bfh = {0};//位图文件头
    bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);//到位图数据的偏移量
    bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;//文件总的大小
    bfh.bfType = (WORD)0x4d42;
	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);//写入位图文件头
    fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);//写入位图信息头 
    byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];//申请内存保存位图数据
    GetDIBits(memDC.m_hDC, (HBITMAP) memBitmap.m_hObject, 0, this->stageH, p, 
        (LPBITMAPINFO) &bih, DIB_RGB_COLORS);//获取位图数据
    fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);//写入位图数据
    delete [] p;
    fclose(fp);
}

KLine::~KLine()
{
	DeleteObject(this->penUP);
	DeleteObject(this->penDown);
	DeleteObject(this->penUPBrush);
	DeleteObject(this->penDownBrush);
	DeleteObject(this->backgroundBrush);
	DeleteObject(this->whitePen);
}
