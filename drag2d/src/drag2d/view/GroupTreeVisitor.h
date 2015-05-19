#ifndef _DRAG2D_GROUP_TREE_VISITOR_H_
#define _DRAG2D_GROUP_TREE_VISITOR_H_

#include <wx/treectrl.h>

namespace d2d
{

class IGroupTreeVisitor
{
public:
	virtual void VisitNonleaf(wxTreeItemId id) {};
	virtual void VisitLeaf(wxTreeItemId id) {};
	virtual ~IGroupTreeVisitor() {}
}; // IGroupTreeVisitor

}

#endif // _DRAG2D_GROUP_TREE_VISITOR_H_