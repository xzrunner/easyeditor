#include "SpriteEditDialog.h"
#include "SpriteEditGLCanvas.h"
#include "dataset/Mesh.h"
#include "dataset/Sprite.h"
#include "operator/EditSpriteMeshOP.h"
#include "operator/EditSpritePartSkeletonOP.h"
#include "operator/EditWholeSkeletonOP.h"

namespace eanim
{

SpriteEditDialog::SpriteEditDialog(wxWindow* parent, Sprite* sprite)
 	: wxDialog(parent, wxID_ANY, "Edit Sprite", wxDefaultPosition, wxSize(800, 600), wxCLOSE_BOX | wxCAPTION | wxMAXIMIZE_BOX)
{
	m_sprite = sprite;
	initLayout();
}

SpriteEditDialog::~SpriteEditDialog()
{
	delete m_editPanel;
}

void SpriteEditDialog::initLayout()
{
	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	buildToolBar(sizer);
	buildEditPanel(sizer);
	SetSizer(sizer);
}

void SpriteEditDialog::buildToolBar(wxSizer* topSizer)
{
	wxBoxSizer* toolSizer = new wxBoxSizer(wxVERTICAL);

	m_btnMerge = new wxButton(this, wxID_ANY, wxT("ºÏ²¢"));
	m_btnMerge->Show(true);
	Connect(m_btnMerge->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SpriteEditDialog::onMergeMesh));
	Connect(m_btnMerge->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(SpriteEditDialog::onUpdateMeshState));
	toolSizer->Add(m_btnMerge, 0);

	m_btnSeparate = new wxButton(this, wxID_ANY, wxT("·ÖÀë"));
	m_btnSeparate->Show(false);
	Connect(m_btnSeparate->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(SpriteEditDialog::onSeparateMesh));
	Connect(m_btnSeparate->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(SpriteEditDialog::onUpdateMeshState));
	toolSizer->Add(m_btnSeparate, 0);

	topSizer->Add(toolSizer, 0);
}

void SpriteEditDialog::buildEditPanel(wxSizer* topSizer)
{
	m_editPanel = new d2d::EditPanel(this);

	if (m_sprite->getMesh()) 
		m_editPanel->setEditOP(new EditSpriteMeshOP(m_editPanel, m_sprite->getMesh()));
	else if (m_sprite->getPartSkeleton()) 
		m_editPanel->setEditOP(new EditSpritePartSkeletonOP(m_editPanel, m_sprite->getPartSkeleton()));
	else if (m_sprite->getWholeSkeleton())
		m_editPanel->setEditOP(new EditWholeSkeletonOP(m_editPanel, m_sprite->getWholeSkeleton()));

	m_editPanel->setCanvas(new SpriteEditGLCanvas(m_editPanel, m_sprite));
	m_editPanel->getCamera()->setScale(0.5f);
	
	topSizer->Add(m_editPanel, 1, wxEXPAND);
}

void SpriteEditDialog::onMergeMesh(wxCommandEvent& event)
{
	Mesh* mesh = m_sprite->getMesh();
	if (mesh) mesh->mergeNodes();
	m_editPanel->SetFocus();
}

void SpriteEditDialog::onSeparateMesh(wxCommandEvent& event)
{
	Mesh* mesh = m_sprite->getMesh();
	if (mesh) mesh->separateNodes();
	m_editPanel->SetFocus();
}

void SpriteEditDialog::onUpdateMeshState(wxUpdateUIEvent& event)
{
	Mesh* mesh = m_sprite->getMesh();
	if (mesh)
	{
		if (mesh->isMerged()) 
		{
			m_btnMerge->Show(false);
			m_btnSeparate->Show(true);
		}
		else
		{
			m_btnMerge->Show(true);
			m_btnSeparate->Show(false);
		}
	}
	else
	{
		m_btnMerge->Show(false);
		m_btnSeparate->Show(false);
	}
}

} // eanim