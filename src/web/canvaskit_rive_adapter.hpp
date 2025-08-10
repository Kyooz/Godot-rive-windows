#pragma once
#include <godot_cpp/variant/packed_byte_array.hpp>
#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/color.hpp>

namespace CKRive {
// Minimal adapter interface to bridge Rive drawing to CanvasKit in Web.
// This is a skeleton to be filled as we map more primitives.

// Fill solid rectangle into RGBA8 buffer using CanvasKit. Returns empty on failure.
godot::PackedByteArray draw_solid_rect_rgba(int w, int h, const godot::Color &color);

// Draw a polyline path with stroke; points are in pixel space.
// width = canvas width, height = canvas height; stroke_w in pixels.
godot::PackedByteArray draw_polyline_rgba(int w, int h, const godot::Array &points, float stroke_w, const godot::Color &color);
}

