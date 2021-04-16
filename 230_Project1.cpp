#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

vector<string> all_lines;
ifstream in_file;
ifstream in_file_two;
ofstream out_file;
ofstream out_file_two;

int syntax_line = -1;
int while_opened = 0;
int temp = 1;
vector<string> all_variables;

void print_module();
void take_all_lines();
bool is_number(string s);
void paranthesis_count(string line, int line_count);
void equal_sign_count(string line, int line_count);
void white_space_eraser(string line);
void while_if_checker(string line, int line_count);
void print_checker(string line, int line_count);
void comment_eraser(string line);
bool isValidVar(string var, int line_count);
string assign_parser(string line, int line_count);
string add_op(string line, int line_count);
string sub_op(string line, int line_count);
string mul_op(string line, int line_count);
string div_op(string line,int line_count);
string par_op(string line, int line_count);
void parser(string line, int line_count);
void syntax_checker(string line, int line_count);
void is_it_all_good(string line, int line_count);


void print_module()
{
	out_file_two << "; ModuleID = 'mylang2ir'" << endl;
	out_file_two << "declare i32 @printf(i*8, ...)" << endl;
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

void white_space_eraser(string line, int i)
{
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

	if (line[5] == '(')
	{

		if (line[line.size() - 1] != ')')
		{

			out_file << "Line " << line_count << ": syntax error.";
			exit(0);
		}
	}
}

void comment_eraser(string line)
{
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] ==	'#')
			{
				i = line.size();
				line = line.substr(0, i);
			}
		}
}

bool isValidVar(string var, int line_count)
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

string assign_parser(string line, int line_count)
{

	int eq_index = line.find('=');
	cout<<"eq index:"<<eq_index<<endl;

	string variable = line.substr(0, eq_index);

	std::vector<string>::iterator it;

	if (isValidVar(variable,line_count)) {

		cout<<"ifi geçtim"<<endl;

		it = find(all_variables.begin(), all_variables.end(), variable);

		if (it == all_variables.end()) {
			cout<<"variable eklendi"<<endl;

			all_variables.push_back(variable);
		}
	}
	else {

		syntax_line = line_count;
	}

	string exp = line.substr(eq_index + 1, line.size());

	cout<<"exp"<<exp<<endl;

	string b= sub_op(exp,line_count);
	if(is_number(b)){

		out_file << "\t" << "store i32 " << b << ", i32* %"<< variable << endl;
	
	}else if(b[0]!='%'){

		out_file << "\t" <<"%! "<< temp << "= load i32 * %" << b<< endl;
		temp++;

		out_file << "\t" << "store i32 %!" << temp-1 << ", i32* %"<< variable << endl;
		
	
		
	}else if(b[0]=='%'){


		out_file << "\t" << "store i32"<< b << ", i32* %"<< variable << endl;
		

	}

	string a= "%!"+to_string(temp-1);

	return a;
}

string add_op(string line, int line_count) 
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

		string right = add_op(line.substr(op_index + 1, line.size()), line_count);
		string left = add_op(line.substr(0, op_index), line_count);

		if(!is_number(left)){

		out_file << "\t" <<"%! "<< temp << "= load i32 * %" << left<< endl;
		
		left= "%!"+to_string(temp);
	
		temp++;
		}
				
		if(!is_number(right)){

		out_file << "\t" <<"%! "<< temp << "= load i32 * %" << right<< endl;
		
		right= "%!"+to_string(temp);

		temp++;
		}
		string a= "%!"+to_string(temp);
		out_file << "\t" << "%!" << temp << " = add i32 " << left << ", " << right << endl;
		temp++;
		return a;
	}
	else {

		return mul_op(line, line_count);
	}

}

string sub_op(string line, int line_count) 
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

		string right = sub_op(line.substr(op_index + 1, line.size()), line_count);
		string left = sub_op(line.substr(0, op_index), line_count);

		if(!is_number(left)){

		out_file << "\t" <<"%! "<< temp << "= load i32 * %" << left<< endl;
		
		left= "%!"+to_string(temp);
	
		temp++;
		}
				
		if(!is_number(right)){

		out_file << "\t" <<"%! "<< temp << "= load i32 * %" << right<< endl;
		
		right= "%!"+to_string(temp);

		temp++;
		}
		string a= "%!"+to_string(temp);
		out_file << "\t" << "%!" << temp << " = sub i32 " << left << ", " << right << endl;
		temp++;
		return a;
	}
	else {
		cout<<"adda return agam"<<endl;
		return add_op(line, line_count);
	}
}

string mul_op(string line, int line_count) 
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

		string right = mul_op(line.substr(op_index + 1, line.size()), line_count);
		string left = mul_op(line.substr(0, op_index), line_count);

		if(!is_number(left)){

		out_file << "\t" <<"%! "<< temp << "= load i32 * %" << left<< endl;
		
		left= "%!"+to_string(temp);
	
		temp++;
		}
				
		if(!is_number(right)){

		out_file << "\t" <<"%! "<< temp << "= load i32 * %" << right<< endl;
		
		right= "%!"+to_string(temp);

		temp++;
		}
		string a= "%!"+to_string(temp);
		out_file << "\t" << "%!" << temp << " = mul i32 " << left << ", " << right << endl;
		temp++;
		return a;

	}
	else {

		return div_op(line, line_count);
	}

}

