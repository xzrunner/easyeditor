#include "Code.h"

#include <ee/FontBlankSprite.h>

#include <easycomplex.h>
#include <easybuilder.h>

#include <sprite2/S2_Sprite.h>

#include <queue>

namespace eui
{
namespace window
{

namespace lua = ebuilder::lua;

Code::Code(ebuilder::CodeGenerator& gen)
	: m_gen(gen)
{
}

struct Node
{
	const ee::Sprite* sprite;
	std::string path;
	std::string name;

	Node(const ee::Sprite* s, const std::string& p) 
		: sprite(s), path(p) {}

}; // Node

void Code::ResolveUI(const std::vector<ee::Sprite*>& sprites)
{
	ebuilder::CodeGenerator *gen_init = new ebuilder::CodeGenerator(), 
		*gen_path = new ebuilder::CodeGenerator();
	std::vector<std::pair<std::string, std::string> > text_nodes;
	{
		lua::Function func(*gen_path, "ui:_init_path", 0);

		gen_path->line("local path = {}");

		std::queue<Node> buffer;
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			Node node(sprites[i], "");
			buffer.push(node);
		}

		while (!buffer.empty()) 
		{
			Node parent = buffer.front(); buffer.pop();
			const ee::Sprite* spr = parent.sprite;
	
			const std::string& spr_name = spr->GetName();
			if (spr_name.empty() || spr_name[0] == '_') {
				continue;
			}

			std::string path, name;
			if (parent.path.empty()) {
				path = spr_name;
				name = spr_name;
			} else {
				path = parent.path + "." + spr_name;
				name = parent.name + "_" + spr_name;
			}

			gen_path->line(lua::assign("path."+name, "\""+path+"\""));

			if (const ee::FontBlankSprite* s = dynamic_cast<const ee::FontBlankSprite*>(spr))
			{
				text_nodes.push_back(std::make_pair("path."+name, "i18n."+s->GetTextID()));
			}
			else if (const ecomplex::Sprite* e_spr = dynamic_cast<const ecomplex::Sprite*>(spr)) 
			{
				const std::vector<s2::Sprite*>& children = e_spr->GetSymbol().GetChildren();
				for (int i = 0, n = children.size(); i < n; ++i) {
					ee::Sprite* child = static_cast<ee::Sprite*>(children[i]->GetUD());
					Node node(child, "");
					buffer.push(node);
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

void Code::ResolveText(const std::vector<ee::Sprite*>& sprites)
{
	std::queue<ee::Sprite*> buffer;
	for (int i = 0, n = sprites.size(); i < n; ++i) {
		buffer.push(sprites[i]);
	}

 	while (!buffer.empty()) 
 	{
		ee::Sprite* spr = buffer.front(); buffer.pop();

		const std::string& name = spr->GetName();
		if (name.empty() || name[0] == '_') {
			continue;
		}

		if (ee::FontBlankSprite* s = dynamic_cast<ee::FontBlankSprite*>(spr))
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
		else if (ecomplex::Sprite* e_spr = dynamic_cast<ecomplex::Sprite*>(spr)) 
		{
			const std::vector<s2::Sprite*>& children = e_spr->GetSymbol().GetChildren();
			for (int i = 0, n = children.size(); i < n; ++i) {
				ee::Sprite* child = static_cast<ee::Sprite*>(children[i]->GetUD());
				buffer.push(child);
			}
		}
 	}
}

}
}