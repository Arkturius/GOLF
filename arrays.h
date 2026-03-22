/**
* arrays.h
*/

#ifndef _ARRAYS_H
# define _ARRAYS_H

# include <string.h>
# include <stdlib.h>

# define	ARRAYS_START_CAP	256

# define	array_type(_type, _name)										\
	typedef struct	_name ## _array											\
	{																		\
		_type	*arr;														\
		u32		len;														\
		u32		cap;														\
	}																		\
	_name

# define	array__arr(_a)			((_a)->arr)
# define	array__len(_a)			((_a)->len)
# define	array__cap(_a)			((_a)->cap)
# define	array_first(_a)			array__arr(_a)
# define	array_last(_a)			(array_first(_a) + array__len(_a) - 1)

# define	array__addr(_a, _index)											\
	({																		\
		const u32	_safe_index = _index;									\
		const u32	_last_index = array__len(_a) - 1;						\
																			\
		array__arr(_a) + ((_safe_index > _last_index)						\
			? _last_index													\
			: _safe_index);													\
	})

# define	array__index(_a, _addr)											\
	({																		\
		_addr - array__arr(_a);												\
	})


# define	array__realloc(_a, _add)										\
	({																		\
		const u32	_elem_size = sizeof(*array__arr(_a));					\
		u32	_new_cap = array__cap(_a) ? array__cap(_a) : ARRAYS_START_CAP;	\
																			\
		while (array__len(_a) + _add >= _new_cap)							\
			_new_cap <<= 1;													\
		array__arr(_a) = realloc											\
		(																	\
			array__arr(_a),													\
			_new_cap * _elem_size											\
		);																	\
		array__cap(_a) = _new_cap;											\
	})

# define	array__swap(_a, _1, _2)											\
	({																		\
		typeof(*array__arr(_a))	_tmp = *(_1);								\
		*(_1) = *(_2);														\
		*(_2) = _tmp;														\
	})

# define	array_push(_a, _element)										\
	({																		\
		if (array__len(_a) + 1 >= array__cap(_a))							\
			array__realloc(_a, 1);											\
		array__arr(_a)[array__len(_a)++] = (_element);						\
		array_last(_a);													\
	})

# define	array_pop(_a)													\
	({																		\
		typeof(*array__arr(_a))	_last = *array_last(_a);					\
		array__len(_a)--;													\
		_last;																\
	})

# define	array_remove(_a, _index)										\
	({																		\
		typeof(array__arr(_a))	_rem = array__addr(_a, _index);				\
		array__swap(_a, array_last(_a), _rem);								\
		array_pop(_a);														\
	})

# define	array_begin(_a)		array_first(_a)
# define	array_end(_a)		(array_last(_a) + 1)
# define	array_rbegin(_a)	array_last(_a)
# define	array_rend(_a)		(array_begin(_a) - 1)

# define	array_foreach(_iterator, _a)									\
	for																		\
	(																		\
		typeof(array__arr(_a)) _iterator = array_begin(_a);					\
		_iterator != array_end(_a);											\
		_iterator++															\
	)

# define	array_foreach_rev(_iterator, _a)								\
	for																		\
	(																		\
		typeof(array__arr(_a))	_iterator = array_rbegin(_a);				\
		_iterator != array_rend(_a)											\
		_iterator--															\
	)

# define	array_free(_a)													\
	({																		\
		free(array__arr(_a));												\
		array__len(_a) = 0;													\
		array__cap(_a) = 0;													\
	})

# define	array_slice(_a, _s, _c)											\
	({																		\
		const unsigned int	_safe_start = (_s) > array__len(_a)				\
			? array__len(_a)												\
			: (_s);															\
		const unsigned int	_safe_max   = array__len(_a) - _safe_start;	\
		const unsigned int	_safe_count = (_c) > _safe_max					\
			? _safe_max														\
			: (_c);															\
																			\
		typeof(*_a) _slice =												\
		{																	\
			.arr = array__addr(_a, _safe_start),							\
			.len = _safe_count,												\
			.cap = _safe_count,												\
		};																	\
		_slice;																\
	})

/* String */

array_type(char, String);

# define	string_appendc(_s, _c)	array_push(_s, _c)
# define	string_terminate(_s)	array_push(_s, 0)
# define	string_reset(_s)		array__len(_s) = 0
# define	string_free(_s)			array_free(_s)
# define	string_print(_s)		printf("%.*s", (_s)->len, (_s)->arr);

# define	string_appendf(_s, _fmt, ...)									\
	({																		\
		const i32	_len = snprintf(NULL, 0, _fmt, ##__VA_ARGS__);			\
																			\
		if (array__len(_s) + _len >= array__cap(_s))						\
			array__realloc(_s, _len);										\
		snprintf(array_end(_s), _len + 1, _fmt, ##__VA_ARGS__);				\
		array__len(_s) += _len;												\
	})

# define	string_append(_s, _e)											\
	({																		\
		const u32	_len = strlen(_e);										\
																			\
		if (array__len(_s) + _len >= array__cap(_s))						\
			array__realloc(_s, _len);										\
		memcpy(array_end(_s), _e, _len);									\
		array__len(_s) += _len;												\
	})

typedef struct	StringView
{
	const char	*str;
	u32			len;
}
SView;

# define	sv_len(_sv)		(_sv)->len
# define	sv_str(_sv)		(_sv)->str

# define	sv(_s)			(SView){.str = _s, .len = strlen(_s)}
# define	sv_l(_s, _l)	(SView){.str = _s, .len = _l}

# define	sv_fmt			"%.*s"
# define	sv_arg(_sv)		sv_len(_sv), sv_str(_sv)

# define	sv_advance(_sv)													\
	({																		\
		if (sv_len(_sv) > 0)												\
		{																	\
			sv_str(_sv)++;													\
			sv_len(_sv)--;													\
		}																	\
	})

#endif // _ARRAYS_H
