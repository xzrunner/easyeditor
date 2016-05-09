#include "Graphics.h"

#include <ee/Sprite.h>

namespace edb
{

void Graphics::Connect(ee::Sprite* from, ee::Sprite* to) 
{
	Node* f = Query(from);
	if (!f) 
	{
		f = new Node;
		connection.insert(std::make_pair(from, f));
	}
	Node* t = Query(to);
	if (!t)
	{
		t = new Node;
		connection.insert(std::make_pair(to, t));
	}
	f->out.push_back(to);
	t->in.push_back(from);
}

void Graphics::Move(ee::Sprite* sprite, const sm::vec2& offset) 
{
	sprite->SetTransform(sprite->GetPosition() + offset, 0);
	Node* node = Query(sprite);
	if (node && !node->out.empty())
		for (size_t i = 0, n = node->out.size(); i < n; ++i)
			Move(node->out[i], offset);
}

Node* Graphics::Query(ee::Sprite* sprite) const 
{
	std::map<ee::Sprite*, Node*>::const_iterator itr
		= connection.find(sprite);
	if (itr == connection.end())
		return NULL;
	else
		return itr->second;
}

void Graphics::Clear()
{
	std::map<ee::Sprite*, Node*>::iterator itr = connection.begin();
	for ( ; itr != connection.end(); ++itr)
		delete itr->second;
	connection.clear();
}

}