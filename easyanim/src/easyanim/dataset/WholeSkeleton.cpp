#include <queue>

#include "WholeSkeleton.h"

#include "dataset/Symbol.h"
#include "tools/Render.h"

namespace eanim
{

WholeSkeleton::WholeSkeleton()
{
	static int id = 0;
	m_name = wxT("¹Ç¼Ü") + wxVariant(id++);

	m_root = NULL;
}

WholeSkeleton::WholeSkeleton(const WholeSkeleton& skeleton)
{
	m_name = skeleton.m_name;
	m_root = skeleton.m_root ? skeleton.m_root->clone() : NULL;
}

WholeSkeleton::~WholeSkeleton()
{
	clear();
}

WholeSkeleton* WholeSkeleton::clone() const
{
	return new WholeSkeleton(*this);
}

void WholeSkeleton::loadFromTextFile(std::ifstream& fin, const std::vector<Node*>& nodes,
									 const std::vector<Body*>& bodies)
{
	clear();

	std::string flag;
	size_t size;
	fin >> flag >> size;
	std::vector<Sprite*> sprites;
	sprites.reserve(size);
	for (size_t i = 0; i < size; ++i)
		sprites.push_back(new Sprite);
	for (size_t i = 0; i < size; ++i)
		sprites[i]->loadFromTextFile(fin, nodes, bodies, sprites);
 
	m_root = size > 0 ? sprites[0] : NULL;
}

void WholeSkeleton::storeToTextFile(std::ofstream& fout, const std::vector<Node*>& nodes,
									const std::vector<Body*>& bodies) const
{
	std::vector<Sprite*> sprites;
	m_root->traverse(FetchAllBonesVisitor(sprites));

	fout << "bone " << sprites.size() << '\n';
	for (size_t i = 0, n = sprites.size(); i < n; ++i)
		sprites[i]->storeToTextFile(fout, nodes, bodies, sprites);
}

void WholeSkeleton::traverse(ee::IVisitor& visitor)
{
	if (m_root) m_root->traverse(visitor);
}

void WholeSkeleton::onDraw()
{
	if (m_root) m_root->drawPosterity();
}

void WholeSkeleton::getBounding(ee::AbstractBV& bounding)
{
	traverse(ComputeBoundingVisitor(bounding));
}

WholeSkeleton::Sprite* WholeSkeleton::queryByPos(const ee::Vector& pos)
{
	WholeSkeleton::Sprite* sprite = NULL;
	traverse(PointQueryVisitor(pos, &sprite));
	return sprite;
}

const wxString& WholeSkeleton::getName() const
{
	return m_name;
}

void WholeSkeleton::tween(const WholeSkeleton& start, const WholeSkeleton& end, float process)
{
	std::vector<Sprite*> sBones, eBones, tBones;
	const_cast<WholeSkeleton&>(start).traverse(FetchAllBonesVisitor(sBones));
	const_cast<WholeSkeleton&>(end).traverse(FetchAllBonesVisitor(eBones));
	traverse(FetchAllBonesVisitor(tBones));
	assert(sBones.size() == eBones.size()
		&& eBones.size() == tBones.size());

	for (size_t i = 0, n = sBones.size(); i < n; ++i)
	{
		Sprite *s = sBones[i], *e = eBones[i], *t = tBones[i];
		float offset = (e->m_relativeCoords.delta - s->m_relativeCoords.delta) * process;
		t->m_relativeCoords.delta = s->m_relativeCoords.delta + offset;
		m_root->computePosterityAbsoluteCoords();
	}
}

void WholeSkeleton::clear()
{
	delete m_root;
}

//////////////////////////////////////////////////////////////////////////
// class WholeSkeleton::Node
//////////////////////////////////////////////////////////////////////////

WholeSkeleton::Node::Node()
{
}

WholeSkeleton::Node::Node(const ee::Vector& p) 
	: pos(p) 
{}

//////////////////////////////////////////////////////////////////////////
// class WholeSkeleton::Body
//////////////////////////////////////////////////////////////////////////

const float WholeSkeleton::Body::NODE_RADIUS = 5.0f;

WholeSkeleton::Body::Body(Symbol* symbol)
{
	m_symbol = symbol;
}

WholeSkeleton::Body::~Body()
{
	clear();
}

void WholeSkeleton::Body::loadFromTextFile(std::ifstream& fin)
{
	clear();

	std::string flag;
	size_t size;
	fin >> flag >> size;
	m_nodes.reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		Node* node = new Node;
		fin >> node->pos;
		m_nodes.push_back(node);
	}
}

