#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

extern "C" {
EM_JS(void, gdext_ck_load, (), {
  if (typeof Module !== 'undefined' && Module._gdext_ck_load) Module._gdext_ck_load();
});

EM_JS(int, gdext_ck_ready, (), {
  if (typeof Module !== 'undefined' && Module._gdext_ck_ready) return Module._gdext_ck_ready();
  return 0;
});

EM_JS(const char*, gdext_ck_version, (), {
  if (typeof Module !== 'undefined' && Module._gdext_ck_version) {
    var s = Module._gdext_ck_version();
    var buf = lengthBytesUTF8(s) + 1;
    var ptr = _malloc(buf);
    stringToUTF8(s, ptr, buf);
    return ptr;
  }
  var ptr0 = _malloc(1);
  HEAP8[ptr0] = 0;
  return ptr0;
});
}

class CanvasKitBridge : public Object {
  GDCLASS(CanvasKitBridge, Object);
public:
  static void _bind_methods() {
    ClassDB::bind_static_method("CanvasKitBridge", D_METHOD("load"), &CanvasKitBridge::load);
    ClassDB::bind_static_method("CanvasKitBridge", D_METHOD("is_ready"), &CanvasKitBridge::is_ready);
    ClassDB::bind_static_method("CanvasKitBridge", D_METHOD("version"), &CanvasKitBridge::version);
  }

  static void load() { gdext_ck_load(); }
  static bool is_ready() { return gdext_ck_ready() != 0; }
  static String version() {
    const char* c = gdext_ck_version();
    String s = String::utf8(c);
    free((void*)c);
    return s;
  }
};

// Register on web only; safe if compiled elsewhere but intended for web.
void register_canvaskit_bridge_types() { ClassDB::register_class<CanvasKitBridge>(); }
void unregister_canvaskit_bridge_types() {}

