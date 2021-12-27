#include <iostream>
#include <fstream>
#include <string>
#include "../LLL/LLL.h"
#include "LASM.h"

using namespace std;

int main(int argc, char** argv)
{
	if (argc != 2)
		return -1;
	else
	{
		string content;
		string str_buf;
		ifstream file(argv[1]);
		while (getline(file, str_buf))
		{
			content += str_buf + '\n';
		}
		LASM::Compiler c;
		c.CompileFromString(content);
		LASM::Runner r;
		return r.RunCommands(c);
	}
	return 0;
}