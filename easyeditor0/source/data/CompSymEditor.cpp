#include "ee0/CompSymEditor.h"

namespace ee0
{

CompSymEditor* CompSymEditor::Clone() const
{
	CompSymEditor* ret = new CompSymEditor;
	ret->m_name      = m_name;
	//ret->m_tag       = m_tag;
	ret->m_filepath  = m_filepath;
	//ret->m_filepaths = m_filepaths;
	//ret->m_dirty     = m_dirty;
	return ret;
}

}