
#include "Builder.h"

#include "dataset/Actor.h"
#include "dataset/Layer.h"
#include "dataset/Scene.h"
#include "dataset/Game.h"
#include "view/Context.h"
#include "view/LibraryPanel.h"
#include "view/SceneItem.h"
#include "behaviors/ChangeImage.h"
#include "behaviors/ChangeScene.h"
#include "behaviors/ScaleSize.h"
#include "behaviors/MouseButton.h"
#include "behaviors/Move.h"
#include "behaviors/Rotate.h"

namespace ebuilder
{
namespace cc
{
	Builder::Builder(wxNotebook* notebook)
		: CodePageBuilder(notebook)
	{
	}

	void Builder::loadPages()
	{
		buildMainPage();
		buildAppDelegatePage();

		std::vector<SceneItem*> scenes;
		Context::Instance()->library->getSceneList()->traverse(
			ee::FetchAllVisitor<SceneItem>(scenes));

		for (size_t i = 0, n = scenes.size(); i < n; ++i)
			insertPage(*scenes[i]);
	}

	void Builder::updatePage(const SceneItem& scene)
	{
		std::string name = scene.getName();
		if (!name.empty()) 
			name[0] = toupper(name[0]);

		int index = getSceneIndex(scene);

		Page* hPage = static_cast<Page*>(m_notebook->GetPage(PAGE_OFFSET + index * 2));
		m_notebook->SetPageText(PAGE_OFFSET + index * 2, name + wxT(".h"));
		Page* cPage = static_cast<Page*>(m_notebook->GetPage(PAGE_OFFSET + index * 2 + 1));
		m_notebook->SetPageText(PAGE_OFFSET + index * 2 + 1, name + wxT(".cpp"));

		buildScenePages(scene, hPage, cPage);
	}

	void Builder::insertPage(const SceneItem& scene)
	{
		std::string name = scene.getName();
		if (!name.empty()) 
			name[0] = toupper(name[0]);

		Page* hPage = new Page(m_notebook, name + wxT(".h"));
		Page* cPage = new Page(m_notebook, name + wxT(".cpp"));

		buildScenePages(scene, hPage, cPage);

		addPage(hPage);
		addPage(cPage);
	}

	void Builder::removePage(const SceneItem& scene)
	{
		int index = PAGE_OFFSET + getSceneIndex(scene) * 2;
		m_notebook->DeletePage(index);
		m_notebook->DeletePage(index);
	}

	void Builder::buildMainPage()
	{
		Page* hPage = new Page(m_notebook, wxT("main.h"));
		Page* cPage = new Page(m_notebook, wxT("main.cpp"));

		CodeGenerator hGen, cGen;
		{
			cpp::HeaderFile header(hGen, "", "main");

			hGen.line("#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers");
			hGen.line();
			hGen.line("// Windows Header Files:");
			hGen.line("#include <windows.h>");
			hGen.line("#include <tchar.h>");
			hGen.line();
			hGen.line("// C RunTime Header Files");
			hGen.line("#include \"CCStdC.h\"");
		}

		cGen.line("#include \"main.h\"");
		cGen.line("#include \"../Classes/AppDelegate.h\"");
		cGen.line("#include \"CCEGLView.h\"");
		cGen.line();
		cGen.line("USING_NS_CC;");
		cGen.line();
		{
			// int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
			cpp::FuncImpl func(cGen, "int APIENTRY", "_tWinMain", 
				4, "HINSTANCE hInstance", "HINSTANCE hPrevInstance", "LPTSTR lpCmdLine", "int nCmdShow");

			// UNREFERENCED_PARAMETER(hPrevInstance);
			cpp::call(cGen, "", cpp::e_null, "UNREFERENCED_PARAMETER", 1, "hPrevInstance");
			// UNREFERENCED_PARAMETER(lpCmdLine);
			cpp::call(cGen, "", cpp::e_null, "UNREFERENCED_PARAMETER", 1, "lpCmdLine");
			cGen.line();
			// AppDelegate app;
			cpp::define(cGen, "AppDelegate", "app");
			// CCEGLView* eglView = CCEGLView::sharedOpenGLView();
			cpp::define(cGen, "CCEGLView*", "eglView", "CCEGLView::sharedOpenGLView()");
			std::string title = "\""+Game::NAME.ToStdString()+"\"";
			// eglView->setViewName("title");
			cpp::call(cGen, "eglView", cpp::e_pointer, "setViewName", 1, title.c_str());
			// eglView->setFrameSize(800, 480);
			cpp::call(cGen, "eglView", cpp::e_pointer, "setFrameSize", 2, "800", "480");
			cGen.line();
			// return CCApplication::sharedApplication()->run();
			cGen.line("return "+cpp::call("CCApplication::sharedApplication()", cpp::e_pointer, "run", 0)+";");
		}

		hPage->SetReadOnly(false);
		cPage->SetReadOnly(false);

		hPage->SetText(hGen.toText());
		cPage->SetText(cGen.toText());

		hPage->SetReadOnly(true);
		cPage->SetReadOnly(true);

		addPage(hPage);
		addPage(cPage);
	}