void WholeSkeleton::Body::storeToTextFile(std::ofstream& fout) const
{
	fout << "nodes " << m_nodes.size() << '\n';
	for (size_t i = 0, n = m_nodes.size(); i < n; ++i)
	{
		Node* node = m_nodes[i];
		fout << node->pos << " ";
	}
	fout << '\n';
}

void WholeSkeleton::Body::onDraw() const
{
	for (size_t i = 0, n = m_nodes.size(); i < n; ++i)
	{
		const ee::Vector& pos = m_nodes[i]->pos;
		ee::PrimitiveDraw::drawRect(pos, NODE_RADIUS * 0.5f, true, 2, ee::Colorf(1.0f, 0.0f, 0.0f));
		ee::PrimitiveDraw::drawCircle(pos, NODE_RADIUS, false, 2, ee::Colorf(0.0f, 1.0f, 0.0f));
	}
}

WholeSkeleton::Node* WholeSkeleton::Body::queryNodeByPos(const ee::Vector& pos)
{
	WholeSkeleton::Node* nearest = NULL;
	float nearestDis = FLT_MAX;

	for (size_t i = 0, n = m_nodes.size(); i < n; ++i)
	{
		float dis = ee::Math::getDistance(pos, m_nodes[i]->pos);
		if (dis < nearestDis) 
		{
			nearest = m_nodes[i];
			nearestDis = dis;
		}
	}

	return nearestDis < NODE_RADIUS ? nearest : NULL;
}

void WholeSkeleton::Body::insertNode(const ee::Vector& pos)
{
	m_nodes.push_back(new Node(pos));
}

void WholeSkeleton::Body::eraseNode(Node* node)
{
	std::vector<Node*>::iterator itr = m_nodes.begin();
	for ( ; itr != m_nodes.end(); ++itr)
	{
		if (*itr == node)
		{
			m_nodes.erase(itr);
			delete node;
			break;
		}
	}
}

void WholeSkeleton::Body::clear()
{
	for_each(m_nodes.begin(), m_nodes.end(), DeletePointerFunctor<Node>());
	m_nodes.clear();
}

//////////////////////////////////////////////////////////////////////////
// class WholeSkeleton::Sprite
//////////////////////////////////////////////////////////////////////////

WholeSkeleton::Sprite::Sprite()
{
	m_body = NULL;
	m_parent = NULL;
}

WholeSkeleton::Sprite::Sprite(Body* body, const ee::Vector& pos)
{
	m_pos = pos;
	m_body = body;
	m_parent = NULL;
	m_layer = ((MIN_LAYER + MAX_LAYER) >> 1);
	initBounding();
}

WholeSkeleton::Sprite::Sprite(const WholeSkeleton::Sprite& sprite, WholeSkeleton::Sprite* parent/* = NULL*/)
	: ee::ImageSprite(sprite), m_relativeCoords(sprite.m_relativeCoords)
{
	m_body = sprite.m_body;
	m_layer = sprite.m_layer;

	m_parent = parent;
	for (size_t i = 0, n = sprite.m_children.size(); i < n; ++i)
		m_children.push_back(new Sprite(*sprite.m_children[i], this));
}

WholeSkeleton::Sprite::~Sprite()
{
	clear();
}

WholeSkeleton::Sprite* WholeSkeleton::Sprite::clone() const
{
	return new WholeSkeleton::Sprite(*this);
}

