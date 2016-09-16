#include "TestPackComplex.h"

#include <simp/NodeFactory.h>
#include <simp/Package.h>
#include <sprite2/DrawNode.h>
#include <sprite2/ComplexSprite.h>
#include <gum/RenderContext.h>
#include <gum/SymbolFactory.h>
#include <gum/SpriteFactory.h>
#include <gum/GUM_GTxt.h>
#include <shaderlab.h>

namespace test
{

void TestPackComplex::Init()
{
	sl::ShaderMgr* sl_mgr = sl::ShaderMgr::Instance();
	sl_mgr->CreateContext(4096);
	sl::RenderContext* sl_rc = sl_mgr->GetContext();
	sl_mgr->CreateShader(sl::SPRITE2, new sl::Sprite2Shader(sl_rc));
	sl_mgr->CreateShader(sl::FILTER, new sl::FilterShader(sl_rc));
	gum::RenderContext::Instance()->SetCamera(0, 0, 1, 1);

 	simp::Package* pkg = new simp::Package("pack/test");
	simp::NodeFactory::Instance()->AddPkg(pkg, "test", 0);

	{
		gum::SymFileType type;
		s2::Symbol* sym = gum::SymbolFactory::Instance()->Create("test", "node", &type);
		s2::Sprite* spr = gum::SpriteFactory::Instance()->Create(sym, type);
		spr->SetPosition(sm::vec2(0, 0));
		m_sprites.push_back(spr);
	}
}

void TestPackComplex::Resize(int width, int height)
{
	gum::RenderContext::Instance()->OnSize(width, height);

	static bool inited = false;
	if (!inited) {
		InitGTxt();
	}
}

void TestPackComplex::Draw() const
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

void TestPackComplex::Update()
{
	for (int i = 0, n = m_sprites.size(); i < n; ++i) 
	{
		s2::Sprite* spr = m_sprites[i];

//		spr->Translate(sm::vec2(0.1, 0.1));

		s2::RenderParams params;
		params.mt = spr->GetTransMatrix();
		params.color = spr->Color();
		spr->Update(params);
	}
}

void TestPackComplex::InitGTxt()
{
	std::vector<std::pair<std::string, std::string> > fonts, user_fonts;
	fonts.push_back(std::make_pair("normal", "SourceHanSansCN-Medium.otf"));
	fonts.push_back(std::make_pair("black", "hkljhw8.ttf"));
	gum::GTxt::Instance()->Init(fonts, user_fonts);
}

}