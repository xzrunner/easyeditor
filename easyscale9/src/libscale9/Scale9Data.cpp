#include "Scale9Data.h"
#include "Sprite.h"

#include <ee/FileHelper.h>
#include <ee/SpriteRenderer.h>
#include <ee/ImageSymbol.h>
#include <ee/Image.h>
#include <ee/SymbolSearcher.h>
#include <ee/SymbolMgr.h>
#include <ee/Exception.h>
#include <ee/SpriteFactory.h>

#include <SM_Calc.h>

#include <fstream>

namespace escale9
{

Scale9Data::Scale9Data()
	: m_width(0)
	, m_height(0)
	, m_type(e_null)
{
	memset(m_sprs, 0, sizeof(m_sprs));
}

Scale9Data::Scale9Data(const Scale9Data& data)
	: m_width(data.m_width)
	, m_height(data.m_height)
	, m_type(data.m_type)
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			ee::Sprite* src = data.m_sprs[i][j];
			if (src) {
				m_sprs[i][j] = src->EEClone();
			} else {
				m_sprs[i][j] = NULL;
			}
		}
	}

	ResizeScale9(m_type, m_sprs, m_width, m_height);
}

Scale9Data::~Scale9Data()
{
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (m_sprs[i][j]) {
				m_sprs[i][j]->RemoveReference();
			}
		}
	}
}

void Scale9Data::ReloadTexture() const
{
	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j) {
			if (m_sprs[i][j]) {
				dynamic_cast<ee::Symbol*>(m_sprs[i][j]->GetSymbol())->ReloadTexture();
			}
		}
	}
}

void Scale9Data::Draw(const s2::RenderParams& params, const s2::Sprite* spr) const
{
	if (spr) {
		const Sprite* scale9_spr = dynamic_cast<const Sprite*>(spr);
		scale9_spr->Draw(params);
	} else {
		DrawScale9(m_type, m_sprs, params);
	}
}

void Scale9Data::GetSize(float& width, float& height) const
{
	width = m_width;
	height = m_height;
}

void Scale9Data::Resize(float width, float height)
{
	if (m_width == width && m_height == height) {
		return;
	}

	m_width = width;
	m_height = height;
	ResizeScale9(m_type, m_sprs, m_width, m_height);	
}

void Scale9Data::Compose(ee::Sprite* sprs[3][3], float width, float height)
{
	m_type = CheckType(sprs);

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (m_sprs[i][j]) {
				m_sprs[i][j]->RemoveReference();
			}

			ee::Sprite* src = sprs[i][j];
			if (src) {
				m_sprs[i][j] = src->EEClone();
			} else {
				m_sprs[i][j] = NULL;
			}
		}
	}

	Resize(width, height);
}

// todo 
void Scale9Data::LoadFromFile(const std::string& filepath)
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	Json::Value spr_val = value["sprite"];

	std::string dir = ee::FileHelper::GetFileDir(filepath);

	m_width = value["width"].asInt();
	m_height = value["height"].asInt();

	int index = 0;

	m_type = Scale9Type(value["type"].asInt());
	switch (m_type)
	{
	case e_9Grid:
		for (size_t i = 0; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				InitSprite(spr_val[i*3+j], &m_sprs[i][j], dir);
			}
		}
		break;
	case e_9GridHollow:
		for (size_t i = 0; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				if (i == 1 && j == 1) continue;
				if (i > 1 || i == 1 && j > 1)
					InitSprite(spr_val[i*3+j-1], &m_sprs[i][j], dir);
				else
					InitSprite(spr_val[i*3+j], &m_sprs[i][j], dir);
			}
		}
		break;
	case e_3GridHor:
		for (size_t i = 0; i < 3; ++i) {
			InitSprite(spr_val[i], &m_sprs[1][i], dir);
		}
		break;
	case e_3GridVer:
		for (size_t i = 0; i < 3; ++i) {
			InitSprite(spr_val[i], &m_sprs[i][1], dir);
		}
		break;
	case e_6GridUpper:
		for (size_t i = 1; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				InitSprite(spr_val[index++], &m_sprs[i][j], dir);
			}
		}
		break;
	}

	ResizeScale9(m_type, m_sprs, m_width, m_height);
}

