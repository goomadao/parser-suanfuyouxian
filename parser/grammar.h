#pragma once
#ifndef _GRAMMAR_H_
#define _GRAMMAR_H_

#include<map>
#include<set>
#include<vector>
#include<iostream>
#include<list>
#include<string>
#include"grammar.h"
using namespace std;

class grammar
{
private:
	map<char, vector< vector<char> > >gl;
	map<char, set<char> >firstvt;
	map<char, set<char> >lastvt;
public:
	friend istream& operator >>(istream& in, grammar & g);
	friend ostream& operator <<(ostream& out, grammar& g);
	void get_firstvt();

};

#endif // !_GRAMMARLIST_H_
