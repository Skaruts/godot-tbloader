#include "map_data.h"

#include "brush.h"
#include "face.h"
#include "platform.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void LMMapData::reset() {
	if (entities != NULL) {
		for (int e = 0; e < entity_count; ++e) {
			LMEntity *ent_inst = &entities[e];
			LMEntityGeometry *entity_geo_inst = &entity_geo[e];

			if (entity_geo_inst != NULL) {
				for (int b = 0; b < ent_inst->brush_count; ++b) {
					LMBrush *brush_inst = &ent_inst->brushes[b];
					LMBrushGeometry *brush_geo_inst = &entity_geo_inst->brushes[b];

					if (brush_geo_inst != NULL) {
						for (int f = 0; f < brush_inst->face_count; ++f) {
							LMFaceGeometry *face_geo_inst = &brush_geo_inst->faces[f];
							if (face_geo_inst != NULL) {
								if (face_geo_inst->vertices != NULL) {
									free(face_geo_inst->vertices);
									face_geo_inst->vertices = NULL;
								}

								if (face_geo_inst->indices != NULL) {
									free(face_geo_inst->indices);
									face_geo_inst->indices = NULL;
								}
							}
						}

						free(brush_inst->faces);
						brush_inst->faces = NULL;

						free(brush_geo_inst->faces);
						brush_geo_inst->faces = NULL;
					}
				}

				if (ent_inst->properties != NULL) {
					for (int p = 0; p < ent_inst->property_count; ++p) {
						if (ent_inst->properties[p].key != NULL) {
							free(ent_inst->properties[p].key);
						}

						if (ent_inst->properties[p].value != NULL) {
							free(ent_inst->properties[p].value);
						}
					}

					free(ent_inst->properties);
					ent_inst->properties = NULL;
				}

				free(ent_inst->brushes);
				ent_inst->brushes = NULL;

				free(entity_geo_inst->brushes);
				entity_geo_inst->brushes = NULL;
			}
		}

		free(entities);
		entities = NULL;

		free(entity_geo);
		entity_geo = NULL;
	}

	entity_count = 0;

	if (textures != NULL) {
		for (int t = 0; t < texture_count; t++) {
			LMTextureData *texture = &textures[t];
			free(texture->name);
		}

		free(textures);
		textures = NULL;
	}

	texture_count = 0;

	if (worldspawn_layers != NULL) {
		free(worldspawn_layers);
		worldspawn_layers = NULL;
	}

	worldspawn_layer_count = 0;
}

void LMMapData::register_worldspawn_layer(const char *name, bool build_visuals) {
	worldspawn_layers = (LMWorldspawnLayer *)realloc(worldspawn_layers, (worldspawn_layer_count + 1) * sizeof(LMWorldspawnLayer));
	LMWorldspawnLayer *layer = &worldspawn_layers[worldspawn_layer_count];
	*layer = { 0 };
	layer->texture_idx = find_texture(name);
	layer->build_visuals = build_visuals;
	worldspawn_layer_count++;
}

int LMMapData::find_worldspawn_layer(int texture_idx) {
	for (int l = 0; l < worldspawn_layer_count; ++l) {
		LMWorldspawnLayer *layer = &worldspawn_layers[l];
		if (layer->texture_idx == texture_idx) {
			return l;
		}
	}

	return -1;
}

int LMMapData::get_worldspawn_layer_count() {
	return worldspawn_layer_count;
}

LMWorldspawnLayer *LMMapData::get_worldspawn_layers() {
	return worldspawn_layers;
}

int LMMapData::register_texture(const char *name) {
	if (textures != NULL) {
		for (int t = 0; t < texture_count; ++t) {
			LMTextureData *texture = &textures[t];
			if (strcmp(texture->name, name) == 0) {
				return t;
			}
		}
	}

	textures = (LMTextureData *)realloc(textures, (texture_count + 1) * sizeof(LMTextureData));
	LMTextureData *texture = &textures[texture_count];
	*texture = { 0 };
	texture->name = STRDUP(name);
	texture_count++;
	return texture_count - 1;
}

