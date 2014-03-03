
#include "CodePageBuilder.h"

#include "view/Context.h"
#include "view/SceneItem.h"
#include "view/LibraryPanel.h"

using namespace ebuilder;

CodePageBuilder::CodePageBuilder(wxNotebook* notebook) 
	: m_notebook(notebook) 
{}

void CodePageBuilder::addPage(CodePage* page) 
{
	m_notebook->AddPage(page, page->getName());
}

int CodePageBuilder::getSceneIndex(const SceneItem& scene)
{
	std::vector<SceneItem*> scenes;
	Context::Instance()->library->getSceneList()->traverse(
		d2d::FetchAllVisitor<SceneItem>(scenes));

	int index = -1;
	for (size_t i = 0, n = scenes.size(); i < n; ++i)
	{
		if (scenes[i] == &scene)
		{
			index = i;
			break;
		}
	}
	assert(index != -1);

	return index;
}
