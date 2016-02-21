#ifndef _EASYRESPACKER_NODE_TO_BIN_H_
#define _EASYRESPACKER_NODE_TO_BIN_H_

namespace erespacker
{

class NodeToBin
{
public:
	static int TransTime(float time);
	static int TransRadian(float r);
	static int TransFloat(float f);
	static int TransFloatX100(float f);
	static int TransBool(bool b);

}; // NodeToBin

}

#endif // _EASYRESPACKER_NODE_TO_BIN_H_