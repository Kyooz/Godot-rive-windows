#include "rive_sprite2d_base.h"

#include <algorithm>

// godot-cpp
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input_event_mouse.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

// rive-cpp
#include <rive/animation/linear_animation.hpp>
#include <rive/animation/linear_animation_instance.hpp>

// extension
#include "rive_exceptions.hpp"
#include "utils/godot_macros.hpp"

const Image::Format SPRITE2D_IMAGE_FORMAT = Image::Format::FORMAT_RGBA8;

RiveSprite2DBase::RiveSprite2DBase(Sprite2D* owner) : owner(owner) {
    this->owner = owner;
    inst.set_props(&props);
    sk.set_props(&props);
    
    // Setup callbacks (same as RiveViewerBase)
    props.on_artboard_changed([this](int index) { _on_artboard_changed(index); });
    props.on_scene_changed([this](int index) { _on_scene_changed(index); });
    props.on_animation_changed([this](int index) { _on_animation_changed(index); });
    props.on_path_changed([this](String path) { _on_path_changed(path); });
    props.on_size_changed([this](float w, float h) { _on_size_changed(w, h); });
    props.on_transform_changed([this]() { _on_transform_changed(); });
    
    elapsed = 0.0f;
}

RiveSprite2DBase::~RiveSprite2DBase() {
    // Cleanup handled automatically by Godot
}

void RiveSprite2DBase::on_ready() {
    elapsed = 0.0f;
    int w = width();
    int h = height();
    props.size(w, h);
}

void RiveSprite2DBase::on_process(double delta) {
    if (props.paused()) {
        return;
    }

    PackedByteArray bytes = frame(delta);
    if (bytes.is_empty()) {
        return;
    }

    // Update the image and texture with the new frame data (same as RiveViewerBase)
    if (!is_null(image) && !is_null(texture)) {
        image->set_data(width(), height(), false, SPRITE2D_IMAGE_FORMAT, bytes);
        texture->update(image);
    }
}

// This is the core method - same logic as RiveViewerBase::frame()
PackedByteArray RiveSprite2DBase::frame(float delta) {
    if (!owner->is_visible_in_tree()) {
        return PackedByteArray();
    }

    if (!exists(inst.file) || !exists(inst.artboard()) || !sk.renderer || !sk.surface) {
        return PackedByteArray();
    }

    elapsed += delta;
    inst.advance(delta);

    PackedByteArray bytes = redraw();

    if (bytes.size() > 0 && !is_null(image) && !is_null(texture)) {
        // Ensure image size matches expected size
        int expected_size = width() * height() * 4; // RGBA8 = 4 bytes per pixel
        if (bytes.size() == expected_size) {
            image->set_data(width(), height(), false, SPRITE2D_IMAGE_FORMAT, bytes);
            texture->set_image(image);
        }
    }

    return bytes;
}

// Same as RiveViewerBase::redraw()
PackedByteArray RiveSprite2DBase::redraw() {
    auto artboard = inst.artboard();

    if (sk.surface && sk.renderer && exists(artboard)) {
        sk.clear();
        inst.draw(sk.renderer.get());
        PackedByteArray bytes = sk.bytes();
        return bytes;
    }

    return PackedByteArray();
}

void RiveSprite2DBase::on_input_event(const Ref<InputEvent>& event) {
    auto mouse_event = dynamic_cast<InputEventMouse*>(event.ptr());
    if (!mouse_event || Engine::get_singleton()->is_editor_hint()) return;

    Vector2 pos = mouse_event->get_position();

    if (auto mouse_button = dynamic_cast<InputEventMouseButton*>(event.ptr())) {
        if (!props.disable_press() && mouse_button->is_pressed()) {
            inst.press_mouse(pos);
            owner->emit_signal("pressed", mouse_event->get_position());
        } else if (!props.disable_press() && mouse_button->is_released()) {
            inst.release_mouse(pos);
            owner->emit_signal("released", mouse_event->get_position());
        }
    }
    if (auto mouse_motion = dynamic_cast<InputEventMouseMotion*>(event.ptr())) {
        if (!props.disable_hover()) inst.move_mouse(pos);
    }
}

