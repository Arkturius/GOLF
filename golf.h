/**
* golf.h
*/

#ifndef _GOLF_H
# define _GOLF_H

# include <stdint.h>

typedef uint64_t	u64;
typedef uint32_t	u32;
typedef uint16_t	u16;
typedef uint8_t		u8;
typedef int64_t		i64;
typedef int32_t		i32;
typedef int16_t		i16;
typedef int8_t		i8;
typedef float		f32;
typedef double		f64;

# include <arrays.h>

# define	GOLF_SIGNATURE	"GOLF"
# define	GOLF_VERSION	0

# if __has_attribute(nonstring)
#  define	__NONSTRING	__attribute__((nonstring))
# else
#  define	__NONSTRING
# endif

typedef struct	GOLF_Location			GOLF_Location;
typedef struct  GOLF_Descriptor			GOLF_Descriptor;
typedef	union	GOLF_v3f				GOLF_v3f;
typedef enum	GOLF_Compression		GOLF_Compression;

typedef struct	GOLF_LibData			GOLF_LibData;
typedef struct	GOLF_Header				GOLF_Header;
typedef struct	GOLF_ObjHeader			GOLF_ObjHeader;
typedef struct	GOLF_MtlHeader			GOLF_MtlHeader;

typedef struct	GOLF_FaceHeader			GOLF_FaceHeader;
typedef struct	GOLF_FaceComponent		GOLF_FaceComponent;
typedef struct	GOLF_Object				GOLF_Object;
typedef enum	GOLF_FaceLayoutMask		GOLF_FaceLayoutMask;

typedef struct	GOLF_Material			GOLF_Material, *GOLF_MaterialP;
typedef enum	GOLF_MaterialMapType	GOLF_MaterialMapType;
typedef enum 	GOLF_MaterialSpecType	GOLF_MaterialSpecType;
typedef			GOLF_Location			GOLF_MaterialMap;

struct GOLF_Location
{
	u64	offset;
	union
	{
		u64	size;
		u32	file;
	}	get;
};

union GOLF_v3f
{
	struct
	{
		f32	x;
		f32	y;
		f32	z;
	};
	f32	v[3];
};

enum	GOLF_Compression 
{
	GOLF_COMPRESSION_NONE,
};

enum	GOLF_FaceLayoutMask
{
	GOLF_FACE_LAYOUT_VERTEX  = 1 << 0,
	GOLF_FACE_LAYOUT_TEXTURE = 1 << 1,
	GOLF_FACE_LAYOUT_NORMAL  = 1 << 2,
	GOLF_FACE_LAYOUT_MTL     = 1 << 3,
};

struct GOLF_LibData
{
	u64		file_size;
	u64		obj_off;
	u64		mtl_off;
	u64		str_off;
	u32		obj_count;
	u32		mtl_count;
};

struct GOLF_Header
{
	/* Magic Number: "GOLF" */
	u8				magic[4] __NONSTRING;
	/* Size of this header in bytes */
	u8				header_size;
	u8				version;
	u8				compression;
	u8				checksum;
	GOLF_LibData	data;
};

struct GOLF_FaceHeader
{
	union
	{
		struct
		{
			u64	layout:4;
			/* Can store at most an EXBIBYTE of data */
			u64 offset:60; 
		};
		u64	packed;
	};
	u32 size;
	u32 name;
};

struct GOLF_FaceComponent
{
	u32	v[3];
};

struct GOLF_ObjHeader
{
	u32				name;
	GOLF_Location	v;
	GOLF_Location	vn;
	GOLF_Location	vt;
};

struct GOLF_MtlHeader
{
	u32				name;
	GOLF_Location	mtl;
};

enum	GOLF_MaterialSpecType
{
	GOLF_SPEC_KA,
	GOLF_SPEC_KD,
	GOLF_SPEC_KS,
	GOLF_SPEC_NS,
	GOLF_SPEC_D,
};

enum	GOLF_MaterialMapType
{
	GOLF_MAP_KA,
	GOLF_MAP_KD,
	GOLF_MAP_KS,
	GOLF_MAP_NS,
	GOLF_MAP_D,
};

