#include "Graphics.h"

namespace edb
{
	void Graphics::connect(d2d::ISprite* from, d2d::ISprite* to) 
	{
		Node* f = query(from);
		if (!f) 
		{
			f = new Node;
			connection.insert(std::make_pair(from, f));
		}
		Node* t = query(to);
		if (!t)
		{
			t = new Node;
			connection.insert(std::make_pair(to, t));
		}
		f->out.push_back(to);
		t->in.push_back(from);
	}

	void Graphics::move(d2d::ISprite* sprite, const d2d::Vector& offset) 
	{
		sprite->setTransform(sprite->getPosition() + offset, 0);
		Node* node = query(sprite);
		if (node && !node->out.empty())
			for (size_t i = 0, n = node->out.size(); i < n; ++i)
				move(node->out[i], offset);
	}

	Node* Graphics::query(d2d::ISprite* sprite) const 
	{
		std::map<d2d::ISprite*, Node*>::const_iterator itr
			= connection.find(sprite);
		if (itr == connection.end())
			return NULL;
		else
			return itr->second;
	}

	void Graphics::clear()
	{
		std::map<d2d::ISprite*, Node*>::iterator itr = connection.begin();
		for ( ; itr != connection.end(); ++itr)
			delete itr->second;
		connection.clear();
	}
}