void RiveSprite2DBase::check_scene_property_changed() {
    if (props.disable_hover() && props.disable_press()) return;
    
    auto scene = inst.scene();
    if (exists(scene)) {
        PackedStringArray input_names = scene->get_input_names();
        for (int i = 0; i < input_names.size(); i++) {
            String prop = input_names[i];
            Ref<RiveInput> input = scene->get_input(i);
            if (input.is_valid()) {
                Variant old_value = cached_scene_property_values.get(prop, input->get_default());
                Variant new_value = input->get_value();
                if (old_value != new_value) {
                    owner->emit_signal("scene_property_changed", scene, prop, new_value, old_value);
                }
                cached_scene_property_values[prop] = new_value;
            }
        }
    }
}

Vector2 RiveSprite2DBase::get_size() const {
    // For Sprite2D, we use a fixed size or get from the current texture
    Ref<Texture2D> tex = owner->get_texture();
    if (tex.is_valid()) {
        return tex->get_size();
    }
    return Vector2(256, 256); // Default size
}

int RiveSprite2DBase::width() const {
    Vector2 size = get_size();
    return std::max((int)size.x, 1);
}

int RiveSprite2DBase::height() const {
    Vector2 size = get_size();
    return std::max((int)size.y, 1);
}

void RiveSprite2DBase::_on_path_changed(String path) {
    try {
        inst.file = RiveFile::Load(path, sk.factory.get());
    } catch (RiveException error) {
        error.report();
    }

    if (exists(inst.file)) {
        if (inst.file->get_artboard_count() > 0) {
            props.artboard(0);
            inst.instantiate();

            auto artboard = inst.artboard();
            if (exists(artboard) && artboard->get_scene_count() > 0) {
                props.scene(0);
                props.animation(-1);
                inst.instantiate();
            } else if (exists(artboard) && artboard->get_animation_count() > 0) {
                props.animation(0);
                inst.instantiate();
            }
        }

        if (Engine::get_singleton()->is_editor_hint()) {
            owner->notify_property_list_changed();
        }
    }
}

void RiveSprite2DBase::_on_artboard_changed(int _index) {
    owner->notify_property_list_changed();
}

void RiveSprite2DBase::_on_scene_changed(int _index) {
    cached_scene_property_values.clear();
    owner->notify_property_list_changed();
}

void RiveSprite2DBase::_on_animation_changed(int _index) {
    // Method intentionally left minimal to avoid position offset issues
}

// This is the key method - same as RiveViewerBase::_on_size_changed()
void RiveSprite2DBase::_on_size_changed(float w, float h) {
    if (!is_null(image)) {
        unref(image);
    }
    if (!is_null(texture)) {
        unref(texture);
    }

    image = Image::create(width(), height(), false, SPRITE2D_IMAGE_FORMAT);
    texture = ImageTexture::create_from_image(image);
    
    // Set the texture on the Sprite2D
    owner->set_texture(texture);
}

// Same as RiveViewerBase::_on_transform_changed()
void RiveSprite2DBase::_on_transform_changed() {
    inst.current_transform = inst.get_transform();

    if (sk.renderer) {
        sk.renderer->transform(inst.current_transform);
    }

    PackedByteArray bytes = redraw();
    if (bytes.size() > 0 && !is_null(image) && !is_null(texture)) {
        int expected_size = width() * height() * 4; // RGBA8 = 4 bytes per pixel
        if (bytes.size() == expected_size) {
            image->set_data(width(), height(), false, SPRITE2D_IMAGE_FORMAT, bytes);
            texture->set_image(image);
        }
    }
}

bool RiveSprite2DBase::advance(float delta) {
    elapsed += delta;
    bool result = inst.advance(delta);
    check_scene_property_changed();
    return result;
}

