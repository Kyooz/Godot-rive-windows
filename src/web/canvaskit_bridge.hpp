#pragma once
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/variant/string.hpp>

class CanvasKitBridge : public godot::Object {
  GDCLASS(CanvasKitBridge, godot::Object);
public:
  static void _bind_methods();
  static void load();
  static bool is_ready();
  static godot::String version();

// Draw a demo rectangle/circle into an RGBA8 buffer and return as PackedByteArray
// Only available on Web builds.
namespace godot { class PackedByteArray; }
godot::PackedByteArray CK_DrawDemoToRGBA(int w, int h);
// Draw a stroked polyline to RGBA8
namespace godot { class Vector2; template <class T> class TypedArray; }
godot::PackedByteArray CK_DrawPolylineToRGBA(int w, int h, const godot::TypedArray<godot::Vector2>& pts, float stroke_w, const godot::Color& color);

};

