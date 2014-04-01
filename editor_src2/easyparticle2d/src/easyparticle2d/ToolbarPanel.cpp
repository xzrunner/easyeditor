#include "ToolbarPanel.h"
//#include "StagePanel.h"
#include "StagePanel.h"
#include "Context.h"

#include <wx/spinctrl.h>

namespace eparticle2d
{

const float ToolbarPanel::EMISSION_RATE		= 100;
const float ToolbarPanel::MIN_SPEED			= 200;
const float ToolbarPanel::MAX_SPEED			= 250;
const float ToolbarPanel::GRAVITY			= 0;
const float ToolbarPanel::SIZE1				= 2;
const float ToolbarPanel::SIZE2				= 1;
const float ToolbarPanel::LIFE_TIME			= 10;
const float ToolbarPanel::PARTICLE_TIME		= 10;
const float ToolbarPanel::DIRECTION			= 0;
const float ToolbarPanel::SPREAD			= 360;
const float ToolbarPanel::RADIAL_ACC		= -2000;
const float ToolbarPanel::TANGENTIAL_ACC	= 1000;

ToolbarPanel::ToolbarPanel(wxWindow* parent)
	: d2d::ToolbarPanel(parent, Context::Instance()->stage)
	, m_image(NULL)
{
	SetSizer(initLayout());	
	enable(false);

	m_startColor.set(255, 255, 255, 255);
	m_endColor.set(0, 0, 0, 255);
}

void ToolbarPanel::load(std::ifstream& fin)
{
	{
		std::string filepath = StringTools::getLine(fin);

		enable(true);
		// todo release symbol
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);
		m_image = static_cast<d2d::ImageSymbol*>(symbol)->getImage();
		if (Context::Instance()->stage->m_particle)
			Context::Instance()->stage->m_particle->setSprite(m_image);
		else
			Context::Instance()->stage->m_particle = new love::graphics::opengl::ParticleSystem(m_image, 1000);
	}
	{
		int rate;
		fin >> rate;
		m_spinEmissionRate->SetValue(rate);
	}
	{
		int min, max;
		fin >> min >> max;
		m_spinMinSpeed->SetValue(min);
		m_spinMaxSpeed->SetValue(max);
	}
	{
		int gravity;
		fin >> gravity;
		m_spinGravity->SetValue(gravity);
	}
	{
		int size1, size2;
		fin >> size1 >> size2;
		m_spinSize1->SetValue(size1);
		m_spinSize2->SetValue(size2);
	}
	{
		m_startColor.load(fin);
		m_endColor.load(fin);
	}
	{
		float time;
		fin >> time;
		m_spinLifeTime->SetValue(time * 10);
	}
	{
		float time;
		fin >> time;
		m_spinParticleTime->SetValue(time * 10);
	}
	{
		int direction;
		fin >> direction;
		m_spinDirection->SetValue(direction);
	}
	{
		int spread;
		fin >> spread;
		m_spinDirection->SetValue(spread);
	}
	{
		int acc;
		fin >> acc;
		m_spinRadialAcc->SetValue(acc);
	}
	{
		int acc;
		fin >> acc;
		m_spinTangentialAcc->SetValue(acc);
	}
	initParticle();
}

void ToolbarPanel::store(std::ofstream& fout) const
{
	fout << m_image->filepath() << std::endl;
	fout << m_spinEmissionRate->GetValue() << std::endl;
	fout << m_spinMinSpeed->GetValue() << " " << m_spinMaxSpeed->GetValue() << std::endl;
	fout << m_spinGravity->GetValue() << std::endl;
	fout << m_spinSize1->GetValue() << " " << m_spinSize2->GetValue() << std::endl;
	m_startColor.store(fout);
	m_endColor.store(fout);
	fout << m_spinLifeTime->GetValue()*0.1f << std::endl;
	fout << m_spinParticleTime->GetValue()*0.1f << std::endl;
	fout << m_spinDirection->GetValue() << std::endl;
	fout << m_spinSpread->GetValue() << std::endl;
	fout << m_spinRadialAcc->GetValue() << std::endl;
	fout << m_spinTangentialAcc->GetValue() << std::endl;
}

