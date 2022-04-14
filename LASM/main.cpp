#include <iostream>
#include <fstream>
#include <string>
#include "../LLL/LLL.h"
#include "LASM.h"

using namespace std;

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		string filename;
		cin>>filename;
		string content;
		string str_buf;
		ifstream file(filename);
		while (getline(file, str_buf))
		{
			content += str_buf + '\n';
		}
		LASM::Compiler c;
		c.CompileFromString(content);
		LASM::Runner r;
		return r.RunCommands(c);
	}
	else if(argc == 2)
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
	else
		return -1;
	return 0;
}