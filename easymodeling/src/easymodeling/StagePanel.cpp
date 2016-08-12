#include "StagePanel.h"
#include "StageCanvas.h"
#include "Context.h"

#include <ee/sprite_msg.h>
#include <ee/panel_msg.h>
#include <ee/subject_id.h>
#include <ee/FileHelper.h>
#include <ee/std_functor.h>

#include <easymodeling.h>
#include <easyshape.h>

#include <algorithm>

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

ee::Sprite* StagePanel::QuerySpriteByPos(const sm::vec2& pos) const
{
	ee::Sprite* result = NULL;
	TraverseSprites(PointQueryVisitor(pos, &result), ee::DT_EDITABLE, false);
	return result;
}

void StagePanel::QuerySpritesByRect(const sm::rect& rect, std::vector<ee::Sprite*>& result) const
{
	TraverseSprites(RectQueryVisitor(rect, result), ee::DT_EDITABLE);
}

Joint* StagePanel::queryJointByPos(const sm::vec2& pos) const
{
	for (size_t i = 0, n = m_joints.size(); i < n; ++i)
		if (m_joints[i]->IsContain(pos))
			return m_joints[i];
	return NULL;
}

void StagePanel::queryJointsByRect(const sm::rect& rect, std::vector<Joint*>& result) const
{
	for (size_t i = 0, n = m_joints.size(); i < n; ++i)
	{
		if (m_joints[i]->IsIntersect(rect))
			result.push_back(m_joints[i]);
	}
}

void StagePanel::removeJoint(Joint* joint)
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
	std::vector<Body*>::const_iterator itr = m_bodies.begin();
	for ( ; itr != m_bodies.end(); ++itr)
	{
		bool next;
		visitor.Visit(*itr, next);
		if (!next) break;
	}
}

void StagePanel::traverseJoints(ee::Visitor& visitor) const
{
	std::vector<Joint*>::const_iterator itr = m_joints.begin();
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

void StagePanel::loadBody(const std::string& filepath, Body& body)
{
	ee::Symbol* bg = NULL;
	std::vector<ee::Shape*> shapes;
	eshape::FileIO::LoadFromFile(filepath.c_str(), shapes, bg);
	for (size_t i = 0, n = shapes.size();  i< n; ++i)
	{
		Fixture* fixture = new Fixture;
		fixture->m_body = &body;

		fixture->m_shape = shapes[i];

		// 		if (eshape::ChainShape* chain = dynamic_cast<eshape::ChainShape*>(shapes[i]))
		// 		{
		// 			fixture->shape = new eshape::ChainShape(chain->getVertices(), true);
		// 		}
		// 		else if (eshape::RectShape* rect = dynamic_cast<eshape::RectShape*>(shapes[i]))
		// 		{
		// 			std::vector<sm::vec2> vertices(4);
		//  			vertices[0] = sm::vec2(rect->m_rect.xmin, rect->m_rect.ymin);
		//  			vertices[1] = sm::vec2(rect->m_rect.xmax, rect->m_rect.ymin);
		//  			vertices[2] = sm::vec2(rect->m_rect.xmax, rect->m_rect.ymax);
		//  			vertices[3] = sm::vec2(rect->m_rect.xmin, rect->m_rect.ymax);
		// 
		// 			fixture->shape = new eshape::ChainShape(vertices, true);
		// 		}
		// 		else if (eshape::CircleShape* circle = dynamic_cast<eshape::CircleShape*>(shapes[i]))
		// 		{
		// 			fixture->shape = new eshape::CircleShape(sm::vec2(), circle->radius);
		// 		}
		body.m_fixtures.push_back(fixture);

		//		shapes[i]->RemoveReference();
	}
}

void StagePanel::loadBody(ee::Sprite* sprite, Body& body)
{
	Fixture* fixture = new Fixture;
	fixture->m_body = &body;

	const float width = sprite->GetSymbol().GetSize().Width(),
		height = sprite->GetSymbol().GetSize().Height();
	fixture->m_shape = new eshape::RectShape(sm::vec2(0, 0), width * 0.5f, height * 0.5f);

	body.m_fixtures.push_back(fixture);
}

void StagePanel::Insert(ee::Sprite* spr)
{
	std::string filepath = ee::FileHelper::GetFilenameAddTag(
		spr->GetSymbol().GetFilepath(), eshape::FILE_TAG, "json");
	if (!ee::FileHelper::IsFileExist(filepath)) {
		return;
	}

	ee::Symbol* bg = NULL;
	std::vector<ee::Shape*> shapes;
	eshape::FileIO::LoadFromFile(filepath.c_str(), shapes, bg);
	Body* body = new Body;
	for (int i = 0, n = shapes.size(); i< n; ++i)
	{
		Fixture* fixture = new Fixture;
		fixture->m_body = body;

		fixture->m_shape = shapes[i];
		body->m_fixtures.push_back(fixture);
	}

	body->m_sprite = spr;
	spr->SetUserData(body);
	m_bodies.push_back(body);
}

void StagePanel::Remove(ee::Sprite* spr)
{
	for (size_t i = 0, n = m_bodies.size(); i < n; ++i)
	{
		if (m_bodies[i]->m_sprite == spr)
		{
			delete m_bodies[i];
			m_bodies.erase(m_bodies.begin() + i);
			break;
		}
	}
}

void StagePanel::Clear()
{
	for_each(m_bodies.begin(), m_bodies.end(), ee::DeletePointerFunctor<Body>());
	m_bodies.clear();
	for_each(m_joints.begin(), m_joints.end(), ee::DeletePointerFunctor<Joint>());
	m_joints.clear();
}

//////////////////////////////////////////////////////////////////////////
// class MultiSpritesImpl::PointQueryVisitor
//////////////////////////////////////////////////////////////////////////

StagePanel::PointQueryVisitor::
	PointQueryVisitor(const sm::vec2& pos, ee::Sprite** pResult)
	: m_pos(pos)
{
	m_pResult = pResult;
	*m_pResult = NULL;
}

void StagePanel::PointQueryVisitor::
	Visit(ee::Object* object, bool& next)
{
	ee::Sprite* sprite = static_cast<ee::Sprite*>(object);
	Body* data = static_cast<Body*>(sprite->GetUserData());
	if (data->IsContain(m_pos))
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
	RectQueryVisitor(const sm::rect& rect, std::vector<ee::Sprite*>& result)
	: m_rect(rect), m_result(result)
{
}

void StagePanel::RectQueryVisitor::
	Visit(ee::Object* object, bool& next)
{
	ee::Sprite* sprite = static_cast<ee::Sprite*>(object);
	Body* data = static_cast<Body*>(sprite->GetUserData());
	if (data->IsIntersect(m_rect))
		m_result.push_back(sprite);
	next = true;
}

}