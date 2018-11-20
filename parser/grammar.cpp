#include "pch.h"
#include "grammar.h"

istream & operator>>(istream & in, grammar & g)
{
	int n;
	cout << "请输入文法个数：";
	in >> n;
	while (n--)
	{
		string s;
		in >> s;

	}
	return in;
	// TODO: 在此处插入 return 语句
}

ostream & operator<<(ostream & out, grammar & g)
{
	
	return out;
	// TODO: 在此处插入 return 语句
}

void grammar::get_firstvt()
{

}