	void Builder::buildAppDelegatePage()
	{
		Page* hPage = new Page(m_notebook, wxT("AppDelegate.h"));
		Page* cPage = new Page(m_notebook, wxT("AppDelegate.cpp"));

		CodeGenerator hGen, cGen;
		{
			cpp::HeaderFile header(hGen, "app", "delegate");
			hGen.line("#include \"cocos2d.h\"");
			hGen.line();
			// class AppDelegate : private cocos2d::CCApplication
			cpp::ClassDef cd(hGen, "AppDelegate", "private cocos2d::CCApplication");
			{
				cpp::ClassDef::AccessControl ac(hGen, cpp::ClassDef::e_public);
				// AppDelegate();
				cpp::funcDef(hGen, "", "AppDelegate", 0);
				// virtual ~AppDelegate();
				cpp::funcDef(hGen, "virtual", "~AppDelegate", 0);
				hGen.line();
				// virtual bool applicationDidFinishLaunching();
				cpp::funcDef(hGen, "virtual bool", "applicationDidFinishLaunching", 0);
				// virtual void applicationDidEnterBackground();
				cpp::funcDef(hGen, "virtual void", "applicationDidEnterBackground", 0);
				// virtual void applicationWillEnterForeground();
				cpp::funcDef(hGen, "virtual void", "applicationWillEnterForeground", 0);
			}
		}

		// for scene_name
		std::vector<SceneItem*> scenes;
		Context::Instance()->library->getSceneList()->traverse(
			ee::FetchAllVisitor<SceneItem>(scenes));

		cGen.line("#include \"AppDelegate.h\"");
		for (size_t i = 0, n = scenes.size(); i < n; ++i)
		{
			std::string scene_name;
			scene_name = scenes[i]->getName();
			if (!scene_name.empty()) 
				scene_name[0] = toupper(scene_name[0]);
			cGen.line("#include \""+scene_name+".h\"");
		}
		cGen.line();
		cGen.line("USING_NS_CC;");
		cGen.line();
		{
			// AppDelegate::AppDelegate()
			cpp::FuncImpl func(cGen, "", "AppDelegate::AppDelegate", 0);
		}
		cGen.line();
		{
			// AppDelegate::~AppDelegate()
			cpp::FuncImpl func(cGen, "", "AppDelegate::~AppDelegate", 0);
		}
		cGen.line();
		{
			// bool AppDelegate::applicationDidFinishLaunching()
			cpp::FuncImpl func(cGen, "bool", "AppDelegate::applicationDidFinishLaunching", 0);

			// CCDirector* pDirector = CCDirector::sharedDirector();
			cpp::define(cGen, "CCDirector*", "pDirector", "CCDirector::sharedDirector()");
			// pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
			cpp::call(cGen, "pDirector", cpp::e_pointer, "setOpenGLView", 
				1, "CCEGLView::sharedOpenGLView()");
			cGen.line();
			// pDirector->setAnimationInterval(1.0 / 60);
			cpp::call(cGen, "pDirector", cpp::e_pointer, "setAnimationInterval", 1, "1.0 / 60");
			cGen.line();
			// CCScene* pScene = CCScene::create();
			cpp::define(cGen, "CCScene*", "pScene", "CCScene::create()");
			// CCLayer* pLayer1 = mygame::Scene0().create();
			std::string game_name = Game::NAME.Lower();
			for (size_t i = 0, n = scenes.size(); i < n; ++i)
			{
				std::string name = "pLayer" + wxString::FromDouble(i);
				std::string scene_name = scenes[i]->getName();
				if (!scene_name.empty()) 
					scene_name[0] = toupper(scene_name[0]);
				std::string new_scene = cpp::call(game_name+"::"+scene_name+"()", cpp::e_reference, "create", 0);
				cpp::define(cGen, "CCLayer*", name, new_scene);
			}
			// CCLayerMultiplex* layer = CCLayerMultiplex::create(pLayer1, pLayer2, NULL);
			std::string new_layer = "CCLayerMultiplex::create(";
			for (size_t i = 0, n = scenes.size(); i < n; ++i)
			{
				std::string name = "pLayer" + wxString::FromDouble(i);
				new_layer += name+", ";
			}
			new_layer += "NULL)";
			cpp::define(cGen, "CCLayerMultiplex*", "layer", new_layer);
			// pScene->addChild(layer, 0);
			cpp::call(cGen, "pScene", cpp::e_pointer, "addChild", 2, "layer", "0");
			cGen.line();
			// pDirector->runWithScene(pScene);
			cpp::call(cGen, "pDirector", cpp::e_pointer, "runWithScene", 1, "pScene");
			cGen.line();
			cGen.line("return true;");
		}
		cGen.line();
		{
			// void AppDelegate::applicationDidEnterBackground()
			cpp::FuncImpl func(cGen, "void", "AppDelegate::applicationDidEnterBackground", 0);
			// CCDirector::sharedDirector()->stopAnimation();
			cpp::call(cGen, "CCDirector::sharedDirector()", cpp::e_pointer, "stopAnimation", 0);
		}
		cGen.line();
		{
			// void AppDelegate::applicationWillEnterForeground()
			cpp::FuncImpl func(cGen, "void", "AppDelegate::applicationWillEnterForeground", 0);
			// CCDirector::sharedDirector()->startAnimation();
			cpp::call(cGen, "CCDirector::sharedDirector()", cpp::e_pointer, "startAnimation", 0);
		}

		hPage->SetReadOnly(false);
		cPage->SetReadOnly(false);

		hPage->SetText(hGen.toText());
		cPage->SetText(cGen.toText());

		hPage->SetReadOnly(true);
		cPage->SetReadOnly(true);

		addPage(hPage);
		addPage(cPage);
	}

