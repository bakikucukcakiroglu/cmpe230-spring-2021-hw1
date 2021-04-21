﻿#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

vector<string> all_lines;
ifstream in_file;
ifstream in_file_two;
ofstream out_file;
ofstream out_file_two;

int syntax_line = -1;
int line_count = 1;
int while_opened = 0;
int temp = 1;
vector<string> all_variables;

void print_module();
void take_all_lines();
bool is_number(string s);
void paranthesis_count(int i);
void equal_sign_count(int i);
void white_space_eraser(int i); // DUZELT
void while_if_checker(int i);
void print_checker(int i);
void comment_eraser(int i); // DUZELT
bool isValidVar(string var);
string assign_parser(string line);
string add_op(string line);
string sub_op(string line);
string mul_op(string line);
string div_op(string line);
string par_op(string line);
void parser(string line);
void syntax_checker(int i);
void is_it_all_good(int i);


void print_module()
{
	out_file_two << "; ModuleID = 'mylang2ir'" << endl;
	out_file_two << "declare i32 @printf(i8*, ...)" << endl; // declare i32 @printf(i8*, ...)
	out_file_two << "@print.str = constant [4 x i8] c\"%d\\0A\\00\"" << endl;
	out_file_two << endl;
	out_file_two << "define i32 @main() {" << endl;
}

void take_all_lines()
{
	string line;
	while (getline(in_file, line))
	{
		cout << line << endl;
		all_lines.push_back(line);
	}
}

bool is_number(string s)
{
	for(int i =0; i<s.size(); i++){

		if(!(s[i]<=57&&s[i]>=48)){

			return false;

		}
	}

	return true;
}

void paranthesis_count(int i)
{

	int open = 0;
    string line = all_lines[i];
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

void equal_sign_count(int i)
{
	int open = 0;
    string line = all_lines[i];
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

void white_space_eraser(int i)
{
        string line = all_lines[i];
		string temp = "";
		for (int k = 0; k < line.size(); k++)
		{
			if (line[k] != ' ')
			{
				temp += line[k];
			}
		}
		all_lines[i] = temp;
}

void while_if_checker(int i)
{
    string line = all_lines[i];
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

void print_checker(int i)
{
    string line = all_lines[i];
	if (line[5] == '(')
	{

		if (line[line.size() - 1] != ')')
		{

			out_file << "Line " << line_count << ": syntax error.";
			exit(0);
		}
	}
}

void comment_eraser(int i)
{
        string line = all_lines[i];
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] ==	'#')
            {
				line = line.substr(0,i);
			}
		}
		all_lines[i] = line;

}

bool isValidVar(string var)
{

	if (var.size() == 0) 
	{
		out_file << "Line " << line_count << ": syntax error.";
		exit(0);
	}
	if (isalpha(var[0]) == 0)
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

	return true;
}

string assign_parser(string line)
{

	int eq_index = line.find('=');
	cout<<"eq index:"<<eq_index<<endl;

	string variable = line.substr(0, eq_index);

	std::vector<string>::iterator it;

	if (isValidVar(variable)) {

		it = find(all_variables.begin(), all_variables.end(), variable);

		if (it == all_variables.end())
        {
			all_variables.push_back(variable);
		}
	}
	else {

		syntax_line = line_count;
	}

	string exp = line.substr(eq_index + 1, line.size());

	string b= sub_op(exp);
	if(is_number(b)){

		out_file << "\t" << "store i32 " << b << ", i32* %"<< variable << endl;
	
	}else if(b[0]!='%'){

		out_file << "\t" <<"%_"<< temp << "= load i32* %" << b<< endl;
		temp++;

		out_file << "\t" << "store i32 %_" << temp-1 << ", i32* %"<< variable << endl;
		
	
		
	}else if(b[0]=='%'){


		out_file << "\t" << "store i32"<< b << ", i32* %"<< variable << endl;
		

	}

	string a= "%_"+to_string(temp-1);

	return a;
}

string add_op(string line)
{

	int op_index = -1;
	int par = 0;
	for (int i = line.size() - 1; i >= 0; i--) {

		if (line[i] == '(') {
			par++;
		}
		if (line[i] == ')') {
			par--;
		}

		if (par == 0 && line[i] == '+') {

			op_index = i;

		}
	}

	if (op_index != -1) {

		string right = add_op(line.substr(op_index + 1, line.size()));
		string left = add_op(line.substr(0, op_index));

		if(!is_number(left)){

		out_file << "\t" <<"%_"<< temp << "= load i32* %" << left<< endl;
		
		left= "%_"+to_string(temp);
	
		temp++;
		}
				
		if(!is_number(right)){

		out_file << "\t" <<"%_"<< temp << "= load i32* %" << right<< endl;
		
		right= "%_"+to_string(temp);

		temp++;
		}
		string a= "%_"+to_string(temp);
		out_file << "\t" << "%_" << temp << " = add i32 " << left << ", " << right << endl;
		temp++;
		return a;
	}
	else {

		return mul_op(line);
	}

}

