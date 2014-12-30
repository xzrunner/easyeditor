#include "Image.h"

#include "utilities/Constants.h"

#include <iostream>

namespace rt
{

Image::Image()
	: m_hres(0)
	, m_vres(0)
{
}

void Image::ReadPPMFile(const char* file_name)
{
	// read-only binary sequential access

	FILE* file = fopen(file_name, "rb");

	if (file == 0){
		std::cout << "could not open file" << std::endl;
	}
	else
		std::cout << "file opened" << std::endl;

	// PPM header

	unsigned char ppm_type;
	if (fscanf(file, "P%c\n", &ppm_type) != 1){
		std::cout << "Invalid PPM signature" << std::endl;
	}

	// only binary PPM supported

	if (ppm_type != '6'){
		std::cout << "Only binary PPM supported" << std::endl;
	}

	// skip comments

	unsigned char dummy;
	while (fscanf(file ,"#%c", &dummy)) 
		while (fgetc(file) != '\n');

	// read image size

	if (fscanf(file, "%d %d\n", &m_hres, &m_vres) != 2){
		std::cout << "Invalid image size" << std::endl;
	}

	if (m_hres <= 0)
		std::cout << "Invalid image width" << std::endl;
	else
		std::cout << "m_width = " << m_hres << std::endl;


	if (m_vres <= 0)
		std::cout << "Invalid image height" << std::endl;
	else
		std::cout << "m_height = " << m_vres << std::endl;


	// maximum value to be found in the PPM file (usually 255)

	unsigned int max_value;
	if (fscanf(file, "%d\n", &max_value) != 1){
		std::cout << "Invalid max value" << std::endl;
	}

	float inv_max_value = 1.0 / (float)max_value;

	// allocate memory

	m_pixels.reserve(m_hres * m_vres);

	// read pixel data

	for (unsigned int y = 0; y < m_vres; y++) {
		for (unsigned int x = 0; x < m_hres; x++) {
			unsigned char red;
			unsigned char green;
			unsigned char blue;

			if (fscanf(file, "%c%c%c", &red, &green, &blue) != 3) {
				std::cout << "Invalid image" << std::endl;
			}

			float r = red   * inv_max_value;
			float g = green * inv_max_value;
			float b = blue  * inv_max_value;

			m_pixels.push_back(RGBColor(r, g, b));
		}
	}

	// close file

	fclose(file);

	std::cout << "finished reading PPM file" << std::endl;
}

const RGBColor& Image::GetColor(int x, int y) const
{
	if (x < 0 || x >= m_hres ||
		y < 0 || y >= m_vres) {
		return RED;
	}
	return m_pixels[m_hres * (m_vres - y - 1) + x];
}

}