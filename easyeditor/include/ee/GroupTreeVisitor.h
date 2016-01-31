#ifndef _EASYEDITOR_GROUP_TREE_VISITOR_H_
#define _EASYEDITOR_GROUP_TREE_VISITOR_H_

#include <wx/treectrl.h>

namespace ee
{

class IGroupTreeVisitor
{
public:
	virtual bool VisitNonleaf(wxTreeItemId id) { return false; };
	virtual bool VisitLeaf(wxTreeItemId id) { return false; };
	virtual ~IGroupTreeVisitor() {}
}; // IGroupTreeVisitor

}

#endif // _EASYEDITOR_GROUP_TREE_VISITOR_H_