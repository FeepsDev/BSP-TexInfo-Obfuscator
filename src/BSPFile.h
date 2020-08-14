/*
	Created by Feeps on 24/07/2020.

	Some structures of this file come from Source SDK Source Code
	https://github.com/ValveSoftware/source-sdk-2013/blob/master/sp/src/public/bspfile.h
*/

#define HEADER_LUMPS 64
#define LUMP_BRUSHSIDES_INDEX 19
#define LUMP_TEXINFO_INDEX 6
#define LUMP_TEXDATA_INDEX 2
#define LUMP_TEXDATASTRINGDATA_INDEX 43

#include <vector>
#include <string>
#include <fstream>

// Valve structs
struct lump_t{
	int	fileofs;	// offset into file (bytes)
	int	filelen;	// length of lump (bytes)
	int	version;	// lump format version
	char fourCC[4];	// lump ident code
};

struct dheader_t{
	int	ident;                  // BSP file identifier
	int	version;                // BSP file version
	lump_t lumps[HEADER_LUMPS]; // lump directory array
	int	mapRevision;            // the map's revision (iteration, version) number
};

// Lump specific structs
struct texinfo_t{
	float textureVecs[2][4];	// [s/t][xyz offset]
	float lightmapVecs[2][4];	// [s/t][xyz offset] - length is in units of texels/area
	int	flags;			        // miptex flags	overrides
	int	texdata;                // Pointer to texture name, size, etc.
};

struct Vector{
	float x;
	float y;
	float z;
};

struct dtexdata_t{
	Vector reflectivity;		// RGB reflectivity
	int	nameStringTableID;	// index into TexdataStringTable
	int	width, height;		// source image
	int	view_width, view_height;
};

struct dbrushside_t{
	unsigned short planenum;	// facing out of the leaf
	short texinfo;	            // texture info
	short dispinfo;         	// displacement info
	short bevel;		        // is the side a bevel plane?
};


// Class used to parse a Source BSP file from Valve 
class BSPFile{
	public:
		// Getters (Don't give a reference)
		const std::vector<texinfo_t>& getTexInfo() const{
			return texInfo;
		};

		const std::vector<dtexdata_t>& getTexData() const{
			return texData;
		};

		const std::vector<std::string>& getTexDataStringData() const{
			return texDataStringData;
		};
		
		const std::vector<dbrushside_t>& getBrushSide() const{
			return brushSide;
		};

		const dheader_t& getHeader() const{
			return header;
		};

		void load(std::ifstream& infile);
	private:
		template<typename type>
		void load_basic_lump(std::ifstream& infile, uint8_t index, std::vector<type>& out);

		dheader_t header;
		std::vector<texinfo_t> texInfo;
		std::vector<dtexdata_t> texData;
		std::vector<std::string> texDataStringData;
		std::vector<dbrushside_t> brushSide;
};