#include "TransOldLRFile.h"
#include "check_params.h"

#include <ee/std_functor.h>

#include <fstream>
#include <algorithm>

namespace edb
{

std::string TransOldLRFile::Command() const
{
	return "trans-old-lr";
}

std::string TransOldLRFile::Description() const
{
	return "trans old lr file";
}

std::string TransOldLRFile::Usage() const
{
	// trans-old-lr e:/test2/test_lr.json
	std::string usage = Command() + " [filepath]";
	return usage;
}

int TransOldLRFile::Run(int argc, char *argv[])
{
	if (!check_number(this, argc, 3)) return -1;
	if (!check_file(argv[2])) return -1;

	Run(argv[2]);

	return 0;
}

void TransOldLRFile::Run(const std::string& filepath)
{
	Json::Value old_val;
	Json::Reader reader;
	std::locale::global(std::locale(""));
	std::ifstream fin(filepath.c_str());
	std::locale::global(std::locale("C"));
	reader.parse(fin, old_val);
	fin.close();

	Json::Value cam_val = old_val["camera"];
	Json::Value size_val = old_val["size"];
	Json::Value old_sprs_val = old_val["sprite"];

	Json::Value old_layer_val = old_val["layer"];
	
	std::vector<Layer*> layers;
	layers.push_back(new Layer("terrain", old_layer_val["terrain"]));
	layers.push_back(new Layer("decoration", old_layer_val["decoration"]));
	layers.push_back(new Layer("unit", old_layer_val["unit"]));
	layers.push_back(new Layer("point", old_layer_val["point"]));
	layers.push_back(new Layer("path", old_layer_val["path"]));
	layers.push_back(new Layer("region", old_layer_val["region"]));
	layers.push_back(new Layer("collision region", old_layer_val["collision region"]));

	Json::Value new_sprs_val;
	int new_spr_count = 0;

	int idx = 0;
	Json::Value spr_val = old_sprs_val[idx++];
	while (!spr_val.isNull()) {
		std::string filepath = spr_val["filepath"].asString();
		bool find = false;
		for (int i = 0, n = layers.size(); i < n; ++i) {
			if (layers[i]->InsertSprite(filepath, spr_val)) {
				find = true;
				break;
			}
		}		
		if (!find) {
			new_sprs_val[new_spr_count++] = spr_val;
		}

		spr_val = old_sprs_val[idx++];
	}

	Json::Value new_val;
	new_val["camera"] = cam_val;
	new_val["size"] = size_val;
	new_val["sprite"] = new_sprs_val;
	for (int i = 0, n = layers.size(); i < n; ++i) {
		 layers[i]->BuildNewJsonVal(new_val["layer"][i]);
	}

	for_each(layers.begin(), layers.end(), ee::DeletePointerFunctor<Layer>());

	Json::StyledStreamWriter writer;
	std::locale::global(std::locale(""));
	std::ofstream fout(filepath.c_str());
	std::locale::global(std::locale("C"));
	writer.write(fout, new_val);
	fout.close();
}

//////////////////////////////////////////////////////////////////////////
// class TransOldLRFile::Layer
//////////////////////////////////////////////////////////////////////////

TransOldLRFile::Layer::
Layer(const std::string& name, const Json::Value& old_val)
{
	m_name = name;
	m_editable = old_val["editable"].asBool();
	m_visible = old_val["visible"].asBool();
	
	int idx = 0;
	Json::Value val = old_val["symbol"][idx++];
	while (!val.isNull()) {
		m_symbol_paths.insert(val.asString());
		val = old_val["symbol"][idx++];
	}
}

bool TransOldLRFile::Layer::
InsertSprite(const std::string& filepath, const Json::Value& spr_val)
{
	std::set<std::string>::iterator itr = m_symbol_paths.find(filepath);
	if (itr == m_symbol_paths.end()) {
		return false;
	} else {
		m_spr_values.push_back(spr_val);
		return true;
	}
}

void TransOldLRFile::Layer::
BuildNewJsonVal(Json::Value& new_val) const
{
	new_val["name"] = m_name;
	new_val["editable"] = m_editable;
	new_val["visible"] = m_visible;
	for (int i = 0, n = m_spr_values.size(); i < n; ++i) {
		new_val["sprite"][i] = m_spr_values[i];
	}
}

}