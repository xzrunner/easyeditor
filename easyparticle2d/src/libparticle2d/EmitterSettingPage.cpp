#include "EmitterSettingPage.h"
#include "StagePanel.h"
#include "SliderCtrlOne.h"
#include "SliderCtrlTwo.h"
#include "config.h"
#include "item_string.h"
#include "ps_config.h"

namespace eparticle2d
{

static const int DEFAULT_DURATION = DURATION_INFINITY;

static const int DEFAULT_COUNT = 20;

static const int DEFAULT_EMISSION_TIME = 150;

static const float DEFAULT_FADEOUT_TIME = 300;

static const int DEFAULT_LIFE_CENTER = 1000, DEFAULT_LIFE_OFFSET = 2000;

static const int DEFAULT_DIRECTION_CENTER = 0, DEFAULT_DIRECTION_OFFSET = 180;

static const int DEFAULT_SCALE_START = 100, DEFAULT_SCALE_END = 100;

static const int DEFAULT_SPD_CENTER = 100, DEFAULT_SPD_OFFSET = 0;

static const int DEFAULT_GRAVITY_CENTER = 0, DEFAULT_GRAVITY_OFFSET = 0;

static const int DEFAULT_RADIAL_ACC_CENTER = 0, DEFAULT_RADIAL_ACC_OFFSET = 0;

static const int DEFAULT_TANGENTIAL_ACC_CENTER = 0, DEFAULT_TANGENTIAL_ACC_OFFSET = 0;

EmitterSettingPage::EmitterSettingPage(wxWindow* parent, StageData* stage_data)
	: SettingPageBase(parent)
	, m_stage_data(stage_data)
{
	InitLayout();
}

void EmitterSettingPage::InitLayout()
{
	wxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	UICallback* cb = m_stage_data;

	// Duration
	SliderCtrlOne* s_duration = new SliderCtrlOne(this, "Duration", ITEM_DURATION_TIME, cb, PS_DURATION, 
		SliderItem("", "", DEFAULT_DURATION, 0, 10));
	sizer->Add(s_duration);
	sizer->AddSpacer(10);
	m_sliders.push_back(s_duration);
	// Emission Time
	SliderCtrlOne* s_emission = new SliderCtrlOne(this, "Emission Time", ITEM_EMISSION_TIME, cb, PS_EMISSION_TIME, 
		SliderItem("", "", DEFAULT_EMISSION_TIME, 10, 10000), 0.001f);
	sizer->Add(s_emission);
	sizer->AddSpacer(10);
	m_sliders.push_back(s_emission);
	// Count
	SliderCtrlOne* s_count = new SliderCtrlOne(this, "Count", ITEM_COUNT, cb, PS_COUNT, 
		SliderItem("", "", DEFAULT_COUNT, 1, 100));
	sizer->Add(s_count);
	sizer->AddSpacer(10);
	m_sliders.push_back(s_count);
	// Fadeout Time
	SliderCtrlOne* s_fadeout = new SliderCtrlOne(this, "Fadeout Time", ITEM_FADEOUT_TIME, cb, PS_FADEOUT_TIME, 
		SliderItem("", "", DEFAULT_FADEOUT_TIME, 10, 2500), 0.001f);
	sizer->Add(s_fadeout);
	sizer->AddSpacer(10);
	m_sliders.push_back(s_fadeout);
	// Life
	SliderCtrlTwo* s_life = new SliderCtrlTwo(this, "Life", ITEM_LIFE, cb, PS_LIFE, 
		SliderItem("center", ITEM_ATTR_CENTER, DEFAULT_LIFE_CENTER, 0, 50000), SliderItem("offset", ITEM_ATTR_OFFSET, DEFAULT_LIFE_OFFSET, 0, 5000), 0.001f);
	sizer->Add(s_life);
	sizer->AddSpacer(10);
	m_sliders.push_back(s_life);
	// Position
	SliderCtrlTwo* s_spd = new SliderCtrlTwo(this, "位置", ITEM_POSITION, cb, PS_POSITION,
		SliderItem("x", ITEM_ATTR_X, 0, -500, 500), SliderItem("y", ITEM_ATTR_Y, 0, -500, 500));
	sizer->Add(s_spd);
	sizer->AddSpacer(10);
	m_sliders.push_back(s_spd);
	// Direction
	SliderCtrlTwo* s_dir = new SliderCtrlTwo(this, "方向", ITEM_DIRECTION, cb, PS_DIRECTION, 
		SliderItem("center", ITEM_ATTR_CENTER, DEFAULT_DIRECTION_CENTER, 0, 360), SliderItem("offset", ITEM_ATTR_OFFSET, DEFAULT_DIRECTION_OFFSET, 0, 360));
	sizer->Add(s_dir);
	sizer->AddSpacer(10);
	m_sliders.push_back(s_dir);
	// Scale
	SliderCtrlTwo* s_scale = new SliderCtrlTwo(this, "缩放", ITEM_SCALE, cb, PS_SCALE, 
		SliderItem("start", ITEM_ATTR_START, DEFAULT_SCALE_START, 0, 500), SliderItem("end", ITEM_ATTR_END, DEFAULT_SCALE_END, 0, 500), 0.01f);
	sizer->Add(s_scale);
	sizer->AddSpacer(10);
	m_sliders.push_back(s_scale);
	// Speed
	SliderCtrlTwo* s_speed = new SliderCtrlTwo(this, "速度", ITEM_SPEED, cb, PS_SPEED, 
		SliderItem("center", ITEM_ATTR_CENTER, DEFAULT_SPD_CENTER, 0, 500), SliderItem("offset", ITEM_ATTR_OFFSET, DEFAULT_SPD_OFFSET, 0, 200));
	sizer->Add(s_speed);
	sizer->AddSpacer(10);
	m_sliders.push_back(s_speed);
	// Gravity
	SliderCtrlTwo* s_gravity = new SliderCtrlTwo(this, "重力", ITEM_GRAVITY, cb, PS_GRAVITY, 
		SliderItem("center", ITEM_ATTR_CENTER, DEFAULT_GRAVITY_CENTER, -1000, 1000), SliderItem("offset", ITEM_ATTR_OFFSET, DEFAULT_GRAVITY_OFFSET, 0, 200));
	sizer->Add(s_gravity);
	sizer->AddSpacer(10);
	m_sliders.push_back(s_gravity);
	// Radial Acceleration
	SliderCtrlTwo* s_radial_acc = new SliderCtrlTwo(this, "法向加速度", ITEM_RADIAL_ACC, cb, PS_RADIAL_ACC, 
		SliderItem("center", ITEM_ATTR_CENTER, DEFAULT_RADIAL_ACC_CENTER, -2000, 2000), SliderItem("offset", ITEM_ATTR_OFFSET, DEFAULT_RADIAL_ACC_OFFSET, 0, 100));
	sizer->Add(s_radial_acc);
	sizer->AddSpacer(10);
	m_sliders.push_back(s_radial_acc);
	// Tangential Acceleration
	SliderCtrlTwo* s_tangential_acc = new SliderCtrlTwo(this, "切向加速度", ITEM_TANGENTIAL_ACC, cb, PS_TANGENTIAL_ACC, 
		SliderItem("center", ITEM_ATTR_CENTER, DEFAULT_RADIAL_ACC_CENTER, 0, 4000), SliderItem("offset", ITEM_ATTR_OFFSET, DEFAULT_RADIAL_ACC_OFFSET, 0, 100));
	sizer->Add(s_tangential_acc);
	sizer->AddSpacer(10);
	m_sliders.push_back(s_tangential_acc);

	SetSizer(sizer);
}

}