#ifdef __cplusplus
extern "C"
{
#endif

#ifndef spatial_index_rtree_cfg_h
#define spatial_index_rtree_cfg_h

//#define SIDX_RTREE_MAX_DEPTH 32

enum RTreeStrategy
{
	LINEAR = 0,
	QUADRATIC,
	RSTAR
};

#define RTREE_STRATEGY LINEAR

#endif // spatial_index_rtree_cfg_h

#ifdef __cplusplus
}
#endif