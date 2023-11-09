#include <tb_loader.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/config_file.hpp>

#include <builder.h>

void TBLoader::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_create_config", "create_config"), &TBLoader::set_create_config);
	ClassDB::bind_method(D_METHOD("get_create_config"), &TBLoader::get_create_config);

	ClassDB::bind_method(D_METHOD("set_map", "map_resource"), &TBLoader::set_map);
	ClassDB::bind_method(D_METHOD("get_map"), &TBLoader::get_map);
	ClassDB::bind_method(D_METHOD("set_inverse_scale", "inverse_scale"), &TBLoader::set_inverse_scale);
	ClassDB::bind_method(D_METHOD("get_inverse_scale"), &TBLoader::get_inverse_scale);

	ClassDB::bind_method(D_METHOD("set_unwrap_uv2", "unwrap_uv2"), &TBLoader::set_unwrap_uv2);
	ClassDB::bind_method(D_METHOD("get_unwrap_uv2"), &TBLoader::get_unwrap_uv2);

	ClassDB::bind_method(D_METHOD("set_collision", "option_collision"), &TBLoader::set_collision);
	ClassDB::bind_method(D_METHOD("get_collision"), &TBLoader::get_collision);
	ClassDB::bind_method(D_METHOD("set_skip_hidden_layers", "option_skip_hidden_layers"), &TBLoader::set_skip_hidden_layers);
	ClassDB::bind_method(D_METHOD("get_skip_hidden_layers"), &TBLoader::get_skip_hidden_layers);
	ClassDB::bind_method(D_METHOD("set_filter_nearest", "option_filter_nearest"), &TBLoader::set_filter_nearest);
	ClassDB::bind_method(D_METHOD("get_filter_nearest"), &TBLoader::get_filter_nearest);
	ClassDB::bind_method(D_METHOD("set_clip_texture_name", "option_clip_texture_name"), &TBLoader::set_clip_texture_name);
	ClassDB::bind_method(D_METHOD("get_clip_texture_name"), &TBLoader::get_clip_texture_name);
	ClassDB::bind_method(D_METHOD("set_skip_texture_name", "option_skip_texture_name"), &TBLoader::set_skip_texture_name);
	ClassDB::bind_method(D_METHOD("get_skip_texture_name"), &TBLoader::get_skip_texture_name);
	ClassDB::bind_method(D_METHOD("set_visual_layer_mask", "option_visual_layer_mask"), &TBLoader::set_visual_layer_mask);
	ClassDB::bind_method(D_METHOD("get_visual_layer_mask"), &TBLoader::get_visual_layer_mask);

	ClassDB::bind_method(D_METHOD("set_entity_path", "entity_path"), &TBLoader::set_entity_path);
	ClassDB::bind_method(D_METHOD("get_entity_path"), &TBLoader::get_entity_path);
	ClassDB::bind_method(D_METHOD("set_texture_path", "texture_path"), &TBLoader::set_texture_path);
	ClassDB::bind_method(D_METHOD("get_texture_path"), &TBLoader::get_texture_path);

	ClassDB::bind_method(D_METHOD("set_build_script_path", "build_script_path"), &TBLoader::set_build_script_path);
	ClassDB::bind_method(D_METHOD("get_build_script_path"), &TBLoader::get_build_script_path);


	ClassDB::bind_method(D_METHOD("clear"), &TBLoader::clear);
	ClassDB::bind_method(D_METHOD("build_meshes"), &TBLoader::build_meshes);



	ADD_PROPERTY(PropertyInfo(Variant::BOOL,   "create_config",  PROPERTY_HINT_NONE),             "set_create_config",        "get_create_config");

	ADD_GROUP("Map", "map_");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "file_path",  PROPERTY_HINT_FILE, "*.map"),    "set_map",                  "get_map");

	ADD_GROUP("Options", "");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "unwrap_uv2",        PROPERTY_HINT_NONE),             "set_unwrap_uv2",          "get_unwrap_uv2");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "world_collisions",  PROPERTY_HINT_NONE),             "set_collision",           "get_collision");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "filter_nearest",    PROPERTY_HINT_NONE),             "set_filter_nearest",      "get_filter_nearest");
	ADD_PROPERTY(PropertyInfo(Variant::INT,  "visual_layer_mask", PROPERTY_HINT_LAYERS_3D_RENDER), "set_visual_layer_mask",   "get_visual_layer_mask");

}

TBLoader::TBLoader() {}
TBLoader::~TBLoader() {}


void TBLoader::clear() {
	while (get_child_count() > 0) {
		auto child = get_child(0);
		remove_child(child);
		child->queue_free();
	}
}

