// todo: code from ps_array

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef motion_trail_array_h
#define motion_trail_array_h

#define MT_ARRAY_INIT(array, n) { \
	for (int i = 0; i < (n) - 1; ++i) { \
		array[i].next = &array[i + 1]; \
	} \
	array[(n) - 1].next = NULL; \
}

#define MT_ARRAY_ALLOC(array, node) { \
	(node) = (array); \
	if (node) { \
		(array) = (node)->next; \
	} \
}

#define MT_ARRAY_FREE(array, node) { \
	(node)->next = (array); \
	(array) = (node); \
}

#endif // motion_trail_array_h

#ifdef __cplusplus
}
#endif