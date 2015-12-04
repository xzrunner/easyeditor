#ifndef _EASYANIM_KEY_DOWN_HANDLER_H_
#define _EASYANIM_KEY_DOWN_HANDLER_H_

#include <drag2d.h>

namespace eanim
{

class LayersMgr;

class KeyDownHandler
{
public:
	KeyDownHandler(LayersMgr* layers);

	void Process(int key_code);

private:
	void SetPrevKeyFrame();
	void SetNextKeyFrame();

private:
	LayersMgr* m_layers;

}; // KeyDownHandler

}

#endif // _EASYANIM_KEY_DOWN_HANDLER_H_