void Scale9Data::DrawScale9(Scale9Type type, ee::Sprite* const sprs[3][3], 
							const s2::RenderParams& params)
{
	switch (type)
	{
	case e_9Grid:
		for (size_t i = 0; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				if (!sprs[i][j]) continue;
				ee::SpriteRenderer::Draw(sprs[i][j], params);
			}
		}
		break;
	case e_9GridHollow:
		for (size_t i = 0; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				if (i == 1 && j == 1) continue;
				if (!sprs[i][j]) continue;
				ee::SpriteRenderer::Draw(sprs[i][j], params);
			}
		}
		break;
	case e_3GridHor:
		for (size_t i = 0; i < 3; ++i) {
			if (!sprs[1][i]) continue;
			ee::SpriteRenderer::Draw(sprs[1][i], params);
		}
		break;
	case e_3GridVer:
		for (size_t i = 0; i < 3; ++i) {
			if (!sprs[i][1]) continue;
			ee::SpriteRenderer::Draw(sprs[i][1], params);
		}
		break;
	case e_6GridUpper:
		for (size_t i = 1; i < 3; ++i) {
			for (size_t j = 0; j < 3; ++j) {
				if (!sprs[i][j]) continue;
				ee::SpriteRenderer::Draw(sprs[i][j], params);
			}
		}
		break;
	}	
}