void TBLoader::build_meshes() {
	clear();

	if (load_config()) load_settings();

	Builder builder(this);
	builder.load_map(m_map_path);
	builder.build_map();
}

void TBLoader::_ready() {
	m_config = (Ref<ConfigFile>)memnew(ConfigFile());

	if (!load_config())
		UtilityFunctions::printerr("TBLoader config file doesn't exist. Click 'Create Config' in the TBLoader node to create one.");
}

void TBLoader::create_config() {
	// if it already exists, load it first, so it doesn't get overwritten
	if (load_config()) load_settings();

	m_config->set_value("map",      "inverse_scale",      m_inverse_scale);
	m_config->set_value("map",      "build_script",       m_build_script_path);

	m_config->set_value("options",  "skip_hidden_layers", m_skip_hidden_layers);
	// m_config->set_value("options",  "clip_texture_name",  m_clip_texture_name);
	m_config->set_value("options",  "skip_texture_name",  m_skip_texture_name);

	m_config->set_value("entities", "entity_path",        m_entity_path);
	m_config->set_value("assets",   "texture_path",       m_texture_path);

	save_config();
}

bool TBLoader::load_config() {
	return m_config->load(m_cfg_path) == OK;
}

void TBLoader::save_config() {
	m_config->save(m_cfg_path);
}

void TBLoader::load_settings() {
	m_inverse_scale          = m_config->get_value("map",      "inverse_scale",      m_inverse_scale);
	m_build_script_path      = m_config->get_value("map",      "build_script",       m_build_script_path);

	m_skip_hidden_layers     = m_config->get_value("options",  "skip_hidden_layers", m_skip_hidden_layers);
	// m_clip_texture_name      = m_config->get_value("options",  "clip_texture_name",  m_clip_texture_name);
	m_skip_texture_name      = m_config->get_value("options",  "skip_texture_name",  m_skip_texture_name);

	m_entity_path            = m_config->get_value("entities", "entity_path",        m_entity_path);
	m_texture_path           = m_config->get_value("assets",   "texture_path",       m_texture_path);
}


void TBLoader::set_create_config(bool enable) {
	create_config();
}

bool TBLoader::get_create_config() {
	return false;
}


void TBLoader::set_map(const String& map) {
	m_map_path = map;
}

String TBLoader::get_map() const {
	return m_map_path;
}


void TBLoader::set_inverse_scale(int scale) {
	m_inverse_scale = scale;
}

int TBLoader::get_inverse_scale() {
	return m_inverse_scale;
}


void TBLoader::set_unwrap_uv2(bool enabled) {
	m_unwrap_uv2 = enabled;
}

bool TBLoader::get_unwrap_uv2() {
	return m_unwrap_uv2;
}


void TBLoader::set_collision(bool enabled) {
	m_collision = enabled;
}

bool TBLoader::get_collision() {
	return m_collision;
}


void TBLoader::set_skip_hidden_layers(bool enabled) {
	m_skip_hidden_layers = enabled;
}

bool TBLoader::get_skip_hidden_layers() {
	return m_skip_hidden_layers;
}


void TBLoader::set_filter_nearest(bool enabled) {
	m_filter_nearest = enabled;
}

bool TBLoader::get_filter_nearest() {
	return m_filter_nearest;
}


void TBLoader::set_clip_texture_name(const String& clip_texture_name) {
	m_clip_texture_name = clip_texture_name;
}

String TBLoader::get_clip_texture_name() {
	return m_clip_texture_name;
}


void TBLoader::set_skip_texture_name(const String& skip_texture_name) {
	m_skip_texture_name = skip_texture_name;
}

String TBLoader::get_skip_texture_name() {
	return m_skip_texture_name;
}


uint32_t TBLoader::get_visual_layer_mask() {
	return m_visual_layer_mask;
}

void TBLoader::set_visual_layer_mask(uint32_t visual_layer_mask) {
	m_visual_layer_mask = visual_layer_mask;
}


void TBLoader::set_entity_path(const String& path) {
	m_entity_path = path;
}

String TBLoader::get_entity_path() {
	return m_entity_path;
}


void TBLoader::set_texture_path(const String& path) {
	if (path.is_empty()) {
		UtilityFunctions::push_warning("WARNING: texture_path should not be empty");
	}
	m_texture_path = path;
}

String TBLoader::get_texture_path() {
	return m_texture_path;
}



void TBLoader::set_build_script_path(const String& path) {
	m_build_script_path = path;
}

String TBLoader::get_build_script_path() {
	return m_build_script_path;
}

