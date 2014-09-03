#include "Code.h"

#include <queue>

#include <easycomplex.h>
#include <easybuilder.h>

namespace ecomplex
{

namespace lua = ebuilder::lua;

Code::Code(ebuilder::CodeGenerator& gen)
	: m_gen(gen)
{
}

void Code::Resolve(const Symbol& symbol)
{
	m_gen.line("function ui:_init_path()");
	m_gen.tab();

	m_gen.line("local path = {}");

	std::queue<Node> buffer;
	Node root(symbol, "");

	buffer.push(root);
	while (!buffer.empty()) 
	{
		Node parent = buffer.front(); buffer.pop();
		const Symbol& parent_symbol = parent.symbol;
		for (int i = 0, n = parent_symbol.m_sprites.size(); i < n; ++i)
		{
			d2d::ISprite* child = parent_symbol.m_sprites[i];
			if (child->name.empty() || child->name[0] == '_') {
				continue;
			}
			
			std::string path, name;
			if (parent.path.empty()) {
				path = child->name;
				name = child->name;
			} else {
				path = parent.path + "." + child->name;
				name = parent.name + "_" + child->name;
			}

			m_gen.line(lua::assign("path."+name, "\""+path+"\""));

			Sprite* s = dynamic_cast<Sprite*>(child);
			if (s) {
				Node next(s->getSymbol(), path);
				next.name = name;
				buffer.push(next);
			}
		}
	}

	m_gen.line("self.v_path = path");

	m_gen.detab();
	m_gen.line("end");
}

}