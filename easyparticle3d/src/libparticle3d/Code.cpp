#include "Code.h"
#include "ToolBarPanel.h"

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
	std::string s = m_toolbar->m_name->GetValue();
	lua::TableAssign ta(m_gen, "['"+s+"']", true);

	lua::assign(m_gen, "['name']", "'"+s+"',");
	
	s = m_toolbar->m_package->GetValue();
	lua::assign(m_gen, "['package']", "'"+s+"',");

	s = wxString::FromDouble(m_toolbar->m_count->GetValue());
	lua::assign(m_gen, "['count']", s+",");

	s = wxString::FromDouble(m_toolbar->m_layer->GetValue());
	lua::assign(m_gen, "['layer']", s+",");

	s = wxString::FromDouble(m_toolbar->m_emission_time->GetValue());
	lua::assign(m_gen, "['emission_time']", s+",");

	s = wxString::FromDouble(m_toolbar->m_min_life->GetValue());
	lua::assign(m_gen, "['min_life']", s+",");

	s = wxString::FromDouble(m_toolbar->m_max_life->GetValue());
	lua::assign(m_gen, "['max_life']", s+",");

	s = wxString::FromDouble(m_toolbar->m_min_hori->GetValue());
	lua::assign(m_gen, "['min_hori']", s+",");

	s = wxString::FromDouble(m_toolbar->m_max_hori->GetValue());
	lua::assign(m_gen, "['max_hori']", s+",");

	s = wxString::FromDouble(m_toolbar->m_min_vert->GetValue());
	lua::assign(m_gen, "['min_vert']", s+",");

	s = wxString::FromDouble(m_toolbar->m_max_vert->GetValue());
	lua::assign(m_gen, "['max_vert']", s+",");

	s = wxString::FromDouble(m_toolbar->m_min_spd->GetValue());
	lua::assign(m_gen, "['min_spd']", s+",");

	s = wxString::FromDouble(m_toolbar->m_max_spd->GetValue());
	lua::assign(m_gen, "['max_spd']", s+",");

	s = wxString::FromDouble(m_toolbar->m_gravity->GetValue());
	lua::assign(m_gen, "['gravity']", s+",");

	s = wxString::FromDouble(m_toolbar->m_inertia->GetValue());
	lua::assign(m_gen, "['inertia']", s+",");

	s = wxString::FromDouble(m_toolbar->m_fadeout_time->GetValue());
	lua::assign(m_gen, "['fadeout_time']", s+",");

	s = m_toolbar->m_bounce->GetValue() ? "true" : "false";
	lua::assign(m_gen, "['bounce']", s+",");

	s = m_toolbar->m_additiveBlend->GetValue() ? "true" : "false";
	lua::assign(m_gen, "['additive_blend']", s+",");

	s = wxString::FromDouble(m_toolbar->m_start_radius->GetValue());
	lua::assign(m_gen, "['start_radius']", s+",");

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

			s = wxString::FromDouble(cp->m_start_scale->GetValue());
			lua::assign(m_gen, "['start_scale']", s+",");

			s = wxString::FromDouble(cp->m_end_scale->GetValue());
			lua::assign(m_gen, "['end_scale']", s+",");

			s = wxString::FromDouble(cp->m_min_rotate->GetValue());
			lua::assign(m_gen, "['min_rotate']", s+",");

			s = wxString::FromDouble(cp->m_max_rotate->GetValue());
			lua::assign(m_gen, "['max_rotate']", s+",");

			s = wxString::FromDouble(cp->m_startz->GetValue());
			lua::assign(m_gen, "['start_z']", s+",");
		}
	}
}

}