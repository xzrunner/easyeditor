#include "Icon.h"

namespace eicon
{

Icon::Icon()
	: m_img(NULL)
{
}

Icon::Icon(d2d::Image* img)
	: m_img(img)
{
	if (m_img) {
		m_img->Retain();
	}
}

Icon::~Icon()
{
	if (m_img) {
		m_img->Release();
	}
}

void Icon::ReloadTexture() const
{
	if (m_img) {
		m_img->reload();
	}
}

void Icon::SetImage(d2d::Image* img)
{
	bool changed = m_img != img;
	d2d::obj_assign((d2d::Object*&)m_img, img);
	if (changed) {
		OnImageChanged();
	}
}

void Icon::Draw(const d2d::Matrix& mt, const d2d::Rect& r) const
{
	if (!m_img) {
		return;
	}

	d2d::Rect region = m_img->getRegion();
	float xmin, xmax, ymin, ymax;
	xmin = region.xMin + region.xLength() * r.xMin;
	xmax = region.xMin + region.xLength() * r.xMax;
	ymin = region.yMin + region.yLength() * r.yMin;
	ymax = region.yMin + region.yLength() * r.yMax;

	d2d::Vector vertices[4];
	vertices[0] = d2d::Math::transVector(d2d::Vector(xmin, ymin), mt);
	vertices[1] = d2d::Math::transVector(d2d::Vector(xmax, ymin), mt);
	vertices[2] = d2d::Math::transVector(d2d::Vector(xmax, ymax), mt);
	vertices[3] = d2d::Math::transVector(d2d::Vector(xmin, ymax), mt);

	int texid;
	d2d::Vector texcoords[4];
	float txmin, txmax, tymin, tymax;
	d2d::DynamicTexAndFont* dt = NULL;
	const d2d::TPNode* n = NULL;
	if (d2d::Config::Instance()->IsUseDTex()) {
		dt = d2d::DynamicTexAndFont::Instance();
		n = dt->Query(m_img->filepath());
	}
	if (n)
	{
		// todo
		// 		float extend = dt->GetExtend();
		// 		int width = dt->GetWidth();
		// 		int height = dt->GetHeight();
		// 		texid = dt->GetTextureID();
		// 		txmin = (n->GetMinX()+extend) / width;
		// 		txmax = (n->GetMaxX()-extend) / width;
		// 		tymin = (n->GetMinY()+extend) / height;
		// 		tymax = (n->GetMaxY()-extend) / height;
		// 
		// 		if (texid != 1) {
		// 			wxLogDebug(_T("img dt's tex = %d"), texid);
		// 		}
		// 
		// 		if (n->IsRotated())
		// 		{
		// 			d2d::Vector tmp = vertices[3];
		// 			vertices[3] = vertices[2];
		// 			vertices[2] = vertices[1];
		// 			vertices[1] = vertices[0];
		// 			vertices[0] = tmp;
		// 		}
	}
	else
	{
		//wxLogDebug("Fail to insert dtex: %s", m_filepath.c_str());

		texid = m_img->textureID();

		float ori_w = m_img->originWidth(),
			ori_h = m_img->originHeight();
		d2d::Rect clip_r = m_img->getRegion();
		txmin = (clip_r.xMin + r.xMin * clip_r.xLength()) / ori_w + 0.5f;
		txmax = (clip_r.xMin + r.xMax * clip_r.xLength()) / ori_w + 0.5f;
		tymin = (clip_r.yMin + r.yMin * clip_r.yLength()) / ori_h + 0.5f;
		tymax = (clip_r.yMin + r.yMax * clip_r.yLength()) / ori_h + 0.5f;
	}
	texcoords[0].set(txmin, tymin);
	texcoords[1].set(txmax, tymin);
	texcoords[2].set(txmax, tymax);
	texcoords[3].set(txmin, tymax);

	//	RenderList::Instance()->Insert(texid, vertices, texcoords);

	d2d::ShaderMgr* shader = d2d::ShaderMgr::Instance();
	shader->sprite();
	shader->Draw(vertices, texcoords, texid);
}

}