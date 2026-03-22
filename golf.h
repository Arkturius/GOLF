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

# define	GOLF_SIGNATURE	"GOLF"
# define	GOLF_VERSION	0

# if __has_attribute(nonstring)
#  define	__NONSTRING	__attribute__((nonstring))
# else
#  define	__NONSTRING
# endif

typedef enum	GOLF_FaceLayoutMask		GOLF_FaceLayoutMask;
typedef enum	GOLF_Compression		GOLF_Compression;
typedef enum	GOLF_MaterialMapType	GOLF_MaterialMapType;
typedef struct	GOLF_Data				GOLF_Data;
typedef struct	GOLF_Header				GOLF_Header;
typedef struct	GOLF_Location			GOLF_Location;
typedef struct	GOLF_ObjHeader			GOLF_ObjHeader;
typedef struct	GOLF_MtlHeader			GOLF_MtlHeader;
typedef struct	GOLF_FaceHeader			GOLF_FaceHeader;
typedef			f32						GOLF_v3f[3];
typedef			u32						GOLF_FaceComponent[3];
typedef			GOLF_FaceComponent		GOLF_Face[];
typedef struct	GOLF_Object				GOLF_Object;
typedef struct	GOLF_MaterialMap		GOLF_MaterialMap;
typedef struct	GOLF_Material			GOLF_Material;

enum	GOLF_Compression 
{
	GOLF_COMPRESSION_NONE,
};

enum	GOLF_FaceLayoutMask
{
	GOLF_FACE_LAYOUT_VERTEX  = 1 << 0,
	GOLF_FACE_LAYOUT_NORMAL  = 1 << 1,
	GOLF_FACE_LAYOUT_TEXTURE = 1 << 2,
	GOLF_FACE_LAYOUT_MTL     = 1 << 3,
};

struct GOLF_Data
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
	u8			magic[4] __NONSTRING;
	/* Size of this header in bytes */
	u8			header_size;
	u8			version;
	u8			compression;
	u8			checksum;
	GOLF_Data	data;
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

struct GOLF_Location
{
	u64	offset;
	u64	size;
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

struct GOLF_MaterialMap
{
	u32	offset;
	u32	filename;
};

typedef enum	GOLF_MaterialMapType
{
	GOLF_MAP_KA,
	GOLF_MAP_KD,
	GOLF_MAP_KS,
	GOLF_MAP_NS,
	GOLF_MAP_D,
}
GOLF_MaterialMapType;

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

#endif
