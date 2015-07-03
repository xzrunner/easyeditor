#ifndef _LIBANIM_LAYERS_LOADER_H_
#define _LIBANIM_LAYERS_LOADER_H_

#include "Symbol.h"

namespace libanim
{

class LayersLoader
{
public:
	void LoadLayers(const Json::Value& value, const std::string& dir,
		std::vector<Symbol::Layer*>& layers) const;

protected:
	virtual std::string GetSymbolPath(const std::string& dir, 
		const Json::Value& json_val) const = 0;

}; // LayersLoader

}

#endif // _LIBANIM_LAYERS_LOADER_H_