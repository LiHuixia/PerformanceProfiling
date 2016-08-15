#pragma once
#include<iostream>
#include<map>
#include<windows.h>
#include<time.h>
using namespace std;
typedef long long LongType;
struct PPNode
{
	string _filename;
	string _function;
	int _line;
	string _desc;   //√Ë ˆ
	PPNode(const char* filename, const char*function, int line, const char* desc)
		:_filename(filename)
		, _function(function)
		, _line(line)
		, _desc(desc)
	{}

	bool operator<(const PPNode& node)const
	{
		return _line < node._line
			|| _function < node._function
			|| _filename < node._filename
			|| _desc < node._desc;
	}
};

struct PPSection
{
	PPSection()
	:_beginTime(0)
	, _constTime(0)
	, _callCount(0)
	{}
	void Begin()
	{
		_beginTime = clock();
		++_callCount;
	}

	void End()
	{
		_constTime += _beginTime;
	}
private:
	LongType _beginTime;
	LongType _constTime;
	LongType _callCount;

};

class PerformanceProfiler
{
public:
	PPSection* CreateSection(const char* filename, const char*function, int line, const char* desc)
	{
		PPNode *node = NULL;
		PPSection *section = NULL;
		map<PPNode, PPSection*>::iterator it = _ppMap.find(node);
		if (it!=_ppMap.end())
		{
			section = it->second;
		}
		else
		{
			section = new PPSection;
			_ppMap.insert(pair<PPNode, PPSection>(node, section));
		}
		return it->second;
	}
private:
	map<PPNode, PPSection*> _ppMap;
};


void test()
{
	PerformanceProfiler pp;
	PPSection* s1 = pp.CreateSection(__FILE__, __FUNCTION__, __LINE__, " ˝æ›ø‚");
	s1->Begin();
	Sleep(1000);
	s1->End();

	PPSection* s2 = pp.CreateSection(__FILE__, __FUNCTION__, __LINE__, "Õ¯¬Á");
	s2->Begin();
	Sleep(1000);
	s2->End();
}