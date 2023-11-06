#ifndef LIBMAP_MAP_DATA_H
#define LIBMAP_MAP_DATA_H

#include "entity.h"
#include "entity_geometry.h"
#include "libmap.h"
#include <stdlib.h>

typedef struct LMTextureData {
	char *name;
	int width;
	int height;
} LMTextureData;

typedef struct LMWorldspawnLayer {
	int texture_idx;
	bool build_visuals;
} LMWorldspawnLayer;

class LMMapData {
public:
	int entity_count = 0;
	LMEntity *entities = NULL;
	LMEntityGeometry *entity_geo = NULL;

	int texture_count = 0;
	LMTextureData *textures = NULL;

	int worldspawn_layer_count = 0;
	LMWorldspawnLayer *worldspawn_layers = NULL;

	void register_worldspawn_layer(const char *name, bool build_visuals);
	int find_worldspawn_layer(int texture_idx);
	int get_worldspawn_layer_count();
	LMWorldspawnLayer *get_worldspawn_layers();

	void set_texture_size(const char *name, int width, int height);
	int get_texture_count();
	LMTextureData *get_textures();
	LMTextureData *get_texture(int texture_idx);

	void set_spawn_type_by_classname(const char *key, int spawn_type);

	void print_entities();
	int get_entity_count();
	const LMEntity *get_entities();

	LMMapData();
	void reset();
	int register_texture(const char *name);
	int find_texture(const char *texture_name);
	const char *get_entity_property(int entity_idx, const char *key);
};

#endif
