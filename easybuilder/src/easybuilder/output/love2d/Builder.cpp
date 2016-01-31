
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
namespace love2d
{
	Builder::Builder(wxNotebook* notebook)
		: CodePageBuilder(notebook)
	{
	}

	void Builder::loadPages()
	{
		buildMainPage();

		std::vector<SceneItem*> scenes;
		Context::Instance()->library->getSceneList()->traverse(
			ee::FetchAllVisitor<SceneItem>(scenes));

		for (size_t i = 0, n = scenes.size(); i < n; ++i)
			insertPage(*scenes[i]);
	}

	void Builder::updatePage(const SceneItem& scene)
	{
		const wxString& name = scene.getName();

		int index = getSceneIndex(scene);

		Page* page = static_cast<Page*>(m_notebook->GetPage(PAGE_OFFSET + index));
		m_notebook->SetPageText(PAGE_OFFSET + index, name + wxT(".lua"));

		buildScenePage(scene, page);
	}

	void Builder::insertPage(const SceneItem& scene)
	{
		const wxString& name = scene.getName();
		Page* page = new Page(m_notebook, name + wxT(".lua"));
		buildScenePage(scene, page);
		addPage(page);
	}

	void Builder::removePage(const SceneItem& scene)
	{
		int index = PAGE_OFFSET + getSceneIndex(scene);
		m_notebook->DeletePage(index);
	}

	void Builder::buildMainPage()
	{
		Page* page = new Page(m_notebook, wxT("main.lua"));
		
		CodeGenerator gen;
		{
			// function love.load()
			lua::Function func(gen, "love.load", 0);

			// easing = require("libraries.easing")
			lua::assign(gen, "easing", lua::require("libraries.easing"));
			// require("libraries.middleclass")
			lua::require(gen, "libraries.middleclass");
			// vector = require("libraries.vector")
			lua::assign(gen, "vector", lua::require("libraries.vector"));
			gen.line();

			// require("scene.actor")
			lua::require(gen, "scene.actor");
			// require("scene.button")
			lua::require(gen, "scene.button");
			gen.line();

			// scenes
			std::vector<SceneItem*> scenes;
			Context::Instance()->library->getSceneList()->traverse(
				ee::FetchAllVisitor<SceneItem>(scenes));

			// require("Scene")
			std::string firstScene;
			for (size_t i = 0, n = scenes.size(); i < n; ++i)
			{
				std::string name = scenes[i]->getName();
				if (!name.empty()) 
					name[0] = toupper(name[0]);
				if (i == 0) firstScene = name;
				lua::require(gen, name);
			}
			gen.line();

			// graphics = {}
			lua::assign(gen, "graphics", "{}");
			gen.line();

			// scenes = {}
			{
				lua::TableAssign tScenes(gen, "scenes");
				for (size_t i = 0, n = scenes.size(); i < n; ++i)
				{
					std::string className, name;
					className = name = scenes[i]->getName();
					if (!className.empty()) 
						className[0] = toupper(className[0]);
					// name = scene()
					std::string new_scene = lua::assign(name, lua::call("", className, 0));
					// name = scene(),
					gen.line(new_scene+",");
				}
			}

			// state = scene()
			lua::assign(gen, "state", lua::call("", firstScene, 0));
		}
		gen.line();
		{
			// function love.draw()
			lua::Function func(gen, "love.draw", 0);
			// state:draw()
			lua::call(gen, "state", "draw", 0);
		}
		gen.line();
		{
			// function love.update(dt)
			lua::Function func(gen, "love.update", 1, "dt");
			// state:update(dt)
			lua::call(gen, "state", "update", 1, "dt");
		}
		gen.line();
		{
			// function love.mousepressed(x, y, button)
			lua::Function func(gen, "love.mousepressed", 3, "x", "y", "button");
			// state:mousepressed(x,y,button)
			lua::call(gen, "state", "mousepressed", 3, "x", "y", "button");
		}
		gen.line();
		{
			// function love.mousereleased(x, y, button)
			lua::Function func(gen, "love.mousereleased", 3, "x", "y", "button");
			// state:mousereleased(x,y,button)
			lua::call(gen, "state", "mousereleased", 3, "x", "y", "button");
		}
		gen.line();
		{
			// function love.keypressed(key)
			lua::Function func(gen, "love.keypressed", 1, "key");
			// state:keypressed(key)
			lua::call(gen, "state", "keypressed", 1, "key");
		}

		page->SetReadOnly(false);
		page->SetText(gen.toText());
		page->SetReadOnly(true);

		addPage(page);
	}

