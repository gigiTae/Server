#include <iostream>
#include "../Common/ThreadPool.h"

class My
{
public:
	void Test() {}
	int t;

private:


};

int main()
{
	My m;
	m.t = 10;

	auto mem_var = &My::t;
	auto mem_fn = &My::Test;

	std::cout << m.*mem_var;

	return 0;
}