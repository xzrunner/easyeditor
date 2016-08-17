#ifndef _EASYDB_GRAPHICS_H_
#define _EASYDB_GRAPHICS_H_

#include <SM_Vector.h>

#include <vector>
#include <map>

namespace ee { class Sprite; }

namespace edb
{

struct Node
{
	std::vector<ee::Sprite*> out, in;
};

class Graphics
{
public:
	void Connect(ee::Sprite* from, ee::Sprite* to);

	void Move(ee::Sprite* spr, const sm::vec2& offset);

	Node* Query(ee::Sprite* spr) const;

	void Clear();

public:
	std::map<ee::Sprite*, Node*> connection;
};

}

#endif // _EASYDB_GRAPHICS_H_