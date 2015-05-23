#include "RectIcon.h"

namespace eicon
{

RectIcon::RectIcon()
	: m_min(0, 0)
	, m_max(1, 1)
{
}

void RectIcon::Draw(const d2d::Matrix& mt) const
{
	if (!m_img) {
		return;
	}

	d2d::Rect region = m_img->getRegion();
	float xmin, xmax, ymin, ymax;
	xmin = region.xMin + region.xLength() * m_min.x;
	xmax = region.xMin + region.xLength() * m_max.x;
	ymin = region.yMin + region.yLength() * m_min.y;
	ymax = region.yMin + region.yLength() * m_max.y;

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
		txmin = m_min.x;
		txmax = m_max.x;
		tymin = m_min.y;
		tymax = m_max.y;
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

void RectIcon::SetHoriRegion(float xmin, float xmax)
{
	m_min.x = xmin;
	m_max.x = xmax;

	if (m_img) {
		float xlen = m_img->originWidth();
		m_region.xMin = xlen * (m_min.x - 0.5f);
		m_region.xMax = xlen * (m_max.x - 0.5f);
	}
}

void RectIcon::SetVertRegion(float ymin, float ymax)
{
	m_min.y = ymin;
	m_max.y = ymax;

	if (m_img) {
		float ylen = m_img->originHeight();
		m_region.yMin = ylen * (m_min.y - 0.5f);
		m_region.yMax = ylen * (m_max.y - 0.5f);	
	}
}

void RectIcon::OnImageChanged()
{
	if (!m_img) {
		return;
	}

	float xlen = m_img->originWidth();
	m_region.xMin = xlen * (m_min.x - 0.5f);
	m_region.xMax = xlen * (m_max.x - 0.5f);

	float ylen = m_img->originHeight();
	m_region.yMin = ylen * (m_min.y - 0.5f);
	m_region.yMax = ylen * (m_max.y - 0.5f);	
}

}