	void Builder::buildScenePages(const SceneItem& scene, Page* hPage, Page* cPage)
	{
		SceneContent content;

		content.name = scene.getName();
		if (!content.name.empty()) 
			content.name[0] = toupper(content.name[0]);

		buildSceneHPage(scene, content);
		buildSceneCppPage(scene, content);

		hPage->SetReadOnly(false);
		cPage->SetReadOnly(false);

		hPage->SetText(content.hGen->toText());
		cPage->SetText(content.cGen->toText());

		hPage->SetReadOnly(true);
		cPage->SetReadOnly(true);
	}

	void Builder::buildSceneHPage(const SceneItem& scene, SceneContent& content)
	{
		std::string game_name = Game::NAME.Lower();

		CodeGenerator& gen = *content.hGen;
		{
			cpp::HeaderFile header(gen, game_name, content.name);

			gen.line("#include \"cocos2d.h\"");
			gen.line();
			{
				cpp::Namespace ns(gen, game_name);
				{
					cpp::ClassDef cd(gen, content.name, "public cocos2d::CCLayer");
					{
						cpp::ClassDef::AccessControl ac(gen, cpp::ClassDef::e_public);

						// virtual bool init();
						cpp::funcDef(gen, "virtual bool", "init", 0);
						gen.line();
						// virtual void menuCloseCallback(CCObject* pSender);
						cpp::funcDef(gen, "virtual void", "menuCloseCallback", 
							1, "CCObject* pSender");
						// menu call back
						gen.block(*content.hCallbackGen);
						gen.line();
						// CREATE_FUNC(Level1Scene);
						cpp::funcDef(gen, "", "CREATE_FUNC", 1, content.name.c_str());
					}
					gen.line();
					{
						cpp::ClassDef::AccessControl ac(gen, cpp::ClassDef::e_private);
						// buttons
						gen.block(*content.hBtnGen);
						gen.line();
					}
				}
			}
		}
	}

