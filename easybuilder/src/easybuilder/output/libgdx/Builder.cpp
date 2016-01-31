
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
namespace libgdx
{
	Builder::Builder(wxNotebook* notebook)
		: CodePageBuilder(notebook)
	{
	}

	void Builder::loadPages()
	{
		buildGamePage();
		buildGameDesktopPage();
		buildGameAndroidPage();

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

		Page* page = static_cast<Page*>(m_notebook->GetPage(PAGE_OFFSET + index));
		m_notebook->SetPageText(PAGE_OFFSET + index, name + ".java");

		buildScenePage(scene, page);
	}

	void Builder::insertPage(const SceneItem& scene)
	{
		std::string name = scene.getName();
		if (!name.empty()) 
			name[0] = toupper(name[0]);

		Page* page = new Page(m_notebook, name + ".java");
		buildScenePage(scene, page);
		addPage(page);
	}

	void Builder::removePage(const SceneItem& scene)
	{
		int index = PAGE_OFFSET + getSceneIndex(scene);
		m_notebook->DeletePage(index);
	}

	void Builder::buildGamePage()
	{
		std::string name = Game::NAME;
		if (!name.empty()) 
			name[0] = toupper(name[0]);

		Page* page = new Page(m_notebook, name+".java");

		CodeGenerator gen;

		gen.line("package com.mygame."+Game::NAME.Lower().ToStdString()+";");
		gen.line();
		gen.line("import com.badlogic.gdx.Game;");
		gen.line("import com.badlogic.gdx.Screen;");
		gen.line();
		{
			java::ClassDef cd(gen, "public", name+" extends Game");
			gen.line();
			std::vector<SceneItem*> scenes;
			Context::Instance()->library->getSceneList()->traverse(
				ee::FetchAllVisitor<SceneItem>(scenes));
			if (!scenes.empty())
			{
				for (size_t i = 0, n = scenes.size(); i < n ;++i)
				{
					std::string cName, vName;
					cName = vName = scenes[i]->getName();
					if (!name.empty()) 
					{
						cName[0] = toupper(cName[0]);
						vName[0] = tolower(vName[0]);
					}
					java::declare(gen, cName, vName);
				}
				gen.line();
			}
			{
				// public Screen getStartScreen()
				java::FuncImpl func(gen, java::e_public, "void", "create", true, 0);
				// setScreen(new MainMenuScreen(this));
				std::string name;
				if (scenes.empty())
					name = "MainMenuScreen";
				else
				{
					name = scenes[0]->getName();
					if (!name.empty()) 
						name[0] = toupper(name[0]);
				}
				std::string new_scene = java::create(name, 1, "this");
				java::call(gen, "", "setScreen", 1, new_scene.c_str());
			}
			gen.line();
			{
				// public void dispose()
				java::FuncImpl func(gen, java::e_public, "void", "dispose", true, 0);
				// super.dispose();
				java::call(gen, "super", "dispose", 0);
				// getScreen().dispose();
				java::call(gen, "getScreen()", "dispose", 0);
			}
		}

		page->SetReadOnly(false);
		page->SetText(gen.toText());
		page->SetReadOnly(true);

		addPage(page);
	}

