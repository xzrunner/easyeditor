#include "BodyManager.h"
#include "IBody.h"
#include "BodyFactory.h"
#include "physics_const.h"
#include "Sprite.h"
#include "FileHelper.h"
#include "Symbol.h"

namespace ee
{

BodyManager* BodyManager::m_instance = NULL;

BodyManager::BodyManager()
{
}

IBody* BodyManager::LoadBody(Sprite* sprite)
{
	std::map<Sprite*, IBody*>::iterator itr = 
		m_map_body.find(sprite);
	IBody* body = CreateBody(sprite);
	if (itr == m_map_body.end()) {
		if (body) {
			sprite->AddReference();
			m_map_body.insert(std::make_pair(sprite, body));
		}
	} else {
		obj_assign<IBody>(itr->second, body);
	}
	return body;
}

void BodyManager::UnloadBody(Sprite* sprite)
{
	std::map<Sprite*, IBody*>::iterator itr = 
		m_map_body.find(sprite);
	if (itr != m_map_body.end()) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
		m_map_body.erase(itr);
	}
}

const IBody* BodyManager::QueryBody(Sprite* sprite) const
{
	std::map<Sprite*, IBody*>::const_iterator itr = 
		m_map_body.find(sprite);
	if (itr != m_map_body.end()) {
		return itr->second;
	} else {
		return NULL;
	}
}

void BodyManager::Update()
{
	std::map<Sprite*, IBody*>::iterator itr =
		m_map_body.begin();
	for ( ; itr != m_map_body.end(); ++itr) {
		IBody* body = itr->second;
		if (!body || !(body->GetBody())) {
			continue;
		}
		Sprite* sprite = itr->first;
		b2Body* b2body = body->GetBody();
		if (body->IsAlive() && b2body->GetType() != b2_staticBody) {
			sm::vec2 pos(b2body->GetPosition().x, b2body->GetPosition().y);
			sprite->SetTransform(pos * BOX2D_SCALE_FACTOR, b2body->GetAngle());
		} else {
			sm::vec2 pos = sprite->GetPosition() / BOX2D_SCALE_FACTOR;
			b2body->SetTransform(b2Vec2(pos.x, pos.y), sprite->GetAngle());
		}
	}
}

IBody* BodyManager::CreateBody(Sprite* sprite)
{
	std::string filepath = FileHelper::GetFilenameAddTag(
		sprite->GetSymbol().GetFilepath(), "shape", "json");
	if (FileHelper::IsFileExist(filepath)) {
		IBody* body = BodyFactory::createBody(filepath, sprite->GetScale().x);
		sm::vec2 pos = sprite->GetPosition() / BOX2D_SCALE_FACTOR;
		body->GetBody()->SetTransform(b2Vec2(pos.x, pos.y), sprite->GetAngle());
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