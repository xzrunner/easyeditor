#ifndef _SPRITE2_UNCOPYABLE_H_
#define _SPRITE2_UNCOPYABLE_H_

namespace s2
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

#endif // _SPRITE2_UNCOPYABLE_H_