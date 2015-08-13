
#include"Calculator.h"



Calculator::Calculator()
{


}


bool Calculator::CheckVariable(string & i_str)
{
	string::iterator itStart = i_str.begin(); 
	map<string,string>::iterator miter;
	string str;
	bool bVariable = false;
	
	for(string::iterator iter = i_str.begin() ; iter != i_str.end() ; iter++)
	{
		char c = (char)(*iter);
		if(c == ' ')
		{
			continue;
		}

		if(c == '*'||c == '/'||c == '+'||c == '-'||c == '(' || c==')' ) // Variable Substitution
		{
			if(bVariable == true)
			{
				miter = m_mVariable.find(str);
				if(miter != m_mVariable.end())
				{
					i_str.replace(itStart,iter,miter->second);
					iter = i_str.begin();
					itStart = i_str.begin();
				}
				else
				{
					cout << str << " is not Assignment Variable" <<endl;
					return false;
				}
			}
			else
			{
				itStart = iter;
				itStart++;
			}


			bVariable =false;
			str.clear();
		}
		else if(c == '=')  // Variable initialization and Assignment
		{
			if(bVariable == true)
			{
				m_qPostfix.push(str);
				i_str.replace(itStart,++iter,"");
				iter = i_str.begin();
				itStart = i_str.begin();	
			}
			else
			{
				cout << " Non-String Variable " << endl;
				return false;
			}

			bVariable =false;
			str.clear();
		}
		else if( ('A'<=c && c<='Z') /*A...Z */ || ('a'<=c && c<='z')/*a...z*/) // char
		{
			bVariable = true;
			str += c;
			
			iter++;	
			if(iter == i_str.end())
			{
				miter = m_mVariable.find(str);
				if(miter != m_mVariable.end())				
				{
					i_str.replace(itStart,iter,miter->second);
				}
				else
				{
					cout << "Variables can not using null charactor or space charactor or special charactor" << endl;
					return false;
				}
				break;
			}
			else
				iter--;
		}
		else
		{
			str += c;

			iter++;	
			if(iter == i_str.end() && bVariable == true)
			{
				miter = m_mVariable.find(str);
				if(miter != m_mVariable.end())				
				{
					i_str.replace(itStart,iter,miter->second);
				}
				else
				{
					cout << " can not find Variables " << endl;
					return false;
				}
				break;
			}
			else
				iter--;
		}
	}

	return true;
}



bool Calculator::Operation(string i_str)
{
	bool bNegative = false; // Minus Number chk
	int nMinusPos = 0; // if nMinusPos is Pos == 0 , not AddSymbol '+'

	string str;
	string symbol;


	for(string::iterator iter = i_str.begin() ; iter != i_str.end(); iter++ )
	{

		char c = (char)(*iter);
		
		if(c == ' ')
			continue;

		if(48 <=c && c <= 57 /* 0...9 */) // number
		{
			str = c;
			do
			{
				iter++;
				c = (char)(*iter);
				if(c == ' ')
					continue;

				if(('0' <=c && c <= '9') || c == '.' /* 0...9 */) // number
					str += c;
				else
				{
					if(bNegative == true)
					{
						float val = atof(str.c_str());
						char buf[20];
						sprintf(buf,"%f",-val);
						str = buf;
						bNegative = false;
					}
					m_qPostfix.push(str);
					
					break;
				}
			}
			while(iter != i_str.end());
			iter --;
			str.clear();
		}
		else // Symbol
		{
			if(c == '(' /* ( */ ) 
			{
					
				if(bNegative == true)
				{
					
				}
				
				m_sSymbol.push(c);
				symbol.clear();
				
				nMinusPos = 0;
				continue;
			}
			else if(c == '-' )
			{
				while(*iter == '-' || *iter == ' ')
				{
					if(*iter != ' ')
						bNegative ^= true;

					iter++;
				}

				do{
					iter--;

				}while(*iter == ' ');


				if( nMinusPos != 0 )
				{
					//cout <<"nMinusPos :" << nMinusPos << endl;
					ChkPriority('+');
				}
			}
			else
			{
				ChkPriority(c);						
			}  //end if
		} // end if
		nMinusPos++;
	}  // end for
	
	while(m_sSymbol.size() != 0)
	{
		str = m_sSymbol.top();
		m_qPostfix.push(str);
		m_sSymbol.pop();	
	}

	return true;
}

