#include "TestComplex.h"

#include <sprite2/ComplexSprite.h>
#include <sprite2/DrawNode.h>
#include <glue/RenderContext.h>
#include <glue/SymbolFactory.h>
#include <glue/SpriteFactory.h>
#include <glue/GLUE_GTxt.h>
#include <shaderlab.h>

namespace test
{

void TestComplex::Init()
{
	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->CreateContext(4096);
	sl::RenderContext* sl_rc = sl_mgr->GetContext();
	sl_mgr->CreateShader(sl::SPRITE2, new sl::Sprite2Shader(sl_rc));
	glue::RenderContext::Instance()->SetCamera(0, 0, 1, 1);

	s2::Symbol* sym = glue::SymbolFactory::Instance()->Create("test_complex.json");
	{
		s2::ComplexSprite* spr = new s2::ComplexSprite(sym);
		spr->SetPosition(sm::vec2(0, 0));

		s2::Sprite* c = spr->FetchChild("action");
		dynamic_cast<s2::ComplexSprite*>(c)->SetAction("c");

		m_sprites.push_back(spr);
	}
}

void TestComplex::Resize(int width, int height)
{
	glue::RenderContext::Instance()->OnSize(width, height);

	static bool inited = false;
	if (!inited) {
		InitGTxt();
	}
}

void TestComplex::Draw() const
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) 
	{
		s2::Sprite* spr = m_sprites[i];
		s2::RenderParams params;
		params.mt = spr->GetTransMatrix();
		params.color = spr->Color();
		s2::DrawNode::Draw(spr, params);
	}
}

void TestComplex::Update()
{
}

void TestComplex::InitGTxt()
{
	std::vector<std::pair<std::string, std::string> > fonts, user_fonts;
	fonts.push_back(std::make_pair("normal", "SourceHanSansCN-Medium.otf"));
	fonts.push_back(std::make_pair("black", "hkljhw8.ttf"));
	glue::GTxt::Instance()->Init(fonts, user_fonts);
}

}