void LMMapData::set_texture_size(const char *name, int width, int height) {
	for (int t = 0; t < texture_count; ++t) {
		LMTextureData *texture = &textures[t];
		if (strcmp(texture->name, name) == 0) {
			texture->width = width;
			texture->height = height;
			return;
		}
	}
}

int LMMapData::get_texture_count() {
	return texture_count;
}

LMTextureData *LMMapData::get_textures() {
	return textures;
}

LMTextureData *LMMapData::get_texture(int texture_idx) {
	if (texture_idx >= 0 && texture_idx < texture_count) {
		return &textures[texture_idx];
	}

	return NULL;
}

int LMMapData::find_texture(const char *texture_name) {
	for (int t = 0; t < texture_count; ++t) {
		LMTextureData *texture = &textures[t];
		if (strcmp(texture->name, texture_name) == 0) {
			return t;
		}
	}

	return -1;
}

void LMMapData::set_spawn_type_by_classname(const char *key, int spawn_type) {
	for (int e = 0; e < entity_count; ++e) {
		LMEntity *ent = &entities[e];
		if (ent->property_count == 0) {
			continue;
		}

		for (int p = 0; p < ent->property_count; ++p) {
			LMProperty *prop = &ent->properties[p];
			if (strcmp(prop->key, "classname") == 0 && strcmp(prop->value, key) == 0) {
				ent->spawn_type = (ENTITY_SPAWN_TYPE)spawn_type;
				break;
			}
		}
	}
}

int LMMapData::get_entity_count() {
	return entity_count;
}

const LMEntity *LMMapData::get_entities() {
	return entities;
}

const char *LMMapData::get_entity_property(int entity_idx, const char *key) {
	if (entity_idx < 0) {
		return NULL;
	}

	const LMEntity *ent = &entities[entity_idx];

	for (int p = 0; p < ent->property_count; ++p) {
		LMProperty *prop = &ent->properties[p];
		if (strcmp(prop->key, key) == 0) {
			return prop->value;
		}
	}

	return NULL;
}

void LMMapData::print_entities() {
	for (int e = 0; e < entity_count; ++e) {
		LMEntity entity_inst = entities[e];
		printf("Entity %d\n", e);
		for (int b = 0; b < entity_inst.brush_count; ++b) {
			LMBrush entity_brush = entity_inst.brushes[b];
			printf("Brush %d\n", b);
			printf("Face Count: %d\n", entity_brush.face_count);

			for (int f = 0; f < entity_brush.face_count; ++f) {
				LMFace brush_face = entity_brush.faces[f];
				printf("Face %d\n", f);
				printf(
						"(%f %f %f) (%f %f %f) (%f %f %f)\n%s %f %f\n[%f %f %f %f] [%f %f %f %f]\n%f %f %f\n\n",
						brush_face.plane_points.v0.x, brush_face.plane_points.v0.y, brush_face.plane_points.v0.z,
						brush_face.plane_points.v1.x, brush_face.plane_points.v1.y, brush_face.plane_points.v1.z,
						brush_face.plane_points.v2.x, brush_face.plane_points.v2.y, brush_face.plane_points.v2.z,

						get_texture(brush_face.texture_idx)->name,

						brush_face.uv_standard.u,
						brush_face.uv_standard.v,

						brush_face.uv_valve.u.axis.x, brush_face.uv_valve.u.axis.y, brush_face.uv_valve.u.axis.z, brush_face.uv_valve.u.offset,
						brush_face.uv_valve.v.axis.x, brush_face.uv_valve.v.axis.y, brush_face.uv_valve.v.axis.z, brush_face.uv_valve.v.offset,

						brush_face.uv_extra.rot,
						brush_face.uv_extra.scale_x,
						brush_face.uv_extra.scale_y);
			}

			putchar('\n');
		}
	}
}

LMMapData::LMMapData() {}