bool Calculator::ChkPriority(char i_c)
{
	int nPriority = ChkString(i_c);

	bool rtnVal = true;

	while( m_sSymbol.size() != 0)
	{
		string strNow ;
		char cNow = m_sSymbol.top();
		
		strNow += cNow;

		int j = ChkString(cNow);

		if(nPriority > j && nPriority != 4) // 4 = )
		{
			break;
		}
		else
		{
			if(nPriority == 4 && j == 1 ) // 1 = '('  4 = ')'
			{
				m_sSymbol.pop();
				rtnVal = false;
				break;
			}
			else
			{
				m_qPostfix.push(strNow);
				m_sSymbol.pop();
			}
		}			
			
	}

	if(rtnVal == true)
		m_sSymbol.push(i_c);

	return rtnVal;
}


int Calculator::ChkString(char i_c)
{
	char c = i_c;
	int i = 0;

	if( c==')')
	{
		i = 4;
	}
	else if( c=='*' || c=='/' ) 
	{
		i = 3;
	}
	else if( c=='+' ) 
	{
		i = 2;
	}
	else if( c=='(' ) 
	{
		i = 1;
	}
	else		// err
		i = 0;

	return i;		
}

void Calculator::Operation2()
{
	string strSymbol,strNumber,strVariable;
	bool bVariable = false;

	
	while(m_qPostfix.size() != 0 )
	{
		strSymbol = m_qPostfix.front();
		
		int nChk = atoi(strSymbol.c_str());
		char cSymbol = *strSymbol.c_str();

		if(nChk != 0 && cSymbol >='0' && cSymbol <='9' ||( cSymbol == '-' && strSymbol.size() > 1  )) //number
		{
			strNumber = strSymbol;
			m_sExpression.push(strNumber);
		}
		else if(cSymbol == '*' || cSymbol == '/' ||cSymbol == '+' || cSymbol == '-') // symbol
		{	
			strNumber = m_sExpression.top();
			float n2 = atof(strNumber.c_str());	
			m_sExpression.pop();

			strNumber = m_sExpression.top();
			float n1 = atof(strNumber.c_str());
			m_sExpression.pop();
			
			float val = Calculation(n1,n2,strSymbol);

			char buf[20];
			sprintf(buf,"%f",val);
			strNumber = buf;

			m_sExpression.push(strNumber);
		}
		else
		{
			strVariable = strSymbol;	
			cout << "Variable = " << strVariable << endl;
			bVariable = true;
		}

		m_qPostfix.pop();
	}

	if(strVariable.empty() == false)
	{
		m_mVariable.insert(make_pair( strVariable,strNumber));
	}	
	
	if(m_sExpression.size() != 0)
	{
		strNumber = m_sExpression.top();
		m_sExpression.pop();

		if(bVariable != true)
			cout <<" Lastval = " <<atof(strNumber.c_str()) <<endl;
		else
			cout << strVariable <<" = " <<atof(strNumber.c_str()) <<endl;
	}
}


float Calculator::Calculation(float i_n1 , float i_n2 , string i_str)
{
	float rtnVal = 0;
	char c = *i_str.begin();
	
	switch(c)
	{
		case '/':
			{
				rtnVal = i_n1 / i_n2;
				cout << i_n1 << " / " << i_n2 << endl;
			}
			break;
		case '*':
			{
				rtnVal = i_n1 * i_n2;
				cout << i_n1 << " * " << i_n2 << endl;
			}
			break;
		case '+':
			{
				rtnVal = i_n1 + i_n2;
				cout << i_n1 << " + " << i_n2 << endl;
			}
			break;
		case '-':
			{
				rtnVal = i_n1 - i_n2;
				cout << i_n1 << " - " << i_n2 << endl;
			}
			break;

		default:
			{
				cout << " === error === " << endl; 
			}
			break;
	}

	return rtnVal;
}




void Calculator::print()
{
	queue<string> qPostfix = m_qPostfix; 

	while(qPostfix.empty() == false)
	{
		cout << qPostfix.front();
		qPostfix.pop();
	}
	
	cout <<endl;
}

void Calculator::Start()
{
	string str;
	while(1)
	{
		char cstr[1000];
		cin.getline(cstr,1000);
		str = cstr;

		if(str.size() == 0)
			continue;

		if(CheckVariable(str) == false)
			continue;

		if(Operation(str) ==  false)
			continue;

		print();		
		Operation2();		
	}
}

int main()
{
	Calculator obj;
	obj.Start();


	return 0;
};






/*
		else if( ('A'<=c && c<='Z')  || ('a'<=c && c<='z') && iter == i_str.begin()) // char
		{
			str = c;
			do
			{
				iter++;
				c = (char)(*iter);
				if(c != '=') // number
				{
					str += c;
				}				
				else
				{
					m_qPostfix.push(str);	
					break;
				}
			}
			while(iter != i_str.end());

			if(iter == i_str.end())
			{
				return false;
			}
		}
*/


