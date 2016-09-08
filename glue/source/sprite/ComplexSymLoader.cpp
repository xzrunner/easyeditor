#include "ComplexSymLoader.h"
#include "FilepathHelper.h"
#include "SpriteFactory.h"

#include <sprite2/ComplexSymbol.h>

#include <fstream>

namespace glue
{

ComplexSymLoader::ComplexSymLoader(s2::ComplexSymbol* sym)
	: m_sym(sym)
{
	if (m_sym) {
		m_sym->AddReference();
	}
}

ComplexSymLoader::~ComplexSymLoader()
{
	if (m_sym) {
		m_sym->RemoveReference();
	}
}

void ComplexSymLoader::LoadJson(const std::string& filepath)
{
	if (!m_sym) {
		return;
	}

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
	m_sym->SetScissor(scissor);

	std::string dir = FilepathHelper::Dir(filepath);

	m_sym->Clear();
	int i = 0;
	Json::Value spr_val = value["sprite"][i++];
	while (!spr_val.isNull()) {
		s2::Sprite* spr = SpriteFactory::Instance()->Create(spr_val, dir);
		m_sym->Add(spr);
		spr_val = value["sprite"][i++];
	}
}

}