string sub_op(string line)
{

	int op_index = -1;
	int par = 0;
	for (int i = line.size() - 1; i >= 0; i--) {

		if (line[i] == '(') {
			par++;
		}
		if (line[i] == ')') {
			par--;
		}

		if (par == 0 && line[i] == '-') {

			op_index = i;

		}
	}

	if (op_index != -1) {

		string right = sub_op(line.substr(op_index + 1, line.size()));
		string left = sub_op(line.substr(0, op_index));

		if(!is_number(left)){

		out_file << "\t" <<"%_"<< temp << "= load i32* %" << left<< endl;
		
		left = "%_" + to_string(temp);
	
		temp++;
		}
				
		if(!is_number(right)){

		out_file << "\t" <<"%_"<< temp << "= load i32* %" << right<< endl;
		
		right = "%_" + to_string(temp);

		temp++;
		}
		string a = "%_" + to_string(temp);
		out_file << "\t" << "%_" << temp << " = sub i32 " << left << ", " << right << endl;
		temp++;
		return a;
	}
	else{
		return add_op(line);
	}
}

string mul_op(string line)
{

	int op_index = -1;
	int par = 0;
	for (int i = line.size() - 1; i >= 0; i--) 
	{

		if (line[i] == '(') {
			par++;
		}
		if (line[i] == ')') {
			par--;
		}

		if (par == 0 && line[i] == '*') {

			op_index = i;

		}
	}

	if (op_index != -1) {

		string right = mul_op(line.substr(op_index + 1, line.size()));
		string left = mul_op(line.substr(0, op_index));

		if(!is_number(left)){

		out_file << "\t" <<"%_"<< temp << "= load i32* %" << left<< endl;
		
		left= "%_"+to_string(temp);
	
		temp++;
		}
				
		if(!is_number(right)){

		out_file << "\t" <<"%_"<< temp << "= load i32* %" << right<< endl;
		
		right= "%_"+to_string(temp);

		temp++;
		}
		string a= "%_"+to_string(temp);
		out_file << "\t" << "%_" << temp << " = mul i32 " << left << ", " << right << endl;
		temp++;
		return a;

	}
	else {

		return div_op(line);
	}

}

string div_op(string line)
{

	int op_index = -1;
	int par = 0;
	for (int i = line.size() - 1; i >= 0; i--) 
	{

		if (line[i] == '(') {
			par++;
		}
		if (line[i] == ')') {
			par--;
		}

		if (par == 0 && line[i] == '/') 
		{

			op_index = i;

		}
	}

	if (op_index != -1) {

		string right = div_op(line.substr(op_index + 1, line.size()));
		string left = div_op(line.substr(0, op_index));

		if(!is_number(left)){

		out_file << "\t" <<"%_"<< temp << "= load i32* %" << left<< endl;
		
		left= "%_"+to_string(temp);
	
		temp++;
		}
				
		if(!is_number(right)){ // 3+(n+y) gibi bir durumda n+y %! olarak dönecek ve 3+%!   3+%%! olacak

		out_file << "\t" <<"%_"<< temp << "= load i32* %" << right<< endl;
		
		right= "%_"+to_string(temp);

		temp++;
		}
		string a= "%_"+to_string(temp);
		out_file << "\t" << "%_" << temp << " = sdiv i32 " << left << ", " << right << endl;
		temp++;
		return a;
	}
	else {

		return par_op(line);
	}


}

string par_op(string line)
{

	if (line[0] == '(' && line[line.size() == ')']) 
	{


		sub_op(line.substr(1, line.size()));
	}
	else {

		if (is_number(line)) {

			return line;
		}
		else if (isValidVar(line)) {

			bool isRegistered = false;

			for (int i = 0; i < all_variables.size(); i++) {

				if (all_variables[i] == line) {

					isRegistered = true;
				}
			}
			if (!isRegistered) {

				all_variables.push_back(line);
			}

			return line;
		}
		else {
			out_file << "Line " << line_count << ": syntax error.";
			exit(0);
		}

	}
	return line; //!!!!
}

