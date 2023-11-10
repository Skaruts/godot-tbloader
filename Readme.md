# TrenchBroom Loader for Godot

This is my personal modified version of TBLoader, where I'm adding features that I need for my own projects, as well as experimenting with ideas I have along the way. 

Please note that this is all experimental and not guaranteed to work properly. Although it seems to.

# Usage

Please refer to TBLoader's [original instructions](https://github.com/codecat/godot-tbloader#usage) for anything you need to know. Most of it works the same, with some differences pointed out below.

# Current major changes and reasoning:

## Bug fixes

- **`fix objects "rolled" wrongly`** : objects rotated around the X axis in TB, would be rotated the opposite way in Godot.

- **`account for 'origin' property on brush entities`** : brush-entities ignored the `origin` property and were always positioned at the center of their geometry, making it imposible to create rotating brush-entities in TB (doors, levers, fans...). This is fixed, such that you can now set their `origin` property to the position where the "hinge" should be, and the node in Godot will be positioned correctly.

  Unfortunately TB doesn't currently have [a way to visually edit the origin](https://github.com/TrenchBroom/TrenchBroom/issues/4347), but you can use a dummy point-entity, place it where the hinge should be, and copy-paste its `origin` to your brush-entity.

## Changes

- **`added a cfg file`** : this makes it a bit easier to have your settings in one place, that all TBLoader nodes can draw from. The TBLoader node comes with a button (a checkbox) to create a new cfg file.

- **`removed hardcoded entities`** : the plugin can provide example entities without having them hardcoded, such that users can always delete all of them and create their own entities from a totally clean slate, if they want. I may build an example project with some more complex entity systems.

- **`switch to Valve (220) map format`** : it provides much better texturing capabilities, and maybe other benefits, and is actualy supported by TBLoader (without my intervention). As far as I'm aware there's even no good reason to use the Standard (Quake1) format. (Even for people mapping for Quake1.)

- **`added pre-/post-build passes`** : the post-build pass mmakes it possible to perform adjustments to the map after building, such as rearraging entities in the tree, setting up GI, lightmaps, occluders, navmeshes, etc. I can't think of a use for the pre-build pass, but I added it just in case.

  This comes in two ways:
  - if the root node of the map scene has a script that implements `_pre/_post_build_setup`, then they get called.
    ```gdscript
    func _pre_build_setup(loader: TBLoader): pass
    func _post_build_setup(loader: TBLoader): pass
    ```

  - a separate build script can be set in the config file, which can also implement `_pre/_post_build_setup`, and they will also be called if they exist.
    ```gdscript
    func _pre_build_setup(map_root: Node3D, loader: TBLoader): pass
    func _post_build_setup(map_root: Node3D, loader: TBLoader): pass
    ```

  - both options work independently of each other, but there's probably no point using both at the same time.

- **`removed automatic handling of clips`** : the way I see it, different projects have different clipping requirements, so there's no point hardcoding this. Some projects may get by with a simple `clip`, others will need `monster_clip`, `player_clip`, `projectile_clip`, among others, all of which will probably use different clip textures, and all of which will require setting up different collision layers and/or masks.

  Users will decide what they want; the plugin makes no assumptions.

  It's easy enough to create an entity for each clip (and to set it up for having transparency in TB), and it can then be setup during post-import as desired. At the same time, some users (like me) might like to keep clip geometries around for runtime debugging purposes. Others can easily get rid of them by simply `queue_free()`-ing their mesh node during post-build. 

- **`simplified names of meshes and collision nodes`** : this is an ongoing attempt at making it a bit easier to do post-build node rearrangements, by having nodes named and arranged in predictable/consistent patterns. It's still a very experimental WIP, and the way I handle this may change in the future.
