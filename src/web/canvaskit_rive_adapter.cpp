#include "canvaskit_rive_adapter.hpp"
#include "canvaskit_bridge.hpp"
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

// For now reuse the demo draw to produce a visible frame while we wire Rive paths.
extern PackedByteArray CK_DrawDemoToRGBA(int w, int h);

namespace CKRive {

PackedByteArray draw_solid_rect_rgba(int w, int h, const Color &color) {
  // TODO: Implement via JS bridge using CanvasKit paint + rect
  return CK_DrawDemoToRGBA(w, h);
}

PackedByteArray draw_polyline_rgba(int w, int h, const Array &points, float stroke_w, const Color &color) {
  // TODO: Implement via JS bridge building a Path from points
  return CK_DrawDemoToRGBA(w, h);
}

} // namespace CKRive