	void Builder::buildSceneCppPage(const SceneItem& scene, SceneContent& content)
	{
		std::string game_name = Game::NAME.Lower();

		CodeGenerator& gen = *content.cGen;
		content.cIncludeGen->line("#include \""+content.name+".h\"");
		gen.block(*content.cIncludeGen);
		gen.line();
		gen.line("USING_NS_CC;");
		gen.line("using namespace "+game_name+";");
		gen.line();
		{
			cpp::FuncImpl func(gen, "bool", content.name+"::init", 0);
			{
				cpp::If _if(gen, "!CCLayer::init()");
				gen.line("return false;");
			}
			gen.line();
			
			const std::vector<Layer*>& layers = scene.getScene()->getAllLayers();
			for (size_t i = 0, n = layers.size(); i < n; ++i)
			{
				const std::vector<Actor*>& actors = layers[i]->getActors();
				for (size_t j = 0, m = actors.size(); j < m; ++j)
				{
					buildActor(*actors[j], content);
					gen.line();
				}
			}

			gen.line("return true;");
		}
		gen.line();
		{
			// void scene_name::menuCloseCallback(CCObject* pSender)
			cpp::FuncImpl func(gen, "void", content.name+"::menuCloseCallback", 1, "CCObject* pSender");
			// CCDirector::sharedDirector()->end();
			cpp::call(gen, "CCDirector::sharedDirector()", cpp::e_pointer, "end", 0);
		}
		gen.line();
		gen.block(*content.cCallbackGen);
	}

	void Builder::buildActor(const Actor& actor, SceneContent& content)
	{
		bool hasMouseButtons = false;
		const std::vector<Behavior*> behaviors = actor.getBehaviors();
		for (size_t i = 0, n = behaviors.size(); i < n; ++i)
		{
			if (behaviors[i]->type() == e_MouseButton)
			{
				hasMouseButtons = true;
				break;
			}
		}
		if (hasMouseButtons)
			buildButton(actor, content);
		else
			buildSprite(actor, content);
	}

	void Builder::buildSprite(const Actor& actor, SceneContent& content)
	{
		CodeGenerator& gen = *content.cGen;

		std::string name = actor.name;

		wxString path = actor.getSymbol().getFilepath();
		std::string filename = "\""+ee::FilenameTools::getFilenameWithExtension(path)+"\"";

		// CCSprite* name = CCSprite::create("filename");
		std::string new_actor = cpp::call("CCSprite::create", 1, filename.c_str());
		cpp::define(gen, "CCSprite*", name, new_actor);

		// name->setPosition(ccp(_x, _y));
		std::string x, y;
		buildPosition(actor.getPosition(), x, y);
		std::string new_pos = cpp::call("ccp", 2, x.c_str(), y.c_str());
		cpp::call(gen, name, cpp::e_pointer, "setPosition", 1, new_pos.c_str());

		bool xMirror, yMirror;
		actor.getMirror(xMirror, yMirror);
		if (xMirror) 
		{
			// name->setFlipX(true);
			cpp::call(gen, name, cpp::e_pointer, "setFlipX", 1, "true");
		}
		if (yMirror) 
		{
			// name->setFlipY(true);
			cpp::call(gen, name, cpp::e_pointer, "setFlipY", 1, "true");
		}

		// behaviors
		const std::vector<Behavior*> behaviors = actor.getBehaviors();
		for (size_t i = 0, n = behaviors.size(); i < n; ++i)
			buildBehavior(name, behaviors[i], *content.cGen, *content.cIncludeGen);

		// this->addChild(name, 0);
		cpp::call(gen, "this", cpp::e_pointer, "addChild", 
			2, name.c_str(), "0");
	}

