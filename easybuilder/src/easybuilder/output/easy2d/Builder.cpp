
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

using namespace ebuilder;
using namespace ebuilder::e2d;

Builder::Builder(wxNotebook* notebook)
	: CodePageBuilder(notebook)
{
}

void Builder::loadPages()
{
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
	m_notebook->SetPageText(PAGE_OFFSET + index * 2, name + ".h");
	Page* cPage = static_cast<Page*>(m_notebook->GetPage(PAGE_OFFSET + index * 2 + 1));
	m_notebook->SetPageText(PAGE_OFFSET + index * 2 + 1, name + ".cpp");

	build(scene, hPage, cPage);
}

void Builder::insertPage(const SceneItem& scene)
{
	std::string name = scene.getName();
	if (!name.empty()) 
		name[0] = toupper(name[0]);

	Page* hPage = new Page(m_notebook, name + ".h");
	Page* cPage = new Page(m_notebook, name + ".cpp");

	build(scene, hPage, cPage);

	addPage(hPage);
	addPage(cPage);
}

void Builder::removePage(const SceneItem& scene)
{
	int index = PAGE_OFFSET + getSceneIndex(scene) * 2;
	m_notebook->DeletePage(index);
	m_notebook->DeletePage(index);
}

void Builder::build(const SceneItem& scene, Page* hPage, Page* cPage)
{
	SceneContent content;
	
	std::string game_name = Game::NAME.ToStdString();
	StringTools::toLower(game_name);
	std::string scene_name = scene.getName().ToStdString();
	StringTools::toLower(scene_name);
	{
		cpp::HeaderFile header(*content.hGen, game_name, scene_name);

		content.hGen->line("#include <easy2d/easy2d.h>");
		content.hGen->line();
		{
			cpp::Namespace space(*content.hGen, game_name);

			// #include "scene.h"
			content.cIncludeGen->line("#include \""+scene.getName().ToStdString()+".h\"");
			{
				content.cGen->block(*content.cIncludeGen);
				content.cGen->line();
				cpp::Namespace space(*content.cGen, game_name);
				buildClass(scene, content);
			}
		}
	}

	hPage->SetReadOnly(false);
	cPage->SetReadOnly(false);

	hPage->SetText(content.hGen->toText());
	cPage->SetText(content.cGen->toText());

	hPage->SetReadOnly(true);
	cPage->SetReadOnly(true);
}

void Builder::buildClass(const SceneItem& scene, SceneContent& content) const
{
	std::string name = scene.getName();
	if (!name.empty()) 
		name[0] = toupper(name[0]);

	// h
	{
		CodeGenerator& gen = *content.hGen;
		cpp::ClassDef cd(gen, name, "public e2d::Scene");
		{
			cpp::ClassDef::AccessControl ac(gen, cpp::ClassDef::e_public);
			// scene(e2d::Game* game);
			cpp::funcDef(gen, "", name, 1, "e2d::Game* game");
			gen.line();
			// virtual void load();
			cpp::funcDef(gen, "virtual void", "load", 0);
			gen.line();
		}
	}
	CodeGenerator& gen = *content.cGen;
	// cpp
	{
		// scene::scene(e2d::Game* game)
		// 	: e2d::Scene(game)
		cpp::FuncImpl func(gen, "", name+"::"+name, "e2d::Scene(game)", 
			1, "e2d::Game* game");
	}
	gen.line();
	{
		// void scene::load()
		cpp::FuncImpl func(gen, "void", name+"::load", 0);

		std::vector<Actor*> actors;
		const std::vector<Layer*>& layers = scene.getScene()->getAllLayers();
		for (size_t i = 0, n = layers.size(); i < n; ++i)
		{
			const std::vector<Actor*>& tmp = layers[i]->getActors();
			copy(tmp.begin(), tmp.end(), back_inserter(actors));
		}

		if (!actors.empty())
		{
			// e2d::GraphicsService* graphics = e2d::Context::Instance()->m_graphicsService;
			cpp::define(gen, "e2d::GraphicsService*", "graphics", "e2d::Context::Instance()->m_graphicsService");

			std::string scale = cpp::call("graphics->getScale", 0);
			// const float scale = graphics->getScale();
			cpp::define(gen, "const float", "scale", scale);

			std::string width = cpp::call("graphics->getWidth", 0) + " / scale";
			std::string height = cpp::call("graphics->getHeight", 0) + " / scale";
			// const float width = graphics->getWidth() / scale;
			cpp::define(gen, "const float", "width", width);
			// const float height = graphics->getHeight() / scale;
			cpp::define(gen, "const float", "height", height);

			gen.line();
		}
		for (size_t i = 0, n = actors.size(); i < n; ++i)
		{
			if (i != 0) gen.line();
			buildActor(*actors[i], content);
		}
	}
}