void RiveSprite2DBase::get_property_list(List<PropertyInfo>* list) const {
    if (owner->is_node_ready()) {
        inst.instantiate();
        if (exists(inst.file)) {
            PackedStringArray artboard_names = inst.file->get_artboard_names();
            String artboard_hint = "";
            for (int i = 0; i < artboard_names.size(); i++) {
                if (i > 0) artboard_hint += ",";
                artboard_hint += artboard_names[i];
            }
            list->push_back(PropertyInfo(Variant::INT, "artboard", PROPERTY_HINT_ENUM, artboard_hint));
        }
        auto artboard = inst.artboard();
        if (exists(artboard)) {
            PackedStringArray scene_names = artboard->get_scene_names();
            String scene_hint = "";
            for (int i = 0; i < scene_names.size(); i++) {
                if (i > 0) scene_hint += ",";
                scene_hint += scene_names[i];
            }
            list->push_back(PropertyInfo(Variant::INT, "scene", PROPERTY_HINT_ENUM, scene_hint));
            PackedStringArray anim_names = artboard->get_animation_names();
            String anim_hint = "";
            for (int i = 0; i < anim_names.size(); i++) {
                if (i > 0) anim_hint += ",";
                anim_hint += anim_names[i];
            }
            list->push_back(PropertyInfo(Variant::INT, "animation", PROPERTY_HINT_ENUM, anim_hint));
        }
        auto scene = inst.scene();
        if (exists(scene)) {
            list->push_back(PropertyInfo(Variant::NIL, "Scene", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_CATEGORY));
            PackedStringArray input_names = scene->get_input_names();
            for (int i = 0; i < input_names.size(); i++) {
                Ref<RiveInput> input = scene->get_input(i);
                if (input.is_valid()) {
                    list->push_back(PropertyInfo(input->get_type(), input_names[i]));
                }
            }
        }
    }
}

bool RiveSprite2DBase::on_set(const StringName& prop, const Variant& value) {
    String name = prop;
    if (name == "artboard") {
        props.artboard((int)value);
        inst.instantiate();
        return true;
    }
    if (name == "scene") {
        props.scene((int)value);
        inst.instantiate();
        return true;
    }
    if (name == "animation") {
        props.animation((int)value);
        inst.instantiate();
        return true;
    }
    inst.instantiate();
    if (exists(inst.scene()) && inst.scene()->get_input_names().has(name)) {
        props.scene_property(name, value);
        return true;
    }
    return false;
}

bool RiveSprite2DBase::on_get(const StringName& prop, Variant& return_value) const {
    String name = prop;
    if (name == "artboard") {
        return_value = props.artboard();
        return true;
    }
    if (name == "scene") {
        return_value = props.scene();
        return true;
    }
    if (name == "animation") {
        return_value = props.animation();
        return true;
    }
    if (props.has_scene_property(name)) {
        return_value = props.scene_property(name);
        return true;
    }
    return false;
}

void RiveSprite2DBase::go_to_artboard(Ref<RiveArtboard> artboard_value) {
    try {
        if (is_null(artboard_value))
            throw RiveException("Attempted to go to null artboard").from(owner, "go_to_artboard").warning();
        props.artboard(artboard_value->get_index());
    } catch (RiveException error) {
        error.report();
    }
}

void RiveSprite2DBase::go_to_scene(Ref<RiveScene> scene_value) {
    try {
        if (is_null(scene_value))
            throw RiveException("Attempted to go to null scene").from(owner, "go_to_scene").warning();
        props.scene(scene_value->get_index());
    } catch (RiveException error) {
        error.report();
    }
}

void RiveSprite2DBase::go_to_animation(Ref<RiveAnimation> animation_value) {
    try {
        if (is_null(animation_value))
            throw RiveException("Attempted to go to null animation").from(owner, "go_to_animation").warning();
        props.animation(animation_value->get_index());
    } catch (RiveException error) {
        error.report();
    }
}