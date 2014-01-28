#include "FileIO.h"
#include "Task.h"

#include <easycomplex.h>

#include <queue>

namespace ecomplex
{
	void FileIO::load(const Task* task, const char* filename)
	{
		complex::Symbol* symbol = task->m_stage->getSymbol();
		symbol->loadFromFile(filename);

		task->m_library->loadFromSymbolMgr(*d2d::SymbolMgr::Instance());
		for (size_t i = 0, n = symbol->m_sprites.size(); i < n; ++i)
			task->m_viewlist->insert(symbol->m_sprites[i]);

		// history op list
		Json::Value value;
		Json::Reader reader;
		std::ifstream fin(filename);
		reader.parse(fin, value);
		fin.close();
		task->m_stage->loadHistoryList(value, symbol->m_sprites);
	}

	void FileIO::store(const Task* task, const char* filename)
	{
//		complex::FileSaver::store(filename, Context::Instance()->stage->getSymbol());

		complex::Symbol* root = task->m_stage->getSymbol();
		complex::FileSaver::storeWithHistory(filename, root, task->m_stage);
		std::queue<const complex::Symbol*> buffer;
		for (size_t i = 0, n = root->m_sprites.size(); i < n ;++i)
			if (complex::Sprite* complex = dynamic_cast<complex::Sprite*>(root->m_sprites[i]))
				buffer.push(&complex->getSymbol());
		while (!buffer.empty())
		{
			const complex::Symbol* symbol = buffer.front(); buffer.pop();
			complex::FileSaver::store(symbol->getFilepath(), symbol);
			for (size_t i = 0, n = symbol->m_sprites.size(); i < n ;++i)
				if (complex::Sprite* complex = dynamic_cast<complex::Sprite*>(symbol->m_sprites[i]))
					buffer.push(&complex->getSymbol());
		}
	}
}