#include "Symbol.h"
#include "ParticleSystem.h"
#include "item_string.h"
#include "ps_config.h"

namespace eparticle2d
{

Symbol::Symbol()
	: m_ps(NULL)
	, m_curr_frame(0)
{
}

Symbol::Symbol(const Symbol& s)
	: d2d::ISymbol(s)
{
}

Symbol::~Symbol()
{
	if (m_ps)
	{
		m_ps->release();
		m_ps = NULL;
	}
}

Symbol* Symbol::clone() const
{
	return new Symbol(*this);
}

void Symbol::reloadTexture() const
{
	m_ps->ReloadTexture();
}

void Symbol::draw(const d2d::Matrix& mt,
				  const d2d::Colorf& mul, 
				  const d2d::Colorf& add,
				  const d2d::ISprite* sprite) const
{
	if (!m_ps) {
		return;
	}

	m_ps->Draw(mt);

	if (sprite) {
		DrawBackground(sprite->getPosition());
	}
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite) const
{
	return d2d::Rect(200, 200);
}

void Symbol::Update(float dt, int frame)
{
	if (m_curr_frame != frame) {
		m_ps->Update(dt);
		m_curr_frame = frame;
	}
}

void Symbol::ResetPS()
{
	m_ps->Reset();
	m_ps->Start();
}

void Symbol::loadResources()
{
	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int version = value["version"].asInt();

 	m_ps = new ParticleSystem(1000);
	if (version == 0) {
		m_ps->SetValue(PS_EMISSION_TIME, UICallback::Data(value[ITEM_EMISSION_TIME].asDouble() * 0.001f));
		m_ps->SetValue(PS_FADEOUT_TIME, UICallback::Data(value[ITEM_FADEOUT_TIME].asDouble() * 0.001f));
		m_ps->SetValue(PS_LIFE, UICallback::Data(value[ITEM_LIFE][ITEM_ATTR_CENTER].asDouble() * 0.001f, value[ITEM_LIFE][ITEM_ATTR_OFFSET].asDouble() * 0.001f));
		m_ps->SetValue(PS_SCALE, UICallback::Data(value[ITEM_SCALE][ITEM_ATTR_START].asDouble() * 0.01f, value[ITEM_SCALE][ITEM_ATTR_END].asDouble() * 0.01f));
		m_ps->SetValue(PS_COS_FREQUENCY, UICallback::Data(value[ITEM_COS][ITEM_COS_FREQUENCY][ITEM_ATTR_CENTER].asDouble() * 0.01f, value[ITEM_COS][ITEM_COS_FREQUENCY][ITEM_ATTR_OFFSET].asDouble() * 0.01f));
	} else {
		m_ps->SetValue(PS_EMISSION_TIME, UICallback::Data(value[ITEM_EMISSION_TIME].asDouble()));
		m_ps->SetValue(PS_FADEOUT_TIME, UICallback::Data(value[ITEM_FADEOUT_TIME].asDouble()));
		m_ps->SetValue(PS_LIFE, UICallback::Data(value[ITEM_LIFE][ITEM_ATTR_CENTER].asDouble(), value[ITEM_LIFE][ITEM_ATTR_OFFSET].asDouble()));
		m_ps->SetValue(PS_SCALE, UICallback::Data(value[ITEM_SCALE][ITEM_ATTR_START].asDouble(), value[ITEM_SCALE][ITEM_ATTR_END].asDouble()));
		m_ps->SetValue(PS_COS_FREQUENCY, UICallback::Data(value[ITEM_COS][ITEM_COS_FREQUENCY][ITEM_ATTR_CENTER].asDouble(), value[ITEM_COS][ITEM_COS_FREQUENCY][ITEM_ATTR_OFFSET].asDouble()));
	}
	m_ps->SetValue(PS_COUNT, UICallback::Data(value[ITEM_COUNT].asInt()));
	m_ps->SetValue(PS_POSITION, UICallback::Data(value[ITEM_POSITION][ITEM_ATTR_X].asInt(), value[ITEM_POSITION][ITEM_ATTR_Y].asInt()));
	m_ps->SetValue(PS_DIRECTION, UICallback::Data(value[ITEM_DIRECTION][ITEM_ATTR_CENTER].asInt(), value[ITEM_DIRECTION][ITEM_ATTR_OFFSET].asInt()));
	m_ps->SetValue(PS_SPEED, UICallback::Data(value[ITEM_SPEED][ITEM_ATTR_CENTER].asInt(), value[ITEM_SPEED][ITEM_ATTR_OFFSET].asInt()));
	m_ps->SetValue(PS_GRAVITY, UICallback::Data(value[ITEM_GRAVITY][ITEM_ATTR_CENTER].asInt(), value[ITEM_GRAVITY][ITEM_ATTR_OFFSET].asInt()));
	m_ps->SetValue(PS_RADIAL_ACC, UICallback::Data(value[ITEM_RADIAL_ACC][ITEM_ATTR_CENTER].asInt(), value[ITEM_RADIAL_ACC][ITEM_ATTR_OFFSET].asInt()));
	m_ps->SetValue(PS_TANGENTIAL_ACC, UICallback::Data(value[ITEM_TANGENTIAL_ACC][ITEM_ATTR_CENTER].asInt(), value[ITEM_TANGENTIAL_ACC][ITEM_ATTR_OFFSET].asInt()));
	m_ps->SetValue(PS_COS_AMPLITUDE, UICallback::Data(value[ITEM_COS][ITEM_COS_AMPLITUDE][ITEM_ATTR_CENTER].asInt(), value[ITEM_COS][ITEM_COS_AMPLITUDE][ITEM_ATTR_OFFSET].asInt()));

	wxString dir = d2d::FilenameTools::getFileDir(m_filepath) + "\\";
	wxString path = d2d::FilenameTools::getAbsolutePath(dir, value["symbol_path"].asString());
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(path);
	m_ps->SetSymbol(symbol);
	symbol->release();
}

void Symbol::DrawBackground(const d2d::Vector& pos) const
{
	d2d::Matrix mat;
	mat.translate(pos.x, pos.y);

	d2d::Rect r = getSize();

	d2d::PrimitiveDraw::rect(mat, r, d2d::LIGHT_GREY_LINE);
}

}