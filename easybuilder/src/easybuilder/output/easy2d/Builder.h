
#ifndef EBUILDER_E2D_BUILDER_H
#define EBUILDER_E2D_BUILDER_H


#include <easybuilder.h>

#include "../CodePageBuilder.h"

namespace ebuilder
{
	class SceneItem;
	class Actor;
	class CodeGenerator;
	class Behavior;

namespace e2d
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
			CodeGenerator *hGen;
			CodeGenerator *cGen, *cIncludeGen;

			SceneContent() {
				hGen = new CodeGenerator;
				cGen = new CodeGenerator;
				cIncludeGen = new CodeGenerator;
			}
			~SceneContent() {
				delete hGen;
				delete cGen;
			}
		};

	private:
		void build(const SceneItem& scene, Page* hPage, Page* cPage);
		void buildClass(const SceneItem& scene, SceneContent& content) const;

		void buildActor(const Actor& actor, SceneContent& content) const;
		void buildButtonBehaviors(const std::string& sprite, const std::string& btn,
			const std::vector<Behavior*>& behaviors, SceneContent& content) const;
		std::string buildBehavior(const std::string& sprite, Behavior* behavior,
			SceneContent& content) const;

		static void buildPosition(const ee::Vector& pos, std::string& x, std::string& y);

	private:
		static const int PAGE_OFFSET = 1;	// designer page

	}; // Builder
}
}

#endif // EBUILDER_E2D_BUILDER_H
