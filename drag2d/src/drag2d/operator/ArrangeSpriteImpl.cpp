#include "ArrangeSpriteImpl.h"

#include "common/typedef.h"
#include "common/Rect.h"
#include "common/Matrix.h"
#include "dataset/ISymbol.h"
#include "view/EditPanel.h"
#include "view/PropertySettingPanel.h"
#include "view/MultiSpritesImpl.h"
#include "history/ArrangeSpriteAtomicOP.h"

namespace d2d
{

ArrangeSpriteImpl::ArrangeSpriteImpl(MultiSpritesImpl* spritesImpl,
									 SpriteSelection* selection,
									 PropertySettingPanel* propertyPanel,
									 EditPanel* editPanel,
									 bool& bDirty) 
	: m_align(spritesImpl)
	, m_propertyPanel(propertyPanel)
	, m_editPanel(editPanel)
	, m_bDirty(bDirty)
{
	m_selection = selection;
	m_selection->retain();
}

ArrangeSpriteImpl::~ArrangeSpriteImpl()
{
	m_selection->release();
}

void ArrangeSpriteImpl::translateSprite(const Vector& delta)
{
	m_selection->traverse(TranslateVisitor(delta));
	if (!m_selection->empty()) 
	{
		if (m_propertyPanel && !m_bDirty)
		{
			m_propertyPanel->enablePropertyGrid(false);
			m_bDirty = true;
		}
		m_editPanel->Refresh();
	}
}

void ArrangeSpriteImpl::rotateSprite(const Vector& dst)
{
	if (m_selection->size() != 1) return;
	m_selection->traverse(RotateVisitor(m_lastPos, dst));
	if (m_propertyPanel && !m_bDirty)
	{
		m_propertyPanel->enablePropertyGrid(false);
		m_bDirty = true;
	}
	m_editPanel->Refresh();
}

void ArrangeSpriteImpl::scaleSprite(const Vector& currPos)
{
	if (!m_isDeformOpen) return;

	float hw = m_selected->getSymbol().getSize().xLength() * 0.5f;
	float hh = m_selected->getSymbol().getSize().yLength() * 0.5f;
	love::Matrix t;
	const Vector& center = m_selected->getCenter();
	t.setTransformation(center.x, center.y, m_selected->getAngle(),
		m_selected->getScale().x, m_selected->getScale().y, 0, 0, m_selected->getShear().x, m_selected->getShear().y);

	Vector ori, fix;
	if (m_ctrlNodeSelected.type == UP)
		ori = Math::transVector(Vector(0.0f, hh), t);
	else if (m_ctrlNodeSelected.type == DOWN)
		ori = Math::transVector(Vector(0.0f, -hh), t);
	else if (m_ctrlNodeSelected.type == LEFT)
		ori = Math::transVector(Vector(-hw, 0.0f), t);
	else if (m_ctrlNodeSelected.type == RIGHT)
		ori = Math::transVector(Vector(hw, 0.0f), t);
	else if (m_ctrlNodeSelected.type == LEFT_UP)
		ori = Math::transVector(Vector(-hw, hh), t);
	else if (m_ctrlNodeSelected.type == RIGHT_UP)
		ori = Math::transVector(Vector(hw, hh), t);
	else if (m_ctrlNodeSelected.type == LEFT_DOWN)
		ori = Math::transVector(Vector(-hw, -hh), t);
	else if (m_ctrlNodeSelected.type == RIGHT_DOWN)
		ori = Math::transVector(Vector(hw, -hh), t);
	Math::getFootOfPerpendicular(center, ori, currPos, &fix);

	float scale = Math::getDistance(center, fix) / Math::getDistance(center, ori);
	scale += (1 - scale) * 0.5f;
	if (m_ctrlNodeSelected.type == UP || m_ctrlNodeSelected.type == DOWN)
		m_selected->setScale(m_selected->getScale().x, scale * m_selected->getScale().y);
	else if (m_ctrlNodeSelected.type == LEFT || m_ctrlNodeSelected.type == RIGHT)
		m_selected->setScale(scale * m_selected->getScale().x, m_selected->getScale().y);
	else		
		m_selected->setScale(scale * m_selected->getScale().x, scale * m_selected->getScale().y);

	Vector offset = (fix - ori) * 0.5f;
	m_selected->translate(offset);

	if (m_propertyPanel && !m_bDirty)
	{
		m_propertyPanel->enablePropertyGrid(false);
		m_bDirty = true;
	}
	m_editPanel->Refresh();
}

void ArrangeSpriteImpl::shearSprite(const Vector& currPos)
{
	if (!m_isDeformOpen) return;

	// fix pos
	Vector pos;
	Vector ctrls[8];
	m_impl->GetSpriteCtrlNodes(m_selected, ctrls);
	if (m_ctrlNodeSelected.type == UP)
		Math::getFootOfPerpendicular(ctrls[0], ctrls[1], currPos, &pos);
	else if (m_ctrlNodeSelected.type == DOWN)
		Math::getFootOfPerpendicular(ctrls[2], ctrls[3], currPos, &pos);
	else if (m_ctrlNodeSelected.type == LEFT)
		Math::getFootOfPerpendicular(ctrls[0], ctrls[2], currPos, &pos);
	else if (m_ctrlNodeSelected.type == RIGHT)
		Math::getFootOfPerpendicular(ctrls[1], ctrls[3], currPos, &pos);

	// M * p = p'
	//
	// x' = e0 * x + e4 * y + e12
	// y' = e1 * x + e5 * y + e13
	// 
	// e0 = c * sx - ky * s * sy
	// e4 = kx * c * sx - s * sy
	// e12 = px
	//
	// pos.x = (c * sx - ky * s * sy) * x + (kx * c * sx - s * sy) * y + px
	// pos.y = (s * sx + ky * c * sy) * x + (kx * s * sx + c * sy) * y + py
	// pos.x = c*sx*x - ky*s*sy*x + kx*c*sx*y - s*sy*y + px
	// pos.y = s*sx*x + ky*c*sy*x + kx*s*sx*y + c*sy*y + py
	// 
	// kx = (pos.x - c*sx*x + ky*s*sy*x + s*sy*y - px) / (c*sx*y)
	// kx = (pos.y - s*sx*x - ky*c*sy*x - c*sy*y - py) / (s*sx*y)
	// ky = (pos.x - c*sx*x - kx*c*sx*y + s*sy*y - px) / (-s*sy*x)
	// ky = (pos.y - s*sx*x - kx*s*sx*y - c*sy*y - py) / (c*sy*x)
	float c = cos(m_selected->getAngle()), s = sin(m_selected->getAngle());
	float sx = m_selected->getScale().x, sy = m_selected->getScale().y;
	float px = m_selected->getPosition().x, py = m_selected->getPosition().y;
	float kx = m_selected->getShear().x,
		ky = m_selected->getShear().y;

	float x, y;
	float hw = m_selected->getSymbol().getSize().xLength() * 0.5f,
		hh = m_selected->getSymbol().getSize().yLength() * 0.5f;

	if (m_ctrlNodeSelected.type == UP)
	{
		x = 0; y = hh;
		if (c != 0)
			kx = (pos.x - c*sx*x + ky*s*sy*x + s*sy*y - px) / (c*sx*y);
		else
			kx = (pos.y - s*sx*x - ky*c*sy*x - c*sy*y - py) / (s*sx*y);
	}
	else if (m_ctrlNodeSelected.type == DOWN)
	{
		x = 0; y = -hh;
		if (c != 0)
			kx = (pos.x - c*sx*x + ky*s*sy*x + s*sy*y - px) / (c*sx*y);
		else
			kx = (pos.y - s*sx*x - ky*c*sy*x - c*sy*y - py) / (s*sx*y);
	}
	else if (m_ctrlNodeSelected.type == LEFT)
	{
		x = -hw; y = 0;
		if (s != 0)
			ky = (pos.x - c*sx*x - kx*c*sx*y + s*sy*y - px) / (-s*sy*x);
		else
			ky = (pos.y - s*sx*x - kx*s*sx*y - c*sy*y - py) / (c*sy*x);
	}
	else if (m_ctrlNodeSelected.type == RIGHT)
	{
		x = hw; y = 0;
		if (s != 0)
			ky = (pos.x - c*sx*x - kx*c*sx*y + s*sy*y - px) / (-s*sy*x);
		else
			ky = (pos.y - s*sx*x - kx*s*sx*y - c*sy*y - py) / (c*sy*x);
	}

	m_selected->setShear(kx, ky);

	if (m_propertyPanel && !m_bDirty)
	{
		m_propertyPanel->enablePropertyGrid(false);
		m_bDirty = true;
	}
	m_editPanel->Refresh();
}

void ArrangeSpriteImpl::SetAutoAlignInvisible()
{
	m_align.SetInvisible();
}

void ArrangeSpriteImpl::Align(const std::vector<ISprite*>& sprites)
{
	m_align.Align(sprites);	
}

void ArrangeSpriteImpl::Align(const ISprite* src, ISprite* dst)
{
	m_align.Align(src, dst);
}

void ArrangeSpriteImpl::OnDraw() const
{
	m_align.Draw();
}

void ArrangeSpriteImpl::GetSpriteCtrlNodes(const ISprite* sprite, Vector nodes[8])
{
	Rect r = sprite->getSymbol().getSize(sprite);
	love::Matrix t;
	t.setTransformation(sprite->getPosition().x, sprite->getPosition().y, sprite->getAngle(),
		sprite->getScale().x, sprite->getScale().y, 0, 0, sprite->getShear().x, sprite->getShear().y);
	// scale
	nodes[0] = Math::transVector(Vector(r.xMin, r.yMax), t);
	nodes[1] = Math::transVector(Vector(r.xMax, r.yMax), t);
	nodes[2] = Math::transVector(Vector(r.xMin, r.yMin), t);
	nodes[3] = Math::transVector(Vector(r.xMax, r.yMin), t);
	// shear
	nodes[4] = Math::transVector(Vector(r.xCenter(), r.yMax), t);
	nodes[5] = Math::transVector(Vector(r.xMin, r.yCenter()), t);
	nodes[6] = Math::transVector(Vector(r.xMax, r.yCenter()), t);
	nodes[7] = Math::transVector(Vector(r.xCenter(), r.yMin), t);
	// fix for offset
	d2d::Vector offset = sprite->getOffset();
	d2d::Vector fix = Math::rotateVector(-offset, sprite->getAngle()) + offset;
	for (int i = 0; i < 8; ++i)
		nodes[i] += fix;
}

//////////////////////////////////////////////////////////////////////////
// class ArrangeSpriteImpl::TranslateVisitor
//////////////////////////////////////////////////////////////////////////

ArrangeSpriteImpl::TranslateVisitor::
TranslateVisitor(const Vector& delta)
	: m_delta(delta)
{
}

void ArrangeSpriteImpl::TranslateVisitor::
visit(Object* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);
	sprite->translate(m_delta);
	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class ArrangeSpriteImpl::RotateVisitor
//////////////////////////////////////////////////////////////////////////

ArrangeSpriteImpl::RotateVisitor::
RotateVisitor(const Vector& start, const Vector& end)
	: m_start(start), m_end(end)
{
}

void ArrangeSpriteImpl::RotateVisitor::
visit(Object* object, bool& bFetchNext)
{
	ISprite* sprite = static_cast<ISprite*>(object);

	Vector center = sprite->getPosition() + sprite->getOffset();
	float angle = Math::getAngleInDirection(center, m_start, m_end);
	sprite->rotate(angle);

	bFetchNext = false;
}

}

}