#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1App.h"


// TODO 1: Create a struct for the map layer
// ----------------------------------------------------

struct MapLayer  
{

	p2SString	name="";
	uint width=0;
	uint height=0;
	uint* tiles=nullptr;

	
		

	~MapLayer()
	{
		if (tiles != nullptr) {
			delete[] tiles;
			tiles = nullptr;
		}
	}
	inline uint Get(int x, int y) const { //inline changed
		return x + (width*y); //this changed the draw type 
	}
};

	// TODO 6: Short function to get the value of x,y



// ----------------------------------------------------
struct TileSet
{
	// TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	SDL_Texture*		backgroundimage;
	SDL_Rect			backgroundrectangle;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<MapLayer*>	maplayers;
	p2List<Collider*>	colliders;
	// TODO 2: Add a list/array of layers to the map!
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path, int& map_length, int& map_height);

	// TODO 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	// TODO 3: Create a method that loads a single laye
	 bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	 bool LoadColliders(pugi::xml_node& node);
	 bool LoadLogic(pugi::xml_node& node, int& map_length, int& map_height);



public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__