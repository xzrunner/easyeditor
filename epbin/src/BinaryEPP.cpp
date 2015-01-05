#include "BinaryEPP.h"
#include "Exception.h"

#include "PNGLoader.h"
#include "PVRLoader.h"
#include "PKMLoader.h"

#include <sstream>

namespace epbin
{

BinaryEPP::BinaryEPP(const std::string& dir, const std::string& filename, 
					 TextureType type)
	: m_dir(dir)
	, m_filename(filename)
	, m_type(type)
{
}

void BinaryEPP::Pack(const std::string& outfile) const
{
	std::ofstream fout(outfile.c_str(), std::ios::binary);

	std::string ext;
	switch (m_type) 
	{
	case TT_PNG4: case TT_PNG8:
		ext = ".png";
		break;
	case TT_PVR:
		ext = ".pvr";
		break;
	case TT_PKM:
		ext = ".pkm";
		break;
	}

	int i = 1;
	while (true)
	{
		std::stringstream ss;
		ss << m_dir << "\\" << m_filename << i++ << ext;

		std::ifstream fin(ss.str().c_str(), std::ios::binary);
		if (fin.fail()) {
			break;
		}
		fin.close();

		TexLoader* loader;
		switch (m_type)
		{
		case TT_PNG4:
			loader = new PNGLoader(false);
			break;
		case TT_PNG8:
			loader = new PNGLoader(true);
			break;
		case TT_PVR:
			loader = new PVRLoader();
			break;
		case TT_PKM:
			loader = new PKMLoader();
			break;
		default:
			throw Exception("BinaryEPP::Pack unknown type: %d\n", m_type);
		}

		loader->Load(ss.str());
		loader->Store(fout);
		delete loader;
	}

	fout.close();
}

}