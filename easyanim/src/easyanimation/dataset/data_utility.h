#ifndef _EASYANIM_DATA_UTILITY_H_
#define _EASYANIM_DATA_UTILITY_H_



namespace eanim
{

class Layer;
class KeyFrame;
class SkeletonData;

Layer* get_curr_layer(int& col);

KeyFrame* get_curr_frame();

SkeletonData* get_curr_skeleton();

}

#endif // _EASYANIM_DATA_UTILITY_H_