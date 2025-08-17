#ifndef _RIVEEXTENSION_RIVE_SPRITE2D_BASE_H_
#define _RIVEEXTENSION_RIVE_SPRITE2D_BASE_H_

// stdlib
#include <functional>
#include <memory>

// godot-cpp
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/dictionary.hpp>

// extension
#include "rive_instance.hpp"
#include "skia_instance.hpp"
#include "viewer_props.hpp"
#include "utils/types.hpp"

using namespace godot;

/**
 * RiveSprite2D base class - works exactly like RiveViewerBase but for Sprite2D
 * Maintains the same functionality and behavior as the original RiveViewer
 */
class RiveSprite2DBase {
private:
    Sprite2D* owner;
    ViewerProps props;
    RiveInstance inst;
    SkiaInstance sk;
    float elapsed = 0.0f;
    Dictionary cached_scene_property_values;
    Ref<Image> image;
    Ref<ImageTexture> texture;

protected:
    void _on_path_changed(String path);
    void _on_artboard_changed(int index);
    void _on_scene_changed(int index);
    void _on_animation_changed(int index);
    void _on_size_changed(float w, float h);
    void _on_transform_changed();
    
    bool advance(float delta);
    PackedByteArray frame(float delta);
    PackedByteArray redraw();
    void check_scene_property_changed();

public:
    RiveSprite2DBase(Sprite2D* owner);
    ~RiveSprite2DBase();
    
    // Core lifecycle methods
    void on_ready();
    void on_process(double delta);
    void on_input_event(const Ref<InputEvent>& event);
    
    // Property management
    void get_property_list(List<PropertyInfo>* p_list) const;
    bool on_set(const StringName& prop, const Variant& value);
    bool on_get(const StringName& prop, Variant& return_value) const;
    
    // Size management
    int width() const;
    int height() const;
    Vector2 get_size() const;
    
    /* Rive API - Same as RiveViewer */
    
    // File management
    void set_file_path(String value) { props.path(value); }
    String get_file_path() const { return props.path(); }
    
    // Fit and alignment
    void set_fit(int value) { props.fit((FIT)value); }
    int get_fit() const { return props.fit(); }
    void set_alignment(int value) { props.alignment((ALIGN)value); }
    int get_alignment() const { return props.alignment(); }
    
    // Input handling
    void set_disable_press(bool value) { props.disable_press(value); }
    bool get_disable_press() const { return props.disable_press(); }
    void set_disable_hover(bool value) { props.disable_hover(value); }
    bool get_disable_hover() const { return props.disable_hover(); }
    
    // Animation control
    void set_paused(bool value) { props.paused(value); }
    bool get_paused() const { return props.paused(); }
    
    // Getters for Rive objects
    float get_elapsed_time() const { return elapsed; }
    Ref<RiveFile> get_file() const { return inst.file; }
    Ref<RiveArtboard> get_artboard() const { return inst.artboard(); }
    Ref<RiveScene> get_scene() const { return inst.scene(); }
    Ref<RiveAnimation> get_animation() const { return inst.animation(); }
    
    // Navigation methods
    void go_to_artboard(Ref<RiveArtboard> artboard);
    void go_to_scene(Ref<RiveScene> scene);
    void go_to_animation(Ref<RiveAnimation> animation);
    
    // Mouse interaction
    void press_mouse(Vector2 position) { inst.press_mouse(position); }
    void release_mouse(Vector2 position) { inst.release_mouse(position); }
    void move_mouse(Vector2 position) { inst.move_mouse(position); }
};

// Macros for easy binding (simplified)
#define RIVE_SPRITE2D_SETGET(type, name) \
    void set_##name(type value) { base.set_##name(value); } \
    type get_##name() const { return base.get_##name(); }

#define RIVE_SPRITE2D_GET(type, name) \
    type get_##name() const { return base.get_##name(); }

