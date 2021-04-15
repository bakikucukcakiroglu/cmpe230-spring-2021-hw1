// 230_Project1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

vector<string> all_lines;
ifstream in_file;
ofstream out_file;

void print_module()
{
	out_file<< "; ModuleID = 'mylang2ir'" << endl;
	out_file << "declare i32 @printf(i*8, ...)" << endl;
	out_file << "@print.str = constant [4 x i8] c\"%d\\0A\\00\"" << endl;
	out_file << endl;
	out_file << "define i32 @main() {" << endl;
}

void take_all_lines()
{
	string line;
	while (getline(in_file, line))
	{
		all_lines.push_back(line);
	}
}

void paranthesis_count(string line, int line_count)
{
	
	int open = 0;

	for (int i = 0; i < line.size(); i++)
	{
			if (line[i] == '(')
			{
				open++;
			}
			if (line[i] == ')')
			{
				open--;
			}
		}
		if (open != 0)
		{
			out_file << "Line " << line_count << ": syntax error.";
			exit(0);
		}
		open = 0;
	}
	


void equal_sign_count(string line, int line_count)
{
		int open = 0;
		for (int k = 0; k < line.size(); k++)
		{
			if (line[k] == '=')
			{
				open++;
			}
		}
		if (open > 1)
		{
			out_file << "Line " << line_count << ": syntax error.";
			exit(0);
		}
	
}

void white_space_eraser()
{
	string line;
	int count = 0;
	for (int i = 0; i < all_lines.size(); i++)
	{
		string temp="";
		for (int k = 0; k < all_lines[i].size(); k++)
		{
			line = all_lines[i];
			if(line[k]!=' ')
			{
				temp += line[k];
			}
		}
		all_lines[i] = temp;
	}
}

void while_if_checker(string line, int line_count)
{
	int open_paranthesis = 0;
	int closed_paranthesis = 0;
	int curly_bracket = 0;
	if (line.substr(0, 5) == "while")
	{
		if (line[5] == '(')
		{
			for (int i = 0; line.size(); i++)
			{
				if (line[i] == '(')
				{
					open_paranthesis++;
				}
				if (line[i] == ')')
				{
					closed_paranthesis++;
				}
				if (line[i] == '{')
				{
					curly_bracket++;
				}
			}
		}
		if (!(open_paranthesis > 0 && curly_bracket == 1))
		{
			exit(0);

		}
	}
	else
	{
		if (line[3] == '(')
		{
			for (int i = 0; line.size(); i++)
			{
				if (line[i] == '(')
				{
					open_paranthesis++;
				}
				if (line[i] == ')')
				{
					closed_paranthesis++;
				}
				if (line[i] == '{')
				{
					curly_bracket++;
				}
			}
		}
		if (!(open_paranthesis > 0 && curly_bracket == 1))
		{
			exit(0);

		}
	}
	

	
}

void syntax_checker()
{
	// = 1'den fazla mý?
	// ( ) sayýsý eþit mi?
	// ! - & | vb. karakterler var mý?
	// ++ , -- , ** , // var mý?

	string line;
	int line_count = 1;
	for (int i = 0; i < all_lines.size(); i++)
	{
		for (int k = 0; k < all_lines[i].size(); k++)
		{
			line = all_lines[i];
			paranthesis_count(line,line_count);
			equal_sign_count(line,line_count);
			if (line.substr(0, 5) == "while" || line.substr(0,3) == "if")
			{
				while_if_checker(line,line_count);
			}


		}
		line_count++;
	}
}

int main(int argc, char* argv[])
{	
	
	string file_name = argv[1];
	in_file.open(file_name);
	out_file.open("file.ll");

	print_module();
	take_all_lines();
	

	return 0;
}

