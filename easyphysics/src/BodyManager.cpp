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

IBody* BodyManager::LoadBody(ee::Sprite* sprite)
{
	std::map<ee::Sprite*, IBody*>::iterator itr = 
		m_map_body.find(sprite);
	IBody* body = CreateBody(sprite);
	if (itr == m_map_body.end()) {
		if (body) {
			sprite->Retain();
			m_map_body.insert(std::make_pair(sprite, body));
		}
	} else {
		ee::obj_assign<IBody>(itr->second, body);
	}
	return body;
}

void BodyManager::UnloadBody(ee::Sprite* sprite)
{
	std::map<ee::Sprite*, IBody*>::iterator itr = 
		m_map_body.find(sprite);
	if (itr != m_map_body.end()) {
		itr->first->Release();
		itr->second->Release();
		m_map_body.erase(itr);
	}
}

const IBody* BodyManager::QueryBody(ee::Sprite* sprite) const
{
	std::map<ee::Sprite*, IBody*>::const_iterator itr = 
		m_map_body.find(sprite);
	if (itr != m_map_body.end()) {
		return itr->second;
	} else {
		return NULL;
	}
}

void BodyManager::Update()
{
	std::map<ee::Sprite*, IBody*>::iterator itr =
		m_map_body.begin();
	for ( ; itr != m_map_body.end(); ++itr) {
		IBody* body = itr->second;
		if (!body || !(body->getBody())) {
			continue;
		}
		ee::Sprite* sprite = itr->first;
		b2Body* b2body = body->getBody();
		if (body->isAlive() && b2body->GetType() != b2_staticBody) {
			ee::Vector pos(b2body->GetPosition().x, b2body->GetPosition().y);
			sprite->SetTransform(pos * BOX2D_SCALE_FACTOR, b2body->GetAngle());
		} else {
			ee::Vector pos = sprite->GetPosition() / BOX2D_SCALE_FACTOR;
			b2body->SetTransform(b2Vec2(pos.x, pos.y), sprite->GetAngle());
		}
	}
}

IBody* BodyManager::CreateBody(ee::Sprite* sprite)
{
	std::string filepath = ee::FileHelper::GetFilenameAddTag(
		sprite->GetSymbol().GetFilepath(), libshape::FILE_TAG, "json");
	if (ee::FileHelper::IsFileExist(filepath)) {
		IBody* body = BodyFactory::createBody(filepath, sprite->GetScale().x);
		ee::Vector pos = sprite->GetPosition() / BOX2D_SCALE_FACTOR;
		body->getBody()->SetTransform(b2Vec2(pos.x, pos.y), sprite->GetAngle());
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