#define RIVE_SPRITE2D_BIND(cls) \
    ADD_PROP_WITH_HINT(cls, Variant::STRING, file_path, PROPERTY_HINT_FILE, "*.riv"); \
    ADD_PROP_WITH_HINT(cls, Variant::INT, fit, PROPERTY_HINT_ENUM, FitEnumPropertyHint); \
    ADD_PROP_WITH_HINT(cls, Variant::INT, alignment, PROPERTY_HINT_ENUM, AlignEnumPropertyHint); \
    ADD_PROP(cls, Variant::BOOL, disable_press); \
    ADD_PROP(cls, Variant::BOOL, disable_hover); \
    ADD_PROP(cls, Variant::BOOL, paused); \
    ADD_SIGNAL(MethodInfo("pressed", PropertyInfo(Variant::VECTOR2, "position"))); \
    ADD_SIGNAL(MethodInfo("released", PropertyInfo(Variant::VECTOR2, "position"))); \
    ADD_SIGNAL(MethodInfo( \
        "scene_property_changed", \
        PropertyInfo(Variant::OBJECT, "scene"), \
        PropertyInfo(Variant::STRING, "property"), \
        PropertyInfo(Variant::VARIANT_MAX, "new_value"), \
        PropertyInfo(Variant::VARIANT_MAX, "old_value") \
    )); \
    BIND_GET(cls, elapsed_time); \
    BIND_GET(cls, file); \
    BIND_GET(cls, artboard); \
    BIND_GET(cls, scene); \
    BIND_GET(cls, animation); \
    ClassDB::bind_method(D_METHOD("go_to_artboard", "artboard"), &cls::go_to_artboard); \
    ClassDB::bind_method(D_METHOD("go_to_scene", "scene"), &cls::go_to_scene); \
    ClassDB::bind_method(D_METHOD("go_to_animation", "animation"), &cls::go_to_animation); \
    ClassDB::bind_method(D_METHOD("press_mouse", "position"), &cls::press_mouse); \
    ClassDB::bind_method(D_METHOD("release_mouse", "position"), &cls::release_mouse); \
    ClassDB::bind_method(D_METHOD("move_mouse", "position"), &cls::move_mouse)

#define RIVE_SPRITE2D_WRAPPER(cls) \
private: \
    RiveSprite2DBase base = RiveSprite2DBase(this); \
public: \
    void _ready() override { \
        Node::set_process_internal(true); \
        Node::set_process(true); \
        base.on_ready(); \
    } \
    void _process_internal(double delta) { \
        base.on_process(delta); \
    } \
    void _process(double delta) { \
        base.on_process(delta); \
    } \
    void _get_property_list(List<PropertyInfo>* list) const { \
        base.get_property_list(list); \
    } \
    bool _set(const StringName& prop, const Variant& value) { \
        return base.on_set(prop, value); \
    } \
    bool _get(const StringName& prop, Variant& return_value) const { \
        return base.on_get(prop, return_value); \
    } \
    RIVE_SPRITE2D_SETGET(String, file_path) \
    RIVE_SPRITE2D_SETGET(int, fit) \
    RIVE_SPRITE2D_SETGET(int, alignment) \
    RIVE_SPRITE2D_SETGET(bool, disable_press) \
    RIVE_SPRITE2D_SETGET(bool, disable_hover) \
    RIVE_SPRITE2D_SETGET(bool, paused) \
    RIVE_SPRITE2D_GET(float, elapsed_time) \
    RIVE_SPRITE2D_GET(Ref<RiveFile>, file) \
    RIVE_SPRITE2D_GET(Ref<RiveArtboard>, artboard) \
    RIVE_SPRITE2D_GET(Ref<RiveScene>, scene) \
    RIVE_SPRITE2D_GET(Ref<RiveAnimation>, animation) \
    void go_to_artboard(Ref<RiveArtboard> artboard) { base.go_to_artboard(artboard); } \
    void go_to_scene(Ref<RiveScene> scene) { base.go_to_scene(scene); } \
    void go_to_animation(Ref<RiveAnimation> animation) { base.go_to_animation(animation); } \
    void press_mouse(Vector2 position) { base.press_mouse(position); } \
    void release_mouse(Vector2 position) { base.release_mouse(position); } \
    void move_mouse(Vector2 position) { base.move_mouse(position); }

#endif // _RIVEEXTENSION_RIVE_SPRITE2D_BASE_H_