	void Builder::buildGameDesktopPage()
	{
		std::string name = Game::NAME;
		if (!name.empty()) 
			name[0] = toupper(name[0]);

		Page* page = new Page(m_notebook, name+"Desktop.java");

		CodeGenerator gen;

		gen.line("package com.mygame."+Game::NAME.Lower().ToStdString()+";");
		gen.line();
		gen.line("import com.badlogic.gdx.backends.lwjgl.LwjglApplication;");
		gen.line("import com.badlogic.gdx.backends.lwjgl.LwjglApplicationConfiguration;");
		gen.line();
		{
			java::ClassDef cd(gen, "public", name+"Desktop");
			{
				// public static void main(String[] argv)
				java::FuncImpl func(gen, java::e_public, "static void", "main", false, 1, "String[] argv");
				// LwjglApplicationConfiguration cfg = new LwjglApplicationConfiguration();
				java::assign(gen, "LwjglApplicationConfiguration cfg", "new LwjglApplicationConfiguration()");
				// cfg.useGL20 = true;
				java::assign(gen, "cfg.useGL20", "true");
				// cfg.width = 800;
				java::assign(gen, "cfg.width", "800");
				// cfg.height = 480;
				java::assign(gen, "cfg.height", "480");
				// new LwjglApplication(new Game(), cfg);
				java::call(gen, "", "new LwjglApplication", 
					2, java::create(name, 0).c_str(), "cfg");
			}
		}

		page->SetReadOnly(false);
		page->SetText(gen.toText());
		page->SetReadOnly(true);

		addPage(page);
	}

	void Builder::buildGameAndroidPage()
	{
		std::string name = Game::NAME;
		if (!name.empty()) 
			name[0] = toupper(name[0]);

		Page* page = new Page(m_notebook, name+"Android.java");

		CodeGenerator gen;

		gen.line("package com.mygame."+Game::NAME.Lower().ToStdString()+";");
		gen.line();
		gen.line("import android.content.pm.ActivityInfo;");
		gen.line("import android.os.Bundle;");
		gen.line();
		gen.line("import com.badlogic.gdx.backends.android.AndroidApplication;");
		gen.line("import com.badlogic.gdx.backends.android.AndroidApplicationConfiguration;");
		gen.line("");
		{
			java::ClassDef cd(gen, "public", "TestEasybuilderAndroid extends AndroidApplication");
			{
				// public void onCreate(Bundle savedInstanceState)
				java::FuncImpl func(gen, java::e_public, "void", "onCreate", true, 1, "Bundle savedInstanceState");	
				// super.onCreate(savedInstanceState);
				java::call(gen, "super", "onCreate", 1, "savedInstanceState");
				// setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
				java::call(gen, "", "setRequestedOrientation", 1, "ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE");
				// AndroidApplicationConfiguration config = new AndroidApplicationConfiguration();
				java::assign(gen, "AndroidApplicationConfiguration config", "new AndroidApplicationConfiguration()");
				// config.useWakelock = true;
				java::assign(gen, "config.useWakelock", "true");
				// config.useGL20 = true;
				java::assign(gen, "config.useGL20", "true");
				// initialize(new Game(), config);
				java::call(gen, "", "initialize", 
					2, java::create(name, 0).c_str(), "config");
			}
		}

		page->SetReadOnly(false);
		page->SetText(gen.toText());
		page->SetReadOnly(true);

		addPage(page);
	}

