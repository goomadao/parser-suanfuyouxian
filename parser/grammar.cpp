#include "pch.h"
#include "grammar.h"

istream & operator>>(istream & in, grammar & g)
{
	int n;


	cout << "******************************************************************\n\n***文法中不能包含除了文法符号和'-'、'>'以外的非法字符（空格会自动被忽略）";
	cout << "\n\n***可以单行输入也可以输入F->(E)|i这样带'|'符号的包含多个产生式的表达式\n\n******************************************************************\n\n\n\n";



	cout << "请输入文法(输入单行quit退出)：\n";
	while (1)
	{
		string ss, s;
		getline(in, ss);
		if (ss == "quit")
			break;
		if (ss.empty())
		{
			continue;
		}
		for (int i = 0; i < ss.size(); ++i)
		{
			if (ss[i] != ' ')
				s += ss[i];
		}
		if (s.size() < 4 || s[1] != '-' || s[2] != '>')
		{
			cout << "输入文法不正确，请重新输入(请不要输入数字等除了文法符号和'-'、'>'以外的非法字符)\n";
			continue;
		}
		map<char, set< vector<char> > >::iterator it = g.grammarMap.find(s[0]);
		if (it == g.grammarMap.end())
		{
			set< vector<char> > t1;
			vector<char> t2;
			for (int i = 3; i < s.size(); ++i)
			{
				if (s[i] == '|')
				{
					if (t2.empty())
					{
						t2.push_back('ξ');
					}
					if (t1.find(t2) == t1.end())
					{
						t1.insert(t2);
					}
					else
					{
						cout << "该行中出现了重复的文法，已去重\n";
					}
					t2.clear();
					continue;
				}


				t2.push_back(s[i]);
				if (!isupper(s[i]))
					g.vt.insert(s[i]);



			}
			if (t2.empty())
			{
				t2.push_back('ξ');
			}
			if (t1.find(t2) == t1.end())
			{
				t1.insert(t2);
			}
			else
			{
				cout << "该行中出现了重复的文法，已去重\n";
			}
			

			g.grammarMap.insert(pair<char, set<vector<char> > >(s[0], t1));
		}
		else
		{
			vector<char> t2;
			for (int i = 3; i < s.size(); ++i)
			{
				if (s[i] == '|')
				{
					if (t2.empty())
					{
						t2.push_back('ξ');
					}
					if (it->second.find(t2) == it->second.end())
					{
						it->second.insert(t2);
					}
					else
					{
						cout << "该行中出现了重复的文法，已去重\n";
					}
					t2.clear();
					continue;
				}


				t2.push_back(s[i]);
				if (!isupper(s[i]))
					g.vt.insert(s[i]);

			}

			if (t2.empty())
			{
				t2.push_back('ξ');
			}
			if (it->second.find(t2) == it->second.end())
			{
				it->second.insert(t2);
			}
			else
			{
				cout << "该行中出现了重复的文法，已去重\n";
			}




		}
		cout << s << "\n";
	}
	return in;
	// TODO: 在此处插入 return 语句
}

ostream & operator<<(ostream & out, grammar & g)
{
	map<char, set<vector<char> > >::iterator i;
	for (i = g.grammarMap.begin(); i != g.grammarMap.end(); ++i)
	{
		out << i->first << "->";
		set<vector<char> >::iterator it;
		for (it=i->second.begin(); it!=i->second.end(); ++it)
		{
			for (int k = 0; k < it->size(); ++k)
			{
				out << it->at(k);
			}
			++it;
			if (it != i->second.end())
				cout << "|";
			--it;
		}
		cout << "\n";
	}
	return out;
	// TODO: 在此处插入 return 语句
}

void grammar::clear()
{
	grammarMap.clear();
	vt.clear();
	firstvt.clear();
	lastvt.clear();
	findMap.clear();
	relation.clear();
}

void grammar::print_vt()
{
	for (set<char>::iterator i = vt.begin(); i != vt.end(); ++i)
		cout << *i << " ";
	cout << "\n";
}

void grammar::get_firstvt()
{
	for (map<char, set<vector<char> > >::iterator i = grammarMap.begin(); i != grammarMap.end(); ++i)
		cal_firstvt(i);
}