struct GOLF_Material
{
	GOLF_v3f			ka;
	GOLF_v3f			kd;
	GOLF_v3f			ks;
	f32					ns;
	f32					d;
	u32					map_count;
	GOLF_MaterialMap	map_array[];
};

# if defined GOLF_BUILDER

array_type(GOLF_v3f,			GOLF_v3fs);
array_type(GOLF_FaceComponent,	GOLF_FaceComponents);
array_type(GOLF_Material,       GOLF_Materials);
array_type(GOLF_MaterialMap,    GOLF_MaterialMaps);
array_type(GOLF_Descriptor,		GOLF_Descriptors);
array_type(GOLF_ObjHeader,		GOLF_ObjHeaders);
array_type(GOLF_MtlHeader,		GOLF_MtlHeaders);
array_type(GOLF_FaceHeader,		GOLF_FaceHeaders);

struct GOLF_Descriptor
{
	u32	start;
	u32	count;
};

typedef struct GOLF_Builder	GOLF_Builder, *GOLF_BuilderP;

struct GOLF_Builder
{
	GOLF_FaceHeaders	face_headers;
	GOLF_FaceComponents	face_components;
	GOLF_Descriptors	face_descriptors;
	GOLF_v3fs			vertices;
	GOLF_v3fs			normals;
	GOLF_v3fs			textures;
	GOLF_ObjHeaders		obj_headers;
	GOLF_MtlHeaders		mtl_headers;
	GOLF_Descriptors	mtl_descriptors;
	GOLF_Materials		materials;
	String				strtab;
};

array_type(GOLF_Builder, GOLF_Builders);

static GOLF_Builders golf_builders = {0};

static inline u32
golf_strtab_add(String *strtab, const char *s)
{
	u32	len = array__len(strtab);

	if (!len)
	{
		string_terminate(strtab);
		len = 1;
	}
	string_append(strtab, s);
	string_terminate(strtab);
	return len;
}

typedef enum GOLF_VertexType
{
	GOLF_VERTEX,
	GOLF_TEXTURE,
	GOLF_NORMAL,
}
GOLF_VertexType;

GOLF_BuilderP
golf_builder_new(void)
{
	return array_push(&golf_builders, (GOLF_Builder){0});
}

void
golf_add_vertex
(
	GOLF_BuilderP	builder,
	GOLF_VertexType	type,
	f32 *			f
)
{
	GOLF_v3fs	*array;
	GOLF_v3f	v;

	switch (type)
	{
		case GOLF_TEXTURE:
		{
			v     = ((GOLF_v3f){.x = f[0], .y = f[1], .z = 0});
			array = &builder->textures;
			break ;
		}
		case GOLF_VERTEX:
		{
			array = &builder->normals;
			v = ((GOLF_v3f){.x = f[0], .y = f[1], .z = f[2]});
			break ;
		}
		case GOLF_NORMAL:
		{
			array = &builder->normals;
			v = ((GOLF_v3f){.x = f[0], .y = f[1], .z = f[2]});
		}
	}
	array_push(array, v);
}

void
golf_add_face
(
	GOLF_BuilderP		builder,
	GOLF_FaceLayoutMask mask,
	i32 *				indices
)
{
	u32	n = 3;

	if (!(mask & GOLF_FACE_LAYOUT_VERTEX))
		return ;
	if (mask & GOLF_FACE_LAYOUT_TEXTURE)
		n += 3;
	if (mask & GOLF_FACE_LAYOUT_NORMAL)
		n += 3;

	GOLF_FaceComponent	comp[3] = {0};

	for (u32 i = 0; i < n; ++i)
		comp[i / 3].v[i % 3] = indices[i];
	for (u32 i = 0; i < n / 3; ++i)
		array_push(&builder->face_components, comp[i]);
}

GOLF_MaterialP
golf_add_material
(
	GOLF_BuilderP,
	u32 map_count
);

void
golf_add_material_spec
(
	GOLF_BuilderP,
	GOLF_MaterialP,
	GOLF_MaterialSpecType,
	f32 *
);

void
golf_add_material_map
(
	GOLF_BuilderP,
	GOLF_MaterialP,
	GOLF_MaterialMapType,
	const char *file
);

#  if defined GOLF_IMPORT
// Import implementation
#  endif

#  if defined GOLF_EXPORT
// Export implementation
#  endif

# endif

#endif