	void Builder::buildButton(const Actor& actor, SceneContent& content)
	{
		std::string actorName = actor.name;
		if (!actorName.empty()) 
			actorName[0] = toupper(actorName[0]);

		std::string btnName = "btn"+actorName;
		std::string funcName = "menuCallback"+actorName;
		std::string menuName = "menu"+actorName;

		wxString path = actor.getSymbol().getFilepath();
		std::string filename = "\""+ee::FilenameTools::getFilenameWithExtension(path)+"\"";

		// void menuCallback(CCObject* pSender);
		cpp::funcDef(*content.hCallbackGen, "void", funcName, 1, "CCObject* pSender");

		CodeGenerator& gen = *content.cGen;

		// CCMenuItemImage* btn;
		cpp::define(*content.hBtnGen, "cocos2d::CCMenuItemImage*", btnName);

		// menu_selector(Scene::menuCallback)
		std::string cb = cpp::call("menu_selector", 1, (content.name+"::"+funcName).c_str());
		// CCMenuItemImage::create(filename, filename, this, menu_selector(Scene::menuCallback))
		std::string new_ben = cpp::call("CCMenuItemImage::create", 4, filename.c_str(), filename.c_str(), "this", cb.c_str());
		// btn = CCMenuItemImage::create(filename, filename, this, menu_selector(Scene::menuCallback));
		cpp::assign(gen, btnName, new_ben);

		// btn->setPosition(ccp(x, y));
		std::string x, y;
		buildPosition(actor.getPosition(), x, y);
		std::string new_pos = cpp::call("ccp", 2, x.c_str(), y.c_str());
		cpp::call(gen, btnName, cpp::e_pointer, "setPosition", 1, new_pos.c_str());

		// CCMenu* menu = CCMenu::create(btn, NULL);
		std::string new_menu = cpp::call("CCMenu::create", 2, btnName.c_str(), "NULL");
		cpp::define(gen, "CCMenu*", menuName, new_menu);
		// menu->setPosition(CCPointZero);
		cpp::call(gen, menuName, cpp::e_pointer, "setPosition", 1, "CCPointZero");
		// this->addChild(menu, 1);
		cpp::call(gen, "this", cpp::e_pointer, "addChild", 2, menuName.c_str(), "1");

		// behaviors
		{
			// void scene_name::menuCallback(CCObject* pSender)
			cpp::FuncImpl func(*content.cCallbackGen, "void", content.name+"::"+funcName, 1, "CCObject* pSender");
		
			const std::vector<Behavior*> behaviors = actor.getBehaviors();
			std::vector<Behavior*> btnBehaviors;
			for (size_t i = 0, n = behaviors.size(); i < n; ++i)
			{
				if (behaviors[i]->type() == e_MouseButton)
				{
					MouseButton* mouseButton = static_cast<MouseButton*>(behaviors[i]);
					copy(mouseButton->satisfies.begin(), mouseButton->satisfies.end(), back_inserter(btnBehaviors));
				}
			}
			for (size_t i = 0, n = btnBehaviors.size(); i < n; ++i)
				buildBehavior(btnName, btnBehaviors[i], *content.cCallbackGen, *content.cIncludeGen);
		}
	}

