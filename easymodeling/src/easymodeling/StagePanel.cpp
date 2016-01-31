
#include "StagePanel.h"
#include "StageCanvas.h"
#include "Context.h"

#include <easymodeling.h>
#include <easyshape.h>
#include <easyphysics.h>

namespace emodeling
{

StagePanel::StagePanel(wxWindow* parent,
					   wxTopLevelWindow* frame)
	: ee::EditPanel(parent, frame)
	, ee::SpritesPanelImpl(GetStageImpl(), Context::Instance()->library)
{
	SetCanvas(new StageCanvas(this));

	RegistSubject(ee::InsertSpriteSJ::Instance());
	RegistSubject(ee::RemoveSpriteSJ::Instance());
	RegistSubject(ee::ClearSpriteSJ::Instance());
}

StagePanel::~StagePanel()
{
	Clear();
}

ee::Sprite* StagePanel::QuerySpriteByPos(const ee::Vector& pos) const
{
	ee::Sprite* result = NULL;
	TraverseSprites(PointQueryVisitor(pos, &result), ee::DT_EDITABLE, false);
	return result;
}

void StagePanel::QuerySpritesByRect(const ee::Rect& rect, std::vector<ee::Sprite*>& result) const
{
	TraverseSprites(RectQueryVisitor(rect, result), ee::DT_EDITABLE);
}

libmodeling::Joint* StagePanel::queryJointByPos(const ee::Vector& pos) const
{
	for (size_t i = 0, n = m_joints.size(); i < n; ++i)
		if (m_joints[i]->isContain(pos))
			return m_joints[i];
	return NULL;
}

void StagePanel::queryJointsByRect(const ee::Rect& rect, std::vector<libmodeling::Joint*>& result) const
{
	for (size_t i = 0, n = m_joints.size(); i < n; ++i)
	{
		if (m_joints[i]->isIntersect(rect))
			result.push_back(m_joints[i]);
	}
}

void StagePanel::removeJoint(libmodeling::Joint* joint)
{
	for (size_t i = 0, n = m_joints.size(); i < n; ++i)
	{
		if (m_joints[i] == joint)
		{
			delete m_joints[i];
			m_joints.erase(m_joints.begin() + i);
			break;
		}
	}
}

void StagePanel::traverseBodies(ee::Visitor& visitor) const
{
	std::vector<libmodeling::Body*>::const_iterator itr = m_bodies.begin();
	for ( ; itr != m_bodies.end(); ++itr)
	{
		bool next;
		visitor.Visit(*itr, next);
		if (!next) break;
	}
}

void StagePanel::traverseJoints(ee::Visitor& visitor) const
{
	std::vector<libmodeling::Joint*>::const_iterator itr = m_joints.begin();
	for ( ; itr != m_joints.end(); ++itr)
	{
		bool next;
		visitor.Visit(*itr, next);
		if (!next) break;
	}
}

void StagePanel::OnNotify(int sj_id, void* ud)
{
	MultiSpritesImpl::OnNotify(sj_id, ud);

	switch (sj_id)
	{
	case ee::MSG_INSERT_SPRITE:
		{
			ee::InsertSpriteSJ::Params* p = (ee::InsertSpriteSJ::Params*)ud;
			Insert(p->spr);
		}
		break;
	case ee::MSG_REMOVE_SPRITE:
		Remove((ee::Sprite*)ud);
		break;
	case ee::MSG_CLEAR_SPRITE:
		Clear();
		break;
	}
}

void StagePanel::loadBody(const wxString& filepath, libmodeling::Body& body)
{
	ee::Symbol* bg = NULL;
	std::vector<ee::Shape*> shapes;
	libshape::FileIO::LoadFromFile(filepath.mb_str(), shapes, bg);
	for (size_t i = 0, n = shapes.size();  i< n; ++i)
	{
		libmodeling::Fixture* fixture = new libmodeling::Fixture;
		fixture->body = &body;

		fixture->shape = shapes[i];

		// 		if (libshape::ChainShape* chain = dynamic_cast<libshape::ChainShape*>(shapes[i]))
		// 		{
		// 			fixture->shape = new libshape::ChainShape(chain->getVertices(), true);
		// 		}
		// 		else if (libshape::RectShape* rect = dynamic_cast<libshape::RectShape*>(shapes[i]))
		// 		{
		// 			std::vector<ee::Vector> vertices(4);
		//  			vertices[0] = ee::Vector(rect->m_rect.xmin, rect->m_rect.ymin);
		//  			vertices[1] = ee::Vector(rect->m_rect.xmax, rect->m_rect.ymin);
		//  			vertices[2] = ee::Vector(rect->m_rect.xmax, rect->m_rect.ymax);
		//  			vertices[3] = ee::Vector(rect->m_rect.xmin, rect->m_rect.ymax);
		// 
		// 			fixture->shape = new libshape::ChainShape(vertices, true);
		// 		}
		// 		else if (libshape::CircleShape* circle = dynamic_cast<libshape::CircleShape*>(shapes[i]))
		// 		{
		// 			fixture->shape = new libshape::CircleShape(ee::Vector(), circle->radius);
		// 		}
		body.fixtures.push_back(fixture);

		//		shapes[i]->Release();
	}
}

void StagePanel::loadBody(ee::Sprite* sprite, libmodeling::Body& body)
{
	libmodeling::Fixture* fixture = new libmodeling::Fixture;
	fixture->body = &body;

	const float width = sprite->GetSymbol().GetSize().Width(),
		height = sprite->GetSymbol().GetSize().Height();
	fixture->shape = new libshape::RectShape(ee::Vector(0, 0), width * 0.5f, height * 0.5f);

	body.fixtures.push_back(fixture);
}

void StagePanel::Insert(ee::Sprite* spr)
{
	wxString filepath = ee::FileHelper::GetFilenameAddTag(
		spr->GetSymbol().GetFilepath(), libshape::FILE_TAG, "json");
	if (!ee::FileHelper::IsFileExist(filepath)) {
		return;
	}

	ee::Symbol* bg = NULL;
	std::vector<ee::Shape*> shapes;
	libshape::FileIO::LoadFromFile(filepath.mb_str(), shapes, bg);
	libmodeling::Body* body = new libmodeling::Body;
	for (int i = 0, n = shapes.size(); i< n; ++i)
	{
		libmodeling::Fixture* fixture = new libmodeling::Fixture;
		fixture->body = body;

		fixture->shape = shapes[i];
		body->fixtures.push_back(fixture);
	}

	body->sprite = spr;
	spr->SetUserData(body);
	m_bodies.push_back(body);
}

void StagePanel::Remove(ee::Sprite* spr)
{
	for (size_t i = 0, n = m_bodies.size(); i < n; ++i)
	{
		if (m_bodies[i]->sprite == spr)
		{
			delete m_bodies[i];
			m_bodies.erase(m_bodies.begin() + i);
			break;
		}
	}
}

void StagePanel::Clear()
{
	for_each(m_bodies.begin(), m_bodies.end(), DeletePointerFunctor<libmodeling::Body>());
	m_bodies.clear();
	for_each(m_joints.begin(), m_joints.end(), DeletePointerFunctor<libmodeling::Joint>());
	m_joints.clear();
}

//////////////////////////////////////////////////////////////////////////
// class MultiSpritesImpl::PointQueryVisitor
//////////////////////////////////////////////////////////////////////////

StagePanel::PointQueryVisitor::
	PointQueryVisitor(const ee::Vector& pos, ee::Sprite** pResult)
	: m_pos(pos)
{
	m_pResult = pResult;
	*m_pResult = NULL;
}

void StagePanel::PointQueryVisitor::
	Visit(ee::Object* object, bool& next)
{
	ee::Sprite* sprite = static_cast<ee::Sprite*>(object);
	libmodeling::Body* data = static_cast<libmodeling::Body*>(sprite->GetUserData());
	if (data->isContain(m_pos))
	{
		*m_pResult = sprite;
		next = false;
	}
	else
	{
		next = true;
	}
}

//////////////////////////////////////////////////////////////////////////
// class MultiSpritesImpl::RectQueryVisitor
//////////////////////////////////////////////////////////////////////////

StagePanel::RectQueryVisitor::
	RectQueryVisitor(const ee::Rect& rect, std::vector<ee::Sprite*>& result)
	: m_rect(rect), m_result(result)
{
}

void StagePanel::RectQueryVisitor::
	Visit(ee::Object* object, bool& next)
{
	ee::Sprite* sprite = static_cast<ee::Sprite*>(object);
	libmodeling::Body* data = static_cast<libmodeling::Body*>(sprite->GetUserData());
	if (data->isIntersect(m_rect))
		m_result.push_back(sprite);
	next = true;
}

}