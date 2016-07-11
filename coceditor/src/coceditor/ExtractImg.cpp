#include "ExtractImg.h"


// #include <SOIL/SOIL.h>
// #include <SOIL/stb_image_write.h>
namespace coceditor
{
	void ExtractImg::trigger(const std::string& path)
	{
// 		std::string dir = ee::FilenameTools::getFileDir(path);
// 		std::string fil = ee::FilenameTools::getFilename(path);
// 	
// // 		// debug
// // 		std::string src = dir+"\\"+wxString::FromDouble(1)+".png";
// // 		int sw, sh, sc;
// // 		unsigned char* src_data = SOIL_load_image(src.c_str(), &sw, &sh, &sc, 0);
// // 		// 
// 
// 		struct Item
// 		{
// 			std::string name;
// 			int pos[8];
// 		};
// 		std::vector<Item> list[4];
// 
//		std::locale::global(std::locale(""));
// 		std::ifstream fin(path.c_str());
//		std::locale::global(std::locale("C"));		
// 		std::string name;
// 		while (fin >> name)
// 		{
// 			Item item;
// 			item.name = name;
// 
// 			int index;
// 			fin >> index;
// 			for (size_t i = 0; i < 8; ++i)
// 				fin >> item.pos[i];
// 
// 			list[index].push_back(item);
// 
// // 			std::string src = dir+"\\"+wxString::FromDouble(index+1)+".png",
// // //				dst = dir+"\\"+fil+"\\"+name+".png";
// // 				dst = dir+"\\ouutput\\"+name+".png";
// // 
// // 			create(src, pos, dst);
// 		}
// 
// 		fin.close();
// 
// 		for (size_t i = 0; i < 4; ++i)
// 		{
// 			if (list[i].empty()) continue;
// 
// 			std::string src = dir+"\\"+wxString::FromDouble(i+1)+".png";
// 			int sw, sh, sc;
// 			unsigned char* src_data = SOIL_load_image(src.c_str(), &sw, &sh, &sc, 0);
// 			for (size_t j = 0, n = list[i].size(); j < n; ++j)
// 			{
// 				const Item& item = list[i][j];
// 
// 				std::string dst = dir+"\\output\\"+item.name+".png";
// 
// 				int left = INT_MAX, top = INT_MAX;
// 				for (int i = 0; i < 8; i += 2)
// 					if (item.pos[i] < left)
// 						left = item.pos[i];
// 				for (int i = 1; i < 8; i += 2)
// 					if (item.pos[i] < top)
// 						top = item.pos[i];
// 
// 				int width, height;
// 				if (item.pos[0] != item.pos[2])
// 					width = abs(item.pos[0] - item.pos[2]);
// 				else
// 					width = abs(item.pos[2] - item.pos[4]);
// 				if (item.pos[1] != item.pos[3])
// 					height = abs(item.pos[1] - item.pos[3]);
// 				else
// 					height = abs(item.pos[3] - item.pos[5]);
// 
// 				unsigned char* dst_data = (unsigned char*) malloc(width * height * sc);
// 				int ptr = 0;
// 				for (size_t i = 0; i < height; ++i)
// 					for (size_t j = 0; j < width; ++j)
// 					{
// 						const int baseFrom = (sw * (top+i) + (left+j))*sc;
// 						for (size_t i = 0; i < sc; ++i)
// 							dst_data[ptr++] = src_data[baseFrom+i];
// 					}
// 
// 				stbi_write_png(dst.c_str(), width, height, sc, dst_data, 0);
// 
// 				free((void*)dst_data);
// 			}
// 		}
	}

	void ExtractImg::create(const std::string& src, int pos[], 
		const std::string& dst)
	{
// 		int left = INT_MAX, top = INT_MAX;
// 		for (int i = 0; i < 8; i += 2)
// 			if (pos[i] < left)
// 				left = pos[i];
// 		for (int i = 1; i < 8; i += 2)
// 			if (pos[i] < top)
// 				top = pos[i];
// 
// 		int width, height;
// 		if (pos[0] != pos[2])
// 			width = abs(pos[0] - pos[2]);
// 		else
// 			width = abs(pos[2] - pos[4]);
// 		if (pos[1] != pos[3])
// 			height = abs(pos[1] - pos[3]);
// 		else
// 			height = abs(pos[3] - pos[5]);
// 
// 		int sw, sh, sc;
// 		unsigned char* src_data = SOIL_load_image(src.c_str(), &sw, &sh, &sc, 0);
// 		unsigned char* dst_data = (unsigned char*) malloc(width * height * sc);
// 		int ptr = 0;
// 		for (size_t i = 0; i < height; ++i)
// 			for (size_t j = 0; j < width; ++j)
// 			{
// 				const int baseFrom = (sw * (top+i) + (left+j))*sc;
// 				for (size_t i = 0; i < sc; ++i)
// 					dst_data[ptr++] = src_data[baseFrom+i];
// 			}
// 
// 		stbi_write_png(dst.c_str(), width, height, sc, dst_data, 0);
// 
// 		free((void*)dst_data);
	}
}