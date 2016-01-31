#include <iostream>


#include <SOIL/SOIL.h>
#include <SOIL/stb_image_write.h>

int main(int argc, char *argv[])
{
	if (argc < 4) {
		std::cerr << "error: Need Src1 and Src2 and output filepath!" << std::endl;
		return 1;
	}

	//////////////////////////////////////////////////////////////////////////

	const char *src1path = argv[1],
		*src2path = argv[2];

	int channels = 4;
	
	int s1w, s1h, format1;
	unsigned char* src1_data = ee::ImageLoader::loadData(src1path, s1w, s1h, format1);

	int s2w, s2h, format2;
	unsigned char* src2_data = ee::ImageLoader::loadData(src2path, s2w, s2h, format2);

	assert(s1w == s2w && s1h == s2h && format1 == format2);

	unsigned char* dst_data = (unsigned char*) malloc(s1w * s1h * channels);
	memset(dst_data, 0, s1w * s1h * channels);

	for (size_t i = 0; i < s1h; ++i)
		for (size_t j = 0; j < s1w; ++j)
		{
 			const int index = (s1w * i + j) * channels;

// 			bool equal = true;
// 			for (size_t k = 0; k < s1c; ++k)
// 				if (src1_data[index+k] != src2_data[index+k])
// 					equal = false;

			if (memcmp(src1_data+index, src2_data+index, channels) == 0)
			{
		//		memset(dst_data+index, 0, s1c);
// 				dst_data[index] = 0;
// 				dst_data[index+1] = 0;
// 				dst_data[index+2] = 0;
// 				dst_data[index+3] = 255;
// 				if (src1_data[index] != 255 || src1_data[index+1] != 255 || src1_data[index+2] != 255)
// 					int zz = 0;
			}
			else
			{
// 				if (equal)
// 					int zz = 0;
				memcpy(dst_data+index, src1_data+index, channels);
			}

// 			if (!equal)
// 			{
// 				for (size_t k = 0; k < s1c; ++k)
// 					dst_data[index+k] = src1_data[index+k];
// 			}

//   			for (size_t k = 0; k < s1c; ++k)
//   			{
//   				int s1 = src1_data[index+k], s2 = src2_data[index+k];
//   				if (s1 != s2)
// 				{
//   					dst_data[index+k] = s1;
// 					assert(!equal);
// 				}
//   				else
// 				{
//   					dst_data[index+k] = 0;
// 
// 					if (k != 3)
// 						assert(equal);
// 				}
//   
//   // 				if (k = s1c - 1)
//   // 					dst_data[index+k] = src2_data[index+k] - src1_data[index+k];
//   // 				else
//   // 					dst_data[index+k] = 0;
//   			}
		}

	stbi_write_png(argv[3], s1w, s1h, channels, dst_data, 0);

	free((void*)dst_data);
}