#pragma once
#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

#include<map>
#include<set>
#include<vector>
#include<iostream>
#include<list>
#include<string>
#include<queue>
#include<stack>
#include"grammar.h"
using namespace std;

class grammar
{
private:
	map<char, set< vector<char> > >grammarMap;
	set<char>vt;
	map<char, set<char> >firstvt;
	map<char, set<char> >lastvt;
	map<char, set<char> >findMap;
	/*
	*-1——<
	* 0——=
	* 1——>
	*/
	map<pair<char, char>, set<short> >relation;

	queue<char>code;
	stack<char>parser;
	int len;
public:
	friend istream& operator >>(istream& in, grammar & g);
	friend ostream& operator <<(ostream& out, grammar& g);

	void clear();

	void print_vt();

	void get_firstvt();
	void cal_firstvt(map<char, set<vector<char> > >::iterator i);
	void print_firstvt();

	void get_lastvt();
	void cal_lastvt(map<char, set<vector<char> > >::iterator i);
	void print_lastvt();

	void get_relation();
	void add_relation(char x, char y, short comp);
	bool print_relation();

	void get_find();

	void parsing();
	void get_code();
	bool comp();
	char get_top();
	char reduction();
	bool findNonTerminal(char x, char y);
	void print_state();

};

#endif // !_GRAMMARLIST_H_
