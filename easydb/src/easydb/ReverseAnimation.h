#ifndef _EASYDB_REVERSE_ANIMATION_H_
#define _EASYDB_REVERSE_ANIMATION_H_

#include <string>

namespace edb
{

class ReverseAnimation
{
public:
	ReverseAnimation(const std::string& dir);

private:
	void Trigger(const std::string& dir) const;

}; // ReverseAnimation

}

#endif // _EASYDB_REVERSE_ANIMATION_H_