void parser(string line)
{

	if (line.find('=') != string::npos)
	{
		assign_parser(line);
	}
	else if (line.substr(0, 6) == "while(")
	{
	    /*
	     * string right = sub_op(line.substr(op_index + 1, line.size()));
		string left = sub_op(line.substr(0, op_index));


	    */
		while_opened++;
		int length= line.length();
		string tempp = line.substr(6, length-8);

		out_file << "\t" <<"br label %whcond" << endl << endl;
		out_file << "whcond:" << endl;
		string tried =  sub_op(tempp);

		out_file << "\t" << "%_" << temp <<"= icmp ne i32 "<< tried<<", 0 "<< endl;
		temp++;
		out_file << "\t" << "br i1 %_"<< temp-1 <<", label %whbody, label %whend" << endl << endl;
		out_file << "whbody:" << endl;
	}
	else if (line.substr(0, 4) == "if(")
	{
		string tempp = line.substr(3, line.length()-5);

		out_file << "\t" << "br label %whcond" << endl << endl;
		out_file << "whcond:" << endl;
        string tried = sub_op(tempp);
        /*if(!is_number(tried)){
            out_file << "\t" <<"%_"<< temp << "= load i32* " << tried<< endl;
            tried = "%_" + to_string(temp);
            temp++;
        }*/


		out_file << "\t" << "%_" << temp << "= icmp ne i32 %_" << tried << ", 0 " << endl;
		temp++;
		out_file << "\t" << "br i1 %_" << temp - 1 << ", label %whbody, label %whend" << endl << endl;
		out_file << "whbody:" << endl;
	}

	else if (line.substr(0, 6) == "print(")
	{
		string tempp = line.substr(6, line.length() - 7);
		string tried = sub_op(tempp);
        if(!is_number(tried)){
            out_file << "\t" <<"%_"<< temp << "= load i32* " << tried<< endl;
            tried = "%_" + to_string(temp);
            temp++;
        }

		temp++;
		out_file << "\t" << "call i32(i8*, ...)* @printf(i8* getelementptr( [4 x i8]* @print.str, i32 0, i32 0), i32 "<< tried << ")" << endl;
	}

	else if (line[0] == '}')
	{
		if (while_opened == 0)
		{
			out_file << "\t" << "br label %whend" << endl << endl;
		}
		else if (while_opened == 1)
		{
			while_opened--;
			out_file << "\t" << "br label %whcond" << endl << endl;
		}
		out_file << "whend:" << endl;
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

void syntax_checker(int i)
{
    string line = all_lines[i];
	is_it_all_good(i);
	paranthesis_count(i);
	equal_sign_count(i);

		if (line.substr(0, 5) == "while" || line.substr(0, 3) == "if")
		{
			while_if_checker(i);
		}
		if (line.substr(0, 5) == "print")
		{
			print_checker(i);
		}

}

void is_it_all_good(int i)
{
    string line = all_lines[i];
	for (int i = 0; i < line.size(); i++)
	{
		if (!(isalpha(line[i]) || isdigit(line[i]) || line[i] == '{' || line[i] == '}' || line[i] == '(' || line[i] == ')' || line[i] == '=' || line[i] == ',' || line[i] == '+'
			|| line[i] == '-' || line[i] == '*' || line[i] == '/' ))
		{
			out_file << "Line " << line_count << ": syntax error.";
			exit(0);
		}
	}
}

int main(int argc, char* argv[])
{
	cout<<"başladık"<<endl;
	string file_name = argv[1];
	in_file.open(file_name);
	out_file.open("file.txt");
	out_file_two.open("file1.ll");
	in_file_two.open("file.txt");

	print_module();
	take_all_lines();
	int i = 0;
	
	while (i < all_lines.size()) 
	{
		
		white_space_eraser(i);// DUZELT
		cout<<all_lines[i]<<endl;
		string line= all_lines[i];
		comment_eraser(i);
		cout << "OUR LINES:" <<all_lines[i] << endl;
		syntax_checker(i); // DUZELT
		cout<<"line "<<line_count<< " syntax checkerı geçti"<<endl;
		parser(all_lines[i]); // DUZELT
		cout<<"line "<<line_count<< " parserı geçti"<<endl;
		i++;
		line_count++;
	}

	for (int i = 0; i < all_variables.size(); i++)
	{
		out_file_two << "%" << all_variables[i] << "= alloca i32" << endl;
	}
	
	out_file_two << endl;

	for (int i = 0; i < all_variables.size(); i++)
	{
		out_file_two << "store i32 0, i32* %" << all_variables[i] << endl;
	}

	out_file_two << endl;

	string line_two;
	while (getline(in_file_two, line_two))
	{
		out_file_two << line_two << endl;
	}

	out_file_two << "\t" << "ret i32 0" << endl;
	out_file_two << "}";

	return 0;
}