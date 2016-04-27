#include "Symbol.h"
#include "Mesh.h"
#include "FileIO.h"
#include "Sprite.h"
#include "MeshFactory.h"

#include <ee/Image.h>
#include <ee/RenderParams.h>
#include <ee/trans_color.h>

#include <shaderlab.h>

namespace emesh
{

Symbol::Symbol()
	: m_image(NULL)
	, m_mesh(NULL)
	, m_pause(false)
{
}

Symbol::Symbol(const Symbol& s)
	: ee::Symbol(s)
{
	s.m_image->Retain();
	m_image = s.m_image;

	m_mesh = s.m_mesh->Clone();
}

Symbol::Symbol(ee::Image* image)
{
	image->Retain();
	m_image = image;

	m_mesh = MeshFactory::Instance()->CreateMesh(*m_image);
}

Symbol::~Symbol()
{
	if (m_image)
	{
		m_image->Release();
		m_image = NULL;
	}
	if (m_mesh)
	{
		m_mesh->Release();
		m_mesh = NULL;
	}
}

Symbol* Symbol::Clone() const 
{ 
	return new Symbol(*this);
}

void Symbol::ReloadTexture() const
{
	if (m_image) {
		m_image->ReloadTexture();
	}
}

void Symbol::Draw(const ee::RenderParams& trans, const ee::Sprite* spr, 
				  const ee::Sprite* root) const
{
	if (!m_mesh) {
		return;
	}

	sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
	mgr->SetShader(sl::SPRITE2);
	sl::Sprite2Shader* shader = static_cast<sl::Sprite2Shader*>(mgr->GetShader());
	shader->SetColor(ee::color2int(trans.color.multi, ee::PT_ABGR),
		ee::color2int(trans.color.add, ee::PT_ABGR));
	shader->SetColorMap(ee::color2int(trans.color.r, ee::PT_ABGR),
		ee::color2int(trans.color.g, ee::PT_ABGR),
		ee::color2int(trans.color.b, ee::PT_ABGR));

	m_mesh->DrawTexture(trans);
	if (!m_pause && spr) 
	{
		const Sprite* s = static_cast<const Sprite*>(spr);
		ee::Vector spd = s->GetSpeed();
		if (spd.x != 0 || spd.y != 0) {
			m_mesh->OffsetUV(spd.x, spd.y);
		}
	}
}

// ee::Rect Symbol::getSize(const ee::Sprite* sprite) const
// {
// //	return m_image->getRegion();
// }

void Symbol::SetMesh(Mesh* mesh)
{
	if (m_mesh) {
		m_mesh->Release();
	}
	mesh->Retain();
	m_mesh = mesh;
}

std::string Symbol::GetImagePath() const
{
	return m_image->GetFilepath();
}

void Symbol::LoadImage(const std::string& filepath)
{
//	ee::BitmapMgr::Instance()->GetItem(filepath, &m_bitmap);
	ee::ImageMgr::Instance()->GetItem(filepath, &m_image);
}

void Symbol::CreateShape()
{
	if (m_mesh) {
		m_mesh->Release();
	}
	m_mesh = MeshFactory::Instance()->CreateMesh(*m_image);
}

void Symbol::LoadResources()
{
	FileIO::Load(m_filepath.c_str(), this);
	InitBounding();
}

void Symbol::InitBounding()
{
	m_region = m_mesh->GetRegion();
}

}