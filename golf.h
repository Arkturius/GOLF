/**
* golf.h
*/

#include <stdint.h>

#define GOLF_SIGNATURE	"GOLF"

typedef enum GOLF_Compression 
{
	GOLF_COMPRESSION_NONE,
} GOLF_Compression;

typedef struct	GOLF_Header
{
	uint8_t		sign[4]; // GOLF
	uint8_t		version;
	uint8_t		compression;
	uint8_t		header_size; // self size
	uint8_t		checksum;
	uint64_t	file_size;
	uint64_t	obj_off;
	uint64_t	mtl_off;
	uint64_t	str_off;
	uint32_t	obj_count;
	uint32_t	mtl_count;
}
GOLF_Header;

typedef enum	GOLF_FaceLayoutMask
{
	GOLF_FACE_LAYOUT_VERTEX  = 1 << 0,
	GOLF_FACE_LAYOUT_NORMAL  = 1 << 1,
	GOLF_FACE_LAYOUT_TEXTURE = 1 << 2,
	GOLF_FACE_LAYOUT_MTL     = 1 << 3,
}
GOLF_FaceLayoutMask;

typedef struct GOLF_FaceHeader
{
	uint64_t layout:4;
	uint64_t offset:60; // can store at most an EXBIBYTE of data
	uint32_t size;
	uint32_t name;
}
GOLF_FaceHeader;

typedef struct	GOLF_ObjHeader
{

}
GOLF_ObjHeader;

typedef struct	GOLF_MtlHeader
{

}
GOLF_MtlHeader;

typedef struct	GOLF_Face
{
	float	v[3];
}
GOLF_Face;