void Builder::buildActor(const Actor& actor, SceneContent& content) const
{
	CodeGenerator& gen = *content.cGen;

	std::string name = actor.name;
	std::string filename = "\""+ee::FilenameTools::getFilenameWithExtension(actor.getSymbol().getFilepath())+"\"";

	std::string x, y;
	buildPosition(actor.getPosition(), x, y);
	std::string angle = wxString::FromDouble(actor.getAngle(), 2) + "f";

	bool xMirror, yMirror;
	actor.getMirror(xMirror, yMirror);

	// e2d::GraphicsSprite* actor23 = loadSprite(path);
	cpp::define(gen, "e2d::GraphicsSprite*", name, cpp::call("loadSprite", 1, filename.c_str()));

	// love::Vector(x, y)
	std::string va_pos = cpp::call("love::Vector", 2, x.c_str(), y.c_str());
	// actor->setTransform(love::Vector(x, y), angle);
 	cpp::call(gen, name, cpp::e_pointer, "setTransform", 2, va_pos.c_str(), angle.c_str());

	if (actor.getScale().x != 1)
	{
		std::string scale = wxString::FromDouble(actor.getScale().x, 2) + "f";
		// actor->setScale(scale);
		cpp::call(gen, name, cpp::e_pointer, "setScale", 1, scale.c_str());
	}
	if (actor.m_isBg)
	{
		// actor->setScale(width / e2d::SCREEN_WIDTH, height / e2d::SCREEN_HEIGHT);
		cpp::call(gen, name, cpp::e_pointer, "setScale", 
			2, "width / e2d::SCREEN_WIDTH", "height / e2d::SCREEN_HEIGHT");
	}
	if (xMirror || yMirror)
	{
		const char* xm = xMirror ? "true" : "false";
		const char* ym = yMirror ? "true" : "false";
		// actor->setMirror(xm, ym);
		cpp::call(gen, name, cpp::e_pointer, "setMirror", 2, xm, ym);
	}

	bool isBtn = false;
	const std::vector<Behavior*>& behaviors = actor.getBehaviors();
	for (size_t i = 0, n = behaviors.size(); i < n; ++i)
	{
		if (behaviors[i]->type() == e_MouseButton)
		{
			isBtn = true;
			break;
		}
	}

	if (isBtn)
	{
		std::string btn_name = "btn_" + name;
		// e2d::Button* btn_name = new e2d::Button(name);
		cpp::define(gen, "e2d::Button*", btn_name, cpp::call("new e2d::Button", 1, name.c_str()));
		buildButtonBehaviors(name, btn_name, behaviors, content);
		// m_foregrounds.push_back(btn_name);
		cpp::call(gen, "m_foregrounds", cpp::e_struct, "push_back", 1, btn_name.c_str());
	}
	else
	{
		for (size_t i = 0, n = behaviors.size(); i < n; ++i)
		{
			// addBehavior(behavior);
			cpp::call(gen, "", cpp::e_null, "addBehavior", 
				1, buildBehavior(name, behaviors[i], content).c_str());
		}

		// new e2d::Actor(name)
		std::string new_actor = cpp::call("new e2d::Actor", 1, name.c_str());
		if (actor.m_isBg)
			// m_backgrounds.push_back(new e2d::Actor(name));
			cpp::call(gen, "m_backgrounds", cpp::e_struct, "push_back", 1, new_actor.c_str());
		else
			// m_foregrounds.push_back(new e2d::Actor(name));
			cpp::call(gen, "m_foregrounds", cpp::e_struct, "push_back", 1, new_actor.c_str());
	}
}

