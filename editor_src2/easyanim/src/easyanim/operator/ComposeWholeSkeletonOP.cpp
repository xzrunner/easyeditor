#include "ComposeWholeSkeletonOP.h"
#include "view/SkeletonEditPanel.h"
#include "tools/Render.h"

namespace eanim
{

ComposeWholeSkeletonOP::ComposeWholeSkeletonOP(SkeletonEditPanel* editPanel, WholeSkeleton* skeleton, 
											   wxTextCtrl* layerTextCtrl)
	: d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>(editPanel, editPanel)
{
	m_skeleton = skeleton;
	m_selected = NULL;
	m_layerTextCtrl = layerTextCtrl;
}

ComposeWholeSkeletonOP::~ComposeWholeSkeletonOP()
{
	translateToCenter();
}

bool ComposeWholeSkeletonOP::onMouseLeftDown(int x, int y)
{
	if (d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::onMouseLeftDown(x, y)) return true;

	d2d::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	m_selected = m_skeleton->queryByPos(pos);

	return false;
}

bool ComposeWholeSkeletonOP::onMouseLeftUp(int x, int y)
{
	if (d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::onMouseLeftUp(x, y)) return true;

	if (m_selected)
	{
		absorbSprite(m_selected);
		m_selected->computePosterityAbsoluteCoords();
		m_editPanel->Refresh();
	}

	updateLayerInfo();

	return false;
}

bool ComposeWholeSkeletonOP::onMouseRightUp(int x, int y)
{
	if (d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::onMouseRightUp(x, y)) return true;

	if (m_selected) 
	{
		m_selected->computePosterityAbsoluteCoords();
		m_editPanel->Refresh();
	}

	updateLayerInfo();

	return false;
}

bool ComposeWholeSkeletonOP::onDraw() const
{
	if (d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::onDraw()) return true;

	if (m_selected) m_selected->drawPosterity();

	return false;
}

void ComposeWholeSkeletonOP::absorbSprite(WholeSkeleton::Sprite* sprite)
{
	if (!m_skeleton->m_root) 
	{
		m_skeleton->m_root = sprite;
		return;
	}
	else if (m_skeleton->m_root == sprite)
	{
		return;
	}

	WholeSkeleton::Sprite* nearest = NULL;
	WholeSkeleton::Node *fromNode = NULL, *toNode = NULL;
	m_skeleton->traverse(NearestNodeVisitor(sprite, &nearest, &fromNode, &toNode));
	assert(nearest && fromNode && toNode);

	d2d::Vector p0 = sprite->getNodeWorldCoords(fromNode),
		p1 = nearest->getNodeWorldCoords(toNode);
	sprite->translate(p1 - p0);
	connect(nearest, sprite, fromNode, toNode);
}

void ComposeWholeSkeletonOP::setSelectedBoneLayer(int layer)
{
	if (m_selected)
	{
		m_selected->setLayer(layer);
		m_editPanel->Refresh();
	}
}

void ComposeWholeSkeletonOP::translateSprite(const d2d::Vector& delta)
{
	d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::translateSprite(delta);

	if (!m_selected) return;

 	if (m_selected->m_parent)
 	{
 		std::vector<WholeSkeleton::Sprite*>::iterator itr = m_selected->m_parent->m_children.begin();
 		for ( ; itr != m_selected->m_parent->m_children.end(); ++itr)
 		{
 			if (*itr == m_selected)
 			{
 				m_selected->m_parent->m_children.erase(itr);
 				break;
 			}
 		}
 		m_selected->m_parent = NULL;
 	}

 	// todo
 	m_selected->computePosterityAbsoluteCoords();
 	m_editPanel->Refresh();
}

void ComposeWholeSkeletonOP::rotateSprite(const d2d::Vector& dst)
{
	if (!m_selected) return;

	if (m_selected->m_parent)
	{
		float angle = d2d::Math::getAngleInDirection(m_selected->getPosition(), m_lastPos, dst);

		d2d::Vector v0 = d2d::Math::rotateVector(m_selected->m_relativeCoords.from->pos, m_selected->getAngle());
		d2d::Vector v1 = d2d::Math::rotateVector(v0, -angle);
		d2d::Vector offset = v1 - v0;
		m_selected->translate(offset);

		m_selected->rotate(angle);

		m_editPanel->Refresh();
	}
	else 
	{
		d2d::ArrangeSpriteOP<d2d::SelectSpritesOP>::rotateSprite(dst);
	}

 	if (m_selected->m_parent)
 	{
 		m_selected->m_relativeCoords.delta = m_selected->getAngle() - m_selected->m_parent->getAngle();
 	}
 	else
 	{
 		m_selected->m_relativeCoords.delta = m_selected->getAngle();
 	}

 	// todo
 	m_selected->computePosterityAbsoluteCoords();
 	m_editPanel->Refresh();
}

void ComposeWholeSkeletonOP::connect(WholeSkeleton::Sprite* parent, WholeSkeleton::Sprite* child,
									 WholeSkeleton::Node* from, WholeSkeleton::Node* to)
{
	if (child->m_parent != parent)
	{
		if (child->m_parent)
		{
			std::vector<WholeSkeleton::Sprite*>::iterator itr 
				= child->m_parent->m_children.begin();
			for ( ; itr != child->m_parent->m_children.end(); ++itr)
			{
				if (*itr == child)
				{
					child->m_parent->m_children.erase(itr);
					break;
				}
			}

		}
		child->m_parent = parent;
		parent->m_children.push_back(child);
	}

	child->m_relativeCoords.from = from;
	child->m_relativeCoords.to = to;
}

void ComposeWholeSkeletonOP::updateLayerInfo()
{
	m_layerTextCtrl->Clear();
	if (m_selected)
	{
		*m_layerTextCtrl << m_selected->m_layer;
	}
}

void ComposeWholeSkeletonOP::translateToCenter()
{
	if (m_skeleton->m_root)
	{
		d2d::AbstractBV* bounding = d2d::BVFactory::createBV(d2d::e_obb);
		m_skeleton->getBounding(*bounding);
		m_skeleton->m_root->translate(-bounding->center());
		m_skeleton->m_root->computePosterityAbsoluteCoords();
		delete bounding;
	}
}

//////////////////////////////////////////////////////////////////////////
// class ComposeWholeSkeletonOP::NearestNodeVisitor
//////////////////////////////////////////////////////////////////////////

ComposeWholeSkeletonOP::NearestNodeVisitor::
NearestNodeVisitor(WholeSkeleton::Sprite* sprite, WholeSkeleton::Sprite** pResult, 
				   WholeSkeleton::Node** pFromNode, WholeSkeleton::Node** pToNode)
{
	m_sprite = sprite;
	m_pResult = pResult;
	m_pFromNode = pFromNode;
	m_pToNode = pToNode;
	m_nearestDis = FLT_MAX;
}

void ComposeWholeSkeletonOP::NearestNodeVisitor::
visit(d2d::ICloneable* object, bool& bFetchNext)
{
	WholeSkeleton::Sprite* toSprite = static_cast<WholeSkeleton::Sprite*>(object);
	if (toSprite == m_sprite)
	{
		bFetchNext = true;
		return;
	}

	const std::vector<WholeSkeleton::Node*> &from = m_sprite->m_body->m_nodes,
		&to = toSprite->m_body->m_nodes;
	for (size_t i = 0, n = from.size(); i < n; ++i)
	{
		for (size_t j = 0, m = to.size(); j < m; ++j)
		{
			d2d::Vector p0 = m_sprite->getNodeWorldCoords(from[i]),
				p1 = toSprite->getNodeWorldCoords(to[j]);
			float dis = d2d::Math::getDistance(p0, p1);
			if (dis < m_nearestDis)
			{
				m_nearestDis = dis;
				*m_pResult = toSprite;
				*m_pFromNode = from[i];
				*m_pToNode = to[j];
			}
		}
	}

	bFetchNext = true;
}

} // eanim