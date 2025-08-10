#pragma once
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/variant/vector2.hpp>
#include <godot_cpp/variant/color.hpp>
#include <rive/shapes/paint/fill_rule.hpp>
#include <rive/shapes/paint/stroke_cap.hpp>
#include <rive/shapes/paint/stroke_join.hpp>

namespace CKRive {
struct StrokeStyle {
  float width = 1.0f;
  rive::StrokeCap cap = rive::StrokeCap::butt;
  rive::StrokeJoin join = rive::StrokeJoin::miter;
  float miter_limit = 4.0f;
};

struct Path2D {
  godot::TypedArray<godot::Vector2> points;
  bool closed = false;
};

// Convert a simple Rive path/contour into point list (temporary simplification).
Path2D convert_simple_polyline(/* TODO: rive path ref */);
}