void Builder::buildButtonBehaviors(const std::string& sprite, const std::string& btn,
								   const std::vector<Behavior*>& behaviors, SceneContent& content) const
{
	std::vector<Behavior*> buffer;
	std::set<Behavior*> pressedSet;
	for (size_t i = 0, n = behaviors.size(); i < n; ++i)
	{
		if (behaviors[i]->type() == e_MouseButton)
		{
			MouseButton* mouseButton = static_cast<MouseButton*>(behaviors[i]);
			copy(mouseButton->satisfies.begin(), mouseButton->satisfies.end(), back_inserter(buffer));
			if (mouseButton->bPress)
			{
				for (size_t j = 0, m = mouseButton->satisfies.size(); j < m; ++j)
					pressedSet.insert(mouseButton->satisfies[j]);
			}
		}
	}

	std::sort(buffer.begin(), buffer.end(), Behavior::Cmp());
	for (size_t i = 0, n = buffer.size(); i < n; ++i)
	{
		Behavior* behavior = buffer[i];
		std::string str = buildBehavior(sprite, behavior, content);
		if (pressedSet.find(behavior) != pressedSet.end())
		{
			// btn->addBehavior(behavior, true);
			cpp::call(*content.cGen, btn, cpp::e_pointer, "addBehavior", 2, str.c_str(), "true");
		}
		else
		{
			// btn->addBehavior(behavior, false);
			cpp::call(*content.cGen, btn, cpp::e_pointer, "addBehavior", 2, str.c_str(), "false");
		}
	}
}

std::string Builder::buildBehavior(const std::string& sprite, Behavior* behavior,
								   SceneContent& content) const
{
	std::string str;
	switch (behavior->type())
	{
	case e_ChangeImage:
		{
			ChangeImage* changeImage = static_cast<ChangeImage*>(behavior);
			std::string path = changeImage->selected->getFilepath();
			std::string name = "\""+ee::FilenameTools::getFilenameWithExtension(path)+"\"";
			// new e2d::ChangeImage(sprite, name)
			str = cpp::call("new e2d::ChangeImage", 2, sprite.c_str(), name.c_str());
		}
		break;
	case e_ChangeScene:
		{
			ChangeScene* changeScene = static_cast<ChangeScene*>(behavior);
			std::string name = changeScene->selected->getName();
			if (!name.empty()) name[0] = toupper(name[0]);
			// new name(m_game)
			std::string new_scene = cpp::call("new "+name, 1, "m_game");
			// new e2d::ChangeScene(m_game, new_scene)
			str = cpp::call("new e2d::ChangeScene", 2, "m_game", new_scene.c_str());

			std::string line = "#include \""+name+".h\"";
			if (!content.cIncludeGen->existLine(line))
				content.cIncludeGen->line(line);
		}
		break;
	case e_ScaleSize:
		{
			ScaleSize* scaleSize = static_cast<ScaleSize*>(behavior);
			std::string scale = wxString::FromDouble(scaleSize->val, 1)+"f";
			// new e2d::ScaleSize(sprite, scale)
			str = cpp::call("new e2d::ScaleSize", 2, sprite.c_str(), scale.c_str());
		}
		break;
	case e_Move:
		{
			Move* move = static_cast<Move*>(behavior);

			std::string sx, sy;
			buildPosition(move->start, sx, sy);
			// love::Vector(sx, sy)
			std::string start = cpp::call("love::Vector", 2, sx.c_str(), sy.c_str());

			std::string ex, ey;
			buildPosition(move->end, ex, ey);
			// love::Vector(ex, ey)
			std::string end = cpp::call("love::Vector", 2, ex.c_str(), ey.c_str());

			std::string speed = wxString::FromDouble(move->speed, 2) + "f";

			// new e2d::Move(sprite, start, end, speed)
			str = cpp::call("new e2d::Move", 4, sprite.c_str(), 
				start.c_str(), end.c_str(), speed.c_str());
		}
		break;
	case e_Rotate:
		{
			Rotate* rotate = static_cast<Rotate*>(behavior);

			std::string start = wxString::FromDouble(rotate->start, 2) + "f";
			std::string end = wxString::FromDouble(rotate->end, 2) + "f";
			std::string speed = wxString::FromDouble(rotate->speed, 2) + "f";

			// new e2d::Rotate(sprite, start, end, speed)
			str = cpp::call("new e2d::Rotate", 4, sprite.c_str(), 
				start.c_str(), end.c_str(), speed.c_str());
		}
		break;
	}
	return str;
}

void Builder::buildPosition(const ee::Vector& pos, std::string& x, std::string& y)
{
	const float px = (pos.x + Game::WIDTH * 0.5f) / Game::WIDTH,
		py = (pos.y + Game::HEIGHT * 0.5f) / Game::HEIGHT;
	x = "width*" + wxString::FromDouble(px, 3) + "f";
	y = "height*" + wxString::FromDouble(py, 3) + "f";
}
