#pragma once

#include "common.h"

// opens a text file and appends comma delimited numbers there

class txtFile
{
public:
	txtFile(char * fileName);
	~txtFile(void);
private:
	ofstream * p_txtfile;
	CRITICAL_SECTION m_writeSection;

public:
	friend txtFile& operator << (txtFile& out,int num);
};
