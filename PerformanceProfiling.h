#pragma once
#include<iostream>
#include<map>
#include<windows.h>
#include<time.h>
#include<assert.h>
#include<stdarg.h>
#include<thread>
using namespace std;
typedef long long LongType;
struct PPNode
{
	string _filename;
	string _function;
	int _line;
	string _desc;   //描述
	PPNode(const char* filename, const char*function, int line, const char* desc)
		:_filename(filename)
		, _function(function)
		, _line(line)
		, _desc(desc)
	{}

	bool operator<(const PPNode& node)const
	{
		if (_line < node._line)
		{
			return true;
		}
		else if (_line>node._line)
		{
			return false;
		}
		else
		{
			if (_function < node._function)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
};

struct PPSection
{
	PPSection()
	:_beginTime(0)
	, _costTime(0)
	, _callCount(0)
	{}
	void Begin()
	{
		if (_refCount == 0)
		{
			_beginTime = clock();
		}
		++_refCount;
		++_callCount;
	}

	void End()
	{
		/*--_refCount;
		if (_refCount == 0)
		{
			_costTime += clock() - _beginTime;
		}*/
		_costTime += clock() - _beginTime;
	}

	LongType _beginTime;
	LongType _costTime;
	LongType _callCount;
	LongType _refCount; //引用计数，方便计算时间

};

class SaveAdapter
{
public:
	virtual void Save(const char* fmt, ...) = 0;
};
//控制台适配器
class ConsoleSaveAdapter :public SaveAdapter
{
public:
	virtual void Save(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		vfprintf(stdout, format, args);
		va_end(args);
	}
};

class FileSaveAdapter :public SaveAdapter
{
public:
	FileSaveAdapter(const char* filename)
	{
		_fout = fopen(filename,"w");
		assert(_fout);
	}
	~FileSaveAdapter()
	{
		if (_fout)
		{
			fclose(_fout);
		}
	}


public:
	virtual void Save(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		vfprintf(_fout, format, args);
		va_end(args);
	}
protected:
	//简单、粗暴防拷贝
	FileSaveAdapter(const FileSaveAdapter&);
	FileSaveAdapter& operator=(const FileSaveAdapter&);
protected:
	FILE* _fout;
};
template<class T>
class Singleton
{
public:
	static T* GetInstance()
	{
		assert(_sInstance);
		return _sInstance;
	}
protected:
	static  T* _sInstance;
};

template<class T>
T* Singleton<T>::_sInstance = new T;
//设为单例
class PerformanceProfiler:public Singleton<PerformanceProfiler>
{
	friend class Singleton<PerformanceProfiler>;
public:
	PPSection* CreateSection(const char* filename, const char*function, int line, const char* desc)
	{
		PPNode node(filename,function,line,desc);
		PPSection *section = NULL;
		map<PPNode, PPSection*>::iterator it = _ppMap.find(node);
		if (it!=_ppMap.end())
		{
			section = it->second;
		}
		else
		{
			section = new PPSection;
			_ppMap.insert(pair<PPNode, PPSection*>(node, section));
		}
		return section;
	}
	void OutPut()
	{
		ConsoleSaveAdapter sa;
		_OutPut(sa);
		FileSaveAdapter fsa("PerformanceProfiler.txt");
		_OutPut(fsa);
	}
protected:
	void _OutPut(SaveAdapter& sa)
	{
		int num = 1;
		map<PPNode, PPSection*>::iterator it = _ppMap.begin();
		while (it != _ppMap.end())
		{
			sa.Save("NO%d:desc:%s\n", num++, it->first._desc.c_str());
			sa.Save("Filename:%s,Function:%s, line:%d\n",
				it->first._filename.c_str()
				, it->first._function.c_str()
				, it->first._line);
			sa.Save("CostTime:%.2f,CallCount:%d\n",
				(double)it->second->_costTime / 1000,
				it->second->_callCount);
			it++;
		}
	}
private:
	map<PPNode, PPSection*> _ppMap;
};

struct Release
{
	~Release()
	{
		PerformanceProfiler::GetInstance()->OutPut();
	}
};
#define PERFORMANCE_PROFILER_EE_BEGIN(sign,desc) \
	PPSection *sign##section = PerformanceProfiler::GetInstance()->CreateSection(__FILE__, __FUNCTION__, __LINE__, desc); \
	sign##section->Begin();
#define PERFORMANCE_PROFILER_EE_END(sign) \
	sign##section->End();


void test();
