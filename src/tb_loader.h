#pragma once

#include <gdextension_interface.h>

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/defs.hpp>

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/config_file.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/vector2i.hpp>
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/vector3i.hpp>
#include <godot_cpp/variant/color.hpp>

#include <vector.h>

using namespace godot;

class TBLoader : public Node3D {
	GDCLASS(TBLoader, Node3D);


private:
	String    m_cfg_path                       = "res://tbloader_config.cfg";

public:
	Ref<ConfigFile> m_config;

	int       m_inverse_scale          = 64;
	bool      m_skip_hidden_layers     = true;
	String    m_entity_path            = "res://scenes/entities";
	String    m_texture_path           = "res://assets/textures";
	String    m_build_script_path      = "";
	String    m_clip_texture_name      = "";
	String    m_skip_texture_name      = "";

	String    m_map_path               = "";
	bool      m_unwrap_uv2             = false;    // global ???
	bool      m_collision              = true;
	bool      m_filter_nearest         = false;
	uint32_t  m_visual_layer_mask      = 1;


protected:
	static void _bind_methods();


private:
	bool load_config();
	void save_config();
	void create_config();

	void load_settings();


public:
	TBLoader();
	~TBLoader();

	void TBLoader::_ready();

	void clear();
	void build_meshes();

	void set_create_config(bool enable);
	bool get_create_config();

	void set_build_script_path(const String& path);
	String get_build_script_path();


	// Map
	void set_map(const String& map);
	String get_map() const;

	// Options
	void set_unwrap_uv2(bool enabled);
	bool get_unwrap_uv2();

	void set_collision(bool enabled);
	bool get_collision();

	void set_filter_nearest(bool enabled);
	bool get_filter_nearest();

	uint32_t get_visual_layer_mask();
	void set_visual_layer_mask(uint32_t visual_layer_mask);




	void set_inverse_scale(int scale);
	int get_inverse_scale();

	void set_skip_hidden_layers(bool enabled);
	bool get_skip_hidden_layers();

	void set_clip_texture_name(const String& clip_texture);
	String get_clip_texture_name();
	void set_skip_texture_name(const String& skip_texture);
	String get_skip_texture_name();

	void set_entity_path(const String& path);
	String get_entity_path();

	void set_texture_path(const String& path);
	String get_texture_path();

	static Vector2 to_vector2(const String& value);
	static Vector2i to_vector2i(const String& value);
	static Vector3 to_vector3(const String& value);
	static Vector3i to_vector3i(const String& value);
	static Color to_color(const String& value);
	static bool to_bool(const String& value);
	static int64_t to_int(const String& value);
	static float to_float(const String& value);
};