void grammar::cal_firstvt(map<char, set<vector<char>>>::iterator i)
{
	if (firstvt.find(i->first) != firstvt.end())//已经计算出了这个非终结符的firstvt集
	{
		return;
	}
	firstvt.insert(pair<char, set<char> >(i->first, set<char>{ }));
	for (set<vector<char> >::iterator it = i->second.begin(); it != i->second.end(); ++it)//循环某个产生式
	{
		if (!isupper(it->at(0)))//以终结符开头，直接把这个终结符加入firstvt集
		{
			firstvt.find(i->first)->second.insert(it->at(0));
		}
		else//以非终结符开头
		{
			if (it->size() > 1 && !isupper(it->at(1)))
			{
				firstvt.find(i->first)->second.insert(it->at(1));
			}
			for (map<char, set<vector<char> > >::iterator i1 = grammarMap.begin(); i1 != grammarMap.end(); ++i1)//计算这个非终结符的firstvt
			{
				if (i1->first == it->at(0))
				{
					cal_firstvt(i1);
					break;
				}
			}
			if (firstvt.find(it->at(0)) == firstvt.end())
			{
				firstvt.insert(pair<char, set<char> >(it->at(0), set<char>{ }));
			}



			for (set<char>::iterator i1 = firstvt.find(it->at(0))->second.begin(); i1 != firstvt.find(it->at(0))->second.end(); ++i1)
			{
				firstvt.find(i->first)->second.insert(*i1);
			}
		}
	}
}

void grammar::print_firstvt()
{
	for (map<char, set<char> >::iterator i = firstvt.begin(); i != firstvt.end(); ++i)
	{
		cout << i->first << ": ";
		for (set<char>::iterator it = i->second.begin(); it != i->second.end(); ++it)
		{
			cout << *it << ", ";
		}
		cout << "\n";
	}
}

void grammar::get_lastvt()
{
	for (map<char, set<vector<char> > >::iterator i = grammarMap.begin(); i != grammarMap.end(); ++i)
		cal_lastvt(i);
}

void grammar::cal_lastvt(map<char, set<vector<char> > >::iterator i)
{
	if (lastvt.find(i->first) != lastvt.end())//已经计算出了这个非终结符的lastvt集
	{
		return;
	}
	lastvt.insert(pair<char, set<char> >(i->first, set<char>{ }));
	for (set<vector<char> >::iterator it = i->second.begin(); it != i->second.end(); ++it)//循环某个产生式
	{
		if (!isupper(it->at(it->size() - 1)))//以终结符结尾，直接把这个终结符加入lastvt集
		{
			lastvt.find(i->first)->second.insert(it->at(it->size() - 1));
		}
		else//以非终结符结尾
		{
			if (it->size() > 1 && !isupper(it->at(it->size() - 2)))
			{
				lastvt.find(i->first)->second.insert(it->at(it->size() - 2));
			}
			for (map<char, set<vector<char> > >::iterator i1 = grammarMap.begin(); i1 != grammarMap.end(); ++i1)//计算这个非终结符的lastvt
			{
				if (i1->first == it->at(it->size() - 1))
				{
					cal_lastvt(i1);
					break;
				}
			}

			if (lastvt.find(it->at(it->size() - 1)) == lastvt.end())
			{
				lastvt.insert(pair<char, set<char> >(it->at(it->size() - 1), set<char>{ }));
			}


			for (set<char>::iterator i1 = lastvt.find(it->at(it->size() - 1))->second.begin(); i1 != lastvt.find(it->at(it->size() - 1))->second.end(); ++i1)
			{
				lastvt.find(i->first)->second.insert(*i1);
			}
		}
	}
}

void grammar::print_lastvt()
{
	for (map<char, set<char> >::iterator i = lastvt.begin(); i != lastvt.end(); ++i)
	{
		cout << i->first << ": ";
		for (set<char>::iterator it = i->second.begin(); it != i->second.end(); ++it)
		{
			cout << *it << ", ";
		}
		cout << "\n";
	}
}

void grammar::get_relation()
{
	for (map<char, set<vector<char> > >::iterator i = grammarMap.begin(); i != grammarMap.end(); ++i)//循环每个非终结符
	{
		for (set<vector<char> >::iterator it = i->second.begin(); it != i->second.end(); ++it)//循环每个产生式
		{
			for (int j = 0; j < it->size() - 1; ++j)//循环每个文法符号
			{
				if (!isupper(it->at(j)) && !isupper(it->at(j + 1)))
				{
					add_relation(it->at(j), it->at(j + 1), 0);
					//cout << it->at(j) << " = " << it->at(j + 1) << "\n";
				}
				if (j < it->size() - 2 && !isupper(it->at(j) && !isupper(it->at(j + 2)) && isupper(it->at(j + 1))))
				{
					add_relation(it->at(j), it->at(j + 2), 0);
					//cout << it->at(j) << " = " << it->at(j + 2) << "\n";
				}
				if (!isupper(it->at(j)) && isupper(it->at(j + 1)))
				{
					for (set<char>::iterator i1 = firstvt.find(it->at(j + 1))->second.begin(); i1 != firstvt.find(it->at(j + 1))->second.end(); ++i1)
					{
						add_relation(it->at(j), *i1, -1);
						//cout << it->at(j) << " < " << *i1 << "\n";
					}
				}
				if (isupper(it->at(j)) && !isupper(it->at(j + 1)))
				{
					for (set<char>::iterator i1 = lastvt.find(it->at(j))->second.begin(); i1 != lastvt.find(it->at(j))->second.end(); ++i1)
					{
						add_relation(*i1, it->at(j + 1), 1);
						//cout << *i1 << " > " << it->at(j + 1) << "\n";
					}
				}
			}
		}
	}
}

