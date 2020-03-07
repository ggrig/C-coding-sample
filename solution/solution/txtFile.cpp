#include "txtFile.h"

txtFile::txtFile(char * fileName):	p_txtfile(NULL)
{
	// open file for appending
	p_txtfile = new ofstream (fileName, fstream::out | fstream::app);

	InitializeCriticalSection(&m_writeSection);
}

txtFile::~txtFile(void)
{
	DeleteCriticalSection(&m_writeSection);

	if (NULL != p_txtfile)
	{
		p_txtfile->close();
		delete p_txtfile;
		p_txtfile = NULL;
	}
}

txtFile& operator << (txtFile& out,int num)
{
	EnterCriticalSection(&out.m_writeSection);
	*out.p_txtfile << num << ',';
	LeaveCriticalSection(&out.m_writeSection);

	return out;
}
