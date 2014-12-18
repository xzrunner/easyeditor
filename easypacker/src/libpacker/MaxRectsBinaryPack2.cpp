#include "MaxRectsBinaryPack2.h"
#include "math.h"

namespace libpacker
{

static const float SCALE = 1.0f;
static const int PADDING = 0;

MaxRectsBinaryPack2::MaxRectsBinaryPack2()
	: m_root(NULL)
{
}

MaxRectsBinaryPack2::~MaxRectsBinaryPack2()
{
	delete m_root;
}

RectSize MaxRectsBinaryPack2::GetSize() const
{
	RectSize sz;
	if (m_root) {
		sz.width = m_root->GetWidth();
		sz.height = m_root->GetHeight();
	}
	return sz;
}

void MaxRectsBinaryPack2::Pack(const std::vector<RectSize>& rects, 
							   std::vector<Rect>& output)
{
	if (rects.empty()) {
		return;
	}

	// prepare data
	int sz = rects.size();
	output.clear();
	output.resize(sz);

	std::vector<Sprite> sprites;
	sprites.reserve(sz);
	for (int i = 0; i < sz; ++i) {
		sprites.push_back(Sprite(&rects[i], &output[i]));
	}
	std::sort(sprites.begin(), sprites.end(), SpriteCmp(e_area));
	
	// compute base size
	float area = 0;
	for (int i = 0; i < sz; ++i) {
		area += rects[i].width * rects[i].height;
	}
	int edge = next_p2((int)ceil(sqrt(area)));

	// insert
	bool success = false;
	while (!success)
	{
		InitRoot(edge, edge);
		for (int i = 0; i < sz; ++i) {
			success = Insert(&sprites[i]);
			if (!success) {
				break;
			}
		}

		edge *= 2;
	}
}

void MaxRectsBinaryPack2::InitRoot(int w, int h)
{
	delete m_root;
	m_root = new d2d::TPNode(w, h);

	d2d::TPNode* c = new d2d::TPNode(w, h);
	m_root->SetChild(c);
}

bool MaxRectsBinaryPack2::Insert(const libpacker::Sprite* sprite) const
{
	d2d::TPNode* n = NULL;
	int w = sprite->size->width * SCALE + PADDING * 2,
		h = sprite->size->height * SCALE + PADDING * 2;
	float scale = 1.0f;
	bool rot = false;
	if (m_root->IsRoomEnough(w, h)) {
		n = m_root->Insert(w, h);		
	} else {
		rot = true;
		n = m_root->Insert(h, w);
	}

	if (!n) {
		return false;
	} else {
		sprite->pos->x = n->GetMinX() + PADDING;
		sprite->pos->y = n->GetMinY() + PADDING;
		if (n->IsRotated() && !rot || !n->IsRotated() && rot) {
			sprite->pos->width = h - PADDING * 2;
			sprite->pos->height = w - PADDING * 2;
		} else {
			sprite->pos->width = w - PADDING * 2;
			sprite->pos->height = h - PADDING * 2;
		}
		return true;
	}
}

}