void grammar::add_relation(char x, char y, short comp)
{
	if (relation.find(pair<char, char>(x, y)) != relation.end())
	{
		relation.find(pair<char, char>(x, y))->second.insert(comp);
	}
	else
	{
		pair<char, char> p1(x, y);
		set<short> s{ comp };
		pair< pair<char, char>, set<short> >p2(p1, s);
		relation.insert(p2);
	}
}

bool grammar::print_relation()
{

	/*for (map<pair<char, char>, set<short> >::iterator i = relation.begin(); i != relation.end(); ++i)
	{
		cout << i->first.first;
		for (set<short>::iterator it = i->second.begin(); it != i->second.end(); ++it)
		{
			cout << " " << char(*it + '=');
		}
		cout << " " << i->first.second << "\n";
	}*/


	bool flag = false;//true代表优先矩阵冲突（两个终结符之间出现两种及以上优先关系）

	cout << "\t";
	for (set<char>::iterator i = vt.begin(); i != vt.end(); ++i)
	{
		cout << *i << "\t";
	}
	cout << "\n\n";
	for (set<char>::iterator i1 = vt.begin(); i1 != vt.end(); ++i1)
	{
		cout << *i1 << "\t";
		for (set<char>::iterator i2 = vt.begin(); i2 != vt.end(); ++i2)
		{
			map<pair<char, char>, set<short> >::iterator i3 = relation.find(pair<char, char>(*i1, *i2));
			if (i3 != relation.end())
			{
				if (i3->second.size() > 1)
					flag = true;
				for (set<short>::iterator i4 = i3->second.begin(); i4 != i3->second.end(); ++i4)
				{
					cout << char(*i4 + '=');
				}
			}
			cout << "\t";
		}
		cout << "\n\n";
	}

	if (flag)
	{
		cout << "\n\nERROR:   算符优先矩阵出现冲突，请检查输入的文法是否正确并重新输入！\n\n";
	}

	return !flag;

}

void grammar::get_find()
{
	for (map<char, set<vector<char> > >::iterator i1 = grammarMap.begin(); i1 != grammarMap.end(); ++i1)
	{
		for (set<vector<char> >::iterator i2 = i1->second.begin(); i2 != i1->second.end(); ++i2)
		{
			if (i2->size() == 1)
			{
				if (findMap.find(i2->at(0)) == findMap.end())
				{
					findMap.insert(pair<char, set<char> >(i2->at(0), { i1->first }));
				}
				else
				{
					findMap.find(i2->at(0))->second.insert(i1->first);
				}
			}
		}
	}
}

void grammar::parsing()
{
	get_code();
	len = code.size();
	cout << "步骤\t栈";
	for (int i = 0; i < len + 2; ++i)
		cout << " ";
	cout << "剩余代码    ";
	for (int i = 0; i < len - 4; ++i)
		cout << " ";
	cout << "  优先关系      动作\t";
	cout << "\n";
	while (code.size() > 1)
	{
		if (comp())//</=移进
		{
			cout << "\n移进";
			parser.push(code.front());
			code.pop();
			print_state();
		}
		else//>规约
		{
			//parser.push(code.front());
			//code.pop();
			cout << "\n规约";
			parser.push(reduction());
			print_state();
		}
	}
	while (parser.size() > 2)
	{
		cout << "\n规约";
		parser.push(reduction());
		print_state();
	}
	while (1)
	{
		bool flag = 1;

		for (map<char, set<vector<char> > >::iterator i1 = grammarMap.begin(); i1 != grammarMap.end(); ++i1)
		{
			for (set<vector<char> >::iterator i2 = i1->second.begin(); i2 != i1->second.end(); ++i2)
			{
				if (i2->size() == 1 && i2->at(0) == parser.top())
				{
					flag = 0;
					cout << "\n规约";
					parser.push(reduction());
					print_state();
				}
			}
		}

		if (flag)
			break;
	}
}