void Scale9Data::ResizeScale9(Scale9Type type, ee::Sprite* const sprs[3][3],
							  float width, float height)
{
	if (type == e_9Grid) 
	{
		const float w0 = sprs[0][0]->GetSymbol()->GetBounding().Size().x,
			        w2 = sprs[0][2]->GetSymbol()->GetBounding().Size().x,
			        w1 = width - w0 - w2;
		const float h0 = sprs[0][0]->GetSymbol()->GetBounding().Size().y,
			        h2 = sprs[0][2]->GetSymbol()->GetBounding().Size().y,
			        h1 = height - h0 - h2;

		ResizeSprite(sprs[0][0], sm::vec2(-w0*0.5f-w1*0.5f, -h0*0.5f-h1*0.5f), w0, h0);
		ResizeSprite(sprs[0][1], sm::vec2(0.0f, -h0*0.5f-h1*0.5f), w1, h0);
		ResizeSprite(sprs[0][2], sm::vec2(w1*0.5f+w2*0.5f, -h0*0.5f-h1*0.5f), w2, h0);

		ResizeSprite(sprs[1][0], sm::vec2(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
		ResizeSprite(sprs[1][1], sm::vec2(0.0f, 0.0f), w1, h1);
		ResizeSprite(sprs[1][2], sm::vec2(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

		ResizeSprite(sprs[2][0], sm::vec2(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
		ResizeSprite(sprs[2][1], sm::vec2(0.0f, h1*0.5f+h2*0.5f), w1, h2);
		ResizeSprite(sprs[2][2], sm::vec2(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);		
	}
	else if (type == e_9GridHollow)
	{
		const float w0 = sprs[0][0]->GetSymbol()->GetBounding().Size().x,
			        w2 = sprs[0][2]->GetSymbol()->GetBounding().Size().x,
			        w1 = width - w0 - w2;
		const float h0 = sprs[0][0]->GetSymbol()->GetBounding().Size().y,
			        h2 = sprs[0][2]->GetSymbol()->GetBounding().Size().y,
			        h1 = height - h0 - h2;

		ResizeSprite(sprs[0][0], sm::vec2(-w0*0.5f-w1*0.5f, -h0*0.5f-h1*0.5f), w0, h0);
		ResizeSprite(sprs[0][1], sm::vec2(0.0f, -h0*0.5f-h1*0.5f), w1, h0);
		ResizeSprite(sprs[0][2], sm::vec2(w1*0.5f+w2*0.5f, -h0*0.5f-h1*0.5f), w2, h0);

		ResizeSprite(sprs[1][0], sm::vec2(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
		ResizeSprite(sprs[1][2], sm::vec2(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

		ResizeSprite(sprs[2][0], sm::vec2(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
		ResizeSprite(sprs[2][1], sm::vec2(0.0f, h1*0.5f+h2*0.5f), w1, h2);
		ResizeSprite(sprs[2][2], sm::vec2(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);
	}
	else if (type == e_6GridUpper)
	{
		const float w0 = sprs[2][0]->GetSymbol()->GetBounding().Size().x,
			        w2 = sprs[2][2]->GetSymbol()->GetBounding().Size().x,
			        w1 = width - w0 - w2;
		const float h2 = sprs[2][0]->GetSymbol()->GetBounding().Size().y,
			        h1 = height - h2;

		ResizeSprite(sprs[1][0], sm::vec2(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
		ResizeSprite(sprs[1][1], sm::vec2(0.0f, 0.0f), w1, h1);
		ResizeSprite(sprs[1][2], sm::vec2(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

		ResizeSprite(sprs[2][0], sm::vec2(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
		ResizeSprite(sprs[2][1], sm::vec2(0.0f, h1*0.5f+h2*0.5f), w1, h2);
		ResizeSprite(sprs[2][2], sm::vec2(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);
	}
	else if (type == e_3GridHor)
	{
		const float w0 = sprs[1][0]->GetSymbol()->GetBounding().Size().x,
			        w2 = sprs[1][2]->GetSymbol()->GetBounding().Size().x,
			        w1 = width - w0 - w2; 

		ResizeSprite(sprs[1][0], sm::vec2(-w0*0.5f-w1*0.5f, 0.0f), w0, height);
		ResizeSprite(sprs[1][1], sm::vec2(0.0f, 0.0f), w1, height);
		ResizeSprite(sprs[1][2], sm::vec2(w1*0.5f+w2*0.5f, 0.0f), w2, height);
	}
	else if (type == e_3GridVer)
	{
		const float h0 = sprs[0][1]->GetSymbol()->GetBounding().Size().y,
			        h2 = sprs[2][1]->GetSymbol()->GetBounding().Size().y,
			        h1 = height - h0 - h2;

		ResizeSprite(sprs[0][1], sm::vec2(0.0f, -h0*0.5f-h1*0.5f), width, h0);
		ResizeSprite(sprs[1][1], sm::vec2(0.0f, 0.0f), width, h1);
		ResizeSprite(sprs[2][1], sm::vec2(0.0f, h1*0.5f+h2*0.5f), width, h2);
	}	
}

void Scale9Data::ResizeSprite(ee::Sprite* spr, const sm::vec2& center, 
							  float width, float height)
{
	if (width <= 0) {
		width = 1;
	}
	if (height <= 0) {
		height = 1;
	}

	const ee::ImageSymbol* sym = dynamic_cast<const ee::ImageSymbol*>(spr->GetSymbol());
	int w = sym->GetImage()->GetOriginWidth(),
		h = sym->GetImage()->GetOriginHeight();
	assert(w != 0 && h != 0);

	spr->SetPosition(center);
	const float times = spr->GetAngle() / SM_PI;
	if (times - (int)(times + 0.01f) < 0.3f)
		spr->SetScale(sm::vec2(width / w, height / h));
	else
		spr->SetScale(sm::vec2(height / w, width / h));

	spr->Translate(sm::rotate_vector(spr->GetOffset(), spr->GetAngle()) - spr->GetOffset());
}

Scale9Type Scale9Data::CheckType(ee::Sprite* sprs[3][3])
{
	Scale9Type type = e_null;

	do {
		// 9 grid
		type = e_9Grid;
		for (int i = 0; i < 3 && type == e_9Grid; ++i) {
			for (int j = 0; j < 3 && type == e_9Grid; ++j) {
				if (!sprs[i][j]) type = e_null;
			}
		}
		if (type != e_null) break;

		// 9 grid hollow
		type = e_9GridHollow;
		for (int i = 0; i < 3 && type == e_9GridHollow; ++i) {
			for (int j = 0; j < 3 && type == e_9GridHollow; ++j) {
				if (i == 1 && j == 1) continue;
				if (!sprs[i][j]) type = e_null;
			}
		}
		if (type != e_null) break;

		// 6 grid upper
		type = e_6GridUpper;
		for (int i = 1; i < 3 && type == e_6GridUpper; ++i) {
			for (int j = 0; j < 3 && type == e_6GridUpper; ++j) {
				if (!sprs[i][j]) type = e_null;
			}
		}
		if (type != e_null) break;

		// 3 grid hor
		type = e_3GridHor;
		for (int i = 0; i < 3 && type == e_3GridHor; ++i) {
			if (!sprs[1][i]) type = e_null;
		}
		if (type != e_null) break;

		// 3 grid ver
		type = e_3GridVer;
		for (int i = 0; i < 3 && type == e_3GridVer; ++i) {
			if (!sprs[i][1]) type = e_null;
		}
		if (type != e_null) break;
	} while (false);

	return type;
}

void Scale9Data::InitSprite(const Json::Value& spr_val, ee::Sprite** pSprite, 
							const std::string& dir)
{
	std::string filepath = ee::SymbolSearcher::GetSymbolPath(dir, spr_val);
	ee::Symbol* sym = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
	if (!sym) {
		std::string filepath = spr_val["filepath"].asString();
		throw ee::Exception("Symbol doesn't exist, [dir]:%s, [file]:%s !", 
			dir.c_str(), filepath.c_str());
	}
	ee::SymbolSearcher::SetSymbolFilepaths(dir, sym, spr_val);
	ee::Sprite* spr = ee::SpriteFactory::Instance()->Create(sym);
	spr->Load(spr_val);
	sym->RemoveReference();

	*pSprite = spr;
}

}