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

int syntax_line=-1;

vector<string> all_variables;

void print_module()
{
	out_file << "; ModuleID = 'mylang2ir'" << endl;
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
		string temp = "";
		for (int k = 0; k < all_lines[i].size(); k++)
		{
			line = all_lines[i];
			if (line[k] != ' ')
			{
				temp += line[k];
			}
		}
		all_lines[i] = temp;
	}
}

void while_if_checker(string line, int line_count)
{

	if (line.substr(0, 5) == "while")
	{
		if (line[5] == '(')
		{
			int length = line.size();

			if (line.substr(length - 2, length) != "){" || line.find('=') != string::npos)
			{

				out_file << "Line " << line_count << ": syntax error.";
				exit(0);
			}
		}
	}
	else
	{
		if (line[3] == '(')
		{
			int length = line.size();

			if (line.substr(length - 2, length) != "){" || line.find('=') != string::npos)
			{

				out_file << "Line " << line_count << ": syntax error.";
				exit(0);
			}
		}
	}
}

void print_checker(string line, int line_count)
{

	if (line[5] = '(')
	{

		if (line[line.size() - 1] != ')')
		{

			out_file << "Line " << line_count << ": syntax error.";
			exit(0);
		}
	}
}

void comment_eraser()
{

	for (int k = 0; k < all_lines.size(); k++)
	{

		for (int i = 0; i < all_lines[k].size(); i++)
		{

			if (all_lines[i] == '#')
			{

				i = all_lines[k].size();

				all_lines[k] = all_lines[k].substr(0, i);
			}
		}
	}
}
bool isValidVar(string var, int line_count)
{

	if (var[0] != '_' || isAlpha(var[0]) == 0)
	{

		out_file << "Line " << line_count << ": syntax error.";
		exit(0);
	}
	for (int i = 1; i < var.length(); i++)
	{
		if (isalnum(var[i]) == 0 || var[i] == ' ')
		{
			out_file << "Line " << line_count << ": syntax error.";
			exit(0);
		}
	}
}
string assign_parser(string line, int line_count)
{

	int eq_index = line.find('=');

	string variable = line.substr(0, eq_index);
	
	std::vector<string>::iterator it;

	if(isValidVar(variable)){

		it = find (all_variables.begin(), all_variables.end(), variable);

		if (it == all_variables.end()){

			all_variables.push_back(variable);
		}
	}else{

		syntax_line=line_count;
	}

	string exp= line.substr(eq_index+1, line.size());
	expr_parser(exp);

}
string expr_parser(string exp, int line_count)
{
}

void parser(string line, int line_count)
{

	if (line.find('=') != string::npos)
	{

		assign_parser(line);
	}
	else if (line.substr(0, 6) == "while(")
	{
	}
	else if (line.substr(0, 4) == "if(")
	{
	}
	else if (line.substr(0.6) = "print(")
	{
	}
	else if (line[0] == '}')
	{
	}
	else if (line.size() == 0)
	{
	}
	else
	{

		out_file << "Line " << line_count << ": syntax error.";
		exit(0);
	}
}

void syntax_checker()
{

	/*
     Koddan boşlukları sileceğiz +
     Comment lineları sileceğiz. +
     Açılan parantez kapandı mı kontrolü +
     Bir line'da birden fazla = var mı diye bakacağız. +
     Bir lineda uygun olmayan karakter var mı diye bakacağız. (harf, sayı, =, (,), {,}, _, ,)
     while, if varsa parantez ve curly parantez kontrolu yapacağız (whileden ve iften sonraki (, son iki char ){olmalı) +
     print ve choose varsa parantez kontrolü yapacağız( print ve choose den sonraki (, son karakter ) olmalı) +


	 exp
	 while(exp){

		
	 }
     if( exp ){

	 }
	 print(exp)
	 exp
     

	 syntax check ---> syntax line 5

	 parser ilk 4 line için çalışacak---> syntax line 4 break 
     
	 
	 
	 */
	// = 1'den fazla m˝?
	// ( ) say˝s˝ e˛it mi?
	// ! - & | vb. karakterler var m˝?
	// ++ , -- , ** , // var m˝?

	string line;
	int line_count = 1;
	for (int i = 0; i < all_lines.size(); i++)
	{
		for (int k = 0; k < all_lines[i].size(); k++)
		{
			line = all_lines[i];
			paranthesis_count(line, line_count);
			equal_sign_count(line, line_count);
			if (line.substr(0, 5) == "while" || line.substr(0, 3) == "if")
			{
				while_if_checker(line, line_count);
			}
			if (line.substr(0, 5) == "print")
			{
				print_checker(line, line_count);
			}
		}
		line_count++;
	}
}

int main(int argc, char *argv[])
{

	string file_name = argv[1];
	in_file.open(file_name);
	out_file.open("file.ll");

	print_module();
	take_all_lines();

	return 0;
}