	void Builder::buildScenePage(const SceneItem& scene, Page* page)
	{
		SceneContent content;

		CodeGenerator& gen = *content.gen;

		std::string name = scene.getName();
		if (!name.empty()) 
			name[0] = toupper(name[0]);
		{
			CodeGenerator& importGen = *content.importGen;
			gen.block(importGen);
			importGen.line("package com.mygame."+Game::NAME.Lower().ToStdString()+";");
			importGen.line();
			importGen.line("import com.badlogic.gdx.Gdx;");
			importGen.line("import com.badlogic.gdx.Screen;");
			importGen.line("import com.badlogic.gdx.graphics.GL10;");
			importGen.line("import com.badlogic.gdx.graphics.OrthographicCamera;");
			importGen.line("import com.badlogic.gdx.graphics.Texture;");
			importGen.line("import com.badlogic.gdx.graphics.g2d.TextureRegion;");
			importGen.line("import com.badlogic.gdx.scenes.scene2d.Stage;");
			importGen.line("import com.badlogic.gdx.scenes.scene2d.ui.Image;");

			gen.block(*content.importStaticGen);
		}
		{
			java::ClassDef cd(gen, "public", name+" implements Screen");

			gen.line();
			// Game game;
			std::string gameName = Game::NAME;
			if (!gameName.empty()) gameName[0] = toupper(gameName[0]);
			java::declare(gen, gameName, "game");
			// Stage stage;
			java::declare(gen, "Stage", "stage");
			// OrthographicCamera guiCam;
			java::declare(gen, "OrthographicCamera", "guiCam");
			gen.line();
			
			std::vector<Actor*> actors;
			const std::vector<Layer*>& layers = scene.getScene()->getAllLayers();
			for (size_t i = 0, n = layers.size(); i < n; ++i)
			{
				const std::vector<Actor*>& tmp = layers[i]->getActors();
				copy(tmp.begin(), tmp.end(), back_inserter(actors));
			}
			
			std::set<std::pair<std::string, std::string> > images;
			for (size_t i = 0, n = actors.size(); i < n; ++i)
			{
				wxString path = actors[i]->getSymbol().getFilepath();
				wxString name = ee::FilenameTools::getFilename(path);
				wxString filename = ee::FilenameTools::getFilenameWithExtension(path);
				images.insert(std::make_pair(name.ToStdString(), filename.ToStdString()));
			}
			std::set<std::pair<std::string, std::string> >::iterator itr;
			for (itr = images.begin(); itr != images.end(); ++itr)
				java::declare(gen, "Texture", itr->first);
			gen.line();
			for (itr = images.begin(); itr != images.end(); ++itr)
				java::declare(gen, "TextureRegion", "r_"+itr->first);
			gen.line();
			for (size_t i = 0, n = actors.size(); i < n; ++i)
				java::declare(gen, "Image", actors[i]->name);
			gen.line();
			{
				// public scene(final Game game)
				std::string params = "final "+gameName+" game";
				java::FuncImpl func(gen, java::e_public, "", name, false, 1, params.c_str());
				// this.game = game;
				java::assign(gen, "this.game", "game");
				gen.line();

				if (!images.empty())
				{
					// stage = new Stage(800, 480, true);
					java::assign(gen, "stage", java::create("Stage", 3, "800", "480", "true"));
					// guiCam = new OrthographicCamera(800, 480);
					java::assign(gen, "guiCam", java::create("OrthographicCamera", 2, "800", "480"));
					// guiCam.position.set(800 / 2, 480 / 2, 0);
					java::call(gen, "guiCam.position", "set", 3, "800 / 2", "480 / 2", "0");
					// stage.setCamera(guiCam);
					java::call(gen, "stage", "setCamera", 1, "guiCam");
					gen.line();

					std::set<std::pair<std::string, std::string> >::iterator itr = images.begin();
					for ( ; itr != images.end(); ++itr)
					{
						// Gdx.files.internal("data/name")
						std::string sTexPath = java::call("Gdx.files.internal", 1, ("\"data/"+(*itr).second+"\"").c_str());
						// new Texture(Gdx.files.internal("data/name"))
						std::string sTex = java::create("Texture", 1, sTexPath.c_str());
						// texture = new Texture(Gdx.files.internal("data/name"));
						java::assign(gen, (*itr).first, sTex);
						// new TextureRegion(texture)
						std::string sTexReg = java::create("TextureRegion", 1, (*itr).first.c_str());
						// name = new TextureRegion(texture);
						java::assign(gen, "r_"+(*itr).first, sTexReg);
					}

					gen.line();
					for (size_t i = 0, n = actors.size(); i < n; ++i)
					{
						buildActor(*actors[i], content);
						if (i != n - 1) gen.line();
					}
				}
			}
			gen.line();
			{
				// public void update(float deltaTime)
				java::FuncImpl func(gen, java::e_public, "void", "update", false, 1, "float deltaTime");
				// if (Gdx.input.justTouched())
				{
					java::If _if(gen, "Gdx.input.justTouched()");
				}
			}
			gen.line();
			{
				// public void draw(float deltaTime)
				java::FuncImpl func(gen, java::e_public, "void", "draw", false, 1, "float deltaTime");
				// Gdx.gl.glClear(GL10.GL_COLOR_BUFFER_BIT);
				java::call(gen, "Gdx.gl", "glClear", 1, "GL10.GL_COLOR_BUFFER_BIT");
				gen.line();
				// stage.act(Math.min(Gdx.graphics.getDeltaTime(), 1 / 30f));
				java::call(gen, "stage", "act", 1, "Math.min(Gdx.graphics.getDeltaTime(), 1 / 30f)");
				// stage.draw();
				java::call(gen, "stage", "draw", 0);
			}
			gen.line();
			{
				// public void render(float delta)
				java::FuncImpl func(gen, java::e_public, "void", "render", true, 1, "float delta");
				// update(delta);
				java::call(gen, "", "update", 1, "delta");
				// draw(delta);
				java::call(gen, "", "draw", 1, "delta");
			}
			gen.line();
			{
				// public void resize(int width, int height)
				java::FuncImpl func(gen, java::e_public, "void", "resize", true, 2, "int width", "int height");
			}
			gen.line();
			{
				// public void show()
				java::FuncImpl func(gen, java::e_public, "void", "show", true, 0);
				// Gdx.input.setInputProcessor(stage);
				java::call(gen, "Gdx.input", "setInputProcessor", 1, "stage");
			}
			gen.line();
			{
				// public void hide()
				java::FuncImpl func(gen, java::e_public, "void", "hide", true, 0);
				// Gdx.input.setInputProcessor(stage);
				java::call(gen, "Gdx.input", "setInputProcessor", 1, "stage");
			}
			gen.line();
			{
				// public void pause()
				java::FuncImpl func(gen, java::e_public, "void", "pause", true, 0);

				// dispose();
				java::call(gen, "", "dispose", 0);
			}
			gen.line();
			{
				// public void resume()
				java::FuncImpl func(gen, java::e_public, "void", "resume", true, 0);
			}
			gen.line();
			{
				// public void dispose()
				java::FuncImpl func(gen, java::e_public, "void", "dispose", true, 0);
//  				// stage.dispose();
//  				java::call(gen, "stage", "dispose", 0);
				
				if (!images.empty())
				{
//					gen.line();
					std::set<std::pair<std::string, std::string> >::iterator itr;
					for (itr = images.begin(); itr != images.end(); ++itr)
					{
						// texture.dispose();
						java::call(gen, itr->first, "dispose", 0);
					}
				}
			}
		}

		content.importGen->line();
		content.importStaticGen->line();

		page->SetReadOnly(false);
		page->SetText(gen.toText());
		page->SetReadOnly(true);
	}

