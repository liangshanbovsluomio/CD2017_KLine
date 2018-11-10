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
	//0-ÇúÏß 1-¹÷×Ó 2-Öù×Ó
	int DrawType;
public:
	IndexRef();
};

