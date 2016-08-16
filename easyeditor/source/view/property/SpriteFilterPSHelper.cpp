#include "SpriteFilterPSHelper.h"
#include "FilterModes.h"
#include "WXHelper.h"
#include "Sprite.h"
#include "SpriteGaussianBlur.h"
#include "StringHelper.h"

#include <sprite2/RenderShader.h>
#include <sprite2/RenderFilter.h>
#include <sprite2/FilterFactory.h>
#include <sprite2/RFEdgeDetection.h>
#include <sprite2/RFGaussianBlur.h>
#include <sprite2/RFOuterGlow.h>

#include <vector>

namespace ee
{

void SpriteFilterPSHelper::InitPS(const Sprite* spr, wxPropertyGrid* pg)
{
	std::vector<std::string> names;
	FilterModes::Instance()->GetAllNameCN(names);
	wxEnumProperty* filter_prop = new wxEnumProperty("Filter", wxPG_LABEL, WXHelper::ToWXStringArray(names));
	s2::FilterMode filter = spr->Shader().filter->GetMode();
	int idx = FilterModes::Instance()->GetIdxFromMode(filter);
	filter_prop->SetValue(idx);
	pg->Append(filter_prop);

	filter_prop->SetExpanded(true);
	CreateSubPS(pg, filter_prop, spr->Shader().filter);
}

bool SpriteFilterPSHelper::FromPS(const std::string& name, const wxAny& value, Sprite* spr)
{
	bool ret = false;
	if (name == "Filter")
	{
		int idx = wxANY_AS(value, int);
		s2::FilterMode filter = FilterModes::Instance()->GetModeFromIdx(idx);
		delete spr->Shader().filter;
		spr->Shader().filter = s2::FilterFactory::Instance()->Create(filter);
		ret = true;
	}
	else
	{
		switch (spr->Shader().filter->GetMode())
		{
		case s2::FM_EDGE_DETECTION:
			if (name == "Filter.Blend")
			{
				float blend = wxANY_AS(value, float);
				s2::RFEdgeDetection* filter = static_cast<s2::RFEdgeDetection*>(spr->Shader().filter);
				filter->SetBlend(blend);
				ret = true;
			}
			break;
		case s2::FM_GAUSSIAN_BLUR:
			if (name == "Filter.Iterations")
			{
				int iterations = wxANY_AS(value, int);
				s2::RFGaussianBlur* filter = static_cast<s2::RFGaussianBlur*>(spr->Shader().filter);
				filter->SetIterations(iterations);
				ret = true;
			}
			break;
		case s2::FM_OUTER_GLOW:
			if (name == "Filter.Iterations")
			{
				int iterations = wxANY_AS(value, int);
				s2::RFOuterGlow* filter = static_cast<s2::RFOuterGlow*>(spr->Shader().filter);
				filter->SetIterations(iterations);
				ret = true;
			}
			break;
		}
	}
	return ret;
}

void SpriteFilterPSHelper::ToPS(const Sprite* spr, wxPropertyGrid* pg)
{
	wxPGProperty* filter_prop = pg->GetProperty("Filter");

	const s2::RenderFilter* filter = spr->Shader().filter;
	filter_prop->SetValue(FilterModes::Instance()->GetIdxFromMode(filter->GetMode()));

	filter_prop->DeleteChildren();
	CreateSubPS(pg, filter_prop, filter);
}

void SpriteFilterPSHelper::CreateSubPS(wxPropertyGrid* pg, wxPGProperty* parent, const s2::RenderFilter* filter)
{
	s2::FilterMode mode = filter->GetMode();
	switch (mode)
	{
	case s2::FM_EDGE_DETECTION:
		{
			const s2::RFEdgeDetection* ed = static_cast<const s2::RFEdgeDetection*>(filter);
			wxPGProperty* prop = new wxFloatProperty("Blend", wxPG_LABEL, ed->GetBlend());
			pg->AppendIn(parent, prop);
		}
		break;
	case s2::FM_GAUSSIAN_BLUR:
		{
			const s2::RFGaussianBlur* gb = static_cast<const s2::RFGaussianBlur*>(filter);
			wxPGProperty* prop = new wxIntProperty("Iterations", wxPG_LABEL, gb->GetIterations());
			pg->AppendIn(parent, prop);
		}
		break;
	case s2::FM_OUTER_GLOW:
		{
			const s2::RFOuterGlow* og = static_cast<const s2::RFOuterGlow*>(filter);
			wxPGProperty* prop = new wxIntProperty("Iterations", wxPG_LABEL, og->GetIterations());
			pg->AppendIn(parent, prop);
		}
		break;
	}
}

}