void WholeSkeleton::Sprite::loadFromTextFile(std::ifstream& fin, const std::vector<Node*>& nodes,
											 const std::vector<Body*>& bodies, const std::vector<Sprite*>& sprites)
{
	clear();

	fin >> m_pos >> m_angle;

	int index;
	fin >> index;
	assert(index != -1);
	m_body = bodies[index];

	size_t size;
	fin >> size;
	m_children.reserve(size);
	for (size_t i = 0; i < size; ++i)
	{
		fin >> index;
		assert(index != -1);
		m_children.push_back(sprites[index]);
		sprites[index]->m_parent = this;
	}

	m_relativeCoords.loadFromTextFile(fin, nodes);

	fin >> m_layer;

	initBounding();
}

void WholeSkeleton::Sprite::storeToTextFile(std::ofstream& fout, const std::vector<Node*>& nodes,
											const std::vector<Body*>& bodies, const std::vector<Sprite*>& sprites) const
{
	fout << m_pos << " " << m_angle << '\n';

	fout << STLTools::getArrayIndex(bodies, m_body) << '\n';

	fout << m_children.size() << " ";
	for (size_t i = 0, n = m_children.size(); i < n; ++i)
		fout << STLTools::getArrayIndex(sprites, m_children[i]) << " ";
	fout << '\n';

	m_relativeCoords.storeToTextFile(fout, nodes);
	fout << '\n';

	fout << m_layer << '\n';
}

void WholeSkeleton::Sprite::loadFromTextFile(std::ifstream& fin)
{
	fin >> m_relativeCoords.delta;
}

void WholeSkeleton::Sprite::storeToTextFile(std::ofstream& fout) const
{
	fout << m_relativeCoords.delta << '\n';
}

void WholeSkeleton::Sprite::traverse(ee::IVisitor& visitor)
{
	std::queue<Sprite*> buffer;
	buffer.push(this);
	while (!buffer.empty())
	{
		Sprite* curr = buffer.front(); buffer.pop();
		bool hasNext;
		visitor.visit(curr, hasNext);
		if (hasNext)
		{
			for (size_t i = 0, n = curr->m_children.size(); i < n; ++i)
				buffer.push(curr->m_children[i]);
		}
		else
		{
			break;
		}
	}
}

void WholeSkeleton::Sprite::remove()
{
	if (m_parent)
	{
		std::vector<Sprite*>::iterator itr = m_parent->m_children.begin();
		for ( ; itr != m_parent->m_children.end(); ++itr)
		{
			if (*itr == this)
			{
				m_parent->m_children.erase(itr);
				break;
			}
		}
	}

	std::vector<Sprite*>::iterator itr = m_children.begin();
	for ( ; itr != m_children.end(); ++itr)
	{
		(*itr)->m_parent = NULL;
	}
}

void WholeSkeleton::Sprite::onDraw() const
{
// 	int times = 0;
// 	drawBone(this, this, NULL, times);

 	glPushMatrix();
 	glTranslatef(m_pos.x, m_pos.y, 0.0f);
 	glRotatef(m_angle * ee::TRANS_RAD_TO_DEG, 0, 0, 1);
 	ee::SpriteDraw::drawSprite(this);
 	m_body->onDraw();
 	glPopMatrix();
}

void WholeSkeleton::Sprite::drawPosterity()
{
	//// method 0
	//traverse(DrawVisitor());

	// method 1
	std::vector<Sprite*> bones;
	traverse(FetchAllBonesVisitor(bones));
	std::sort(bones.begin(), bones.end(), BoneCmp());
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	for (size_t i = 0, n = bones.size(); i < n; ++i)
		bones[i]->onDraw();
}

int WholeSkeleton::Sprite::getLayer() const
{
	return m_layer;
}

void WholeSkeleton::Sprite::setLayer(int layer)
{
	if (layer < MIN_LAYER) layer = MIN_LAYER;
	else if (layer > MAX_LAYER) layer = MAX_LAYER;
	m_layer = layer;
}

Symbol* WholeSkeleton::Sprite::getSymbol()
{
	return m_body->getSymbol();
}

