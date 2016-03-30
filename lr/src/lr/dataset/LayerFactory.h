#ifndef _LR_LAYER_FACTORY_H_
#define _LR_LAYER_FACTORY_H_

#include "layer_type.h"

#include <ee/CameraModes.h>

namespace lr
{

class Layer;
class LibraryPanel;

class LayerFactory
{
public:
	static Layer* Create(int id, LibraryPanel* library, LayerType type, ee::CameraMode cam);

}; // LayerFactory

}

#endif // _LR_LAYER_FACTORY_H_