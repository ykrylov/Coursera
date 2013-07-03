// knapsack_1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>

#include "ksp.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if( argc < 3 )
	{
		std::cerr << "Invalid number of parameters" << std::endl;
		return 1;
	}
	std::ifstream input( argv[1] );
#ifndef _test
	std::ofstream output( argv[2] );
	ksp solver(input, output);
#else
  ksp solver(input, std::cout);
#endif
	return 0;
}