ee::Vector WholeSkeleton::Sprite::getNodeWorldCoords(Node* node) const
{
	return m_pos + ee::Math::rotateVector(node->pos, m_angle);
}

void WholeSkeleton::Sprite::clear()
{
	for_each(m_children.begin(), m_children.end(), DeletePointerFunctor<Sprite>());
	m_children.clear();
}

void WholeSkeleton::Sprite::initBounding()
{
	const float hWidth = getSymbol()->getWidth() * 0.5f,
		hHeight = getSymbol()->getHeight() * 0.5f;

	m_bounding = ee::BVFactory::createBV(ee::e_obb);
	ee::Rect aabb;
	aabb.xMin = -hWidth;
	aabb.xMax = hWidth;
	aabb.yMin = -hHeight;
	aabb.yMax = hHeight;
	aabb.translate(m_pos);
	m_bounding->initFromRect(aabb);
}

void WholeSkeleton::Sprite::drawBone(const Sprite* bone, const Sprite* curr, const Sprite* currChild, int& times) const
{
	if (curr->m_parent)
	{
		drawBone(bone, curr->m_parent, curr, times);

		if (bone == curr)
		{
			glPushMatrix();
		
			ee::Vector offset = ee::Math::rotateVector(curr->m_relativeCoords.from->pos, curr->m_relativeCoords.delta);
			glTranslatef(-offset.x, -offset.y, 0.0f);

			glRotatef(curr->m_relativeCoords.delta * ee::TRANS_RAD_TO_DEG, 0, 0, 1);
			ee::SpriteDraw::drawSprite(bone);
			bone->m_body->onDraw();
			glPopMatrix();

			for (size_t i = 0; i < times; ++i)
				glPopMatrix();
		}
		else
		{
			++times;
			glPushMatrix();
			ee::Vector offset = ee::Math::rotateVector(currChild->m_relativeCoords.to->pos - curr->m_relativeCoords.from->pos, curr->m_relativeCoords.delta);
			glTranslatef(offset.x, offset.y, 0.0f);
			glRotatef(curr->m_relativeCoords.delta * ee::TRANS_RAD_TO_DEG, 0, 0, 1);
		}
	}
	else
	{
		if (bone == curr)
		{
			glPushMatrix();
			glTranslatef(bone->getPosition().x, bone->getPosition().y, 0.0f);
			glRotatef(bone->getAngle() * ee::TRANS_RAD_TO_DEG, 0, 0, 1);
			ee::SpriteDraw::drawSprite(bone);
			bone->m_body->onDraw();
			glPopMatrix();

			for (size_t i = 0; i < times; ++i)
				glPopMatrix();
		}
		else
		{
			++times;
			glPushMatrix();
			ee::Vector offset = curr->getPosition() + 
				ee::Math::rotateVector(currChild->m_relativeCoords.to->pos, curr->getAngle());
			glTranslatef(offset.x, offset.y, 0.0f);
			glRotatef(curr->getAngle() * ee::TRANS_RAD_TO_DEG, 0, 0, 1);
		}
	}
}

void WholeSkeleton::Sprite::computePosterityAbsoluteCoords()
{
	traverse(PosterityAbsoluteCoordsVisitor());
}

//////////////////////////////////////////////////////////////////////////
// class WholeSkeleton::Sprite::RelativeCoords
//////////////////////////////////////////////////////////////////////////

void WholeSkeleton::Sprite::RelativeCoords::
loadFromTextFile(std::ifstream& fin, const std::vector<Node*>& nodes)
{
	int iFrom, iTo;
	fin >> delta >> iFrom >> iTo;
	from = iFrom == -1 ? NULL : nodes[iFrom];
	to = iTo == -1 ? NULL : nodes[iTo];
}

void WholeSkeleton::Sprite::RelativeCoords::
storeToTextFile(std::ofstream& fout, const std::vector<Node*>& nodes) const
{
	fout << delta << " " 
		<< STLTools::getArrayIndex(nodes, from) << " "
		<< STLTools::getArrayIndex(nodes, to);
}

