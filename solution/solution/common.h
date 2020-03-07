// common.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _CRT_RAND_S

#include <stdio.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#include <windows.h>    // for HANDLE
#include <process.h>    // for _beginthread()
#include <conio.h>		// for _getch()
#include <stdlib.h>	
#include <sstream>
#include <iostream>		// std::cout
#include <iomanip>      // std::setw
#include <fstream> 

using namespace std;

#define QUEUE_MAX_SIZE				100
#define QUEUE_PUSH_ALLOWED_SIZE		80
