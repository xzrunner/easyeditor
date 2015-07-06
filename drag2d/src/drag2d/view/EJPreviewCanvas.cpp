//#include <GL/glew.h>
//
//#include "EJPreviewCanvas.h"
//
//#include "render/Shader.h"
//#include "common/filetools.h"
//
//// for test
//// #include "ejoy2d/lib/sprite.h"
//// #include "ejoy2d/lib/texture.h"
//#include "dataset/ImageSprite.h"
//#include "dataset/Image.h"
//
//#include <easycoco.h>
//
//namespace d2d
//{
//
//BEGIN_EVENT_TABLE(EJPreviewCanvas, OrthoCanvas)
//	EVT_TIMER(TIMER_ID, EJPreviewCanvas::onTimer)
//END_EVENT_TABLE()
//
//EJPreviewCanvas::EJPreviewCanvas(EditPanel* stage, const std::vector<const ISprite*>& sprites)
//	: OrthoCanvas(stage)
//	, m_timer(this, TIMER_ID)
//{
//	const int FPS = 60;
//	m_timer.Start(1000 / FPS);
//
//	prepareSprites(sprites);
//}
//
//void EJPreviewCanvas::initGL()
//{
//	OrthoCanvas::initGL();
////	static_cast<LibraryPanel*>(Context::Instance()->library)->ReloadTexture();
//	Shader::Instance()->Release();
//}
//
//void EJPreviewCanvas::OnDraw()
//{
////	drawStageData();
//}
//
//void EJPreviewCanvas::onTimer(wxTimerEvent& event)
//{
//	Refresh();
//}
//
//void EJPreviewCanvas::prepareSprites(const std::vector<const ISprite*>& sprites)
//{
//// 	if (sprites.empty())
//// 		return;
//// 
//// 	const ImageSprite* sprite = dynamic_cast<const ImageSprite*>(sprites[0]);
//// 	if (!sprite)
//// 		return;
//// 
//// // 	glewInit();
//// // 	shader_init();
//// 
//// 	Image* img = sprite->getSymbol().getImage();
////  	const char * msg = texture_load(0, Texture2DPixelFormat_RGBA8888, 
////  		img->getRegion().xLength(), img->getRegion().yLength(), (void*)img->GetPixelData());
//
//	//////////////////////////////////////////////////////////////////////////
//
//	const char* folder = "3q293t4hweo";
//	MkDirRF(folder);
//
//	libcoco::PackLuaFile pack;
////	pack.pack(sprites, "d:/test/pack");
//	pack.pack(sprites, folder);
//
//	std::string cmd = "ejoy2d.exe ejoy2d/preview/play.lua";
//	WinExec(cmd.c_str(), SW_SHOWMAXIMIZED);
//
////	RemDirRF(folder);
//}
//
//}