/*
	Created by Feeps on 24/07/2020.
*/

#include "BSPFile.h"

void BSPFile::load(std::ifstream& infile){
	infile.seekg(0);
	infile.read(reinterpret_cast<char*>(&header), sizeof(dheader_t));

	load_basic_lump(infile, (uint8_t) LUMP_TEXINFO_INDEX, texInfo); // Load texinfo
	load_basic_lump(infile, (uint8_t) LUMP_TEXDATA_INDEX, texData); // Load texdata

	// Load string data (We don't need TexDataStringTable (Lump 44) to read the data because the strings are null terminated)
	lump_t& lump = header.lumps[LUMP_TEXDATASTRINGDATA_INDEX];
	infile.seekg(lump.fileofs);
	while(infile.tellg() < lump.fileofs + lump.filelen){
		std::string line;
		std::getline(infile, line, '\0');
		texDataStringData.push_back(line);
	}

	load_basic_lump(infile, (uint8_t) LUMP_BRUSHSIDES_INDEX, brushSide); // Load brushsides
}

template<typename type>
void BSPFile::load_basic_lump(std::ifstream& infile, uint8_t index, std::vector<type>& out){
	lump_t& lump = header.lumps[index];
	out.resize(lump.filelen/sizeof(type));

	infile.seekg(lump.fileofs);
	infile.read(reinterpret_cast<char*>(out.data()), lump.filelen);
}