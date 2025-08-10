#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "rive_viewer.hpp"
#include "rive_viewer_2d.hpp"


// Optional: web bridge registration (only linked on web build)
#ifdef __EMSCRIPTEN__
void register_canvaskit_bridge_types();
void unregister_canvaskit_bridge_types();
#endif

using namespace godot;

void initialize_rive_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    ClassDB::register_class<RiveViewer>();
    ClassDB::register_class<RiveViewer2D>();
    ClassDB::register_class<RiveFile>();

    // Register web bridge if linked in web builds
    #ifdef __EMSCRIPTEN__
    register_canvaskit_bridge_types();
    #endif

    ClassDB::register_class<RiveArtboard>();
    ClassDB::register_class<RiveScene>();
    ClassDB::register_class<RiveInput>();

    #ifdef __EMSCRIPTEN__
    unregister_canvaskit_bridge_types();
    #endif

    ClassDB::register_class<RiveListener>();
    ClassDB::register_class<RiveAnimation>();
}

void uninitialize_rive_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
    // Initialization.
    GDExtensionBool GDE_EXPORT rive_library_init(
        GDExtensionInterfaceGetProcAddress p_get_proc_address,
        GDExtensionClassLibraryPtr p_library,
        GDExtensionInitialization *r_initialization
    ) {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_rive_module);
        init_obj.register_terminator(uninitialize_rive_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}