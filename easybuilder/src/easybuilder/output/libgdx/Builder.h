
#ifndef EBUILDER_LIBGDX_BUILDER_H
#define EBUILDER_LIBGDX_BUILDER_H


#include <easybuilder.h>

#include "../CodePageBuilder.h"

namespace ebuilder
{
	class Actor;
	class CodeGenerator;
	class Behavior;

namespace libgdx
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
			CodeGenerator *gen, *importGen, *importStaticGen;

			SceneContent() {
				gen = new CodeGenerator;
				importGen = new CodeGenerator;
				importStaticGen = new CodeGenerator;
			}
			~SceneContent() {
				delete gen;
			}
		};

	private:
		void buildGamePage();
		void buildGameDesktopPage();
		void buildGameAndroidPage();

		void buildScenePage(const SceneItem& scene, Page* page);
		void buildActor(const Actor& actor, SceneContent& content);
		void buildBehaviors(const std::string& name, const std::vector<Behavior*>& behaviors, 
			SceneContent& content);
		void buildBehavior(const std::string& name, Behavior* behavior, SceneContent& content,
			std::vector<std::string>& actions);

		static void buildPosition(const Actor& actor, std::string& x, std::string& y);
		static void buildPosition(const ee::Vector& pos, std::string& x, std::string& y);

	private:
		static const int PAGE_OFFSET = 6;	// designer + Screen + Game + GameInstance + GameInstanceDesktop + GameInstanceAndroid

	}; // Builder

} // libgdx
} // ebuilder

#endif // EBUILDER_LIBGDX_BUILDER_H
