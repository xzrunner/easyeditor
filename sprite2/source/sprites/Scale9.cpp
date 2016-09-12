#include "Scale9.h"
#include "S2_Sprite.h"
#include "S2_Symbol.h"
#include "DrawNode.h"
#include "ImageSymbol.h"
#include "Texture.h"

#include <SM_Calc.h>

#include <string.h>
#include <assert.h>

namespace s2
{

Scale9::Scale9()
	: m_type(S9_NULL)
	, m_width(0)
	, m_height(0)
{
	memset(m_grids, 0, sizeof(m_grids));
}

Scale9::Scale9(const Scale9& s9)
{
	this->operator = (s9);
}

Scale9& Scale9::operator = (const Scale9& s9)
{
	m_type = s9.m_type;
	m_width = s9.m_width;
	m_height = s9.m_height;
	for (int i = 0; i < 9; ++i) {
		Sprite* spr = s9.m_grids[i];
		if (spr) {
			m_grids[i] = VI_CLONE(Sprite, spr);
		} else {
			m_grids[i] = NULL;
		}
	}
	return *this;
}

Scale9::~Scale9()
{
	for (int i = 0; i < 9; ++i) {
		if (m_grids[i]) {
			m_grids[i]->RemoveReference();
		}
	}
}

void Scale9::Draw(const RenderParams& params) const
{
	for (int i = 0; i < 9; ++i) {
		if (m_grids[i]) {
			DrawNode::Draw(m_grids[i], params);
		}
	}
}

void Scale9::SetSize(float width, float height)
{
	if (m_width == width && m_height == height) {
		return;
	}

	m_width = width;
	m_height = height;

	switch (m_type)
	{
	case S9_9GRID:
		{
			float w0 = m_grids[S9_DOWN_LEFT]->GetSymbol()->GetBounding().Size().x,
				  w2 = m_grids[S9_DOWN_RIGHT]->GetSymbol()->GetBounding().Size().x,
				  w1 = width - w0 - w2;
			float h0 = m_grids[S9_DOWN_LEFT]->GetSymbol()->GetBounding().Size().y,
				  h2 = m_grids[S9_TOP_LEFT]->GetSymbol()->GetBounding().Size().y,
				  h1 = height - h0 - h2;

			ResizeSprite(m_grids[S9_DOWN_LEFT], sm::vec2(-w0*0.5f-w1*0.5f, -h0*0.5f-h1*0.5f), w0, h0);
			ResizeSprite(m_grids[S9_DOWN_CENTER], sm::vec2(0.0f, -h0*0.5f-h1*0.5f), w1, h0);
			ResizeSprite(m_grids[S9_DOWN_RIGHT], sm::vec2(w1*0.5f+w2*0.5f, -h0*0.5f-h1*0.5f), w2, h0);

			ResizeSprite(m_grids[S9_MID_LEFT], sm::vec2(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
			ResizeSprite(m_grids[S9_MID_CENTER], sm::vec2(0.0f, 0.0f), w1, h1);
			ResizeSprite(m_grids[S9_MID_RIGHT], sm::vec2(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

			ResizeSprite(m_grids[S9_TOP_LEFT], sm::vec2(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
			ResizeSprite(m_grids[S9_TOP_CENTER], sm::vec2(0.0f, h1*0.5f+h2*0.5f), w1, h2);
			ResizeSprite(m_grids[S9_TOP_RIGHT], sm::vec2(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);		
		}
		break;
	case S9_9GRID_HOLLOW:
		{
			float w0 = m_grids[S9_DOWN_LEFT]->GetSymbol()->GetBounding().Size().x,
				  w2 = m_grids[S9_DOWN_RIGHT]->GetSymbol()->GetBounding().Size().x,
				  w1 = width - w0 - w2;
			float h0 = m_grids[S9_DOWN_LEFT]->GetSymbol()->GetBounding().Size().y,
				  h2 = m_grids[S9_TOP_LEFT]->GetSymbol()->GetBounding().Size().y,
				  h1 = height - h0 - h2;

			ResizeSprite(m_grids[S9_DOWN_LEFT], sm::vec2(-w0*0.5f-w1*0.5f, -h0*0.5f-h1*0.5f), w0, h0);
			ResizeSprite(m_grids[S9_DOWN_CENTER], sm::vec2(0.0f, -h0*0.5f-h1*0.5f), w1, h0);
			ResizeSprite(m_grids[S9_DOWN_RIGHT], sm::vec2(w1*0.5f+w2*0.5f, -h0*0.5f-h1*0.5f), w2, h0);

			ResizeSprite(m_grids[S9_MID_LEFT], sm::vec2(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
			ResizeSprite(m_grids[S9_MID_RIGHT], sm::vec2(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

			ResizeSprite(m_grids[S9_TOP_LEFT], sm::vec2(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
			ResizeSprite(m_grids[S9_TOP_CENTER], sm::vec2(0.0f, h1*0.5f+h2*0.5f), w1, h2);
			ResizeSprite(m_grids[S9_TOP_RIGHT], sm::vec2(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);
		}
		break;
	case S9_6GRID_UPPER:
		{
			float w0 = m_grids[S9_TOP_LEFT]->GetSymbol()->GetBounding().Size().x,
				  w2 = m_grids[S9_TOP_RIGHT]->GetSymbol()->GetBounding().Size().x,
				  w1 = width - w0 - w2;
			float h2 = m_grids[S9_TOP_LEFT]->GetSymbol()->GetBounding().Size().y,
				  h1 = height - h2;

			ResizeSprite(m_grids[S9_MID_LEFT], sm::vec2(-w0*0.5f-w1*0.5f, 0.0f), w0, h1);
			ResizeSprite(m_grids[S9_MID_CENTER], sm::vec2(0.0f, 0.0f), w1, h1);
			ResizeSprite(m_grids[S9_MID_RIGHT], sm::vec2(w1*0.5f+w2*0.5f, 0.0f), w2, h1);

			ResizeSprite(m_grids[S9_TOP_LEFT], sm::vec2(-w0*0.5f-w1*0.5f, h1*0.5f+h2*0.5f), w0, h2);
			ResizeSprite(m_grids[S9_TOP_CENTER], sm::vec2(0.0f, h1*0.5f+h2*0.5f), w1, h2);
			ResizeSprite(m_grids[S9_TOP_RIGHT], sm::vec2(w1*0.5f+w2*0.5f, h1*0.5f+h2*0.5f), w2, h2);
		}
		break;
	case S9_3GRID_HORI:
		{
			float w0 = m_grids[S9_MID_LEFT]->GetSymbol()->GetBounding().Size().x,
				  w2 = m_grids[S9_MID_RIGHT]->GetSymbol()->GetBounding().Size().x,
				  w1 = width - w0 - w2; 

			ResizeSprite(m_grids[S9_MID_LEFT], sm::vec2(-w0*0.5f-w1*0.5f, 0.0f), w0, height);
			ResizeSprite(m_grids[S9_MID_CENTER], sm::vec2(0.0f, 0.0f), w1, height);
			ResizeSprite(m_grids[S9_MID_RIGHT], sm::vec2(w1*0.5f+w2*0.5f, 0.0f), w2, height);
		}
		break;
	case S9_3GRID_VERT:
		{
			float h0 = m_grids[S9_DOWN_CENTER]->GetSymbol()->GetBounding().Size().y,
				  h2 = m_grids[S9_TOP_CENTER]->GetSymbol()->GetBounding().Size().y,
				  h1 = height - h0 - h2;

			ResizeSprite(m_grids[S9_DOWN_CENTER], sm::vec2(0.0f, -h0*0.5f-h1*0.5f), width, h0);
			ResizeSprite(m_grids[S9_MID_CENTER], sm::vec2(0.0f, 0.0f), width, h1);
			ResizeSprite(m_grids[S9_TOP_CENTER], sm::vec2(0.0f, h1*0.5f+h2*0.5f), width, h2);
		}
		break;
	}
}

void Scale9::Build(SCALE9_TYPE type, int w, int h, Sprite* grids[9])
{
	m_type = type;
	m_width = m_height = 0;
	for (int i = 0; i < 9; ++i) 
	{
		Sprite* dst = m_grids[i];
		if (dst) {
			dst->RemoveReference();
		}	
		Sprite* src = grids[i];
		if (src) {
			m_grids[i] = VI_CLONE(Sprite, src);
		} else {
			m_grids[i] = NULL;
		}
	}
	SetSize(w, h);
}

void Scale9::GetGrids(std::vector<Sprite*>& grids) const
{
	for (int i = 0; i < 9; ++i) {
		if (m_grids[i]) {
			grids.push_back(m_grids[i]);
		}
	}
}

SCALE9_TYPE Scale9::CheckType(Sprite* grids[9])
{
	SCALE9_TYPE type = S9_NULL;
	do {
		// S9_9GRID
		type = S9_9GRID;
		for (int i = 0; i < 9; ++i) {
			if (!grids[i]) {
				type = S9_NULL;
				break;
			}
		}
		if (type != S9_NULL) break;

		// S9_9GRID_HOLLOW
		type = S9_9GRID_HOLLOW;
		for (int i = 0; i < 9; ++i) {
			if (i == 4) {
				continue;
			}
			if (!grids[i]) {
				type = S9_NULL;
				break;
			}
		}
		if (type != S9_NULL) break;

		// S9_6GRID_UPPER
		type = S9_6GRID_UPPER;
		for (int i = 3; i < 9; ++i) {
			if (!grids[i]) {
				type = S9_NULL;
				break;
			}
		}
		if (type != S9_NULL) break;

		// S9_3GRID_HORI
		if (grids[3] && grids[4] && grids[5]) {
			type = S9_3GRID_HORI;			
		}

		// S9_3GRID_VERT
		if (grids[1] && grids[4] && grids[7]) {
			type = S9_3GRID_VERT;
		}
	} while (false);
	return type;
}

void Scale9::ResizeSprite(Sprite* spr, const sm::vec2& center, 
						  float width, float height)
{
	if (width < 0) { width = 1; }
	if (height < 0) { height = 1; }

	Symbol* sym = spr->GetSymbol();
	ImageSymbol* img_sym = VI_DOWNCASTING<ImageSymbol*>(sym);
	if (!img_sym) {
		return;
	}

	sm::vec2 sz = img_sym->GetTexture()->GetOriSize();
	assert(sz.x != 0 && sz.y != 0);

	spr->SetPosition(center);

	const sm::vec2& old_scale = spr->GetScale();
	sm::vec2 new_scale;
	const float times = spr->GetAngle() / SM_PI;
	if (times - (int)(times + 0.01f) < 0.3f) {
		new_scale.Set(width / sz.x, height / sz.y);
	} else {
		new_scale.Set(height / sz.x, width / sz.y);
	}
	if (old_scale.x < 0) {
		new_scale.x = -new_scale.x;
	}
	if (old_scale.y < 0) {
		new_scale.y = -new_scale.y;
	}
	spr->SetScale(new_scale);

	spr->Translate(sm::rotate_vector(spr->GetOffset(), spr->GetAngle()) - spr->GetOffset());
}

}