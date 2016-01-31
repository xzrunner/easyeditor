
#include "Adapter.h"
#include "Actor.h"
#include "Button.h"
#include "Scene.h"

#include "ChangeImage.h"
#include "ChangeScene.h"
#include "ScaleSize.h"
#include "Move.h"
#include "Rotate.h"

#include "dataset/Actor.h"
#include "dataset/Layer.h"
#include "dataset/Scene.h"
#include "behaviors/Behavior.h"
#include "behaviors/ChangeImage.h"
#include "behaviors/ChangeScene.h"
#include "behaviors/ScaleSize.h"
#include "behaviors/MouseButton.h"
#include "behaviors/Move.h"
#include "behaviors/Rotate.h"
#include "view/SceneItem.h"

using namespace ebuilder::preview;

Adapter::Adapter(Panel* panel)
	: m_panel(panel)
{
}

Scene* Adapter::builder2Scene(const ebuilder::Scene& scene)
{
	Scene* result = new Scene;

	m_mapToScene.insert(std::make_pair(&scene, result));

	result->m_color.r = scene.m_color.Red() / 255.0f;
	result->m_color.g = scene.m_color.Green() / 255.0f;
	result->m_color.b = scene.m_color.Blue() / 255.0f;

	const std::vector<ebuilder::Layer*>& layers = scene.getAllLayers();
	for (size_t i = 0, n = layers.size(); i < n; ++i)
	{
		const std::vector<ebuilder::Actor*>& actors = layers[i]->getActors();
		for (size_t j = 0, m = actors.size(); j < m; ++j)
		{
			ebuilder::Actor* actor = actors[j];
			Button* button = builder2Button(*actor);
			if (button)
				result->m_items.push_back(button);
			else
				result->m_items.push_back(builder2Actor(result, *actor));
		}
	}

	return result;
}

Actor* Adapter::builder2Actor(Scene* scene, const ebuilder::Actor& actor)
{
	Actor* ret = new Actor(actor);

	const std::vector<ebuilder::Behavior*>& behaviors = actor.getBehaviors();
	for (size_t i = 0, n = behaviors.size(); i < n; ++i)
		scene->m_behaviors.push_back(builder2Behavior(ret, *behaviors[i]));

	return ret;
}

Button* Adapter::builder2Button(const ebuilder::Actor& actor)
{
	const std::vector<ebuilder::Behavior*>& behaviors = actor.getBehaviors();

	Button* button = NULL;
	for (size_t i = 0, n = behaviors.size(); i < n; ++i)
	{
		if (behaviors[i]->type() == e_MouseButton)
		{
			button = new Button(actor);
			break;
		}
	}

	if (button)
	{
		for (size_t i = 0, n = behaviors.size(); i < n; ++i)
		{
			if (behaviors[i]->type() == e_MouseButton)
			{
				ebuilder::MouseButton* mouseButton = 
					static_cast<ebuilder::MouseButton*>(behaviors[i]);

				std::vector<Behavior*>& behaviors = 
					mouseButton->bPress ? button->m_pressBehaviors : button->m_liftBehaviors;
				for (size_t j = 0, m = mouseButton->satisfies.size(); j < m; ++j)
					behaviors.push_back(builder2Behavior(button, *mouseButton->satisfies[j]));
			}
		}
	}
	
	return button;
}

Behavior* Adapter::builder2Behavior(ee::ISprite* sprite, const ebuilder::Behavior& behavior)
{
	Behavior* result = NULL;
	switch (behavior.type())
	{
	case e_ChangeImage:
		{
			ebuilder::ChangeImage& changeImage = static_cast<ebuilder::ChangeImage&>
				(const_cast<ebuilder::Behavior&>(behavior));
			result = new ChangeImage(sprite, changeImage.selected);
		}
		break;
	case e_ChangeScene:
		{
			ebuilder::ChangeScene& changeScene = static_cast<ebuilder::ChangeScene&>
				(const_cast<ebuilder::Behavior&>(behavior));
			Scene* scene = getScene(static_cast<ebuilder::SceneItem&>(*changeScene.selected).getScene());
			result = new ChangeScene(m_panel, scene);
		}
		break;
	case e_ScaleSize:
		{
			ebuilder::ScaleSize& scaleSize = static_cast<ebuilder::ScaleSize&>
				(const_cast<ebuilder::Behavior&>(behavior));
			result = new ScaleSize(sprite, scaleSize.val);
		}
		break;
	case e_Move:
		{
			ebuilder::Move& move = static_cast<ebuilder::Move&>
				(const_cast<ebuilder::Behavior&>(behavior));
			result = new Move(sprite, move.start, move.end, move.speed);
		}
		break;
	case e_Rotate:
		{
			ebuilder::Rotate& rotate = static_cast<ebuilder::Rotate&>
				(const_cast<ebuilder::Behavior&>(behavior));
			result = new Rotate(sprite, rotate.start, rotate.end, rotate.speed);
		}
		break;
	}
	return result;
}

Scene* Adapter::getScene(const ebuilder::Scene* scene)
{
	std::map<const ebuilder::Scene*, ebuilder::preview::Scene*>::iterator itr 
		= m_mapToScene.find(scene);
	if (itr != m_mapToScene.end())
		return itr->second;
	else
		return builder2Scene(*scene);
}
