#include<iostream>
#include<map>
#include<stack>
#include<queue>
#include<stdlib.h>
#include<stdio.h>
//#include<algorithm>

using namespace std;

class Calculator
{
	
private:
	queue<string> m_qPostfix; 
	stack<char> m_sSymbol;
	stack<string> m_sExpression;
	map<string,string> m_mVariable;
	
public:
	bool Operation(string i_str);
	void Operation2();
	bool CheckVariable(string& i_str);

	float Calculation(float i_n1 , float i_n2 , string i_str);


	int ChkString(char i_c);	
	bool ChkPriority(char i_c);

	Calculator();
	void Start();

	void print();

};
