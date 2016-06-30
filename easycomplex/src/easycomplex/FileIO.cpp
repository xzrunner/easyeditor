#include "FileIO.h"
#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/ViewlistPanel.h>

#include <easycomplex.h>

#include <sprite2/S2_Sprite.h>

#include <queue>

namespace ecomplex
{

void FileIO::load(const Task* task, const char* filename)
{
	ecomplex::Symbol* symbol = task->m_stage->GetSymbol();
	symbol->LoadFromFile(filename);

	task->m_library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());
	const std::vector<s2::Sprite*>& children = symbol->GetChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		ee::Sprite* child = static_cast<ee::Sprite*>(children[i]->GetUD());
		task->m_viewlist->Insert(child);
	}

	//task->m_stage->loadHistoryList(filename, symbol->m_sprites);

	//// rm resetinitstate
	//const_cast<task*>(task)->getstagepanel()->getcanvas()->resetinitstate();
}

void FileIO::store(const Task* task, const char* filename)
{
//		ecomplex::FileSaver::store(filename, Context::Instance()->stage->getSymbol());

	ecomplex::Symbol* root = task->m_stage->GetSymbol();
	ecomplex::FileStorer::StoreWithHistory(filename, root);
	std::queue<const ecomplex::Symbol*> buffer;
	const std::vector<s2::Sprite*>& children = root->GetChildren();
	for (size_t i = 0, n = children.size(); i < n ;++i) {
		ee::Sprite* child = static_cast<ee::Sprite*>(children[i]->GetUD());
		if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(child))
			buffer.push(&complex->GetSymbol());
	}
	while (!buffer.empty())
	{
		const ecomplex::Symbol* symbol = buffer.front(); buffer.pop();
		ecomplex::FileStorer::Store(symbol->GetFilepath().c_str(), symbol);
		const std::vector<s2::Sprite*>& children = symbol->GetChildren();
		for (size_t i = 0, n = children.size(); i < n ;++i) {
			ee::Sprite* child = static_cast<ee::Sprite*>(children[i]->GetUD());
			if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(child))
				buffer.push(&complex->GetSymbol());
		}
	}
}

}