	void Builder::buildBehavior(const std::string& name, Behavior* behavior, 
		CodeGenerator& gen, CodeGenerator& genInclude)
	{
		switch (behavior->type())
		{
		case e_ChangeImage:
			{
				ChangeImage* changeImage = static_cast<ChangeImage*>(behavior);
				std::string path = changeImage->selected->getFilepath();
				std::string filename = "\""+ee::FilenameTools::getFilenameWithExtension(path)+"\"";

				// CCTextureCache::sharedTextureCache()->addImage(tex_name)
				std::string sTex = cpp::call("CCTextureCache::sharedTextureCache()->addImage", 1, filename.c_str());
				// name->setTexture(texture);
				cpp::call(gen, name, cpp::e_pointer, "setTexture", 1, sTex.c_str());
			}
			break;
		case e_ChangeScene:
			{
				std::vector<SceneItem*> scenes;
				Context::Instance()->library->getSceneList()->traverse(
					ee::FetchAllVisitor<SceneItem>(scenes));

				ChangeScene* changeScene = static_cast<ChangeScene*>(behavior);
				int index = -1;
				for (size_t i = 0, n = scenes.size(); i < n; ++i)
				{
					if (scenes[i] == changeScene->selected)
					{
						index = i;
						break;
					}
				}
				assert(index != -1);
				std::string _index = wxString::FromDouble(index);

				std::string line = "#include \""+changeScene->selected->getName()+".h\"";
				if (!genInclude.existLine(line))
					genInclude.line(line);

				// ((CCLayerMultiplex*)m_pParent)->switchTo(index);
				cpp::call(gen, "((CCLayerMultiplex*)m_pParent)", cpp::e_pointer, "switchTo", 1, _index.c_str());
			}
			break;
		case e_ScaleSize:
			{
				ScaleSize* scaleSize = static_cast<ScaleSize*>(behavior);
				std::string scale = wxString::FromDouble(scaleSize->val, 1)+"f";

				// name->setScale(scale);
				cpp::call(gen, name, cpp::e_pointer, "setScale", 1, scale.c_str());
			}
			break;
		case e_Move:
			{
				Move* move = static_cast<Move*>(behavior);

				std::string ex, ey;
				buildPosition(move->end, ex, ey);

				const float duration = ee::Math::getDistance(move->start, move->end) / move->speed;
				std::string _duration = wxString::FromDouble(duration, 1) + "f";

				// ccp(x, y)
				std::string pos = cpp::call("ccp", 2, ex.c_str(), ey.c_str());
				// CCMoveTo::create(duration, pos)
				std::string action = cpp::call("CCMoveTo::create", 2, _duration.c_str(), pos.c_str());
				// name->runAction(action);
				cpp::call(gen, name, cpp::e_pointer, "runAction", 1, action.c_str()); 
			}
			break;
		case e_Rotate:
			{
				Rotate* rotate = static_cast<Rotate*>(behavior);

				std::string end = wxString::FromDouble(rotate->end/**180/ee::PI*/, 1) + "f";

				const float duration = fabs(rotate->end - rotate->start) / rotate->speed;
				std::string _duration = wxString::FromDouble(duration, 1) + "f";

				// CCRotateTo::create(duration, end);
				std::string action = cpp::call("CCRotateTo::create", 2, _duration.c_str(), end.c_str());
				// name->runAction(action);
				cpp::call(gen, name, cpp::e_pointer, "runAction", 1, action.c_str()); 
			}
			break;
		}
	}

	void Builder::buildPosition(const ee::Vector& pos, std::string& x, std::string& y)
	{
		const float px = pos.x + Game::WIDTH * 0.5f,
			py = pos.y + Game::HEIGHT * 0.5f;
		x = wxString::FromDouble(px, 1) + "f";
		y = wxString::FromDouble(py, 1) + "f";
	}
}
}
