
#include "Panel.h"
#include "Canvas.h"
#include "Button.h"
#include "Scene.h"
#include "Behavior.h"

using namespace ebuilder::preview;

Panel::Panel(wxWindow* parent, wxTopLevelWindow* frame,
			 ebuilder::LibraryPanel* libraryPanel)
	: ee::EditPanel(parent, frame)
	, m_scene(NULL)
{
	setEditOP(new MouseActionOP(this));
	setCanvas(m_canvas = new Canvas(this, libraryPanel));
}

Panel::~Panel()
{
	delete m_scene;
}

Scene* Panel::getScene() const 
{
	return m_scene;
}

void Panel::setScene(Scene* scene) 
{
	m_scene = scene;
	m_canvas->setBgColor(m_scene->m_color);

	scene->triggerAllBehaviors();
}

//////////////////////////////////////////////////////////////////////////
// class Panel::MouseActionOP
//////////////////////////////////////////////////////////////////////////

Panel::MouseActionOP::
MouseActionOP(Panel* panel)
	: ee::AbstractEditOP(panel)
	, m_panel(panel)
{
}

bool Panel::MouseActionOP::
onMouseLeftDown(int x, int y)
{
	ee::Vector pos = m_editPanel->transPosScreenToProject(x, y);
	
	Button* button = m_panel->getScene()->queryButtonByPos(pos);
	if (button)
	{
		for (size_t i = 0, n = button->m_pressBehaviors.size(); i < n; ++i)
			button->m_pressBehaviors[i]->trigger();
		if (!button->m_pressBehaviors.empty())
			m_editPanel->Refresh();
	}

	return false;
}

bool Panel::MouseActionOP::
onMouseLeftUp(int x, int y)
{
	ee::Vector pos = m_editPanel->transPosScreenToProject(x, y);

	Button* button = m_panel->getScene()->queryButtonByPos(pos);
	if (button)
	{
		for (size_t i = 0, n = button->m_liftBehaviors.size(); i < n; ++i)
			button->m_liftBehaviors[i]->trigger();
		if (!button->m_liftBehaviors.empty())
			m_editPanel->Refresh();
	}

	return false;
}
