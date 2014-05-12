
#ifndef EBUILDER_CC_BUILDER_H
#define EBUILDER_CC_BUILDER_H

#include <drag2d.h>
#include <easybuilder.h>

#include "../CodePageBuilder.h"

namespace ebuilder
{
	class Actor;
	class CodeGenerator;
	class Behavior;

namespace cc
{
	class Page;

	class Builder : public CodePageBuilder
	{
	public:
		Builder(wxNotebook* notebook);

		virtual void loadPages();

		virtual void updatePage(const SceneItem& scene);

		virtual void insertPage(const SceneItem& scene);

		virtual void removePage(const SceneItem& scene);

	private:
		struct SceneContent
		{
			CodeGenerator *hGen, *hCallbackGen, *hBtnGen;
			CodeGenerator *cGen, *cIncludeGen, *cCallbackGen;

			std::string name;

			SceneContent() {
				hGen = new CodeGenerator;
				hCallbackGen = new CodeGenerator;
				hBtnGen = new CodeGenerator;
				cGen = new CodeGenerator;
				cIncludeGen = new CodeGenerator;
				cCallbackGen = new CodeGenerator;
			}
			~SceneContent()
			{
				delete hGen;
				delete cGen;
			}
		};

	private:
		void buildMainPage();
		void buildAppDelegatePage();

		void buildScenePages(const SceneItem& scene, Page* hPage, Page* cPage);
		void buildSceneHPage(const SceneItem& scene, SceneContent& content);
		void buildSceneCppPage(const SceneItem& scene, SceneContent& content);
		void buildActor(const Actor& actor, SceneContent& content);
		void buildSprite(const Actor& actor, SceneContent& content);
		void buildButton(const Actor& actor, SceneContent& content);
		void buildBehavior(const std::string& name, Behavior* behavior, 
			CodeGenerator& gen, CodeGenerator& genInclude);

		static void buildPosition(const d2d::Vector& pos, std::string& x, std::string& y);

	private:
		static const int PAGE_OFFSET = 5;	// designer page + main + AppDelegate

	}; // Builder

} // cc
} // ebuilder

#endif // EBUILDER_CC_BUILDER_H
