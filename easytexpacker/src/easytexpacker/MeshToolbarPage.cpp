#include "MeshToolbarPage.h"
#include "StagePanel.h"
#include "const.h"

#include <easyshape.h>

namespace etexpacker
{

MeshToolbarPage::MeshToolbarPage(wxWindow* parent, StagePanel* stage)
	: wxPanel(parent)
	, m_stage(stage)
{
	InitLayout();
}

void MeshToolbarPage::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	// dir
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("src dir"));
		wxSizer* sz = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		m_src_dir = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(150, -1), wxTE_READONLY);
		sz->Add(m_src_dir);

		sz->AddSpacer(5);

		wxButton* btn = new wxButton(this, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MeshToolbarPage::OnSetSrcDir));
		sz->Add(btn);

		sizer->Add(sz);
	}
	sizer->AddSpacer(10);
	// save
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("save physics"));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MeshToolbarPage::OnSaveToFile));
		sizer->Add(btn);		
	}
	sizer->AddSpacer(10);
	// output image
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("save image ..."));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MeshToolbarPage::OnSaveImage));
		sizer->Add(btn);		
	}
	SetSizer(sizer);
}

void MeshToolbarPage::OnSetSrcDir(wxCommandEvent& event)
{
 	ee::ZoomViewOP* op = static_cast<ee::ZoomViewOP*>(m_stage->GetEditOP());
 	op->SetMouseMoveFocus(false);

	wxDirDialog dlg(NULL, "Src Dir", wxEmptyString, wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST);
	if (dlg.ShowModal() == wxID_OK) {
		wxString dir = dlg.GetPath();
		LoadBodies(dir);
		m_src_dir->SetValue(dir);
	}

	op->SetMouseMoveFocus(true);
}

void MeshToolbarPage::OnSaveToFile(wxCommandEvent& event)
{
	Json::Value value;

	b2Body* b = m_stage->getWorld()->GetBodyList();
	int idx = 0;
	while (b != NULL) {
		char* filepath = static_cast<char*>(b->GetUserData());
		if (filepath) {
			Json::Value item_val;
			item_val["filepath"] = filepath;
			item_val["pos"]["x"] = (b->GetPosition().x + HALF_WIDTH) * ephysics::BOX2D_SCALE_FACTOR;
			item_val["pos"]["y"] = (b->GetPosition().y + HALF_H_EIGHT) * ephysics::BOX2D_SCALE_FACTOR;
			item_val["angle"] = b->GetAngle();
			value[idx++] = item_val;
		}
		b = b->GetNext();
	}

	wxString filepath = m_src_dir->GetValue() + "//position.json";
	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.fn_str());
	std::locale::global(std::locale("C"));	
	writer.write(fout, value);
	fout.close();
}

void MeshToolbarPage::OnSaveImage(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Open Physics Result"), wxEmptyString, wxEmptyString, "*.json", wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK) {
		wxString physics_filepath = dlg.GetPath();

		Json::Value value;
		Json::Reader reader;
		std::locale::global(std::locale(""));
		std::ifstream fin(physics_filepath.fn_str());
		std::locale::global(std::locale("C"));
		reader.parse(fin, value);
		fin.close();

		const int width = 1024,
			height = 2048;
		ee::Snapshoot ss(width, height);
		int i = 0;
		Json::Value item_val = value[i++];

		while (!item_val.isNull()) {
			std::string filepath = item_val["filepath"].asString();
			ee::Symbol* symbol = ee::SymbolMgr::Instance()->FetchSymbol(filepath);
			ee::Sprite* sprite = ee::SpriteFactory::Instance()->Create(symbol);

			ee::Vector pos;
			pos.x = item_val["pos"]["x"].asDouble() - width * 0.5f;
			pos.y = item_val["pos"]["y"].asDouble() - height * 0.5f;
			float angle = item_val["angle"].asDouble();
			sprite->SetTransform(pos, angle);
			ss.DrawSprite(sprite);

			sprite->Release();
			symbol->Release();

			item_val = value[i++];
		}

		wxString outpath = ee::FileHelper::GetFileDir(physics_filepath) + "\\image.png";
		ss.SaveToFile(outpath.ToStdString());
	}
}

void MeshToolbarPage::LoadBodies(const wxString& dir)
{
	b2BodyDef bd;
	bd.type = b2_dynamicBody;
 	bd.linearDamping = 0;
 	bd.angularDamping = 0;
	bd.allowSleep = true;
	bd.bullet = false;
	bd.active = true;
	bd.gravityScale = 1;

	b2FixtureDef fd;
 	fd.density = 1;
 	fd.friction = 0.2f;
 	fd.restitution = 0;
 	fd.isSensor = false;
 	fd.filter.categoryBits = 1;
 	fd.filter.maskBits = 0xffff;
 	fd.filter.groupIndex = 0;

	wxArrayString files;
	ee::FileHelper::FetchAllFiles(dir.ToStdString(), files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (!ee::FileType::IsType(filepath, ee::FileType::e_shape)) {
			continue;
		}

		std::vector<ee::Shape*> shapes;
		std::string bg_filepath;
		eshape::FileIO::LoadFromFile(filepath, shapes, bg_filepath);
		for (int i = 0, n = shapes.size(); i < n; ++i)
		{
			eshape::PolygonShape* polygon = dynamic_cast<eshape::PolygonShape*>(shapes[i]);
			if (!polygon) {
				continue;
			}

			b2Body* body = m_stage->getWorld()->CreateBody(&bd);

			b2Vec2 pos(ee::Random::GetNum(-HALF_WIDTH*4, HALF_WIDTH*4), ee::Random::GetNum(HALF_WIDTH*5, HALF_WIDTH*100));
			body->SetTransform(pos, 0);

			const std::vector<ee::Vector>& src = polygon->GetVertices();
			for (int i = 0, n = src.size(); i < n; i += 3) {
				b2Vec2 dst[3];
				for (int j = 0; j < 3; ++j) {
					dst[j].x = src[i+j].x / ephysics::BOX2D_SCALE_FACTOR;
					dst[j].y = src[i+j].y / ephysics::BOX2D_SCALE_FACTOR;
				}

				b2PolygonShape shape;
				shape.Set(&dst[0], 3);
				fd.shape = &shape;

				body->CreateFixture(&fd);
			}

			char* buf = new char[bg_filepath.size() + 1];
			int len = bg_filepath.copy(buf, bg_filepath.size(), 0);
			buf[len] = '\0';
			body->SetUserData(buf);
		}
	}
}

}