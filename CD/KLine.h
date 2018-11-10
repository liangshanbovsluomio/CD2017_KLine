#pragma once
#include <afx.h>
#include <vector>
#include "Quote.h"
#include <afxwin.h>
#include "Index.h"
#include "SecondMap.h"

class KLine
{
private:
	int stageW,stageH,picWh,picHt,picLOX,picHOY,picRightLeft,maxKlineNumber;
	double topPrice,botPrice,scral;
	HDC hdc;
	CDC *pDC;
	CPen penUP,penDown,whitePen;
	HBRUSH penUPBrush,penDownBrush,backgroundBrush;
	
public:
	int kLineSpace,kLineWidth,kLineFrom;
	COLORREF BackgroundColor,kLineUpColor,kLineDownColor,kLineMA5Color,kLineMA10Color,kLineMA20Color;
	LPCTSTR StockName;
	vector<Quote> Quotes;
	vector<CIndex> MainIndexs;
	vector<SecondMap> SecIndexs;
	//当前鼠标的位置
	int PointX,PointY;
	bool ShowCur;
public:
	KLine(void);
	~KLine();
	void DrawKLine(HWND hwnd,const char* savefile);
private:
	void InitDraw(const RECT& rect);
	void DrawPriceScral(HWND hwnd,double _pr);
	void DrawPriceScral(CDC& cdc,double _pr);
	CPoint GetPoint(double _pr,int _index);
	vector<CDateTime> GetTimeSpan(vector<Quote> quotes);
	void DrawKLineSharp(HWND hwnd,Quote quote,int _index);
	void DrawKLineSharp(CDC& cdc,Quote quote,int _index);
	void DrawQX(CDC& cdc,vector<CPoint> values,COLORREF color,int maxY);
	void DrawStike(CDC& cdc,vector<CPoint> values,COLORREF color,vector<COLORREF> colors,int y);
	void DrawBar(CDC& cdc,vector<CPoint> values,COLORREF color,vector<COLORREF> colors,int y);
	double GetScr(double _max,double _min,int scrCount);
    double GetBigerPrice(double _pr,double _scr);
	double GetSmallerPrice(double _pr,double _scr);
	void DrawLine(CDC& cdc,int x,int y,int x2,int y2,COLORREF c,int ps_style=PS_SOLID);
	int DrawText(CDC& cdc,int x,int y,const CString cs,COLORREF c,COLORREF bgc=RGB(0,0,0));
	Quote GetKLineQuoteByPoint(int pointX);
	void DrawSecondIndex(CDC& cdc,SecondMap&  secondMap,int index);
	void SaveToPic(CDC& memDC,CBitmap& memBitmap,const char* filename);
};