string div_op(string line,int line_count) 
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

		string right = div_op(line.substr(op_index + 1, line.size()), line_count);
		string left = div_op(line.substr(0, op_index),line_count);

		if(!is_number(left)){

		out_file << "\t" <<"%! "<< temp << "= load i32 * %" << left<< endl;
		
		left= "%!"+to_string(temp);
	
		temp++;
		}
				
		if(!is_number(right)){ // 3+(n+y) gibi bir durumda n+y %! olarak dönecek ve 3+%!   3+%%! olacak

		out_file << "\t" <<"%! "<< temp << "= load i32 * %" << right<< endl;
		
		right= "%!"+to_string(temp);

		temp++;
		}
		string a= "%!"+to_string(temp);
		out_file << "\t" << "%!" << temp << " = sdiv i32 " << left << ", " << right << endl;
		temp++;
		return a;
	}
	else {
		cout<<"para gidiyom agam"<<endl;
		return par_op(line,line_count);
	}


}

string par_op(string line, int line_count) 
{

	if (line[0] == '(' && line[line.size() == ')']) 
	{
		cout<<"ananı siktim"<<endl;

		sub_op(line.substr(1, line.size()), line_count);
	}
	else {

		cout<<"434"<<endl;
		cout<<line<<endl;

		if (is_number(line)) {

			cout<<"sayıyım agam"<<endl;
			return line;

		}
		else if (isValidVar(line,line_count)) {
			
			cout<< "validim" <<endl;
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

			cout<< "ananın amına gir"<<endl;
			
			out_file << "Line " << line_count << ": syntax error.";
			exit(0);
		}

	}



	return line; //!!!!


}

void parser(string line, int line_count)
{

	if (line.find('=') != string::npos)
	{
		cout<< "girdim 454"<<endl;
		assign_parser(line,line_count);
	}
	else if (line.substr(0, 6) == "while(")
	{
		while_opened++;
		int length= line.length();
		string tempp = line.substr(6, length-8);
		cout<<"whilin conditionu: "<<tempp<<endl;
		out_file << "\t" <<"br label% whcond" << endl << endl;
		out_file << "whcond :" << endl;
		
		out_file << "\t" <<"%! "<< temp << "= load i32 * %" << sub_op(tempp, line_count)<< endl;
		temp++;
		out_file << "\t" << "%!" << temp <<"= icmp ne i32 % !"<< temp-1<<", 0 "<< endl;
		temp++;
		out_file << "\t" << "br i1 %!"<<temp-1 <<", label% whbody, label% whend" << endl << endl;
		out_file << "whbody :" << endl;
	}
	else if (line.substr(0, 4) == "if(")
	{
		string tempp = line.substr(3, line.length()-5);
		out_file << "\t" << "br label% whcond" << endl << endl;
		out_file << "whcond :" << endl;


		out_file << "\t" << "%! " << temp << "= load i32 * %" << sub_op(tempp, line_count) << endl;
		temp++;
		out_file << "\t" << "%!" << temp << "= icmp ne i32 % !" << temp - 1 << ", 0 " << endl;
		temp++;
		out_file << "\t" << "br i1 %!" << temp - 1 << ", label% whbody, label% whend" << endl << endl;
		out_file << "whbody :" << endl;
	}

	else if (line.substr(0, 6) == "print(")
	{

		string tempp = line.substr(6, line.length() - 7);

		par_op(tempp, line_count);
		
		out_file << "\t" << "%!" <<temp <<"= load i32 *" << "%" <<tempp <<endl;
		temp++;
		out_file << "\t" << "call i32(i8, ...) @printf(i8 * getelementptr([4 x i8] * @print.str, i32 0, i32 0), i32 % t7)" << endl;
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

void syntax_checker(string line, int line_count)
{
	is_it_all_good(line, line_count);
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
	line_count++;
}

void is_it_all_good(string line, int line_count)
{
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
	out_file_two.open("file1.txt");
	in_file_two.open("file.txt");

	print_module();
	take_all_lines();
	int i = 0;
	
	while (i < all_lines.size()) 
	{
		
		white_space_eraser(all_lines[i], i);
		
		cout<<all_lines[i]<<endl;
		string line= all_lines[i];
		comment_eraser(line);
		syntax_checker(all_lines[i], i);
		cout<<"line "<<i+1<< " syntax checkerı geçti"<<endl;
		parser(all_lines[i], i);
		cout<<"line "<<i+1<< " parserı geçti"<<endl;
		i++;
	}

	for (int i = 0; i < all_variables.size(); i++)
	{
		out_file_two << "%" << all_variables[i] << "= alloca i32" << endl;
	}
	
	out_file_two << endl;

	for (int i = 0; i < all_variables.size(); i++)
	{
		out_file_two << "store i32 0, i32*%" << all_variables[i] << endl;
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