	void Builder::buildScenePage(const SceneItem& scene, Page* page)
	{
		SceneContent content;

		// name = class("name", Scene)
		content.name = scene.getName();
		if (!content.name.empty()) 
			content.name[0] = toupper(content.name[0]);

		CodeGenerator& gen = *content.gen;
		// require("scene.scene")
		lua::require(gen, "scene.scene");
		gen.line();
		lua::assign(gen, content.name, lua::call("", "class", 2, ("\""+content.name+"\"").c_str(), "Scene"));
		gen.line();
		{
			// function scene:initialize()
			lua::Function func(gen, content.name+":initialize", 0);

			// Scene.initialize(self)
			lua::call(gen, "", "Scene.initialize", 1, "self");
			gen.line();
			gen.block(*content.genRequire);
			gen.line();
			gen.block(*content.genImages);
			gen.line();
	//		lua::TableAssign tActors(*content.genActors, "self.actors");
			gen.block(*content.genActors);
			gen.line();
	//		lua::TableAssign tButtons(*content.genButtons, "self.buttons");
			gen.block(*content.genButtons);
			gen.line();
			const std::vector<Layer*>& layers = scene.getScene()->getAllLayers();
			for (size_t i = 0, n = layers.size(); i < n; ++i)
			{
				const std::vector<Actor*>& actors = layers[i]->getActors();
				for (size_t j = 0, m = actors.size(); j < m; ++j)
					buildActor(*actors[j], content);
			}
		}

		page->SetReadOnly(false);
		page->SetText(gen.toText());
		page->SetReadOnly(true);
	}