void grammar::get_code()
{
	string c;
	cout << "\n请输入分析代码(输入\"quit\"退出)(如果需要输入\"quit\"请在中间加空格，空格会自动被忽略)：\n";
	while (getline(cin, c))
	{
		if (c == "quit")
		{
			break;
		}
		for (int i = 0; i < c.size(); ++i)
		{
			if (c[i] != ' ')
			{
				code.push(c[i]);
			}
		}
	}
	code.push('#');
	/*while (!code.empty())
	{
		cout << code.front();
		code.pop();
	}*/
	parser.push('#');
}

bool grammar::comp()
{
	if (isupper(code.front()))
	{
		return 1;
	}
	char top = get_top();
	if (top == '#')
	{
		return 1;
	}
	if (*(relation.find(pair<char, char>(top, code.front()))->second.begin()) < 1)
	{
		return 1;
	}
	else
	{
		return 0;
	}

}

char grammar::get_top()
{
	stack<char>temp;
	while (!parser.empty())
	{
		if (!isupper(parser.top()))
		{
			char ret = parser.top();
			while (!temp.empty())
			{
				parser.push(temp.top());
				temp.pop();
			}
			return ret;
		}
		temp.push(parser.top());
		parser.pop();
	}
	return 0;
}

char grammar::reduction()
{
	string sub = "";
	while (isupper(parser.top()))
	{
		sub = parser.top() + sub;
		parser.pop();
	}
	if (parser.top() != '#')
	{
		char now = parser.top();
		sub = now + sub;
		parser.pop();
		while (!parser.empty())
		{
			if (isupper(parser.top()))
			{
				//cout << "supper:";
				sub = parser.top() + sub;
				parser.pop();
			}
			else
			{
				//cout << "relation:";
				if (parser.top() == '#')
					break;
				if (*(relation.find(pair<char, char>(parser.top(), now))->second.begin()) == -1)
				{
					break;
				}
				else
				{
					now = parser.top();
					sub = parser.top() + sub;
					parser.pop();
				}
			}
			//cout << sub << "\n";
		}
	}
	
	//cout << sub << "\n";
	for (map<char, set<vector<char> > >::iterator i1 = grammarMap.begin(); i1 != grammarMap.end(); ++i1)
	{
		for (set<vector<char> >::iterator i2 = i1->second.begin(); i2 != i1->second.end(); ++i2)
		{
			if (i2->size() == sub.size())
			{
				bool flag = 1;
				for (int i = 0; i < i2->size(); ++i)
				{
					/*if (i2->at(i) == sub[i])
					{
						continue;
					}
					else
					{
						if (!findNonTerminal(sub[i], i2->at(i)))
						{
							flag = 0;
							break;
						}
					}*/
					if (i2->at(i) != sub[i])
					{
						flag = 0;
						break;
					}
				}
				if (flag)
				{
					return i1->first;
				}
			}
		}
	}
	for (map<char, set<vector<char> > >::iterator i1 = grammarMap.begin(); i1 != grammarMap.end(); ++i1)
	{
		for (set<vector<char> >::iterator i2 = i1->second.begin(); i2 != i1->second.end(); ++i2)
		{
			if (i2->size() == sub.size())
			{
				//for (int i = 0; i < i2->size(); ++i) cout << i2->at(i);cout << "\n";
				
				bool flag = 1;
				for (int i = 0; i < i2->size(); ++i)
				{
					//cout << i2->at(i) << "\n";
					if (!findNonTerminal(sub[i], i2->at(i)))
					{
						flag = 0;
						break;
					}
				}
				if (flag)
				{
					return i1->first;
				}
			}
		}
	}
}

bool grammar::findNonTerminal(char x, char y)
{
	if (x == y)
	{
		return true;
	}
	if (findMap.find(x) == findMap.end())
		return false;
	for (set<char>::iterator i=findMap.find(x)->second.begin();i!=findMap.find(x)->second.end();++i)
	{
		if (findNonTerminal(*i, y))
		{
			return true;
		}
	}
	return false;
}

void grammar::print_state()
{
	stack<char>ts;
	while (!parser.empty())
	{
		ts.push(parser.top());
		parser.pop();
	}
	while (!ts.empty())
	{
		cout << ts.top();
		parser.push(ts.top());
		ts.pop();
	}
	cout << "\t\t";
	queue<char>tc;
	while (!code.empty())
	{
		cout << code.front();
		tc.push(code.front());
		code.pop();
	}
	cout << "\t\t";
	code = tc;
}





