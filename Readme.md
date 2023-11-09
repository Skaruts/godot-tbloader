# TrenchBroom Loader for Godot

This is my personal modified version of TBLoader, where I'm adding features I need for my projects, as well as experimenting with ideas I have along the way. 

Please note that this is all very experimental and may not work as intended. Although it seems to.

# Usage

Please refer to TBLoader's [original instructions](https://github.com/codecat/godot-tbloader#usage) for anything you need to know. Most of it works the same, with some nunces pointed out below.

# Current major changes and reasoning:

## Bug fixes

- **`fix objects "rolled" wrongly`** : objects rotated along the X axis in TB, would be rotated the opposite way in Godot.

## Changes

- **`added a cfg file`** : this makes it a bit easier to have your settings in one place, that all TBLoader nodes can draw from. The TBLoader node comes with a button (a checkbox) to create a new cfg file. This is largely untested yet.

- **`removed hardcoded entities`** : the plugin can provide example entities without having them hardcoded, such that users can, if they want, delete all of them and create their own entities from a clean slate. I may even build an example project with some more complex entity systems. 

- **`switch to Valve (220) map format`** : It provides much better texturing capabilities, and maybe other benefits, and is actualy supported by TBLoader (without my intervention). As far as I'm aware there's even no good reason to use the Standard (Quake1) format. (Even for people mapping for Quake1.)

- **`added pre-/post-build passes`** : the post-build pass allows performing adjustments to the map after building, such as rearraging entities in the tree, if needed, and setting up GI or lightmaps, occluders, navmeshes, etc. I can't think of a use for the pre-build pass, but I added it just in case.

  This comes in two ways (same applies to both passes):
  - if the root node of the map scene has a script that implements `_pre/_post_build_setup`, then it gets called.
    ```gdscript
    func _pre_build_setup(loader: TBLoader): pass
    func _post_build_setup(loader: TBLoader): pass
    ```

  - a separate build script can be set in the config file, which can also implement `_pre/_post_build_setup`, and it will also be called if it exists.
    ```gdscript
    func _pre_build_setup(map_root: Node3D, loader: TBLoader): pass
    func _post_build_setup(map_root: Node3D, loader: TBLoader): pass
    ```

- **`removed automatic handling of clips`** : the way I see it, different projects have different clipping requirements. Some will get by with a simple `clip`, others will need `monster_clip`, `player_clip`, `projectile_clip`, and maybe others, all of which will probably use different clip textures, and all of which will require setting up different collision layers and/or masks. It's easy enough to create an entity for each clip, and everything can be setup during post-import. At the same time, some users (like me) might like to keep clip geometry around for runtime debugging purposes. Others can easily enough get rid of their geometry during post-build phase, by simply `queue_free()`-ing their mesh node (the collison has to stay).

- **`account for 'origin' property on brush entities`** : this makes it possible to make rotating brush entities in TB (doors, levers, valves, fans, etc), by setting their `origin` property (vec3) to the position where the "hinge" should be. Unfortunately TB doesn't currently have [a way to visually edit the origin](https://github.com/TrenchBroom/TrenchBroom/issues/4347), but you can use a dummy point-entity, place it where the hinge should be, and copy-paste the `origin` to the brush entity.

- **`simplified names of meshes and collision nodes`** : this is an ongoing attempt at making it a bit easier to do post-build node rearrangements, by having nodes named and arranged in predictable/consistent patterns. It's still a very experimental WIP, and the way I handle this may change in the future.