wxSizer* ToolbarPanel::initLayout()
{
	wxBoxSizer* topSizer = new wxBoxSizer(wxVERTICAL);
	
	topSizer->AddSpacer(10);
	// Open
	{
		wxButton* btn = new wxButton(this, wxID_ANY, wxT("Open..."));
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ToolbarPanel::onOpenImage));
		topSizer->Add(btn);
	}
	topSizer->AddSpacer(10);
	// Emission Rate
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Emission Rate:")));

		m_spinEmissionRate = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
			wxSP_ARROW_KEYS, 0, 1000, EMISSION_RATE);
		Connect(m_spinEmissionRate->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetEmissionRate));
		sizer->Add(m_spinEmissionRate);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Speed
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Speed:"));
		wxBoxSizer* speedSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("min:")));	

			m_spinMinSpeed = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
				wxSP_ARROW_KEYS, 0, 1000, MIN_SPEED);
			Connect(m_spinMinSpeed->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetSpeed));
			sizer->Add(m_spinMinSpeed);

			speedSizer->Add(sizer);
		}
		{
			wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("max:")));	

			m_spinMaxSpeed = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
				wxSP_ARROW_KEYS, 0, 1000, MAX_SPEED);
			Connect(m_spinMaxSpeed->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetSpeed));
			sizer->Add(m_spinMaxSpeed);

			speedSizer->Add(sizer);
		}
		topSizer->Add(speedSizer);
	}
	topSizer->AddSpacer(10);
	// Gravity
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Gravity:")));

		m_spinGravity = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
			wxSP_ARROW_KEYS, 0, 1000, GRAVITY);
		Connect(m_spinGravity->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetGravity));
		sizer->Add(m_spinGravity);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Size
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Size:"));
		wxBoxSizer* sizeSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("size1:")));	

			m_spinSize1 = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
				wxSP_ARROW_KEYS, 0, 100, SIZE1);
			Connect(m_spinSize1->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetSize));
			sizer->Add(m_spinSize1);

			sizeSizer->Add(sizer);
		}
		{
			wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("size2:")));	

			m_spinSize2 = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
				wxSP_ARROW_KEYS, 0, 100, SIZE2);
			Connect(m_spinSize2->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetSize));
			sizer->Add(m_spinSize2);

			sizeSizer->Add(sizer);
		}
		topSizer->Add(sizeSizer);
	}
	topSizer->AddSpacer(10);
	// Color
	{
		wxStaticBox* bounding = new wxStaticBox(this, wxID_ANY, wxT("Color:"));
		wxBoxSizer* colorSizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("start ")));

			m_startColor.btn = new wxButton(this, wxID_ANY);
			Connect(m_startColor.btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ToolbarPanel::onSetColorStart));
			sizer->Add(m_startColor.btn);
			sizer->AddSpacer(10);
			{
				m_startColor.sr = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
					wxSP_ARROW_KEYS, 0, 255, m_startColor.val.r);
				Connect(m_startColor.sr->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetColorFromSpin));
				sizer->Add(m_startColor.sr);
			}
			{
				m_startColor.sg = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
					wxSP_ARROW_KEYS, 0, 255, m_startColor.val.g);
				Connect(m_startColor.sg->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetColorFromSpin));
				sizer->Add(m_startColor.sg);
			}
			{
				m_startColor.sb = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
					wxSP_ARROW_KEYS, 0, 255, m_startColor.val.b);
				Connect(m_startColor.sb->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetColorFromSpin));
				sizer->Add(m_startColor.sb);
			}
			{
				m_startColor.sa = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
					wxSP_ARROW_KEYS, 0, 255, m_startColor.val.a);
				Connect(m_startColor.sa->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetColorFromSpin));
				sizer->Add(m_startColor.sa);
			}

			colorSizer->Add(sizer);
		}
		colorSizer->AddSpacer(10);
		{
			wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

			sizer->Add(new wxStaticText(this, wxID_ANY, wxT("end ")));

			m_endColor.btn = new wxButton(this, wxID_ANY);
			Connect(m_endColor.btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(ToolbarPanel::onSetColorEnd));
			sizer->Add(m_endColor.btn);
			sizer->AddSpacer(10);
			{
				m_endColor.sr = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
					wxSP_ARROW_KEYS, 0, 255, m_endColor.val.r);
				Connect(m_endColor.sr->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetColorFromSpin));
				sizer->Add(m_endColor.sr);
			}
			{
				m_endColor.sg = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
					wxSP_ARROW_KEYS, 0, 255, m_endColor.val.g);
				Connect(m_endColor.sg->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetColorFromSpin));
				sizer->Add(m_endColor.sg);
			}
			{
				m_endColor.sb = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
					wxSP_ARROW_KEYS, 0, 255, m_endColor.val.b);
				Connect(m_endColor.sb->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetColorFromSpin));
				sizer->Add(m_endColor.sb);
			}
			{
				m_endColor.sa = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(50, -1), 
					wxSP_ARROW_KEYS, 0, 255, m_endColor.val.a);
				Connect(m_endColor.sa->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetColorFromSpin));
				sizer->Add(m_endColor.sa);
			}

			colorSizer->Add(sizer);
		}
		topSizer->Add(colorSizer);
	}
	topSizer->AddSpacer(10);
	// Life Time
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Life Time:")));

		m_spinLifeTime = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
			wxSP_ARROW_KEYS, 0, 1000, LIFE_TIME);
		Connect(m_spinLifeTime->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetLifeTime));
		sizer->Add(m_spinLifeTime);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Particle Life
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Particle Life:")));

		m_spinParticleTime = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
			wxSP_ARROW_KEYS, 0, 1000, PARTICLE_TIME);
		Connect(m_spinParticleTime->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetParticleLife));
		sizer->Add(m_spinParticleTime);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Direction
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Direction:")));

		m_spinDirection = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
			wxSP_ARROW_KEYS, 0, 360, DIRECTION);
		Connect(m_spinDirection->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetDirection));
		sizer->Add(m_spinDirection);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Spread
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Spread:")));

		m_spinSpread = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
			wxSP_ARROW_KEYS, 0, 360, SPREAD);
		Connect(m_spinSpread->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetSpread));
		sizer->Add(m_spinSpread);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Radial Acceleration
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Radial Acceleration:")));

		m_spinRadialAcc = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
			wxSP_ARROW_KEYS, -10000, 10000, RADIAL_ACC);
		Connect(m_spinRadialAcc->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetRadialAcceleration));
		sizer->Add(m_spinRadialAcc);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);
	// Tangential Acceleration
	{
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
		sizer->Add(new wxStaticText(this, wxID_ANY, wxT("Tangential Acceleration:")));

		m_spinTangentialAcc = new wxSpinCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(70, -1), 
			wxSP_ARROW_KEYS, -10000, 10000, TANGENTIAL_ACC);
		Connect(m_spinTangentialAcc->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler(ToolbarPanel::onSetTangentialAcceleration));
		sizer->Add(m_spinTangentialAcc);

		topSizer->Add(sizer);
	}
	topSizer->AddSpacer(10);

	return topSizer;
}

