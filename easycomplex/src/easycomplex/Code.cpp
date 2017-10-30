#include "Code.h"

#include <ee/FontBlankSprite.h>

#include <easycomplex.h>
#include <easybuilder.h>

#include <sprite2/Sprite.h>

#include <queue>

namespace ecomplex
{

namespace lua = ebuilder::lua;

Code::Code(ebuilder::CodeGenerator& gen)
	: m_gen(gen)
{
}

struct Node
{
	const Symbol& sym;
	std::string path;
	std::string name;

	Node(const Symbol& _symbol, const std::string& _path) 
		: sym(_symbol), path(_path) {}
}; // Node

void Code::ResolveUI(const Symbol& sym)
{
	ebuilder::CodeGenerator *gen_init = new ebuilder::CodeGenerator(), 
		*gen_path = new ebuilder::CodeGenerator();
	std::vector<std::pair<std::string, std::string> > text_nodes;
	{
		lua::Function func(*gen_path, "ui:_init_path", 0);

		gen_path->line("local path = {}");

		std::queue<Node> buffer;
		Node root(sym, "");

		buffer.push(root);
		while (!buffer.empty()) 
		{
			Node parent = buffer.front(); buffer.pop();
			const Symbol& parent_symbol = parent.sym;
			auto& children = parent_symbol.GetAllChildren();
			for (int i = 0, n = children.size(); i < n; ++i)
			{
				auto child = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
				
				int name_id = child->GetName();
				if (name_id == -1 || s2::SprNameMap::IsTmpName(name_id)) {
					continue;
				}

				CU_STR cname;
				s2::SprNameMap::Instance()->IDToStr(child->GetName(), cname);

				std::string path, name;
				if (parent.path.empty()) {
					path = cname.c_str();
					name = cname.c_str();
				} else {
					path = parent.path + "." + cname.c_str();
					name = parent.name + "_" + cname.c_str();
				}

				gen_path->line(lua::assign("path."+name, "\""+path+"\""));

				if (auto s = std::dynamic_pointer_cast<Sprite>(child)) 
				{
					Node next(*std::dynamic_pointer_cast<Symbol>(s->GetSymbol()), path);
					next.name = name;
					buffer.push(next);
				} 
				else if (auto s = std::dynamic_pointer_cast<ee::FontBlankSprite>(child))
				{
					text_nodes.push_back(std::make_pair("path."+name, "i18n."+s->GetTextID()));
				}
			}
		}

		gen_path->line("self.v_path = path");
	}
	{
		lua::Function func(*gen_init, "ui:init", 0);
		gen_init->line("local spr = self.v_uis_spr");
		gen_init->line("local path = self.v_path");
		gen_init->line();
		for (int i = 0, n = text_nodes.size(); i < n; ++i)
		{
			const std::pair<std::string, std::string>& text = text_nodes[i];
			gen_init->line(lua::assign(text.first, text.second));
		}
	}

	m_gen.block(*gen_init);
	m_gen.line();
	m_gen.block(*gen_path);
}

void Code::ResolveText(const Symbol& sym)
{
	std::queue<const Symbol*> buffer;
	buffer.push(&sym);
 	while (!buffer.empty()) 
 	{
 		const Symbol* parent = buffer.front(); buffer.pop();
		auto& children = parent->GetAllChildren();
 		for (int i = 0, n = children.size(); i < n; ++i)
 		{
			auto child = std::dynamic_pointer_cast<ee::Sprite>(children[i]);

			int name_id = child->GetName();
			if (name_id == -1 || s2::SprNameMap::IsTmpName(name_id)) {
				continue;
			}
			 
 			if (auto s = std::dynamic_pointer_cast<ee::FontBlankSprite>(child))
 			{
				std::string content = s->GetTextContext();
				size_t pos = 0;
				std::string replace("\\n");
				while ((pos = content.find('\n', pos)) != std::string::npos) {
					content.replace(pos, 1, replace);
					pos += replace.length();
				}

 				m_gen.line(lua::assign("[\""+s->GetTextID()+"\"]", "\""+content+"\","));
 			}
 		}
 	}
}

}