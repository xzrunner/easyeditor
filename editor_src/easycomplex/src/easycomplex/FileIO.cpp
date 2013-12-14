#include "FileIO.h"
#include "Task.h"

#include <easycomplex.h>

#include <queue>

namespace ecomplex
{
	void FileIO::load(const Task* task, const char* filename)
	{
		d2d::ComplexSymbol* symbol = task->m_stage->getSymbol();
		symbol->loadFromFile(filename);

		task->m_library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());
		for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
			task->m_viewlist->insert(symbol->m_sprites[i]);
	}

	void FileIO::store(const Task* task, const char* filename)
	{
//		libcomplex::FileSaver::store(filename, Context::Instance()->stage->getSymbol());

		d2d::ComplexSymbol* root = task->m_stage->getSymbol();
		libcomplex::FileSaver::store(filename, root);
		std::queue<const d2d::ComplexSymbol*> buffer;
		for (size_t i = 0, n = root->m_sprites.size(); i < n ;++i)
			if (d2d::ComplexSprite* complex = dynamic_cast<d2d::ComplexSprite*>(root->m_sprites[i]))
				buffer.push(&complex->getSymbol());
		while (!buffer.empty())
		{
			const d2d::ComplexSymbol* symbol = buffer.front(); buffer.pop();
			libcomplex::FileSaver::store(symbol->getFilepath(), symbol);
			for (size_t i = 0, n = symbol->m_sprites.size(); i < n ;++i)
				if (d2d::ComplexSprite* complex = dynamic_cast<d2d::ComplexSprite*>(symbol->m_sprites[i]))
					buffer.push(&complex->getSymbol());
		}
	}
}