	void Builder::buildActor(const Actor& actor, SceneContent& content)
	{
		std::string name = actor.name;
		std::string path = actor.getSymbol().getFilepath();
		std::string image = buildImage(path, content);

		std::string x, y;
		transCoords(actor.getPosition(), x, y);

		std::string angle = wxString::FromDouble(actor.getAngle(), 2);

		std::string sx, sy;
		transScale(actor, sx, sy);

		std::string sw = wxString::FromDouble(actor.getSymbol().getSize().xLength(), 1);
		std::string sh = wxString::FromDouble(actor.getSymbol().getSize().yLength(), 1);

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
			// Button(image, x, y, angle, sx, sy, sw, sh)
			std::string new_btn = lua::call("", "Button", 8, image.c_str(), x.c_str(), y.c_str(), 
				angle.c_str(), sx.c_str(), sy.c_str(), sw.c_str(), sh.c_str());
			// local btn = Button(image, x, y, angle, sx, sy, sw, sh)
			lua::assign(*content.genButtons, "local", name, new_btn);
			// table.insert(self.buttons, Button(image, x, y, angle, sx, sy, sw, sh))
			lua::call(*content.genButtons, "", "table.insert", 2, "self.buttons", name.c_str());

			for (size_t i = 0, n = behaviors.size(); i < n; ++i)
			{
				if (behaviors[i]->type() == e_MouseButton)
				{
					MouseButton* mouseButton = static_cast<MouseButton*>(behaviors[i]);
					std::string func = mouseButton->bPress ? "addPressAction" : "addReleaseAction";
					for (size_t j = 0, m = mouseButton->satisfies.size(); j < m; ++j)
						buildBehavior(name, func, mouseButton->satisfies[j], content);
				}
				else
				{
					buildBehavior(name, "addAction", behaviors[i], content);
				}
			}
		}
		else
		{
			// Actor(image, x, y, angle, sx, sy, sw, sh)
			std::string new_actor = lua::call("", "Actor", 8, image.c_str(), x.c_str(), y.c_str(), 
				angle.c_str(), sx.c_str(), sy.c_str(), sw.c_str(), sh.c_str());
			// local actor = Actor(image, x, y, angle, sx, sy, sw, sh)
			lua::assign(*content.genActors, "local", name, new_actor);
			// table.insert(self.actors, Actor(image, x, y, angle, sx, sy, sw, sh))
			lua::call(*content.genActors, "", "table.insert", 2, "self.actors", name.c_str());

			// behaviors
			for (size_t i = 0, n = behaviors.size(); i < n; ++i)
				buildBehavior(name, "addAction", behaviors[i], content);
		}
	}

	void Builder::buildBehavior(const std::string& name, const std::string& actionFunc,
		Behavior* behavior, SceneContent& content)
	{
		CodeGenerator& gen = *content.gen;
		switch (behavior->type())
		{
		case e_ChangeImage:
			{
				// require("scene.changeimage")
				std::string require = lua::require("scene.changeimage");
				if (!content.genRequire->existLine(require))
					content.genRequire->line(require);

				ChangeImage* changeImage = static_cast<ChangeImage*>(behavior);
				std::string path = changeImage->selected->getFilepath();

				// image
				std::string image = buildImage(path, content);
				// ChangeImage(image)
				std::string new_action = lua::call("", "ChangeImage", 1, image.c_str());
				// name:addAction(ChangeImage(image))
				lua::call(gen, name, actionFunc, 1, new_action.c_str());
			}
			break;
		case e_ChangeScene:
			{
				// require("scene.changescene")
				std::string require = lua::require("scene.changescene");
				if (!content.genRequire->existLine(require))
					content.genRequire->line(require);

				ChangeScene* changeScene = static_cast<ChangeScene*>(behavior);
				std::string scene = changeScene->selected->getName();
				scene = "\""+scene+"\"";

				// ChangeScene("scene")
				std::string new_action = lua::call("", "ChangeScene", 1, scene.c_str());
				// name:addAction(ChangeScene("scene"))
				lua::call(gen, name, actionFunc, 1, new_action.c_str());
			}
			break;
		case e_ScaleSize:
			{
				// require("scene.scalesize")
				std::string require = lua::require("scene.scalesize");
				if (!content.genRequire->existLine(require))
					content.genRequire->line(require);

				ScaleSize* scaleSize = static_cast<ScaleSize*>(behavior);
				std::string scale = wxString::FromDouble(scaleSize->val, 1);

				// ScaleSize(scale)
				std::string new_action = lua::call("", "ScaleSize", 1, scale.c_str());
				// name:addAction(ScaleSize(scene))
				lua::call(gen, name, actionFunc, 1, new_action.c_str());
			}
			break;
		case e_Move:
			{
				// require("scene.moveto")
				std::string require = lua::require("scene.moveto");
				if (!content.genRequire->existLine(require))
					content.genRequire->line(require);

				Move* move = static_cast<Move*>(behavior);

				std::string sx, sy;
				transCoords(move->start, sx, sy);

				std::string ex, ey;
				transCoords(move->end, ex, ey);

				const float duration = ee::Math::getDistance(move->start, move->end) / move->speed;
				std::string _duration = wxString::FromDouble(duration, 1);

				// vector(sx, sy)
				std::string new_begin = lua::call("", "vector", 2, sx.c_str(), sy.c_str());
				// vector(ex, ey)
				std::string new_end = lua::call("", "vector", 2, ex.c_str(), ey.c_str());
				// MoveTo(beginning, ending, during)
				std::string new_action = lua::call("", "MoveTo", 3, new_begin.c_str(), new_end.c_str(), _duration.c_str());
				// name:addAction(MoveTo(beginning, ending, during))
				lua::call(gen, name, actionFunc, 1, new_action.c_str());
			}
			break;
		case e_Rotate:
			{
				// require("scene.rotateto")
				std::string require = lua::require("scene.rotateto");
				if (!content.genRequire->existLine(require))
					content.genRequire->line(require);

				Rotate* rotate = static_cast<Rotate*>(behavior);

				std::string start = wxString::FromDouble(rotate->start*180/ee::PI, 1);
				std::string end = wxString::FromDouble(rotate->end*180/ee::PI, 1);

				const float duration = fabs(rotate->end - rotate->start) / rotate->speed;
				std::string _duration = wxString::FromDouble(duration, 1);

				// RotateTo(beginning, ending, during)
				std::string new_action = lua::call("", "RotateTo", 3, start.c_str(), end.c_str(), _duration.c_str());
				// name:addAction(RotateTo(beginning, ending, during))
				lua::call(gen, name, actionFunc, 1, new_action.c_str());
			}
			break;
		}
	}

	std::string Builder::buildImage(const std::string& filepath, SceneContent& content)
	{
		std::string filename = "\"media/"+ee::FilenameTools::getRelativePath(Game::DLG_PATH, filepath)+"\"";

		// loadImage(filename)
		std::string loadImg = lua::call("", "loadImage", 1, filename.c_str());
		if (!content.genImages->existLine(loadImg))
			content.genImages->line(loadImg);

		// graphics[filename]
		std::string image = "graphics["+filename+"]";

		return image;
	}

	void Builder::transCoords(const ee::Vector& pos, std::string& x, std::string& y)
	{
		float px = pos.x + Game::WIDTH * 0.5f;
		float py = -pos.y + Game::HEIGHT * 0.5f;
		x = wxString::FromDouble(px, 1);
		y = wxString::FromDouble(py, 1);
	}

	void Builder::transScale(const Actor& actor, std::string& sx, std::string& sy)
	{
		float s = actor.getScale().x;
		bool xMirror, yMirror;
		actor.getMirror(xMirror, yMirror);

		sx = wxString::FromDouble(xMirror ? -s : s, 1);
		sy = wxString::FromDouble(yMirror ? -s : s, 1);
	}

} // love2d
} // ebuilder