//////////////////////////////////////////////////////////////////////////
// class WholeSkeleton::Sprite::PosterityAbsoluteCoordsVisitor
//////////////////////////////////////////////////////////////////////////

void WholeSkeleton::Sprite::PosterityAbsoluteCoordsVisitor::
visit(ee::ICloneable* object, bool& bFetchNext)
{
	WholeSkeleton::Sprite* bone = static_cast<WholeSkeleton::Sprite*>(object);
	if (bone->m_parent)
	{
		bone->translate(-bone->getPosition());
		bone->rotate(-bone->getAngle());

		bone->rotate(bone->m_parent->getAngle() + bone->m_relativeCoords.delta);

		bone->translate(
			bone->m_parent->getPosition()
			+ ee::Math::rotateVector(bone->m_relativeCoords.to->pos, bone->m_parent->getAngle())
			- ee::Math::rotateVector(bone->m_relativeCoords.from->pos, bone->getAngle())
			);
	}

	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class WholeSkeleton::BoneCmp
//////////////////////////////////////////////////////////////////////////

bool WholeSkeleton::BoneCmp::operator() (const Sprite* s0, const Sprite* s1) const
{
	return s0->getLayer() < s1->getLayer();
}

//////////////////////////////////////////////////////////////////////////
// class WholeSkeleton::FetchAllBonesVisitor
//////////////////////////////////////////////////////////////////////////

WholeSkeleton::FetchAllBonesVisitor::FetchAllBonesVisitor(std::vector<Sprite*>& bones)
	: m_bones(bones)
{
}

void WholeSkeleton::FetchAllBonesVisitor::visit(ee::ICloneable* object, bool& bFetchNext)
{
	WholeSkeleton::Sprite* bone = static_cast<WholeSkeleton::Sprite*>(object);
	m_bones.push_back(bone);
	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class WholeSkeleton::DrawVisitor
//////////////////////////////////////////////////////////////////////////

WholeSkeleton::DrawVisitor::DrawVisitor()
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}

void WholeSkeleton::DrawVisitor::visit(ee::ICloneable* object, bool& bFetchNext)
{
	WholeSkeleton::Sprite* bone = static_cast<WholeSkeleton::Sprite*>(object);
	bone->onDraw();
	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class WholeSkeleton::ComputeBoundingVisitor
//////////////////////////////////////////////////////////////////////////

WholeSkeleton::ComputeBoundingVisitor::ComputeBoundingVisitor(ee::AbstractBV& bounding)
	: m_bounding(bounding)
{
}

void WholeSkeleton::ComputeBoundingVisitor::visit(ee::ICloneable* object, bool& bFetchNext)
{
	Sprite* sprite = static_cast<Sprite*>(object);

	ee::Rect aabb;
	aabb.xMin = aabb.xMax = sprite->getSymbol()->getWidth() * 0.5f;
	aabb.yMin = aabb.yMax = sprite->getSymbol()->getHeight() * 0.5f;
	aabb.xMin = -aabb.xMin;
	aabb.yMin = -aabb.yMin;
	aabb.translate(sprite->getPosition());

	m_bounding.combine(aabb);

	bFetchNext = true;
}

//////////////////////////////////////////////////////////////////////////
// class WholeSkeleton::PointQueryVisitor
//////////////////////////////////////////////////////////////////////////

WholeSkeleton::PointQueryVisitor::
PointQueryVisitor(const ee::Vector& pos, WholeSkeleton::Sprite** pSelected)
	: m_pos(pos)
{
	m_pSelected = pSelected;
	*m_pSelected = NULL;
}

void WholeSkeleton::PointQueryVisitor::
visit(ee::ICloneable* object, bool& bFetchNext)
{
	WholeSkeleton::Sprite* sprite = static_cast<WholeSkeleton::Sprite*>(object);
	if (sprite->isContain(m_pos))
	{
		*m_pSelected = sprite;
		bFetchNext = false;
	}
	else
	{
		bFetchNext = true;
	}
}

} // eanim