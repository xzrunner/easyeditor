#include "MaxRectsBinaryPack2.h"
#include "math.h"

namespace libpacker
{

static const float SCALE = 1.0f;
static const int PADDING = 0;

MaxRectsBinaryPack2::MaxRectsBinaryPack2()
{
}

MaxRectsBinaryPack2::~MaxRectsBinaryPack2()
{
	for_each(m_roots.begin(), m_roots.end(), DeletePointerFunctor<d2d::TPNode>());
}

void MaxRectsBinaryPack2::GetSize(std::vector<RectSize>& sizes) const
{
	for (int i = 0, n = m_roots.size(); i < n; ++i) {
		d2d::TPNode* root = m_roots[i];
		sizes.push_back(RectSize(root->GetWidth(), root->GetHeight()));
	}
}

void MaxRectsBinaryPack2::Pack(PACK_STRATEGY strategy, const std::vector<RectSize>& rects, 
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

	// compute area
	int area = 0;
	for (int i = 0; i < sz; ++i) {
		area += rects[i].width * rects[i].height;
	}

	switch(strategy)
	{
	case PACK_AUTO:
		PackAuto(sprites, area, output);
		break;
	case PACK_SQUARE:
		PackSquare(sprites, area, output);
		break;
	case PACK_SQUARE_MULTI:
		PackSquareMulti(sprites, area, output);
		break;
	default:
		output.clear();
	}
}

d2d::TPNode* MaxRectsBinaryPack2::NewRoot(int w, int h)
{
	d2d::TPNode* root = new d2d::TPNode(w, h);
	d2d::TPNode* c = new d2d::TPNode(w, h);
	root->SetChild(c);
	return root;
}

bool MaxRectsBinaryPack2::Insert(d2d::TPNode* root, const libpacker::Sprite* sprite, int tex_id) const
{
	d2d::TPNode* n = NULL;
	int w = sprite->size->width * SCALE + PADDING * 2,
		h = sprite->size->height * SCALE + PADDING * 2;
	float scale = 1.0f;
	bool rot = false;
	if (root->IsRoomEnough(w, h)) {
		n = root->Insert(w, h);		
	} else {
		rot = true;
		n = root->Insert(h, w);
	}

	if (!n) {
		return false;
	} else {
		sprite->pos->tex_id = tex_id;
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

void MaxRectsBinaryPack2::PackAuto(const std::vector<Sprite>& sprites, int area, 
								   std::vector<Rect>& output)
{
	int edge = next_p2((int)ceil(sqrt((float)area)));
	int w = edge, h = (edge >> 1);

	bool success = false;
	while (!success)
	{
		d2d::TPNode* root = NewRoot(w, h);
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			success = Insert(root, &sprites[i]);
			if (!success) {
				break;
			}
		}

		if (success) {
			m_roots.push_back(root);
		} else {
			delete root;
		}

 		if (w == h) {
 			w *= 2;
 		} else if (w == h * 2) {
 			w /= 2;
 			h *= 2;
 		} else {
 			assert(h == w * 2);
 			w *= 2;
 		}
	}
}

void MaxRectsBinaryPack2::PackSquare(const std::vector<Sprite>& sprites, int area, 
									 std::vector<Rect>& output)
{
	int edge = next_p2((int)ceil(sqrt((float)area)));
	bool success = false;
	while (!success)
	{
		d2d::TPNode* root = NewRoot(edge, edge);
		for (int i = 0, n = sprites.size(); i < n; ++i) {
			success = Insert(root, &sprites[i]);
			if (!success) {
				break;
			}
		}

		if (success) {
			m_roots.push_back(root);
		} else {
			delete root;
		}

		edge *= 2;
	}
}

static inline 
int CalArea(const std::vector<Sprite>& list) {
	int area = 0;
	for (int i = 0, n = list.size(); i < n; ++i) {
		area += list[i].pos->width * list[i].pos->height;
	}
	return area;
}

void MaxRectsBinaryPack2::PackSquareMulti(const std::vector<Sprite>& sprites, int area, 
										  std::vector<Rect>& output)
{
	static const float AREA_SCALE_LIMIT = 0.8f;
//	static const float AREA_LIMIT = 64 * 64;
	static const int EDGE_LIMIT = 256;
	static const int LEFT_AREA_LIMIT = 25000;

	int edge = next_p2((int)ceil(sqrt((float)area)));

	std::vector<Sprite> curr_list = sprites;
	
	int curr_tex = 0;
	while (!curr_list.empty()) 
	{
		std::vector<Sprite> success_list, fail_list;

		d2d::TPNode* root = NewRoot(edge, edge);
		for (int i = 0, n = curr_list.size(); i < n; ++i) 
		{
			const Sprite& spr = curr_list[i];
			bool success = Insert(root, &spr, m_roots.size());
			if (success) {
				spr.pos->tex_id = curr_tex;
				success_list.push_back(spr);
			} else {
				fail_list.push_back(spr);
			}
		}

		int used_area = CalArea(success_list);
		float used_area_rate = (float)used_area / (edge*edge);
		int left_area = edge*edge - used_area;
		if (success_list.empty())
		{
			edge = edge * 2;
			delete root;
		}
		else if (used_area_rate > AREA_SCALE_LIMIT || edge <= EDGE_LIMIT/* || left_area < LEFT_AREA_LIMIT*/) 
		{
			curr_list = fail_list;
			area = area - used_area;
			edge = next_p2((int)ceil(sqrt((float)area)));
			m_roots.push_back(root);
			++curr_tex;
		} 
		else 
		{
			edge = edge / 2;
			delete root;
		}
	}
}

}