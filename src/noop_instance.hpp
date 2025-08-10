#ifndef _RIVEEXTENSION_NOOP_INSTANCE_HPP_
#define _RIVEEXTENSION_NOOP_INSTANCE_HPP_

// stdlib
#include <cstdio>

// godot-cpp
#include <godot_cpp/variant/builtin_types.hpp>

// rive
#include <utils/no_op_factory.hpp>
#include <rive/renderer.hpp>

// extension
#include "utils/types.hpp"
#include "viewer_props.hpp"

using namespace godot;
using namespace rive;

// Stub renderer class for NoOp implementation
class NoOpRenderer : public Renderer {
public:
    void save() override {}
    void restore() override {}
    void transform(const Mat2D& transform) override {}
    void clipPath(RenderPath* path) override {}
    void drawPath(RenderPath* path, RenderPaint* paint) override {}
    void drawImage(const RenderImage* image, BlendMode blendMode, float opacity) override {}
    void drawImageMesh(const RenderImage* image, rcp<RenderBuffer> vertices_f32, rcp<RenderBuffer> uvCoords_f32, rcp<RenderBuffer> indices_u16, uint32_t vertexCount, uint32_t indexCount, BlendMode blendMode, float opacity) override {}
};

struct NoOpInstance {
    ViewerProps *props;
    Ptr<NoOpFactory> factory = rivestd::make_unique<NoOpFactory>();
    Ptr<NoOpRenderer> renderer = rivestd::make_unique<NoOpRenderer>();
    bool surface = true; // Fake surface existence

    void set_props(ViewerProps *p) {
        props = p;
    }

    void resize(int width, int height) {
        // No-op implementation
    }

    void clear() {
        // No-op implementation
    }

    void render() {
        // No-op implementation
    }

    PackedByteArray bytes() {
        // Return empty array for no-op implementation
        return PackedByteArray();
    }

    PackedByteArray get_pixels() {
        // Return empty array for no-op implementation
        return PackedByteArray();
    }
};

#endif
