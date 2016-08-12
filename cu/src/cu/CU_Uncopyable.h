#ifndef _CLASS_UTILITY_UNCOPYABLE_H_
#define _CLASS_UTILITY_UNCOPYABLE_H_

namespace cu
{

class Uncopyable
{
protected:
	Uncopyable() {}
	~Uncopyable() {}

private:
	Uncopyable(const Uncopyable&);
	Uncopyable& operator = (const Uncopyable&);

}; // Uncopyable

}

#endif // _CLASS_UTILITY_UNCOPYABLE_H_