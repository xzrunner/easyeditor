#include "BodyManager.h"
#include "IBody.h"
#include "BodyFactory.h"
#include "const.h"

#include <easyshape.h>

namespace ephysics
{

BodyManager* BodyManager::m_instance = NULL;

BodyManager::BodyManager()
{
}

IBody* BodyManager::LoadBody(d2d::ISprite* sprite)
{
	std::map<d2d::ISprite*, IBody*>::iterator itr = 
		m_map_body.find(sprite);
	IBody* body = CreateBody(sprite);
	if (itr == m_map_body.end()) {
		if (body) {
			sprite->Retain();
			m_map_body.insert(std::make_pair(sprite, body));
		}
	} else {
		d2d::obj_assign((d2d::Object*&)itr->second, body);
	}
	return body;
}

void BodyManager::UnloadBody(d2d::ISprite* sprite)
{
	std::map<d2d::ISprite*, IBody*>::iterator itr = 
		m_map_body.find(sprite);
	if (itr != m_map_body.end()) {
		itr->first->Release();
		itr->second->Release();
		m_map_body.erase(itr);
	}
}

const IBody* BodyManager::QueryBody(d2d::ISprite* sprite) const
{
	std::map<d2d::ISprite*, IBody*>::const_iterator itr = 
		m_map_body.find(sprite);
	if (itr != m_map_body.end()) {
		return itr->second;
	} else {
		return NULL;
	}
}

void BodyManager::Update()
{
	std::map<d2d::ISprite*, IBody*>::iterator itr =
		m_map_body.begin();
	for ( ; itr != m_map_body.end(); ++itr) {
		IBody* body = itr->second;
		if (!body || !(body->getBody())) {
			continue;
		}
		d2d::ISprite* sprite = itr->first;
		b2Body* b2body = body->getBody();
		if (body->isAlive() && b2body->GetType() != b2_staticBody) {
			d2d::Vector pos(b2body->GetPosition().x, b2body->GetPosition().y);
			sprite->setTransform(pos * BOX2D_SCALE_FACTOR, b2body->GetAngle());
		} else {
			d2d::Vector pos = sprite->getPosition() / BOX2D_SCALE_FACTOR;
			b2body->SetTransform(b2Vec2(pos.x, pos.y), sprite->getAngle());
		}
	}
}

IBody* BodyManager::CreateBody(d2d::ISprite* sprite)
{
	wxString filepath = d2d::FilenameTools::getFilenameAddTag(
		sprite->getSymbol().getFilepath(), libshape::FILE_TAG, "json");
	if (d2d::FilenameTools::isExist(filepath)) {
		IBody* body = BodyFactory::createBody(filepath, sprite->getScale().x);
		d2d::Vector pos = sprite->getPosition() / BOX2D_SCALE_FACTOR;
		body->getBody()->SetTransform(b2Vec2(pos.x, pos.y), sprite->getAngle());
		return body;
	} else {
		return NULL;
	}
}

BodyManager* BodyManager::Instance()
{
	if (!m_instance) {
		m_instance = new BodyManager();
	}
	return m_instance;
}

}