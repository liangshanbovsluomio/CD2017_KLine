#pragma once
#include <afxwin.h>
#include <vector>
using namespace std;

class CIndex
{
public:
	CString IndexName;
	vector<double> values;
	COLORREF color;
	vector<COLORREF> colors;
	//0-���� 1-���� 2-����
	int DrawType;
public:
	CIndex(void);
	virtual ~CIndex(void);
};

