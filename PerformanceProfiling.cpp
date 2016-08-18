#include"PerformanceProfiling.h"


void run(int n)
{
	while (n--)
	{
		PERFORMANCE_PROFILER_EE_BEGIN(net, "����");
		Sleep(1000);
		PERFORMANCE_PROFILER_EE_END(net);
		PERFORMANCE_PROFILER_EE_BEGIN(sql, "���ݿ�");
		Sleep(1000);
		PERFORMANCE_PROFILER_EE_END(sql);
		PERFORMANCE_PROFILER_EE_BEGIN(linux, "LINUX");
		Sleep(1000);
		PERFORMANCE_PROFILER_EE_END(linux);	
	}
	
}

void test()
{
	thread t1(run, 1);
	thread t2(run, 2);
	thread t3(run, 3);

	t1.join();
	t2.join();
	t3.join();
	Release rs;
	/*PPSection* s1 = pp.CreateSection(__FILE__, __FUNCTION__, __LINE__, "���ݿ�");
	s1->Begin();
	Sleep(1000);
	s1->End();

	PPSection* s2 = pp.CreateSection(__FILE__, __FUNCTION__, __LINE__, "����");
	s2->Begin();
	Sleep(1000);
	s2->End();*/
}