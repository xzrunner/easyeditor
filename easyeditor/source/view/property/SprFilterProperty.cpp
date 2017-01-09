#include "SprFilterProperty.h"
#include "FilterModes.h"
#include "WXHelper.h"
#include "Sprite.h"
#include "StringHelper.h"
#include "FilepathProperty.h"
#include "Image.h"
#include "Exception.h"
#include "ExceptionDlg.h"

#include <shaderlab/ShaderMgr.h>
#include <shaderlab/HeatHazeProg.h>
#include <shaderlab/FilterShader.h>
#include <shaderlab/ColGradingProg.h>
#include <sprite2/RenderShader.h>
#include <sprite2/RenderFilter.h>
#include <sprite2/FilterFactory.h>
#include <sprite2/RFEdgeDetection.h>
#include <sprite2/RFGaussianBlur.h>
#include <sprite2/RFOuterGlow.h>
#include <sprite2/RFHeatHaze.h>
#include <sprite2/RFColGrading.h>

#include <vector>

namespace ee
{

SprFilterProperty::SprFilterProperty(wxWindow* parent)
	: m_parent(parent)
{
}

void SprFilterProperty::InitPS(const Sprite* spr, wxPropertyGrid* pg)
{
	std::vector<std::string> names;
	FilterModes::Instance()->GetAllNameCN(names);
	wxEnumProperty* filter_prop = new wxEnumProperty("Filter", wxPG_LABEL, WXHelper::ToWXStringArray(names));
	s2::FilterMode filter = s2::FM_NULL;
	const s2::RenderFilter* rf = spr->GetShader().GetFilter();
	if (rf) {
		filter = rf->GetMode();
	}
	int idx = FilterModes::Instance()->Mode2ID(filter);
	filter_prop->SetValue(idx);
	pg->Append(filter_prop);
	if (rf) {
		filter_prop->SetExpanded(true);
		CreateSubPS(pg, filter_prop, rf);
	}
}

bool SprFilterProperty::FromPS(const std::string& name, const wxAny& value, Sprite* spr)
{
	bool ret = false;

	if (name == "Filter")
	{
		int idx = wxANY_AS(value, int);
		s2::FilterMode filter = FilterModes::Instance()->ID2Mode(idx);
		s2::RenderShader rs = spr->GetShader();
		rs.SetFilter(filter);
		spr->SetShader(rs);
		ret = true;
	}
	else if (s2::RenderFilter* rf = const_cast<s2::RenderFilter*>(spr->GetShader().GetFilter()))
	{
		switch (spr->GetShader().GetFilter()->GetMode())
		{
		case s2::FM_EDGE_DETECTION:
			if (name == "Filter.Blend")
			{
				float blend = wxANY_AS(value, float);
				s2::RFEdgeDetection* filter = static_cast<s2::RFEdgeDetection*>(rf);
				filter->SetBlend(blend);
				ret = true;
			}
			break;
		case s2::FM_GAUSSIAN_BLUR:
			if (name == "Filter.Iterations")
			{
				int iterations = wxANY_AS(value, int);
				s2::RFGaussianBlur* filter = static_cast<s2::RFGaussianBlur*>(rf);
				filter->SetIterations(iterations);
				ret = true;
			}
			break;
		case s2::FM_OUTER_GLOW:
			if (name == "Filter.Iterations")
			{
				int iterations = wxANY_AS(value, int);
				s2::RFOuterGlow* filter = static_cast<s2::RFOuterGlow*>(rf);
				filter->SetIterations(iterations);
				ret = true;
			}
			break;
		case s2::FM_HEAT_HAZE:
			{
				sl::HeatHazeProg* prog = NULL;
				sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
				sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
				if (shader) {
					prog = static_cast<sl::HeatHazeProg*>(shader->GetProgram(sl::FM_HEAT_HAZE));
				}
				s2::RFHeatHaze* filter = static_cast<s2::RFHeatHaze*>(rf);

				if (name == "Filter.Filepath")
				{
					std::string filepath = wxANY_AS(value, wxString);
					filter->SetFilepath(filepath);
					if (prog) {
						prog->SetDistortionMapTex(GetTextureID(filepath));
					}
					ret = true;
				}
				else if (name == "Filter.Distortion")
				{
					float old_dist, old_rise;
					filter->GetFactor(old_dist, old_rise);
					float dist = wxANY_AS(value, float);
					filter->SetFactor(dist, old_rise);
					if (prog) {
						prog->SetFactor(dist, old_rise);
					}
					ret = true;
				}
				else if (name == "Filter.Rise")
				{
					float old_dist, old_rise;
					filter->GetFactor(old_dist, old_rise);
					float rise = wxANY_AS(value, float);
					filter->SetFactor(old_dist, rise);
					if (prog) {
						prog->SetFactor(old_dist, rise);
					}
					ret = true;
				}
			}
			break;
		case s2::FM_COL_GRADING:
			{
				sl::ColGradingProg* prog = NULL;
				sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
				sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
				if (shader) {
					prog = static_cast<sl::ColGradingProg*>(shader->GetProgram(sl::FM_COL_GRADING));
				}
				s2::RFColGrading* filter = static_cast<s2::RFColGrading*>(rf);
				std::string filepath = wxANY_AS(value, wxString);
				filter->SetFilepath(filepath);
				if (prog) {
					prog->SetLUTTex(GetTextureID(filepath));
				}
				ret = true;
			}
			break;
		}
	}

	return ret;
}

void SprFilterProperty::ToPS(const Sprite* spr, wxPropertyGrid* pg)
{
	wxPGProperty* filter_prop = pg->GetProperty("Filter");
	
	s2::FilterMode filter = s2::FM_NULL;
	const s2::RenderFilter* rf = spr->GetShader().GetFilter();
	if (rf) {
		filter = rf->GetMode();
	}
	filter_prop->SetValue(FilterModes::Instance()->Mode2ID(filter));

	filter_prop->DeleteChildren();

	if (rf) {
		CreateSubPS(pg, filter_prop, rf);
	}
}

static void set_filepath_cb(const std::string& filepath, void* ud)
{
	s2::RenderFilter* filter = static_cast<s2::RenderFilter*>(ud);
	if (s2::RFHeatHaze* hh = dynamic_cast<s2::RFHeatHaze*>(filter)) 
	{
		hh->SetFilepath(filepath);

		sl::HeatHazeProg* prog = NULL;
		sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
		sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
		if (shader) {
			prog = static_cast<sl::HeatHazeProg*>(shader->GetProgram(sl::FM_HEAT_HAZE));
		}
		if (prog) {
			Image* img = ImageMgr::Instance()->GetItem(filepath);
			if (img) {
				prog->SetDistortionMapTex(img->GetTexID());
			}
		}
	} 
	else if (s2::RFColGrading* cg = dynamic_cast<s2::RFColGrading*>(filter))
	{
		cg->SetFilepath(filepath);

		sl::ColGradingProg* prog = NULL;
		sl::ShaderMgr* mgr = sl::ShaderMgr::Instance();
		sl::FilterShader* shader = static_cast<sl::FilterShader*>(mgr->GetShader(sl::FILTER));
		if (shader) {
			prog = static_cast<sl::ColGradingProg*>(shader->GetProgram(sl::FM_COL_GRADING));
		}
		if (prog) {
			Image* img = ImageMgr::Instance()->GetItem(filepath);
			if (img) {
				prog->SetLUTTex(img->GetTexID());
			}
		}
	}
}

void SprFilterProperty::CreateSubPS(wxPropertyGrid* pg, wxPGProperty* parent, const s2::RenderFilter* filter)
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
	case s2::FM_HEAT_HAZE:
		{
			const s2::RFHeatHaze* hh = static_cast<const s2::RFHeatHaze*>(filter);

			FilepathProperty* file_prop = new FilepathProperty("Filepath", wxPG_LABEL, hh->GetFilepath());
			file_prop->SetCallback(set_filepath_cb, const_cast<s2::RenderFilter*>(filter));
			pg->AppendIn(parent, file_prop);

			float dist, rise;
			hh->GetFactor(dist, rise);
			wxPGProperty* dist_prop = new wxFloatProperty("Distortion", wxPG_LABEL, dist);
			pg->AppendIn(parent, dist_prop);
			wxPGProperty* rise_prop = new wxFloatProperty("Rise", wxPG_LABEL, rise);
			pg->AppendIn(parent, rise_prop);
		}
		break;
	case s2::FM_COL_GRADING:
		{
			const s2::RFColGrading* cg = static_cast<const s2::RFColGrading*>(filter);
			FilepathProperty* file_prop = new FilepathProperty("Filepath", wxPG_LABEL, cg->GetFilepath());
			file_prop->SetCallback(set_filepath_cb, const_cast<s2::RenderFilter*>(filter));
			pg->AppendIn(parent, file_prop);
		}
		break;
	}
}

int SprFilterProperty::GetTextureID(const std::string& filepath) const
{
	int ret = 0;
	try {
		Image* img = ImageMgr::Instance()->GetItem(filepath);
		if (img) {
			ret = img->GetTexID();
		}
	} catch (ee::Exception& e) {
		ee::ExceptionDlg dlg(m_parent, e);
		dlg.ShowModal();
	}
	return ret;
}

}