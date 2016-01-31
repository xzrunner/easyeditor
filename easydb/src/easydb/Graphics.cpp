#include "Graphics.h"

namespace edb
{
	void Graphics::connect(ee::Sprite* from, ee::Sprite* to) 
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

	void Graphics::move(ee::Sprite* sprite, const ee::Vector& offset) 
	{
		sprite->SetTransform(sprite->GetPosition() + offset, 0);
		Node* node = query(sprite);
		if (node && !node->out.empty())
			for (size_t i = 0, n = node->out.size(); i < n; ++i)
				move(node->out[i], offset);
	}

	Node* Graphics::query(ee::Sprite* sprite) const 
	{
		std::map<ee::Sprite*, Node*>::const_iterator itr
			= connection.find(sprite);
		if (itr == connection.end())
			return NULL;
		else
			return itr->second;
	}

	void Graphics::clear()
	{
		std::map<ee::Sprite*, Node*>::iterator itr = connection.begin();
		for ( ; itr != connection.end(); ++itr)
			delete itr->second;
		connection.clear();
	}
}