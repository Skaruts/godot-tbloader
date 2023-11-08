@tool class_name TBPlugin extends EditorPlugin


var map_control: Control = null
var editing_loader: WeakRef = weakref(null)


func _enter_tree():
	set_icons(true)

	map_control = create_map_control()
	map_control.set_visible(false)
	add_control_to_container(EditorPlugin.CONTAINER_SPATIAL_EDITOR_MENU, map_control)


func _exit_tree():
	set_icons(false)

	remove_control_from_container(EditorPlugin.CONTAINER_SPATIAL_EDITOR_MENU, map_control)
	map_control.queue_free()
	map_control = null


func _handles(object):
	return object is TBLoader


func _make_visible(visible: bool):
	map_control.set_visible(visible)


func _edit(object):
	editing_loader = weakref(object)


func create_map_control() -> Control:
	var button_build_meshes = Button.new()
	button_build_meshes.flat = true
	button_build_meshes.text = "(Re) Build Map"
	button_build_meshes.connect("pressed", Callable(self, "build_meshes"))

	var ret = HBoxContainer.new()
	ret.add_child(button_build_meshes)
	return ret


func build_meshes():
	var loader = editing_loader.get_ref()

	var map_root:Node3D = loader.owner
	if not map_root:
		printerr("Can't build map: TBLoader has no owner. Make sure TBloader is NOT the scene root.")
		return

	extra_build_pass(map_root, loader, "_pre_build_setup")
	loader.build_meshes()
	extra_build_pass(map_root, loader, "_post_build_setup")


func extra_build_pass(map_root:Node3D, loader:TBLoader, func_name:String) -> void:
	if map_root.has_method(func_name):
		map_root.call(func_name, loader)

	if not loader.map_post_build_script_path: return

	if ResourceLoader.exists(loader.map_post_build_script_path):
		var script = ResourceLoader.load(loader.map_post_build_script_path).new()
		if script.has_method(func_name):
			script.call(func_name, map_root, loader)



func set_icons(on):
	var editor_interface = get_editor_interface()
	var base_control = editor_interface.get_base_control()
	var theme = base_control.theme

	if on:
		var texture = ResourceLoader.load("res://addons/tbloader/icons/tbloader.svg")
		theme.set_icon("TBLoader", "EditorIcons", texture)
	else:
		theme.clear_icon("TBLoader", "EditorIcons")