	void Builder::buildActor(const Actor& actor, SceneContent& content)
	{
		CodeGenerator& gen = *content.gen;

		std::string path = actor.getSymbol().getFilepath();
		std::string name = actor.name;
		std::string tex_name = "r_"+ee::FilenameTools::getFilename(path);

		std::string _x, _y;
		buildPosition(actor, _x, _y);

		// std::string _w = wxString::FromDouble(width, 1) + "f";
		// std::string _h = wxString::FromDouble(height, 1) + "f";

		std::string _ox = wxString::FromDouble(actor.getSymbol().getSize().xLength()*0.5f, 1) + "f";
		std::string _oy = wxString::FromDouble(actor.getSymbol().getSize().yLength()*0.5f, 1) + "f";

		std::string _angle = wxString::FromDouble(actor.getAngle()*180/ee::PI, 2) + "f";

		float s = actor.getScale().x;
		bool xMirror, yMirror;
		actor.getMirror(xMirror, yMirror);
		std::string _sx = wxString::FromDouble(xMirror ? -s : s, 1) + "f";
		std::string _sy = wxString::FromDouble(yMirror ? -s : s, 1) + "f";

		// actor = new Image(textureRegion);
		java::assign(gen, name, java::create("Image", 1, tex_name.c_str()));
		// actor.setPosition(x, y);
		java::call(gen, name, "setPosition", 2, _x.c_str(), _y.c_str());
		// actor.setRotation(degrees);
		if (_angle != "0.00f")
			java::call(gen, name, "setRotation", 1, _angle.c_str());
		// actor.setOrigin(ox, oy);
		java::call(gen, name, "setOrigin", 2, _ox.c_str(), _oy.c_str());
		// actor.setScale(sx, sy);
		if (_sx != "1.0f" || _sy != "1.0f")
			java::call(gen, name, "setScale", 2, _sx.c_str(), _sy.c_str());
		// behaviors
		buildBehaviors(name, actor.getBehaviors(), content);
		// stage.addActor(actor);
		java::call(gen, "stage", "addActor", 1, name.c_str());
	}

