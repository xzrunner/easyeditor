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

IBody* BodyManager::LoadBody(Sprite* spr)
{
	std::map<Sprite*, IBody*>::iterator itr = 
		m_map_body.find(spr);
	IBody* body = CreateBody(spr);
	if (itr == m_map_body.end()) {
		if (body) {
			spr->AddReference();
			m_map_body.insert(std::make_pair(spr, body));
		}
	} else {
		cu::RefCountObjAssign(itr->second, body);
	}
	return body;
}

void BodyManager::UnloadBody(Sprite* spr)
{
	std::map<Sprite*, IBody*>::iterator itr = 
		m_map_body.find(spr);
	if (itr != m_map_body.end()) {
		itr->first->RemoveReference();
		itr->second->RemoveReference();
		m_map_body.erase(itr);
	}
}

const IBody* BodyManager::QueryBody(Sprite* spr) const
{
	std::map<Sprite*, IBody*>::const_iterator itr = 
		m_map_body.find(spr);
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
		Sprite* spr = itr->first;
		b2Body* b2body = body->GetBody();
		if (body->IsAlive() && b2body->GetType() != b2_staticBody) {
			sm::vec2 pos(b2body->GetPosition().x, b2body->GetPosition().y);
			spr->SetPosition(pos * BOX2D_SCALE_FACTOR);
			spr->SetAngle(b2body->GetAngle());
		} else {
			sm::vec2 pos = spr->GetPosition() / BOX2D_SCALE_FACTOR;
			b2body->SetTransform(b2Vec2(pos.x, pos.y), spr->GetAngle());
		}
	}
}

IBody* BodyManager::CreateBody(Sprite* spr)
{
	std::string filepath = FileHelper::GetFilenameAddTag(
		spr->GetSymbol()->GetFilepath(), "shape", "json");
	if (FileHelper::IsFileExist(filepath)) {
		IBody* body = BodyFactory::createBody(filepath, spr->GetScale().x);
		sm::vec2 pos = spr->GetPosition() / BOX2D_SCALE_FACTOR;
		body->GetBody()->SetTransform(b2Vec2(pos.x, pos.y), spr->GetAngle());
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