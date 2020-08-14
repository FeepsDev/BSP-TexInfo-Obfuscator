/*
	Created by Feeps on 24/07/2020.
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <unordered_set>
#include <map>
#include "BSPFile.h"
#include "SurfaceFlags.h"

void obfuscateFlags(BSPFile& bsp, std::ofstream& out);
void obfuscateNoDraw(BSPFile& bsp, std::ofstream& out);

int main(int argc, char **argv) {
	if (argc <= 2){
		std::cout << "Incorrect usage: You must specify an obfuscation method and the location of a BSP file !" << std::endl;
		std::cout << "Usage: " << std::endl;
		std::cout << '\t' << argv[0] << " -flags <file>" << std::endl;
		std::cout << '\t' << argv[0] << " -nodraw <file>" << std::endl;
		std::cout << std::endl << "Open the program in a console if you have not done so." << std::endl;
		getchar(); // Don't close the console directly
		return 0;
	}

	const char* method = argv[1];
	if(strcmp(method, "-flags")  != 0 && strcmp(method, "-nodraw") != 0){
		std::cerr << "You specified an invalid method !" << std::endl;
		return 0;
	}

	const std::string fileName = argv[2];
	std::ifstream file(fileName, std::ios::binary);

	if (file.is_open()){
		// Read the BSP file
		BSPFile bsp;
		try{
			bsp.load(file);
		}catch(std::exception& e){
			std::cerr << "Failed to read the file, is this a valid BSP file ?" << std::endl;
			std::cerr << '\t' << e.what() << std::endl;
			return 0;
		}

		std::cout << "File read, starting obfuscation... (" << fileName << ")" << std::endl;

		// Copy the file
		std::ofstream out(fileName + ".obfuscated", std::ios::binary);
		if (!out.is_open()){
			std::cerr << "Failed to create a new file" << std::endl;
			return 0;
		}
		file.seekg(0);
		out << file.rdbuf();

		// Obfuscate
		if (strcmp(method, "-flags") == 0)
			obfuscateFlags(bsp, out);
		else
			obfuscateNoDraw(bsp, out);

		std::cout << "Done !" << std::endl;
	}else{
		std::cerr << "Failed to open the file ! (" << fileName << ")" << std::endl;
	}

	return 0;
}

void obfuscateFlags(BSPFile& bsp, std::ofstream& out){
	const std::vector<dbrushside_t>& brushSides = bsp.getBrushSide();
	const std::vector<texinfo_t>& texInfos = bsp.getTexInfo();

	// Used to get the texture name
	const std::vector<std::string>& texStringData = bsp.getTexDataStringData();   
	const std::vector<dtexdata_t>& texdatas = bsp.getTexData();

	// Get all different surface flags
	std::unordered_set<int> flags;
	for(const dbrushside_t& side : brushSides)
		flags.insert(texInfos[side.texinfo].flags);

	// Ask the user which texinfo he want per flag and store the choice on repTex
	std::map<int, int> repTex;
	for(const int& flag : flags){
		// Print current flags
		std::cout << std::endl << "Current flag: ";
		const std::unordered_set<sf::Flags>& enumFlags = sf::getFlagsByIndex(flag);
		if (enumFlags.size() > 0)
			for (const sf::Flags& f : enumFlags)
				std::cout << f << ", ";
		else
			std::cout << "No Flags";

		std::cout << std::endl;

		// Get all different texture with the corresponding flags
		std::map<int, int> aTex;
		std::vector<int> index; // Just a list to show to the user an incremential list (and not nameStringTableID)
		for (unsigned int i = 0; i < texInfos.size(); i++){
			const texinfo_t& tex = texInfos[i];
			const dtexdata_t& texdata = texdatas[tex.texdata];
			const int& strTableId = texdata.nameStringTableID;

			if(tex.flags == flag && aTex.count(strTableId) == 0){
				aTex[strTableId] = i;
				index.push_back(strTableId);
				std::cout << index.size() - 1 << ": " << texStringData[strTableId] << std::endl;
			}            
		}

		// Give the user a choice
		unsigned int sel = 0;
		do{
			std::cout << "Choose a valid texture: ";
			if(!(std::cin >> sel)){
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}while(sel >= index.size());

		repTex[flag] = aTex[index[sel]];
	}

	// Save into the file
	out.seekp(bsp.getHeader().lumps[LUMP_BRUSHSIDES_INDEX].fileofs);
	for(dbrushside_t side : brushSides){
		const texinfo_t& texInfo = texInfos[side.texinfo];
		side.texinfo = repTex[texInfo.flags];
		out.write(reinterpret_cast<char*>(&side), sizeof(dbrushside_t));
	}
}

void obfuscateNoDraw(BSPFile& bsp, std::ofstream& out){
	const std::vector<dbrushside_t>& brushSide = bsp.getBrushSide();
	const std::vector<std::string>& texDataStringData = bsp.getTexDataStringData();

	for(unsigned int i = 0; i < texDataStringData.size(); i++)
		std::cout << i << ": " << texDataStringData[i] << std::endl; 

	unsigned int sel = 0;
	do{
		std::cout << "Choose a valid texture: ";
		if(!(std::cin >> sel)){
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}while(sel >= texDataStringData.size());

	// Find a texinfo that have this texture
	const std::vector<texinfo_t>& texinfos = bsp.getTexInfo();
	const std::vector<dtexdata_t>& texdatas = bsp.getTexData();
	unsigned int texIndex = 0;
	for(unsigned int i = 0; i < texinfos.size(); i++){
		const texinfo_t& tex = texinfos[i];
		const dtexdata_t& data = texdatas[tex.texdata];
		if(data.nameStringTableID == sel){
			texIndex = i;
			break;
		}
	}

	out.seekp(bsp.getHeader().lumps[LUMP_BRUSHSIDES_INDEX].fileofs);
	for(dbrushside_t side : brushSide){
		side.texinfo = texIndex;
		out.write(reinterpret_cast<char*>(&side), sizeof(dbrushside_t));
	}
}