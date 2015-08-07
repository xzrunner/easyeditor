#include "LayerFactory.h"
#include "Layer.h"
#include "ShapeLayer.h"

namespace lr
{

Layer* LayerFactory::Create(LibraryPanel* library, LayerType type)
{
	if (type == LT_DEFAULT) {
		return new Layer(library);
	} else if (type == LT_SHAPE) {
		return new ShapeLayer(library);
	} else {
		assert(0);
		return NULL;
	}
}

}