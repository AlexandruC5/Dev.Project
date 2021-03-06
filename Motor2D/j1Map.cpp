#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
#include "j1Window.h"
#include "j1Player.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if(map_loaded == false)
		return;
	App->render->Blit(data.backgroundimage, 0,100, &data.backgroundrectangle, 0.5f);
	
	
	
	p2List_item<MapLayer*>* item_layer = data.maplayers.start;
	MapLayer* layer = nullptr;
	p2List_item<TileSet*>* item_tileset = data.tilesets.start;
	while (item_layer != nullptr) {

		for (uint i = 0; i < item_layer->data->width; i++)  {
			for (uint j = 0; j < item_layer->data->height; j++) {
				SDL_Texture* texture = item_tileset->data->texture;
				iPoint position = MapToWorld(i, j);
				SDL_Rect tile = item_tileset->data->GetTileRect(item_layer->data->tiles[item_layer->data->Get(i, j)]);
				App->render->Blit(texture, position.x, position.y, &tile);
			}
		}
		item_layer = item_layer->next;


	}
		// TODO 9: Complete the draw function

}


iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// TODO 2: clean up all layer data
	// Remove all layers
	p2List_item<MapLayer*>* layer;
	layer = data.maplayers.start;

	while (layer != NULL)
	{
		RELEASE(layer->data);
		layer = layer->next;
	}

	data.maplayers.clear();

	p2List_item<Collider*>* collider;
	collider = data.colliders.start;
	while (collider != NULL)
	{
		collider->data->to_delete = true;
		collider = collider->next;
	}
	data.colliders.clear();

	
	// Clean up the pugui tree

	
	//Clean up background

	App->tex->UnLoad(data.backgroundimage);
	data.backgroundimage = nullptr;
	 
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name, int& map_length, int& map_height)
{
	bool ret = true;
	
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}



	// Load layer info ----------------------------------------------
	pugi::xml_node maplayer;
	for (maplayer = map_file.child("map").child("layer"); maplayer && ret; maplayer = maplayer.next_sibling("layer"))
	{
		MapLayer* set = new MapLayer();

		if (ret == true)
		{
			ret = LoadLayer(maplayer, set);
		}


		data.maplayers.add(set);
	}

	//Load coliders and Logic

	pugi::xml_node object;
	p2SString object_name;
	for (object = map_file.child("map").child("objectgroup"); object && ret; object = object.next_sibling("objectgroup"))
	{
		object_name = object.attribute("name").as_string();
			if (object_name == "Collisions")
			{
				LoadColliders(object);
			}
			else if (object_name == "Logic")
			{
				LoadLogic(object,map_length,map_height);
			}
	}
	
	//Load image layer

	pugi::xml_node backgroundimage = map_file.child("map").child("imagelayer");
	data.backgroundimage = App->tex->Load(PATH(folder.GetString(), backgroundimage.child("image").attribute("source").as_string()));
	data.backgroundrectangle = { 0,0,backgroundimage.child("image").attribute("width").as_int(), backgroundimage.child("image").attribute("height").as_int() };


	

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		// TODO 4: Add info here about your loaded layers
		// Adapt this vcode with your own variables
		
		p2List_item<MapLayer*>* item_layer = data.maplayers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

// TODO 3: Create the definition for a function that loads a single layer
bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	layer->tiles = new uint[layer->width*layer->height];

	memset(layer->tiles, 0, sizeof(uint)*layer->width*layer->height);

	pugi::xml_node data = node.child("data");

	uint i = 0u;

	for (pugi::xml_node tile = data.child("tile"); tile; tile = tile.next_sibling("tile"))
	{
		layer->tiles[i] = tile.attribute("gid").as_uint();
		i++;
	}
	return true;
}




bool j1Map::LoadLogic(pugi::xml_node & node, int & map_length, int& map_height)
{
	bool ret = true;

	pugi::xml_node object;
	p2SString name;
	for (object = node.child("object"); object; object = object.next_sibling("object"))
	{
		name = object.attribute("name").as_string();
		if (name == "Start_Point")
		{
			App->player->position.x = object.attribute("x").as_int();
			App->player->position.y = object.attribute("y").as_int();

			App->render->virtualCamPosX = -(App->player->position.x * (int)App->win->GetScale()-100);
			if (App->render->virtualCamPosX < 0)
			{
				App->render->virtualCamPosX = 0;
			}

			App->render->virtualCamPosY = -(App->player->position.y *(int)App->win->GetScale() -250 );
			
		}

		if (name == "End_Point")
		{
			App->player->End_Position.x = object.attribute("x").as_int();
			App->player->End_Position.y = object.attribute("y").as_int();
		}
	}

	pugi::xml_node property;
	for (property = node.child("properties").child("property"); property; property = property.next_sibling("property"))
	{
		p2SString name = property.attribute("name").as_string();
		if (name == "map_length")
		{
			map_length = property.attribute("value").as_int();
			
		}
		if (name == "map_height")
		{
			map_height = property.attribute("value").as_int();
		}
	}




	return ret;
}



bool j1Map::LoadColliders(pugi::xml_node& node)
{
	
	pugi::xml_node object;
	COLLIDER_TYPE collider_type;
	p2SString type;
	for (object = node.child("object"); object; object = object.next_sibling("object"))

	{

		SDL_Rect shape;
		shape.x = (int)object.attribute("x").as_float();
		LOG("x:%i", shape.x);
		shape.y = (int)object.attribute("y").as_float();
		shape.w = (int)object.attribute("width").as_float();
		shape.h = (int)object.attribute("height").as_float();
		type = object.attribute("type").as_string();
		LOG("string:%s", type.GetString());

		if (type == "floor")
		{
			data.colliders.add(App->collision->AddCollider(shape, COLLIDER_FLOOR));
			
		}

		else if (type == "platform_floor")
		{
			data.colliders.add(App->collision->AddCollider(shape, COLLIDER_PLATFORM));
		}

		
		else {
			LOG("collider type undefined");
			continue;

		}
	}

	return true;
}





