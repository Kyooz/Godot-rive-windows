#ifndef RIVEEXTENSION_RIVE_SPRITE2D_H
#define RIVEEXTENSION_RIVE_SPRITE2D_H

// godot-cpp
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/input_event.hpp>

// extension
#include "rive_sprite2d_base.h"

using namespace godot;

/**
 * RiveSprite2D - Optimized Rive node that inherits from Sprite2D
 * 
 * Features:
 * - All RiveViewer functionality (.riv files, timelines, state machines)
 * - Native Sprite2D transformation support (scale, rotation, position)
 * - GPU-accelerated rendering when available
 * - Optimized performance with frame skipping and visibility culling
 * - Reduced CPU overhead compared to Control-based RiveViewer
 * 
 * Usage:
 * - Drop-in replacement for RiveViewer with Sprite2D benefits
 * - Use scale, rotation, and position properties normally
 * - Load .riv files via file_path property
 * - Control animations via scene and animation properties
 * - Handle input events via pressed/released signals
 */
class RiveSprite2D : public Sprite2D {
    GDCLASS(RiveSprite2D, Sprite2D);
    RIVE_SPRITE2D_WRAPPER(RiveSprite2D);

protected:
    static void _bind_methods() {
        RIVE_SPRITE2D_BIND(RiveSprite2D);
    }

public:
    RiveSprite2D() = default;
    ~RiveSprite2D() = default;
    
    // Input handling optimized for Sprite2D
    void _input(const Ref<InputEvent>& event) override {
        base.on_input_event(event);
    }
    
    // Custom size handling
    Vector2 get_size() const {
        return base.get_size();
    }
};

#endif // RIVEEXTENSION_RIVE_SPRITE2D_H