void ToolbarPanel::enable(bool enable)
{
	m_spinEmissionRate->Enable(enable);
	m_spinMinSpeed->Enable(enable);
	m_spinMaxSpeed->Enable(enable);
	m_spinGravity->Enable(enable);
	m_spinSize1->Enable(enable);
	m_spinSize2->Enable(enable);
	m_startColor.enable(enable);
	m_endColor.enable(enable);
	m_spinLifeTime->Enable(enable);
	m_spinParticleTime->Enable(enable);
	m_spinDirection->Enable(enable);
	m_spinSpread->Enable(enable);
	m_spinRadialAcc->Enable(enable);
	m_spinTangentialAcc->Enable(enable);
}

void ToolbarPanel::initParticle()
{
	love::graphics::opengl::ParticleSystem* particle = Context::Instance()->stage->m_particle;

	particle->setEmissionRate(m_spinEmissionRate->GetValue());
	particle->setSpeed(m_spinMinSpeed->GetValue(), m_spinMaxSpeed->GetValue());
	particle->setGravity(m_spinGravity->GetValue());
	std::vector<float> sizes;
	sizes.push_back(m_spinSize1->GetValue());
	sizes.push_back(m_spinSize2->GetValue());
	particle->setSize(sizes);
	std::vector<love::graphics::Color> colors;
	colors.push_back(love::graphics::Color(m_startColor.val.r, m_startColor.val.g, m_startColor.val.b, m_startColor.val.a));
	colors.push_back(love::graphics::Color(m_endColor.val.r, m_endColor.val.g, m_endColor.val.b, m_endColor.val.a));
	particle->setColor(colors);
	particle->setPosition(400, 300);
	particle->setLifetime(m_spinLifeTime->GetValue()*0.1f);
	particle->setParticleLife(m_spinParticleTime->GetValue()*0.1f);
	particle->setDirection(m_spinDirection->GetValue());
	particle->setSpread(m_spinSpread->GetValue());
	particle->setRadialAcceleration(m_spinRadialAcc->GetValue());
	particle->setTangentialAcceleration(m_spinTangentialAcc->GetValue());
	particle->stop();
}