	void Builder::buildBehaviors(const std::string& name, const std::vector<Behavior*>& behaviors, 
		SceneContent& content)
	{
		CodeGenerator& gen = *content.gen;

		// actions 
		std::vector<std::string> actions;
		for (size_t i = 0, n = behaviors.size(); i < n; ++i)
			buildBehavior(name, behaviors[i], content, actions);

		if (!actions.empty())
		{
			std::string line = "import static com.badlogic.gdx.scenes.scene2d.actions.Actions.*;";
			if (!content.importStaticGen->existLine(line)) 
				content.importStaticGen->line(line);
		}

		if (actions.size() == 1)
		{
			java::call(gen, name, "addAction", 1, actions[0].c_str());
		}
		else if (actions.size() > 1)
		{
			std::string str;
			for (size_t j = 0, m = actions.size(); j < m; ++j)
			{
				str += actions[j];
				if (j != m - 1) str += ", ";
			}
			java::call(gen, name, "addAction", 
				1, java::call("parallel", 1, str.c_str()).c_str());
		}

		// mouse buttons
		bool hasMouseButtons = false;
		for (size_t i = 0, n = behaviors.size(); i < n; ++i)
		{
			if (behaviors[i]->type() == e_MouseButton)
			{
				hasMouseButtons = true;
				break;
			}
		}
		if (hasMouseButtons)
		{
			std::string line = "import com.badlogic.gdx.scenes.scene2d.InputEvent;";
			if (!content.importGen->existLine(line)) 
				content.importGen->line(line);
			line = "import com.badlogic.gdx.scenes.scene2d.InputListener;";
			if (!content.importGen->existLine(line)) 
				content.importGen->line(line);

			std::vector<MouseButton*> downList, upList;
			for (size_t i = 0, n = behaviors.size(); i < n; ++i)
			{
				if (behaviors[i]->type() == e_MouseButton)
				{
					MouseButton* mouseButton = static_cast<MouseButton*>(behaviors[i]);
					if (mouseButton->bPress)
						downList.push_back(mouseButton);
					else
						upList.push_back(mouseButton);
				}
			}

			// actor.addListener(new InputListener());
			java::Listener lsn(gen, name, "addListener", "InputListener");
			{
				// public boolean touchDown(InputEvent event, float x, float y, int pointer, int button)
				java::FuncImpl func(gen, java::e_public, "boolean", "touchDown", false,
					5, "InputEvent event", "float x", "float y", "int pointer", "int button");
				if (!downList.empty())
				{
					// behaviors
					std::vector<Behavior*> behaviors;
					for (size_t i = 0, n = downList.size(); i < n; ++i)
						copy(downList[i]->satisfies.begin(), downList[i]->satisfies.end(), back_inserter(behaviors));
					buildBehaviors(name, behaviors, content);
				}
				// return true;
				gen.line("return true;");
			}
			gen.line();
			{
				// public void touchUp(InputEvent event, float x, float y, int pointer, int button)
				java::FuncImpl func(gen, java::e_public, "void", "touchUp", false,
					5, "InputEvent event", "float x", "float y", "int pointer", "int button");
				if (!upList.empty())
				{
					// behaviors
					std::vector<Behavior*> behaviors;
					for (size_t i = 0, n = upList.size(); i < n; ++i)
						copy(upList[i]->satisfies.begin(), upList[i]->satisfies.end(), back_inserter(behaviors));
					buildBehaviors(name, behaviors, content);			
				}
			}
		}
	}

