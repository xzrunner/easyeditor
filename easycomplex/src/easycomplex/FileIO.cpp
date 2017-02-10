#include "FileIO.h"
#include "Task.h"

#include <ee/SymbolMgr.h>
#include <ee/ViewlistPanel.h>
#include <ee/FileHelper.h>
#include <ee/SymbolType.h>

#include <easycomplex.h>

#include <sprite2/S2_Sprite.h>

#include <queue>

namespace ecomplex
{

void FileIO::load(const Task* task, const char* filename)
{
	ecomplex::Symbol* sym = task->m_stage->GetSymbol();
	sym->LoadFromFile(filename);

	task->m_library->LoadFromSymbolMgr(*ee::SymbolMgr::Instance());
	const std::vector<s2::Sprite*>& children = sym->GetAllChildren();
	for (int i = 0, n = children.size(); i < n; ++i) {
		ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
		task->m_viewlist->Insert(child);
	}

	//task->m_stage->loadHistoryList(filename, sym->m_sprs);

	//// rm resetinitstate
	//const_cast<task*>(task)->getstagepanel()->getcanvas()->resetinitstate();
}

void FileIO::store(const Task* task, const char* filename)
{
//		ecomplex::FileSaver::store(filename, Context::Instance()->stage->getSymbol());

	ecomplex::Symbol* root = task->m_stage->GetSymbol();
	ecomplex::FileStorer::StoreWithHistory(filename, root, ee::FileHelper::GetFileDir(filename));
	std::queue<std::pair<const ecomplex::Symbol*, std::string> > buffer;
	const std::vector<s2::Sprite*>& children = root->GetAllChildren();
	std::string parent_dir = ee::FileHelper::GetFileDir(root->GetFilepath());
	for (size_t i = 0, n = children.size(); i < n ;++i) {
		ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
		if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(child)) {
			const ecomplex::Symbol* sym = dynamic_cast<const ecomplex::Symbol*>(complex->GetSymbol());
			buffer.push(std::make_pair(sym, parent_dir));
		}
	}
	while (!buffer.empty())
	{
		const ecomplex::Symbol* sym = buffer.front().first;
		std::string parent_dir = buffer.front().second;
		buffer.pop();
		const std::string& filepath = sym->GetFilepath();
		if (filepath != ee::SYM_GROUP_TAG) {
			ecomplex::FileStorer::Store(filepath, sym, parent_dir);
		}
		const std::vector<s2::Sprite*>& children = sym->GetAllChildren();
		std::string child_parent_dir;
		if (filepath != ee::SYM_GROUP_TAG) {
			child_parent_dir = ee::FileHelper::GetFileDir(filepath);
		} else {
			child_parent_dir = parent_dir;
		}
		for (size_t i = 0, n = children.size(); i < n ;++i) {
			ee::Sprite* child = dynamic_cast<ee::Sprite*>(children[i]);
			if (ecomplex::Sprite* complex = dynamic_cast<ecomplex::Sprite*>(child)) {
				const ecomplex::Symbol* sym = dynamic_cast<const ecomplex::Symbol*>(complex->GetSymbol());
				buffer.push(std::make_pair(sym, child_parent_dir));
			}
		}
	}
}

}