#include "CheckForStore.h"
#include "dataset/Layer.h"

#include <ee/Visitor.h>
#include <ee/Shape.h>

#include <wx/msgdlg.h>

namespace lr
{

void CheckForStore::CheckCollisionLayer(Layer* layer)
{
	class CheckTypeVisitor : public ee::RefVisitor<ee::Shape>
	{
	public:
		virtual void Visit(const ee::ShapePtr& shape, bool& next) override
		{
			std::string name = shape->GetName();
			if (name == "block" ||
				name == "swamp" ||
				name == "hblock" ||
				name == "river") {
				next = true;
			} else {
				wxMessageBox(name.c_str(), "err collision type");
				next = false;
			}
		}
	}; // CheckTypeVisitor

	layer->TraverseShape(CheckTypeVisitor());
}

}