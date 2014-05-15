#include "TransToGif.h"

#include <drag2d.h>
#include <easyanim.h>
#include <glfw.h>

namespace edb
{

void TransToGif::trigger(const std::string& srcdir, const std::string& dstdir)
{
	glfwInit();
	if(!glfwOpenWindow(800, 600, 8, 8, 8, 8, 24, 8, GLFW_WINDOW))
	{
		glfwTerminate();
		return;
	}
	d2d::Snapshoot ss;

	wxArrayString files;
	d2d::FilenameTools::fetchAllFiles(srcdir, files);
	for (int i = 0, n = files.size(); i < n; ++i)
	{
		wxFileName filename(files[i]);
		filename.Normalize();
		wxString filepath = filename.GetFullPath();
		if (d2d::FileNameParser::isType(filepath, d2d::FileNameParser::e_anim))
		{
			Json::Value value;
			Json::Reader reader;
			std::locale::global(std::locale(""));
			std::ifstream fin(filepath.fn_str());
			std::locale::global(std::locale("C"));
			reader.parse(fin, value);
			fin.close();

			std::string name = value["name"].asString();
			if (name.empty()) {
				continue;
			}
			d2d::ISymbol* symbol = d2d::SymbolMgr::Instance()->fetchSymbol(filepath);

			int max_frame = static_cast<anim::Symbol*>(symbol)->getMaxFrameIndex();
			int width = symbol->getSize().xLength();
			int height = symbol->getSize().yLength();
			AnimatedGifSaver saver(width, height);
			for (int i = 0; i < max_frame; ++i)
			{
				unsigned char* rgba = ss.outputAnimToMemory(symbol, i + 1, true);
				unsigned char* rgb = formatRGBA(rgba, width, height);
				saver.AddFrame(rgb, 1 / 30.0f);
				delete[] rgba;
				delete[] rgb;
			}
			std::string filename = dstdir + "//" + name + ".gif";
			saver.Save(filename.c_str());
			
			symbol->release();
		}
	}
}

void TransToGif::test()
{
	AnimatedGifSaver saver(32, 32);

	unsigned char pixels[32 * 32 * 3];
	int ptr = 0;
	for (int i = 0, n = 32 * 32; i < n; ++i) {
		pixels[ptr++] = 0;
		pixels[ptr++] = 255;
		pixels[ptr++] = 0;
	}
	saver.AddFrame(pixels, 3);

	unsigned char pixels2[32 * 32 * 3];	
	int ptr2 = 0;
	for (int i = 0, n = 32 * 32; i < n; ++i) {
		pixels2[ptr2++] = (i % 255);
		pixels2[ptr2++] = (i % 255);
		pixels2[ptr2++] = (i % 255);
	}
	saver.AddFrame(pixels2, 1.5);

	//ptr = 0;
	//for (int i = 0, n = 32 * 32; i < n; ++i) {
	//	pixels[ptr++] = 255;
	//	pixels[ptr++] = 0;
	//	pixels[ptr++] = 0;
	//}
	//saver.AddFrame(pixels, 1.5);
	//ptr = 0;
	//for (int i = 0, n = 32 * 32; i < n; ++i) {
	//	pixels[ptr++] = 0;
	//	pixels[ptr++] = 0;
	//	pixels[ptr++] = 255;
	//}
	//saver.AddFrame(pixels, 1.5);

	saver.Save("e:/test_zz.gif");
}

//void TransToGif::test()
//{
//	// red and white RGB pixels
//#define R 255,0,0
//#define W 255,255,255
//
//
//	// Lets define a few frames for this little demo...
//
//	// ...red and white RGB pixels
//#define R 255,0,0
//#define W 255,255,255
//
//	// ...frames sizes
//	const int  SX=5;
//	const int  SY=7;
//
//	// ...and, the frames themselves
//	// (note: they are defined bottom-to-top (a-la OpenGL) so they appear upside-down).
//
//	Byte frame0[SX*SY*3] ={
//		W,W,W,W,W,
//		W,W,R,W,W,
//		W,R,W,R,W,
//		W,R,W,R,W,
//		W,R,W,R,W,
//		W,W,R,W,W,
//		W,W,W,W,W,
//	};
//
//	Byte frame1[SX*SY*3] ={
//		W,W,W,W,W,
//		W,W,R,W,W,
//		W,W,R,W,W,
//		W,W,R,W,W,
//		W,R,R,W,W,
//		W,W,R,W,W,
//		W,W,W,W,W,
//	};
//
//	Byte frame2[SX*SY*3]={
//		W,W,W,W,W,
//		W,R,R,R,W,
//		W,R,W,W,W,
//		W,W,R,W,W,
//		W,W,W,R,W,
//		W,R,R,W,W,
//		W,W,W,W,W,
//	};
//
//	Byte frame3[SX*SY*3]={
//		W,W,W,W,W,
//		W,R,R,W,W,
//		W,W,W,R,W,
//		W,W,R,W,W,
//		W,W,W,R,W,
//		W,R,R,W,W,
//		W,W,W,W,W,
//	};
//
//	AnimatedGifSaver saver(SX,SY);
//
//	saver.AddFrame(frame0,3.0); // first frame: three secs
//	saver.AddFrame(frame1,1.5); // first frame: one and half secs
//	saver.AddFrame(frame2,1.5); // first frame: one and half secs
//	saver.AddFrame(frame3,1.5); // first frame: one and half secs
//
//	saver.Save("e:/0123.gif");
//}

unsigned char* TransToGif::formatRGBA(const unsigned char* rgba, int width, int height)
{
	int ptr = 0;
	unsigned char* ret = new unsigned char[width * height * 3];
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			int p = ((height - 1 - i) * width + j) * 4;
			ret[ptr++] = rgba[p++];
			ret[ptr++] = rgba[p++];
			ret[ptr++] = rgba[p++];

			//if (j < width && i < height)
			//{
			//	int p = ((height - i) * width + j) * 4;
			//	ret[ptr++] = rgba[p++];
			//	ret[ptr++] = rgba[p++];
			//	ret[ptr++] = rgba[p++];
			//}
			//else
			//{
			//	ret[ptr++] = 255;
			//	ret[ptr++] = 255;
			//	ret[ptr++] = 255;
			//}
		}
	}
	return ret;
}

}