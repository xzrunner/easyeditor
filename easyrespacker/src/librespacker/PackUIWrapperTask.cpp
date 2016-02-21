#include "PackUIWrapperTask.h"
#include "PackUI.h"
#include "pack_ui_cfg.h"

#include <ee/FileHelper.h>

namespace erespacker
{

PackUIWrapperTask::PackUIWrapperTask(const std::string& filepath, const Json::Value& value)
	: PackUITask(filepath)
	, m_id(-1)
{
	std::string wrapper_filepath = value["wrapper filepath"].asString();
	wrapper_filepath = ee::FileHelper::GetAbsolutePathFromFile(filepath, wrapper_filepath);
	wrapper_filepath = ee::FileHelper::FormatFilepathAbsolute(wrapper_filepath);

	m_cb_w = value["clipbox"]["w"].asInt();
	m_cb_h = value["clipbox"]["h"].asInt();
	
	m_type = value["user type"].asString();

	PackUI::Instance()->Instance()->AddListener(wrapper_filepath, this);
}

void PackUIWrapperTask::OnKnownPackID(const std::string& filepath, int id)
{
	m_id = id;
}

void PackUIWrapperTask::Output(const std::string& dir, Json::Value& value) const
{
	Json::Value item_val;
	item_val["filepath"] = ee::FileHelper::GetRelativePath(dir, m_filepath);
	item_val["wrapper id"] = m_id;
	item_val["type"] = m_type;
	item_val["clipbox"]["w"] = m_cb_w;
	item_val["clipbox"]["h"] = m_cb_h;
	value[value.size()] = item_val;
}

}