#include "Symbol.h"
#include "ParticleSystem.h"
#include "item_string.h"

namespace eparticle2d
{

Symbol::Symbol()
	: m_ps(NULL)
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

	static clock_t time = 0;
	clock_t curr = clock();
	if (time != 0) {
		m_ps->Update((float)(curr - time) / CLOCKS_PER_SEC);
		m_ps->Draw(mt);
	}
	time = curr;
}

d2d::Rect Symbol::getSize(const d2d::ISprite* sprite) const
{
//	return m_region;
	return d2d::Rect(200, 200);
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

 	m_ps = new ParticleSystem(1000);
 	m_ps->OnSetKeyValue(PS_EMISSION_TIME, value[ITEM_EMISSION_TIME].asInt());
	m_ps->OnSetKeyValue(PS_COUNT, value[ITEM_COUNT].asInt());
	m_ps->OnSetKeyValue(PS_FADEOUT_TIME, value[ITEM_FADEOUT_TIME].asInt());
	m_ps->OnSetKeyValue(PS_LIFE, value[ITEM_LIFE][ITEM_ATTR_CENTER].asInt(), value[ITEM_LIFE][ITEM_ATTR_OFFSET].asInt());
	m_ps->OnSetKeyValue(PS_POSITION, value[ITEM_POSITION][ITEM_ATTR_X].asInt(), value[ITEM_POSITION][ITEM_ATTR_Y].asInt());
	m_ps->OnSetKeyValue(PS_DIRECTION, value[ITEM_DIRECTION][ITEM_ATTR_CENTER].asInt(), value[ITEM_DIRECTION][ITEM_ATTR_OFFSET].asInt());
	m_ps->OnSetKeyValue(PS_SCALE, value[ITEM_SCALE][ITEM_ATTR_START].asInt(), value[ITEM_SCALE][ITEM_ATTR_END].asInt());
	m_ps->OnSetKeyValue(PS_SPEED, value[ITEM_SPEED][ITEM_ATTR_CENTER].asInt(), value[ITEM_SPEED][ITEM_ATTR_OFFSET].asInt());
	m_ps->OnSetKeyValue(PS_GRAVITY, value[ITEM_GRAVITY][ITEM_ATTR_CENTER].asInt(), value[ITEM_GRAVITY][ITEM_ATTR_OFFSET].asInt());
	m_ps->OnSetKeyValue(PS_RADIAL_ACC, value[ITEM_RADIAL_ACC][ITEM_ATTR_CENTER].asInt(), value[ITEM_RADIAL_ACC][ITEM_ATTR_OFFSET].asInt());
	m_ps->OnSetKeyValue(PS_TANGENTIAL_ACC, value[ITEM_TANGENTIAL_ACC][ITEM_ATTR_CENTER].asInt(), value[ITEM_TANGENTIAL_ACC][ITEM_ATTR_OFFSET].asInt());
	m_ps->OnSetKeyValue(PS_COS_AMPLITUDE, value[ITEM_COS][ITEM_COS_AMPLITUDE][ITEM_ATTR_CENTER].asInt(), value[ITEM_COS][ITEM_COS_AMPLITUDE][ITEM_ATTR_OFFSET].asInt());
	m_ps->OnSetKeyValue(PS_COS_FREQUENCY, value[ITEM_COS][ITEM_COS_FREQUENCY][ITEM_ATTR_CENTER].asInt(), value[ITEM_COS][ITEM_COS_FREQUENCY][ITEM_ATTR_OFFSET].asInt());

	wxString dir = d2d::FilenameTools::getFileDir(m_filepath) + "\\";
	wxString path = d2d::FilenameTools::getAbsolutePath(dir, value["symbol_path"].asString());
	d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(path);
	m_ps->SetSymbol(symbol);
	symbol->release();
}

void Symbol::ResetPS()
{
	m_ps->Reset();
	m_ps->Start();
}

}