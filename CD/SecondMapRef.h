#pragma once
#include "IndexRef.h"
public ref class SecondMapRef
{
public:
	cli::array<cli::array<IndexRef^>^>^ IndexRefs;
public:
	SecondMapRef(void);
};