void ToolbarPanel::onOpenImage(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Choose image"), wxEmptyString, 
		wxEmptyString, wxT("*.png;*.jpg"), wxFD_OPEN);
	if (dlg.ShowModal() == wxID_OK)
	{
		enable(true);
		// todo release symbol
		d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(dlg.GetPath());
		m_image = static_cast<d2d::ImageSymbol*>(symbol)->getImage();
		if (Context::Instance()->stage->m_particle)
			Context::Instance()->stage->m_particle->setSprite(m_image);
		else
			Context::Instance()->stage->m_particle = new love::graphics::opengl::ParticleSystem(m_image, 1000);
		initParticle();
	}
}

void ToolbarPanel::onSetEmissionRate(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setEmissionRate(m_spinEmissionRate->GetValue());
}

void ToolbarPanel::onSetSpeed(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setSpeed(m_spinMinSpeed->GetValue(), m_spinMaxSpeed->GetValue());
}

void ToolbarPanel::onSetGravity(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setGravity(m_spinGravity->GetValue());
}

void ToolbarPanel::onSetSize(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setSpeed(m_spinSize1->GetValue(), m_spinSize2->GetValue());
}

void ToolbarPanel::onSetColorStart(wxCommandEvent& event)
{
	wxColourDialog dialog(this);
	dialog.SetTitle(wxT("Start Color"));
	if (dialog.ShowModal() == wxID_OK)
	{
		wxColour color = dialog.GetColourData().GetColour();
		m_startColor.set(color.Red(), color.Green(), color.Blue(), color.Alpha());
		resetParticleColor();
	}
}

void ToolbarPanel::onSetColorEnd(wxCommandEvent& event)
{
	wxColourDialog dialog(this);
	dialog.SetTitle(wxT("End Color"));
	if (dialog.ShowModal() == wxID_OK)
	{
		wxColour color = dialog.GetColourData().GetColour();
		m_endColor.set(color.Red(), color.Green(), color.Blue(), color.Alpha());
		resetParticleColor();
	}
}

void ToolbarPanel::onSetColorFromSpin(wxSpinEvent& event)
{
	m_startColor.set(m_startColor.sr->GetValue(), m_startColor.sg->GetValue(), m_startColor.sb->GetValue(), m_startColor.sa->GetValue());
	m_endColor.set(m_endColor.sr->GetValue(), m_endColor.sg->GetValue(), m_endColor.sb->GetValue(), m_endColor.sa->GetValue());
	resetParticleColor();
}

void ToolbarPanel::resetParticleColor()
{
	std::vector<love::graphics::Color> colors;
	colors.push_back(m_startColor.val);
	colors.push_back(m_endColor.val);
	Context::Instance()->stage->m_particle->setColor(colors);
}

void ToolbarPanel::onSetLifeTime(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setLifetime(m_spinLifeTime->GetValue() * 0.1f);
}

void ToolbarPanel::onSetParticleLife(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setParticleLife(m_spinParticleTime->GetValue() * 0.1f);
}

void ToolbarPanel::onSetDirection(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setDirection(m_spinDirection->GetValue() * d2d::PI / 180.0f);
}

void ToolbarPanel::onSetSpread(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setSpread(m_spinSpread->GetValue() * d2d::PI / 180.0f);
}

void ToolbarPanel::onSetRadialAcceleration(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setRadialAcceleration(m_spinRadialAcc->GetValue());
}

void ToolbarPanel::onSetTangentialAcceleration(wxSpinEvent& event)
{
	Context::Instance()->stage->m_particle->setTangentialAcceleration(m_spinTangentialAcc->GetValue());
}

//////////////////////////////////////////////////////////////////////////
// class ToolbarPanel::Color
//////////////////////////////////////////////////////////////////////////

void ToolbarPanel::Color::set(int r, int g, int b, int a)
{
	val.set(r, g, b, a);

	wxImage img(btn->GetSize().GetWidth(), btn->GetSize().GetHeight());
	img.SetRGB(wxRect(btn->GetSize()), r, g, b);
	wxBitmap bitmap(img);
	btn->SetBitmap(bitmap);

	sr->SetValue(r);
	sg->SetValue(g);
	sb->SetValue(b);
	sa->SetValue(a);
}

void ToolbarPanel::Color::enable(bool enable)
{
	btn->Enable(enable);
	sr->Enable(enable);
	sg->Enable(enable);
	sb->Enable(enable);
	sa->Enable(enable);
}

void ToolbarPanel::Color::load(std::ifstream& fin)
{
	int r, g, b, a;
	fin >> r >> g >> b >> a;
	set(r, g, b, a);
}

void ToolbarPanel::Color::store(std::ofstream& fout) const
{
	fout << sr->GetValue() << " " << sg->GetValue() << " " << sb->GetValue() << " " << sa->GetValue() << std::endl;
}

}