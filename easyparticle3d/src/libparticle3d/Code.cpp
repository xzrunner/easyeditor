#include "Code.h"
#include "ToolBarPanel.h"
#include "ps_config.h"

#include <ps/particle3d.h>

#include <easybuilder.h>

namespace lua = ebuilder::lua;

namespace eparticle3d
{

Code::Code(ebuilder::CodeGenerator& gen, ToolbarPanel* toolbar)
	: m_gen(gen)
	, m_toolbar(toolbar)
{
}

void Code::resolve()
{
	d2d::UICallback::Data data;

	std::string s = m_toolbar->m_name->GetValue();
	lua::TableAssign ta(m_gen, "['"+s+"']", true);

	lua::assign(m_gen, "['name']", "'"+s+"',");
	
	s = m_toolbar->m_package->GetValue();
	lua::assign(m_gen, "['package']", "'"+s+"',");

	m_toolbar->GetValue(PS_COUNT, data);
	s = wxString::FromDouble(data.val0);
	lua::assign(m_gen, "['count']", s+",");

	s = wxString::FromDouble(m_toolbar->m_layer->GetValue());
	lua::assign(m_gen, "['layer']", s+",");

	m_toolbar->GetValue(PS_EMISSION_TIME, data);
	s = wxString::FromDouble(data.val0);
	lua::assign(m_gen, "['emission_time']", s+",");

	m_toolbar->GetValue(PS_LIFE_TIME, data);
	s = wxString::FromDouble(data.val0 - data.val1);
	lua::assign(m_gen, "['min_life']", s+",");
	s = wxString::FromDouble(data.val0 + data.val1);
	lua::assign(m_gen, "['max_life']", s+",");

	s = wxString::FromDouble(m_toolbar->m_min_hori->GetValue());
	lua::assign(m_gen, "['min_hori']", s+",");

	s = wxString::FromDouble(m_toolbar->m_max_hori->GetValue());
	lua::assign(m_gen, "['max_hori']", s+",");

	s = wxString::FromDouble(m_toolbar->m_min_vert->GetValue());
	lua::assign(m_gen, "['min_vert']", s+",");

	s = wxString::FromDouble(m_toolbar->m_max_vert->GetValue());
	lua::assign(m_gen, "['max_vert']", s+",");

	m_toolbar->GetValue(PS_SPEED, data);
	s = wxString::FromDouble(data.val0 - data.val1);
	lua::assign(m_gen, "['min_spd']", s+",");
	s = wxString::FromDouble(data.val0 + data.val1);
	lua::assign(m_gen, "['max_spd']", s+",");

	m_toolbar->GetValue(PS_ANGULAR_SPEED, data);
	s = wxString::FromDouble(data.val0 - data.val1);
	lua::assign(m_gen, "['min_angular_spd']", s+",");
	s = wxString::FromDouble(data.val0 + data.val1);
	lua::assign(m_gen, "['max_angular_spd']", s+",");

	m_toolbar->GetValue(PS_DISTURBANCE_RADIUS, data);
	s = wxString::FromDouble(data.val0 - data.val1);
	lua::assign(m_gen, "['min_disturbance_radius']", s+",");
	s = wxString::FromDouble(data.val0 + data.val1);
	lua::assign(m_gen, "['max_disturbance_radius']", s+",");

	m_toolbar->GetValue(PS_DISTURBANCE_SPD, data);
	s = wxString::FromDouble(data.val0 - data.val1);
	lua::assign(m_gen, "['min_disturbance_spd']", s+",");
	s = wxString::FromDouble(data.val0 + data.val1);
	lua::assign(m_gen, "['max_disturbance_spd']", s+",");

	m_toolbar->GetValue(PS_GRAVITY, data);
	s = wxString::FromDouble(data.val0);
	lua::assign(m_gen, "['gravity']", s+",");

	m_toolbar->GetValue(PS_LINEAR_ACC, data);
	s = wxString::FromDouble(data.val0 - data.val1);
	lua::assign(m_gen, "['min_linear_acc']", s+",");
	s = wxString::FromDouble(data.val0 + data.val1);
	lua::assign(m_gen, "['max_linear_acc']", s+",");

	s = wxString::FromDouble(m_toolbar->m_inertia->GetValue());
	lua::assign(m_gen, "['inertia']", s+",");

	m_toolbar->GetValue(PS_FADEOUT_TIME, data);
	s = wxString::FromDouble(data.val0);
	lua::assign(m_gen, "['fadeout_time']", s+",");

	s = m_toolbar->m_bounce->GetValue() ? "true" : "false";
	lua::assign(m_gen, "['bounce']", s+",");

	s = m_toolbar->m_additiveBlend->GetValue() ? "true" : "false";
	lua::assign(m_gen, "['additive_blend']", s+",");

	m_toolbar->GetValue(PS_START_RADIUS, data);
	s = wxString::FromDouble(data.val0);
	lua::assign(m_gen, "['start_radius']", s+",");

	s = m_toolbar->m_radius_3d->GetValue() ? "true" : "false";
	lua::assign(m_gen, "['start_radius_3d']", s+",");

	s = m_toolbar->m_orient_to_movement->GetValue() ? "true" : "false";
	lua::assign(m_gen, "['orient_to_movement']", s+",");

	s = m_toolbar->m_orient_to_parent->GetValue() ? "true" : "false";
	lua::assign(m_gen, "['orient_to_parent']", s+",");

	{
		lua::TableAssign ta(m_gen, "['components']", true);
		for (size_t i = 0, n = m_toolbar->m_children.size(); i < n; ++i)
		{
			ToolbarPanel::ChildPanel* cp = m_toolbar->m_children[i];
			lua::TableAssign ta(m_gen, "");

			s = cp->m_name->GetValue();
			lua::assign(m_gen, "['name']", "'"+s+"',");

			cp->GetValue(PS_SCALE, data);
			s = wxString::FromDouble(data.val0);
			lua::assign(m_gen, "['start_scale']", s+",");
			s = wxString::FromDouble(data.val1);
			lua::assign(m_gen, "['end_scale']", s+",");

			cp->GetValue(PS_ROTATE, data);
			s = wxString::FromDouble(data.val0);
			lua::assign(m_gen, "['min_rotate']", s+",");
			s = wxString::FromDouble(data.val1);
			lua::assign(m_gen, "['max_rotate']", s+",");

			const ps_color4f& mul_col = cp->GetMulColor();
			s = wxString::FromDouble(mul_col.r);
			lua::assign(m_gen, "['mul_col_r']", s+",");
			s = wxString::FromDouble(mul_col.g);
			lua::assign(m_gen, "['mul_col_g']", s+",");
			s = wxString::FromDouble(mul_col.b);
			lua::assign(m_gen, "['mul_col_b']", s+",");

			const ps_color4f& add_col = cp->GetAddColor();
			s = wxString::FromDouble(add_col.r);
			lua::assign(m_gen, "['add_col_r']", s+",");
			s = wxString::FromDouble(add_col.g);
			lua::assign(m_gen, "['add_col_g']", s+",");
			s = wxString::FromDouble(add_col.b);
			lua::assign(m_gen, "['add_col_b']", s+",");

			cp->GetValue(PS_ALPHA, data);
			s = wxString::FromDouble(data.val0);
			lua::assign(m_gen, "['start_alpha']", s+",");
			s = wxString::FromDouble(data.val1);
			lua::assign(m_gen, "['end_alpha']", s+",");

			s = wxString::FromDouble(cp->m_startz->GetValue());
			lua::assign(m_gen, "['start_z']", s+",");
		}
	}
}

}