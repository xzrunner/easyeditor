#ifndef _EASYEDITOR_EDITED_FILE_STACK_H_
#define _EASYEDITOR_EDITED_FILE_STACK_H_

#include <CU_Singleton.h>

#include <string>
#include <vector>

namespace ee
{

class EditedFileStack
{
public:
	void Push(const std::string& filepath);
	void Pop();
	std::string Top() const;

	void Clear();

	std::string GetBottom() const;
	void SetBottom(const std::string& filepath);
	
private:
	std::vector<std::string> m_files;

	SINGLETON_DECLARATION(EditedFileStack)

}; // EditedFileStack

}

#endif // _EASYEDITOR_EDITED_FILE_STACK_H_