#include "Code.h"

#include <ee/FontBlankSprite.h>

#include <easycomplex.h>
#include <easybuilder.h>

#include <sprite2/Sprite.h>

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
	const ee::SprConstPtr& spr;
	std::string path;
	std::string name;

	Node(const ee::SprConstPtr& spr, const std::string& p) 
		: spr(spr), path(path) {}

}; // Node

void Code::ResolveUI(const std::vector<ee::SprPtr>& sprs)
{
	ebuilder::CodeGenerator *gen_init = new ebuilder::CodeGenerator(), 
		*gen_path = new ebuilder::CodeGenerator();
	std::vector<std::pair<CU_STR, CU_STR> > text_nodes;
	{
		lua::Function func(*gen_path, "ui:_init_path", 0);

		gen_path->line("local path = {}");

		std::queue<Node> buffer;
		for (int i = 0, n = sprs.size(); i < n; ++i) {
			Node node(sprs[i], "");
			buffer.push(node);
		}

		while (!buffer.empty()) 
		{
			Node parent = buffer.front(); buffer.pop();
			const ee::SprConstPtr& spr = parent.spr;
	
			CU_STR spr_name;
			s2::SprNameMap::Instance()->IDToStr(spr->GetName(), spr_name);
			if (spr_name.empty() || spr_name[0] == '_') {
				continue;
			}

			CU_STR path, name;
			if (parent.path.empty()) {
				path = spr_name;
				name = spr_name;
			} else {
				path = CU_STR(parent.path.c_str()) + "." + spr_name;
				name = CU_STR(parent.name.c_str()) + "_" + spr_name;
			}

			gen_path->line(lua::assign(("path."+name).c_str(), "\""+path+"\""));

			if (auto s = std::dynamic_pointer_cast<const ee::FontBlankSprite>(spr))
			{
				text_nodes.push_back(std::make_pair("path."+name, "i18n."+ CU_STR(s->GetTextID().c_str())));
			}
			else if (auto e_spr = std::dynamic_pointer_cast<const ecomplex::Sprite>(spr)) 
			{
				auto& children = std::dynamic_pointer_cast<const s2::ComplexSymbol>(e_spr->GetSymbol())->GetAllChildren();
				for (int i = 0, n = children.size(); i < n; ++i) {
					auto child = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
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
			auto& text = text_nodes[i];
			gen_init->line(lua::assign(text.first.c_str(), text.second.c_str()));
		}
	}

	m_gen.block(*gen_init);
	m_gen.line();
	m_gen.block(*gen_path);
}

void Code::ResolveText(const std::vector<ee::SprPtr>& sprs)
{
	std::queue<ee::SprPtr> buffer;
	for (int i = 0, n = sprs.size(); i < n; ++i) {
		buffer.push(sprs[i]);
	}

 	while (!buffer.empty()) 
 	{
		auto& spr = buffer.front(); buffer.pop();

		CU_STR name;
		s2::SprNameMap::Instance()->IDToStr(spr->GetName(), name);
		if (name.empty() || name[0] == '_') {
			continue;
		}

		if (auto s = std::dynamic_pointer_cast<ee::FontBlankSprite>(spr))
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
		else if (auto e_spr = std::dynamic_pointer_cast<ecomplex::Sprite>(spr)) 
		{
			auto& children = std::dynamic_pointer_cast<const s2::ComplexSymbol>(e_spr->GetSymbol())->GetAllChildren();
			for (int i = 0, n = children.size(); i < n; ++i) {
				auto child = std::dynamic_pointer_cast<ee::Sprite>(children[i]);
				buffer.push(child);
			}
		}
 	}
}

}
}