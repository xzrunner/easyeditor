
#include "ChangeScene.h"
#include "Panel.h"
#include "Scene.h"

using namespace ebuilder::preview;

ChangeScene::ChangeScene(Panel* panel, Scene* scene)
	: m_panel(panel)
	, m_scene(scene)
{
}

ChangeScene::~ChangeScene()
{
	delete m_scene;
}

void ChangeScene::trigger()
{
	m_panel->setScene(m_scene);
}
