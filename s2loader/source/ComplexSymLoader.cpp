#include "s2loader/ComplexSymLoader.h"
#include "s2loader/SpriteFactory.h"
#include "s2loader/SprTransLoader.h"

#include <sprite2/Sprite.h>
#include <sprite2/ComplexSymbol.h>
#include <simp/NodeComplex.h>
#include <gum/FilepathHelper.h>

#include <fstream>

namespace s2loader
{

ComplexSymLoader::ComplexSymLoader(s2::ComplexSymbol& sym)
	: m_sym(sym)
{
}

void ComplexSymLoader::LoadJson(const CU_STR& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	sm::rect scissor;
	scissor.xmin = static_cast<float>(value["xmin"].asInt());
	scissor.xmax = static_cast<float>(value["xmax"].asInt());
	scissor.ymin = static_cast<float>(value["ymin"].asInt());
	scissor.ymax = static_cast<float>(value["ymax"].asInt());
	m_sym.SetScissor(scissor);

	CU_STR dir = gum::FilepathHelper::Dir(filepath);

	m_sym.Clear();
	for (int i = 0, n = value["sprite"].size(); i < n; ++i) {
		auto spr = SpriteFactory::Instance()->Create(value["sprite"][i], dir);
		if (spr) {
			m_sym.Add(spr);
		}
	}

//	LoadJsonAction(value, m_sym);
}

void ComplexSymLoader::LoadBin(const simp::NodeComplex* node)
{
	sm::rect scissor;
	scissor.xmin = node->scissor[0];
	scissor.ymin = node->scissor[1];
	scissor.xmax = node->scissor[2];
	scissor.ymax = node->scissor[3];
	m_sym.SetScissor(scissor);

	CU_VEC<s2::SprPtr> children;
	children.reserve(node->sprs_n);

	m_sym.Clear();
	for (int i = 0; i < node->sprs_n; ++i) 
	{
		auto spr = SpriteFactory::Instance()->Create(node->sprs[i]);
		children.push_back(spr);
		if (!spr) {
			continue;
		}

		SprTransLoader::Load(spr, node->trans[i]);
		m_sym.Add(spr);
	}

	CU_VEC<s2::ComplexSymbol::Action> dst;
	dst.reserve(node->actions_n);
	for (int i = 0; i < node->actions_n; ++i) 
	{
		const simp::NodeComplex::Action& src_action = node->actions[i];
		s2::ComplexSymbol::Action dst_action;
		dst_action.name = src_action.name;
		dst_action.sprs.reserve(src_action.n);
		bool succ = true;
		for (int j = 0; j < src_action.n; ++j) 
		{
			auto& child = children[src_action.idx[j]];
			if (!child) {
				succ = false;
				break;
			}
			dst_action.sprs.push_back(child);
		}
		if (succ) {
			dst.push_back(dst_action);
		}
	}
	m_sym.SetActions(dst);
}

//void ComplexSymLoader::LoadJsonAction(const Json::Value& val, s2::ComplexSymbol* sym)
//{
//	const CU_VEC<s2::Sprite*>& children = sym->GetChildren();
//
//	CU_VEC<Action> src;
//	LoadJsonAction(val, src);
//	CU_VEC<s2::ComplexSymbol::Action> dst;
//	dst.reserve(src.size());
//	for (int i = 0, n = src.size(); i < n; ++i) {
//		const Action& src_action = src[i];
//		s2::ComplexSymbol::Action dst_action;
//		dst_action.name = src_action.name;
//		dst_action.sprs.reserve(src_action.idx.size());
//		for (int j = 0, m = src_action.idx.size(); j < m; ++j) {
//			int idx = src_action.idx[j];
//			if (idx >= 0) {
//				dst_action.sprs.push_back(children[idx]);
//			}
//		}
//		dst.push_back(dst_action);
//	}
//	sym->SetActions(dst);
//}
//
//void ComplexSymLoader::LoadJsonAction(const Json::Value& val, CU_VEC<Action>& actions)
//{
//	if (!val.isMember("action")) {
//		return;
//	}
//
//	const Json::Value& actions_val = val["action"];
//	for (int i = 0, n = actions_val.size(); i < n; ++i) {
//		const Json::Value& src = actions_val[i];
//		Action dst;
//		dst.name = src["name"].asString().c_str();
//		dst.idx.reserve(src["sprite"].size());
//		for (int j = 0, m = src["sprite"].size(); j < m; ++j) {
//			dst.idx.push_back(src["sprite"][j].asInt());
//		}
//		actions.push_back(dst);
//	}
//}

}