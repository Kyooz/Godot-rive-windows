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
};

