#pragma once
#include "ColorRef.h"
using namespace System;
using namespace System::Collections;
public ref class IndexRef{
public:
	String^ Indexname;
	cli::array<double>^ values;
    ColorRef^ color;
	cli::array<ColorRef^>^ colors;
	//0-���� 1-���� 2-����
	int DrawType;
public:
	IndexRef();
};

