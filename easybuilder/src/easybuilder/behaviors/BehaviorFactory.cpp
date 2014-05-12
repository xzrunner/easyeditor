
#include "BehaviorFactory.h"

#include "ChangeImage.h"
#include "ChangeImagePanel.h"
#include "ChangeScene.h"
#include "ChangeScenePanel.h"
#include "ScaleSize.h"
#include "ScaleSizePanel.h"

#include "MouseButton.h"
#include "MouseButtonPanel.h"

#include "Move.h"
#include "MovePanel.h"
#include "Rotate.h"
#include "RotatePanel.h"

#include "view/Context.h"
#include "view/LibraryPanel.h"
#include "view/MediaPage.h"
#include "view/ScenesPage.h"
#include "view/ListObserver.h"

using namespace ebuilder;

Behavior* BehaviorFactory::createBehavior(BehaviorType type,
										  Actor* actor)
{
	Behavior* behavior = NULL;

	switch (type)
	{
	case e_ChangeImage:
		{
 			Context* context = Context::Instance();
			ebuilder::ChangeImage* changeImage
 				= new ChangeImage(actor, context->library->getMediaList());
			changeImage->registerListener(context->library->getMediaPage());
 			behavior = changeImage;
		}
		break;
	case e_ChangeScene:
		{
			Context* context = Context::Instance();
			ChangeScene* changeScene = new ChangeScene(actor, 
				context->library->getSceneList());
			changeScene->registerListener(context->library->getScenePage());
			behavior = changeScene;
		}
		break;
	case e_ScaleSize:
		behavior = new ScaleSize(actor);
		break;

	case e_MouseButton:
		behavior = new MouseButton(actor);
		break;

	case e_Move:
		behavior = new Move(actor);
		break;
	case e_Rotate:
		behavior = new Rotate(actor);
		break;
	}

	return behavior;
}

BehaviorPanel* BehaviorFactory::createBehaviorPanel(wxWindow* parent,
													Behavior* behavior)
{
	BehaviorPanel* behaviorPanel = NULL;

	switch (behavior->type())
	{
	case e_ChangeImage:
		{
			Context* context = Context::Instance();
			behaviorPanel = new ChangeImagePanel(parent, 
				context->library->getMediaList(), static_cast<ChangeImage*>(behavior));
			context->library->getMediaPage()->getListObserver()->registerListener(behaviorPanel);
		}
		break;
	case e_ChangeScene:
		{
			Context* context = Context::Instance();
			behaviorPanel = new ChangeScenePanel(parent, 
				context->library->getSceneList(), static_cast<ChangeScene*>(behavior));
			context->library->getScenePage()->getListObserver()->registerListener(behaviorPanel);
		}
		break;
	case e_ScaleSize:
		behaviorPanel = new ScaleSizePanel(parent, static_cast<ScaleSize*>(behavior));
		break;

	case e_MouseButton:
		behaviorPanel = new MouseButtonPanel(parent, static_cast<MouseButton*>(behavior));
		break;

	case e_Move:
		behaviorPanel = new MovePanel(parent, static_cast<Move*>(behavior));
		break;
	case e_Rotate:
		behaviorPanel = new RotatePanel(parent, static_cast<Rotate*>(behavior));
		break;
	}

	return behaviorPanel;
}
