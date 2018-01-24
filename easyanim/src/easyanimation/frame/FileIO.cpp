#include "FileIO.h"
#include "DefaultFileLoader.h"
#include "BodymovinLoader.h"
#include "FlashLoader.h"
#include "FileSaver.h"
#include "frame/LoadFromBin.h"
#include "dataset/DataMgr.h"
#include "view/ViewMgr.h"
#include "view/LibraryPanel.h"
#include "message/messages.h"

#include <ee/FileHelper.h>

#include <gum/ExtendSymFile.h>
#include <gum/FilepathHelper.h>

#include <fstream>

namespace eanim
{

std::string FileIO::m_filepath;

void FileIO::Load(const std::string& filepath)
{
	FileIO::m_filepath = filepath;

	auto ext = gum::FilepathHelper::Extension(filepath);
	if (ext == ".bin") {
		LoadFromBin::Load(filepath);
		return;
	}

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();
	
	int type = gum::ExtendSymFile::GetType(value);
	switch (type)
	{
	case gum::SYM_SPINE:
		break;
	case gum::SYM_BODYMOVIN:
		{
			std::string dir = ee::FileHelper::GetFileDir(filepath);
			BodymovinLoader::Load(value, dir);
		}
		break;
	case gum::SYM_UNKNOWN:
		{
			std::string dir = ee::FileHelper::GetFileDir(filepath);
			DefaultFileLoader::Load(value, dir);
		}
		break;
	}
}

void FileIO::StoreSingle(const std::string& filepath)
{
	FileSaver::StoreSingle(filepath);
}

void FileIO::StoreTemplate(const std::string& filepath)
{
	FileSaver::StoreTemplate(filepath);
}

void FileIO::Reload()
{
	if (m_filepath.empty()) return;

	DataMgr::Instance()->GetLayers().Clear();
	ViewMgr::Instance()->library->Clear();
	SetSelectedSJ::Instance()->Set(-1, -1);

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(m_filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	int type = gum::ExtendSymFile::GetType(value);
	switch (type)
	{
	case gum::SYM_SPINE:
		break;
	case gum::SYM_BODYMOVIN:
		break;
	case gum::SYM_UNKNOWN:
		{
			std::string dir = ee::FileHelper::GetFileDir(m_filepath);
			DefaultFileLoader::Reload(value, dir);
		}
		break;
	}
}

void FileIO::StoreAsGif(const std::string& src, const std::string& dst)
{
	FileSaver::StoreAsGif(src, dst);
}

void FileIO::StoreAsPng(const std::string& src, const std::string& dst)
{
	FileSaver::StoreAsPng(src, dst);
}

} // eanim