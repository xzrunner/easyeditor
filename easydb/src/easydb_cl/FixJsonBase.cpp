#include "FixJsonBase.h"

#include <wx/filename.h>
#include <fstream>
#include <drag2d.h>

namespace edb
{

void FixJsonBase::FixComplex(const wxString& path) const
{
	wxFileName filename(path);
	filename.Normalize();
	wxString filepath = filename.GetFullPath();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	bool dirty = false;

	int i = 0;
	Json::Value spriteVal = value["sprite"][i++];
	while (!spriteVal.isNull()) {
		Json::Value& val = value["sprite"][i-1];
		if (FixSprite(path, val)) {
			dirty = true;
		}
		spriteVal = value["sprite"][i++];
	}

	if (dirty) {
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.fn_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value);
		fout.close();
	}
}

void FixJsonBase::FixAnim(const wxString& path) const
{
	wxFileName filename(path);
	filename.Normalize();
	wxString filepath = filename.GetFullPath();

	Json::Value value;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.fn_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, value);
	fin.close();

	bool dirty = false;

	int i = 0;
	Json::Value layerVal = value["layer"][i++];
	while (!layerVal.isNull()) {
		int j = 0;
		Json::Value frameVal = layerVal["frame"][j++];
		while (!frameVal.isNull()) {
			int k = 0;
			Json::Value entryVal = frameVal["actor"][k++];
			while (!entryVal.isNull()) {
				Json::Value& val = value["layer"][i-1]["frame"][j-1]["actor"][k-1];
				if (FixSprite(path, val)) {
					dirty = true;
				}
				entryVal = frameVal["actor"][k++];
			}

			frameVal = layerVal["frame"][j++];
		}

		layerVal = value["layer"][i++];
	}

	if (dirty) {
		Json::StyledStreamWriter writer;
		std::locale::global(std::locale(""));
		std::ofstream fout(filepath.fn_str());
		std::locale::global(std::locale("C"));	
		writer.write(fout, value);
		fout.close();
	}
}


}