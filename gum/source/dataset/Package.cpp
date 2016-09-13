#include "Package.h"
#include "Image.h"

#include <boost/lexical_cast.hpp>

namespace gum
{

Package::Package(const std::string& filepath)
	: simp::Package(filepath)
{
}

void Package::LoadAllImages()
{
	int n = m_images.size();
	for (int i = 0; i < n; ++i)
	{
		std::string s_idx = boost::lexical_cast<std::string>(i + 1);
		std::string filepath = m_filepath + "." + s_idx + ".ept";
		Image* img = new Image;
		img->LoadBin(filepath);
		m_images.push_back(img);
	}
}

}