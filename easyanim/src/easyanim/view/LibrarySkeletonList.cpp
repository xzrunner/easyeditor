#include "LibrarySkeletonList.h"
#include "SkeletonEditDialog.h"
#include "StagePanel.h"
#include "LibraryPanel.h"

#include "dataset/Symbol.h"
#include "frame/Context.h"

namespace eanim
{

LibrarySkeletonList::LibrarySkeletonList(wxWindow* parent)
	: ee::LibraryList(parent, LibraryPanel::SKELETON_LIST_NAME)
{
}

void LibrarySkeletonList::loadFromTextFile(std::ifstream& fin)
{
	clear();

	std::string flag;
	size_t size;
	fin >> flag >> size;
	if (size == 0) return;

	std::vector<WholeSkeleton::Node*> nodes;
	std::vector<WholeSkeleton::Body*> bodies;
	loadDataFromSymbolList(nodes, bodies);

	for (size_t i = 0; i < size; ++i)
	{
		WholeSkeleton* skeleton = new WholeSkeleton;
		skeleton->loadFromTextFile(fin, nodes, bodies);
		insert(skeleton);
	}
}

void LibrarySkeletonList::storeToTextFile(std::ofstream& fout) const
{
	if (m_items.empty())
	{
		fout << "skeleton 0" << '\n';
		return;
	}

	std::vector<WholeSkeleton::Node*> nodes;
	std::vector<WholeSkeleton::Body*> bodies;
	loadDataFromSymbolList(nodes, bodies);

	fout << "skeleton " << m_items.size() << '\n';
	for (size_t i = 0, n = m_items.size(); i < n; ++i)
	{
		WholeSkeleton* skeleton = static_cast<WholeSkeleton*>(m_items[i]);
		skeleton->storeToTextFile(fout, nodes, bodies);
	}
}

void LibrarySkeletonList::onListDoubleClicked(wxCommandEvent& event)
{
 	SkeletonEditDialog dlg(this, static_cast<WholeSkeleton*>(m_items[event.GetInt()]));
 	dlg.ShowModal();
	Context::Instance()->stage->resetCanvas();
}

void LibrarySkeletonList::loadDataFromSymbolList(std::vector<WholeSkeleton::Node*>& nodes,
												 std::vector<WholeSkeleton::Body*>& bodies) const
{
	int index = 0;
	while (true)
	{
		Symbol* symbol = Context::Instance()->library->getSymbol(index++);
		if (!symbol) break;

		WholeSkeleton::Body* body = symbol->getSkeletonBody();
		if (body)
		{
			bodies.push_back(body);

			const std::vector<WholeSkeleton::Node*>& src = body->getNodes();
			copy(src.begin(), src.end(), back_inserter(nodes));
		}
	}
}

} // eanim