
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
		: d2d::EditPanel(parent, frame)
		, d2d::SpritesPanelImpl(this, Context::Instance()->library)
	{
		m_canvas = new StageCanvas(this);
	}

	StagePanel::~StagePanel()
	{
		clear();
	}

	void StagePanel::clear()
	{
		d2d::EditPanel::clear();
		clearSprites();
	}

	void StagePanel::removeSprite(d2d::ISprite* sprite)
	{
		for (size_t i = 0, n = m_bodies.size(); i < n; ++i)
		{
			if (m_bodies[i]->sprite == sprite)
			{
				delete m_bodies[i];
				m_bodies.erase(m_bodies.begin() + i);
				break;
			}
		}

		d2d::SpritesPanelImpl::removeSprite(sprite);
	}

	void StagePanel::insertSprite(d2d::ISprite* sprite)
	{
		d2d::SpritesPanelImpl::insertSprite(sprite);

		wxString filepath = d2d::FilenameTools::getFilenameAddTag(
			sprite->getSymbol().GetFilepath(), libshape::FILE_TAG, "json");
		if (!d2d::FilenameTools::isExist(filepath)) {
			return;
		}

		d2d::ISymbol* bg = NULL;
		std::vector<d2d::IShape*> shapes;
		libshape::FileIO::LoadFromFile(filepath.mb_str(), shapes, bg);
		libmodeling::Body* body = new libmodeling::Body;
		for (int i = 0, n = shapes.size(); i< n; ++i)
		{
			libmodeling::Fixture* fixture = new libmodeling::Fixture;
			fixture->body = body;

			fixture->shape = shapes[i];
			body->fixtures.push_back(fixture);
		}

		body->sprite = sprite;
		sprite->SetUserData(body);
		m_bodies.push_back(body);
	}

	void StagePanel::clearSprites()
	{
		d2d::SpritesPanelImpl::clearSprites();

		for_each(m_bodies.begin(), m_bodies.end(), DeletePointerFunctor<libmodeling::Body>());
		m_bodies.clear();
		for_each(m_joints.begin(), m_joints.end(), DeletePointerFunctor<libmodeling::Joint>());
		m_joints.clear();
	}

	d2d::ISprite* StagePanel::querySpriteByPos(const d2d::Vector& pos) const
	{
		d2d::ISprite* result = NULL;
		traverseSprites(PointQueryVisitor(pos, &result), d2d::DT_EDITABLE, false);
		return result;
	}

	void StagePanel::querySpritesByRect(const d2d::Rect& rect, std::vector<d2d::ISprite*>& result) const
	{
		traverseSprites(RectQueryVisitor(rect, result), d2d::DT_EDITABLE);
	}

	libmodeling::Joint* StagePanel::queryJointByPos(const d2d::Vector& pos) const
	{
		for (size_t i = 0, n = m_joints.size(); i < n; ++i)
			if (m_joints[i]->isContain(pos))
				return m_joints[i];
		return NULL;
	}

	void StagePanel::queryJointsByRect(const d2d::Rect& rect, std::vector<libmodeling::Joint*>& result) const
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

	void StagePanel::traverseBodies(d2d::IVisitor& visitor) const
	{
		std::vector<libmodeling::Body*>::const_iterator itr = m_bodies.begin();
		for ( ; itr != m_bodies.end(); ++itr)
		{
			bool hasNext;
			visitor.Visit(*itr, hasNext);
			if (!hasNext) break;
		}
	}

	void StagePanel::traverseJoints(d2d::IVisitor& visitor) const
	{
		std::vector<libmodeling::Joint*>::const_iterator itr = m_joints.begin();
		for ( ; itr != m_joints.end(); ++itr)
		{
			bool hasNext;
			visitor.Visit(*itr, hasNext);
			if (!hasNext) break;
		}
	}

	void StagePanel::loadBody(const wxString& filepath, libmodeling::Body& body)
	{
		d2d::ISymbol* bg = NULL;
		std::vector<d2d::IShape*> shapes;
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
			// 			std::vector<d2d::Vector> vertices(4);
			//  			vertices[0] = d2d::Vector(rect->m_rect.xMin, rect->m_rect.yMin);
			//  			vertices[1] = d2d::Vector(rect->m_rect.xMax, rect->m_rect.yMin);
			//  			vertices[2] = d2d::Vector(rect->m_rect.xMax, rect->m_rect.yMax);
			//  			vertices[3] = d2d::Vector(rect->m_rect.xMin, rect->m_rect.yMax);
			// 
			// 			fixture->shape = new libshape::ChainShape(vertices, true);
			// 		}
			// 		else if (libshape::CircleShape* circle = dynamic_cast<libshape::CircleShape*>(shapes[i]))
			// 		{
			// 			fixture->shape = new libshape::CircleShape(d2d::Vector(), circle->radius);
			// 		}
			body.fixtures.push_back(fixture);

			//		shapes[i]->Release();
		}
	}

	void StagePanel::loadBody(d2d::ISprite* sprite, libmodeling::Body& body)
	{
		libmodeling::Fixture* fixture = new libmodeling::Fixture;
		fixture->body = &body;

		const float width = sprite->getSymbol().GetSize().xLength(),
			height = sprite->getSymbol().GetSize().yLength();
		fixture->shape = new libshape::RectShape(d2d::Vector(0, 0), width * 0.5f, height * 0.5f);

		body.fixtures.push_back(fixture);
	}

	//////////////////////////////////////////////////////////////////////////
	// class MultiSpritesImpl::PointQueryVisitor
	//////////////////////////////////////////////////////////////////////////

	StagePanel::PointQueryVisitor::
		PointQueryVisitor(const d2d::Vector& pos, d2d::ISprite** pResult)
		: m_pos(pos)
	{
		m_pResult = pResult;
		*m_pResult = NULL;
	}

	void StagePanel::PointQueryVisitor::
		Visit(d2d::Object* object, bool& bFetchNext)
	{
		d2d::ISprite* sprite = static_cast<d2d::ISprite*>(object);
		libmodeling::Body* data = static_cast<libmodeling::Body*>(sprite->GetUserData());
		if (data->isContain(m_pos))
		{
			*m_pResult = sprite;
			bFetchNext = false;
		}
		else
		{
			bFetchNext = true;
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// class MultiSpritesImpl::RectQueryVisitor
	//////////////////////////////////////////////////////////////////////////

	StagePanel::RectQueryVisitor::
		RectQueryVisitor(const d2d::Rect& rect, std::vector<d2d::ISprite*>& result)
		: m_rect(rect), m_result(result)
	{
	}

	void StagePanel::RectQueryVisitor::
		Visit(d2d::Object* object, bool& bFetchNext)
	{
		d2d::ISprite* sprite = static_cast<d2d::ISprite*>(object);
		libmodeling::Body* data = static_cast<libmodeling::Body*>(sprite->GetUserData());
		if (data->isIntersect(m_rect))
			m_result.push_back(sprite);
		bFetchNext = true;
	}
}