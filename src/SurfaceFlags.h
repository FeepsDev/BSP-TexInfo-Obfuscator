/*
	Created by Feeps on 24/07/2020.
*/

#ifndef SURFACEFLAGS_H
#define SURFACEFLAGS_H

#include <unordered_set>

namespace sf{
	enum Flags{
		LIGHT,     // value will hold the light strength
		SKY2D,     // don't draw, indicates we should skylight + draw 2d sky but not draw the 3D skybox
		SKY,       // don't draw, but add to skybox
		WARP,      // turbulent water warp
		TRANS,     // texture is translucent
		NOPORTAL,  // the surface can not have a portal placed on it
		TRIGGER,   // FIXME: This is an xbox hack to work around elimination of trigger surfaces, which breaks occluders
		NODRAW,    // don't bother referencing the texture
		HINT,      // make a primary bsp splitter
		SKIP,      // completely ignore, allowing non-closed brushes
		NOLIGHT,   // Don't calculate light
		BUMPLIGHT, // calculate three lightmaps for the surface for bumpmapping
		NOSHADOWS, // Don't receive shadows
		NODECALS,  // Don't receive decals
		NOCHOP,    // Don't subdivide patches on this surface
		HITBOX,    // surface is part of a hitbox
	};

	std::unordered_set<Flags> getFlagsByIndex(int mask){
		std::unordered_set<Flags> flagsSet;

		for (int i = 0; i != HITBOX; i++)
			if ((mask & (1 << i)) != 0)
				flagsSet.insert(static_cast<Flags>(i));
		
		return flagsSet;
	}

	std::ostream& operator<< (std::ostream& os, Flags v){
		switch (v){
			case LIGHT:
				return os << "SURF_LIGHT";
			case SKY2D:
				return os << "SURF_SKY2D";
			case SKY:
				return os << "SURF_SKY";
			case WARP:
				return os << "SURF_WARP";
			case TRANS:
				return os << "SURF_TRANS";
			case NOPORTAL:
				return os << "SURF_NOPORTAL";
			case TRIGGER:
				return os << "SURF_TRIGGER";
			case NODRAW:
				return os << "SURF_NODRAW";
			case HINT:
				return os << "SURF_HINT";
			case SKIP:
				return os << "SURF_SKIP";
			case NOLIGHT:
				return os << "SURF_NOLIGHT";
			case BUMPLIGHT:
				return os << "SURF_BUMPLIGHT";
			case NOSHADOWS:
				return os << "SURF_NOSHADOWS";
			case NODECALS:
				return os << "SURF_NODECALS";
			case NOCHOP:
				return os << "SURF_NOCHOP";
			case HITBOX:
				return os << "SURF_HITBOX";
		}
		return os << v; // Print integer
	}
}

#endif