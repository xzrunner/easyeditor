#include "FileIO.h"
#include "Task.h"

#include <easycomplex.h>
#include <queue>

namespace ecomplex
{

void FileIO::load(const Task* task, const char* filename)
{
	ecomplex::Symbol* symbol = task->m_stage->getSymbol();
	symbol->loadFromFile(filename);

	task->m_library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());
	for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
		task->m_viewlist->insert(symbol->m_sprites[i]);

	task->m_stage->loadHistoryList(filename, symbol->m_sprites);

	//// rm resetinitstate
	//const_cast<task*>(task)->getstagepanel()->getcanvas()->resetinitstate();
}

void FileIO::store(const Task* task, const char* filename)
{
//		ecomplex::FileSaver::store(filename, Context::Instance()->stage->getSymbol());

	ecomplex::Symbol* root = task->m_stage->getSymbol();
	ecomplex::FileSaver::storeWithHistory(filename, root, task->m_stage);
	std::queue<const ecomplex::Symbol*> buffer;
	for (size_t i = 0, n = root->m_sprites.size(); i < n ;++i)
		if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(root->m_sprites[i]))
			buffer.push(&complex->getSymbol());
	while (!buffer.empty())
	{
		const ecomplex::Symbol* symbol = buffer.front(); buffer.pop();
		ecomplex::FileSaver::store(symbol->getFilepath(), symbol);
		for (size_t i = 0, n = symbol->m_sprites.size(); i < n ;++i)
			if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(symbol->m_sprites[i]))
				buffer.push(&complex->getSymbol());
	}
}

}