#include "SkeletonEditPanel.h"
#include "SkeletonEditGLCanvas.h"
#include "SkeletonSymbolList.h"
#include "operator/ComposeWholeSkeletonOP.h"
#include "dataset/Sprite.h"
#include "dataset/Symbol.h"

namespace eanim
{

const wxString SkeletonEditPanel::BONE_SYMBOL_LIST_NAME = wxT("bone_symbol");

SkeletonEditPanel::SkeletonEditPanel(wxWindow* parent, WholeSkeleton* skeleton,
									 SkeletonSymbolList* symbolList, wxTextCtrl* layerTextCtrl)
	: EditPanel(parent), MultiSpritesImpl(parent)
{
	m_skeleton = skeleton;
	m_symbolList = symbolList;

	SetDropTarget(new DragSymbolTarget(this));

	m_editOP = new ComposeWholeSkeletonOP(this, m_skeleton, layerTextCtrl);

	m_canvas = new SkeletonEditGLCanvas(this, skeleton, symbolList);
	m_camera->setScale(0.5f);
}

void SkeletonEditPanel::traverseSprites(ee::IVisitor& visitor, ee::TraverseType type/* = e_allExisting*/,
										bool order/* = true*/) const
{
	m_skeleton->traverse(visitor);
}

void SkeletonEditPanel::removeSprite(ee::ISprite* sprite)
{
	WholeSkeleton::Sprite* bone = dynamic_cast<WholeSkeleton::Sprite*>(sprite);
	assert(bone);
	bone->remove();
	bone->release();
}

void SkeletonEditPanel::insertSprite(int symbolIndex, wxCoord x, wxCoord y)
{
	Symbol* symbol = m_symbolList->getSymbol(symbolIndex);
	assert(symbol && symbol->getSkeletonBody());

	ee::Vector pos = transPosScreenToProject(x, y);
	WholeSkeleton::Sprite* sprite = new WholeSkeleton::Sprite(symbol->getSkeletonBody(), pos);

	ComposeWholeSkeletonOP* editOP = dynamic_cast<ComposeWholeSkeletonOP*>(m_editOP);
	assert(editOP);
	editOP->absorbSprite(sprite);

	Refresh();
}

void SkeletonEditPanel::setSelectedBoneLayer(int layer)
{
	ComposeWholeSkeletonOP* editOP = dynamic_cast<ComposeWholeSkeletonOP*>(m_editOP);
	assert(editOP);
	editOP->setSelectedBoneLayer(layer);
}

//////////////////////////////////////////////////////////////////////////
// class SkeletonEditPanel::DragSymbolTarget
//////////////////////////////////////////////////////////////////////////

SkeletonEditPanel::DragSymbolTarget::DragSymbolTarget(SkeletonEditPanel* panel)
{
	m_panel = panel;
}

bool SkeletonEditPanel::DragSymbolTarget::OnDropText(wxCoord x, wxCoord y, const wxString& data)
{
	wxString sType = data.substr(0, data.find(","));
	wxString sIndex = data.substr(data.find(",") + 1);

	if (sType == SkeletonEditPanel::BONE_SYMBOL_LIST_NAME)
	{
		long index;
		sIndex.ToLong(&index);
		m_panel->insertSprite(index, x, y);
		return true;
	}
	else
	{
		return false;
	}
}

} // eanim