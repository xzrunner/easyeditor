#ifndef _GUM_PACKAGE_H_
#define _GUM_PACKAGE_H_

#include <simp/Package.h>

#include <string>
#include <vector>

namespace gum
{

class Image;

class Package : public simp::Package
{
public:
	Package(const std::string& filepath);

	void LoadAllImages();	

private:
	std::vector<Image*> m_images;

}; // Package

}

#endif // _GUM_PACKAGE_H_