#include "LayerFactory.h"
#include "Layer.h"
#include "ShapeLayer.h"

#include <assert.h>

namespace lr
{

Layer* LayerFactory::Create(int id, LibraryPanel* library, LayerType type)
{
	if (type == LT_DEFAULT) {
		return new Layer(id, library);
	} else if (type == LT_SHAPE) {
		return new ShapeLayer(id, library);
	} else {
		assert(0);
		return NULL;
	}
}

}