	void Builder::buildBehavior(const std::string& name, Behavior* behavior, SceneContent& content,
		std::vector<std::string>& actions)
	{
		CodeGenerator& gen = *content.gen;

		switch (behavior->type())
		{
		case e_ChangeImage:
			{
				std::string line = "import com.badlogic.gdx.scenes.scene2d.utils.TextureRegionDrawable;";
				if (!content.importGen->existLine(line)) 
					content.importGen->line(line);

				ChangeImage* changeImage = static_cast<ChangeImage*>(behavior);
				std::string path = changeImage->selected->getFilepath();
				std::string tex_region_name = "r_"+ee::FilenameTools::getFilename(path);

				// new TextureRegionDrawable(name)
				std::string sTex = java::create("TextureRegionDrawable", 1, tex_region_name.c_str());
				// actor.setDrawable(new TextureRegionDrawable(name));
				java::call(gen, name, "setDrawable", 1, sTex.c_str());
			}
			break;
		case e_ChangeScene:
			{
				ChangeScene* changeScene = static_cast<ChangeScene*>(behavior);
				std::string scene_name = changeScene->selected->getName().ToStdString();

				std::string scene = scene_name;
				if (!scene.empty()) 
					scene[0] = tolower(scene[0]);
				scene = "game."+scene;
				{
					std::string className = scene_name;
					if (!className.empty()) 
						className[0] = toupper(className[0]);
					// if (screen != null)
					java::If _if(gen, scene+" == null");
					// screen = new Screen;
					java::assign(gen, scene, java::create(className, 1, "game"));
				}
				// game.setScreen(screen);
				java::call(gen, "game", "setScreen", 1, scene.c_str());
			}
			break;
		case e_ScaleSize:
			{
				ScaleSize* scaleSize = static_cast<ScaleSize*>(behavior);
				std::string scale = wxString::FromDouble(scaleSize->val, 1)+"f";

				// actor.scale(scale);
				java::call(gen, name, "setScale", 1, scale.c_str());
			}
			break;
		case e_Move:
			{
				Move* move = static_cast<Move*>(behavior);

				std::string ex, ey;
				buildPosition(move->end, ex, ey);

				const float duration = ee::Math::getDistance(move->start, move->end) / move->speed;
				std::string _duration = wxString::FromDouble(duration, 1) + "f";

				// moveTo(x, y, duration)
				actions.push_back(java::call("moveTo", 3, ex.c_str(), ey.c_str(), _duration.c_str()));
			}
			break;
		case e_Rotate:
			{
				Rotate* rotate = static_cast<Rotate*>(behavior);

				std::string end = wxString::FromDouble(rotate->end*180/ee::PI, 1) + "f";

				const float duration = fabs(rotate->end - rotate->start) / rotate->speed;
				std::string _duration = wxString::FromDouble(duration, 1) + "f";

				// rotateTo(rotation, duration)
				actions.push_back(java::call("rotateTo", 2, end.c_str(), _duration.c_str()));
			}
			break;
		}
	}

	void Builder::buildPosition(const Actor& actor, std::string& x, std::string& y)
	{
		const float px = actor.getPosition().x + Game::WIDTH * 0.5f - actor.getSymbol().getSize().xLength() * 0.5f,
			py = actor.getPosition().y + Game::HEIGHT * 0.5f - actor.getSymbol().getSize().yLength() * 0.5f;
		x = wxString::FromDouble(px, 1) + "f";
		y = wxString::FromDouble(py, 1) + "f";
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
