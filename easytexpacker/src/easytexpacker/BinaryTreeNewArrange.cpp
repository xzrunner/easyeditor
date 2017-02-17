#include "BinaryTreeNewArrange.h"
#include "Context.h"
#include "config.h"

#include <ee/ImageSprite.h>
#include <ee/TPNode.h>
#include <ee/Image.h>
#include <ee/Math2D.h>

namespace etexpacker
{

static const float PADDING = 0;

BinaryTreeNewArrange::BinaryTreeNewArrange()
	: m_root(NULL)
	, m_tex_account(0)
{
}

BinaryTreeNewArrange::~BinaryTreeNewArrange()
{
	delete m_root;
	m_map_images.clear();
}

void BinaryTreeNewArrange::Arrange(const std::vector<ee::ImageSprite*>& sprs)
{
	m_tex_account = 0;

	float tot_w = Context::Instance()->width,
		tot_h = Context::Instance()->height;
	float scale = 1;

	std::vector<ee::ImageSprite*> sorted(sprs);
	SortByMaxEdge(sorted);

	float x_offset = 0;
	while (!sorted.empty())
	{
		std::vector<ee::ImageSprite*> remain;

		InitRoot();
		for (int i = 0, n = sorted.size(); i < n; ++i)
		{
			ee::ImageSprite* s = sorted[i];
			bool success = Insert(*s);
			if (!success) {
				sm::rect r = s->GetSymbol()->GetBounding();
				float w = r.Width() * scale + PADDING*2;
				float h = r.Height() * scale + PADDING*2;
				if ((w > tot_w || h > tot_h) &&
					(w > tot_h || h > tot_w)) {
					sm::vec2 pos;
					pos.x = tot_w * INVALID_SPRITE_OFFSET_FACTOR;
					pos.y = tot_h * INVALID_SPRITE_OFFSET_FACTOR;
					s->SetPosition(pos);
					s->SetAngle(0);
				} else {
					remain.push_back(s);
				}
			} else {
				s->Translate(sm::vec2(x_offset, 0.0f));
			}
		}
		x_offset += Context::Instance()->width * TEXTURE_X_OFFSET_FACTOR;

		sorted = remain;

		m_tex_account++;
	}
}

void BinaryTreeNewArrange::InitRoot()
{
	int w = Context::Instance()->width;
	int h = Context::Instance()->height;

	delete m_root;
	m_root = new ee::TPNode(w, h);

	ee::TPNode* c = new ee::TPNode(w, h);
	m_root->SetChild(c);
}

bool BinaryTreeNewArrange::
Insert(ee::ImageSprite& img) const
{
	std::map<std::string, ee::TPNode*>::const_iterator itr 
		= m_map_images.find(img.GetSymbol()->GetFilepath());
	if (itr != m_map_images.end()) {
		return false;
	}

	ee::TPNode* n = NULL;
	ee::Image* image = img.GetSymbol()->GetImage();
	int w = image->GetClippedRegion().Width(),
		h = image->GetClippedRegion().Height();
	float scale = 1.0f;
	if (m_root->IsRoomEnough(w, h)) {
		n = m_root->Insert(w*scale+PADDING*2, h*scale+PADDING*2);		
	} else {
		n = m_root->Insert(h*scale+PADDING*2, w*scale+PADDING*2);
	}

	if (!n) {
		return false;
	} else {
		sm::vec2 pos;
		pos.x = n->GetCenterX();
		pos.y = n->GetCenterY();
		img.SetPosition(pos);
		float angle = n->IsRotated() ? SM_PI*0.5f : 0;
		img.SetAngle(angle);
		return true;
	}

// 	if (w < h) {
// 		n->is_rotated = !n->is_rotated;
// 	}
}

}