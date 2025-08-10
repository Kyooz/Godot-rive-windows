#include "register_types.h"
#include <godot_cpp/core/class_db.hpp>

using namespace godot;

extern "C" GDExtensionBool GDE_EXPORT rive_library_init(
    GDExtensionInterfaceGetProcAddress p_get_proc_address,
    const GDExtensionClassLibraryPtr p_library,
    GDExtensionInitialization *r_initialization)
{
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_scene_initializer([]() {
        // Minimal init path for Web build; classes are registered in register_types.cpp
    });

    init_obj.register_scene_terminator([]() {
        // Minimal terminate path for Web build
    });

    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}

