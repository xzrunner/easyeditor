#ifndef _DRAG2D_GROUP_TREE_VISITOR_H_
#define _DRAG2D_GROUP_TREE_VISITOR_H_

#include <wx/treectrl.h>

namespace d2d
{

class IGroupTreeVisitor
{
public:
	virtual bool VisitNonleaf(wxTreeItemId id) { return false; };
	virtual bool VisitLeaf(wxTreeItemId id) { return false; };
	virtual ~IGroupTreeVisitor() {}
}; // IGroupTreeVisitor

}

#endif // _DRAG2D_GROUP_TREE_VISITOR_H_