// Pre-JS injected by Emscripten for the Web GDExtension side module.
// It dynamically loads CanvasKit (Skia WASM) from CDN and exposes readiness helpers.

(function(){
  var CK_URL_BASE = 'https://unpkg.com/canvaskit-wasm@0.38.2/bin/';
  var CK_JS = CK_URL_BASE + 'canvaskit.js';
  var CK_WASM = CK_URL_BASE + 'canvaskit.wasm';

  var _ckState = { loaded: false, loading: false, error: null, CanvasKit: null, surface: null, canvas: null, w: 0, h: 0 };

  function ensureScript(src){
    return new Promise(function(resolve, reject){
      var s = document.createElement('script');
      s.src = src;
      s.async = true;
      s.onload = resolve;
      s.onerror = function(){ reject(new Error('Failed to load ' + src)); };
      document.head.appendChild(s);
    });
  }

  function loadCanvasKit(){
    if (_ckState.loaded || _ckState.loading) return _ckState.promise || Promise.resolve();
    _ckState.loading = true;
    _ckState.promise = ensureScript(CK_JS).then(function(){
      if (typeof CanvasKitInit !== 'function') throw new Error('CanvasKitInit not found');
      return CanvasKitInit({ locateFile: function(file){ return CK_URL_BASE + file; } });
    }).then(function(CK){
      _ckState.CanvasKit = CK;
      _ckState.loaded = true;
      console.log('[GDExt] CanvasKit loaded, version:', CK.SKIA_VERSION);
    }).catch(function(err){
      _ckState.error = String(err);
      console.error('[GDExt] CanvasKit load error:', err);
    });
    return _ckState.promise;
  }

  function ensureSurface(w, h){
    if (!_ckState.loaded) return false;
    if (_ckState.surface && _ckState.w === w && _ckState.h === h) return true;
    // Create or resize a hidden canvas for offscreen rendering
    if (!_ckState.canvas){
      var c = document.createElement('canvas');
      c.width = w; c.height = h; c.style.display = 'none';
      document.body.appendChild(c);
      _ckState.canvas = c;
    } else {
      _ckState.canvas.width = w; _ckState.canvas.height = h;
    }
    _ckState.w = w; _ckState.h = h;
    if (_ckState.surface){ _ckState.surface.dispose(); _ckState.surface = null; }
    // Use 2D Canvas backend to keep things simple for readPixels
    _ckState.surface = _ckState.CanvasKit.MakeCanvasSurface(_ckState.canvas);
    if (!_ckState.surface){ console.error('[GDExt] Failed to make CanvasSurface'); return false; }
    return true;
  }

  function drawDemo(w, h){
    var sk = _ckState.CanvasKit;
    var surface = _ckState.surface;
    var canvas = surface.getCanvas();
    canvas.clear(sk.TRANSPARENT);

    var paint = new sk.Paint();
    paint.setAntiAlias(true);
    paint.setStyle(sk.PaintStyle.Fill);
    paint.setColor(sk.Color(66, 135, 245, 255));

    // Draw a rectangle
    canvas.drawRect(sk.LTRBRect(w*0.1, h*0.1, w*0.9, h*0.9), paint);

    // Draw a circle outline
    var stroke = new sk.Paint();
    stroke.setAntiAlias(true);
    stroke.setStyle(sk.PaintStyle.Stroke);
    stroke.setStrokeWidth(4.0);
    stroke.setColor(sk.Color(255, 80, 80, 255));
    canvas.drawCircle(w*0.5, h*0.5, Math.min(w,h)*0.35, stroke);

    paint.delete(); stroke.delete();
    surface.flush();
  }

  // Expose functions on Module for EM_JS to call.
  if (typeof Module === 'undefined') { Module = {}; }
  Module._gdext_ck_load = function(){ loadCanvasKit(); };
  Module._gdext_ck_ready = function(){ return _ckState.loaded ? 1 : 0; };
  Module._gdext_ck_version = function(){ try { return _ckState.CanvasKit ? _ckState.CanvasKit.SKIA_VERSION : ''; } catch(e){ return ''; } };

  // Draw and readback into linear RGBA8 buffer at ptr (Emscripten heap)
  Module._gdext_ck_draw_rgba = function(w, h, ptr, bufSize){
    try{
      if (!_ckState.loaded) return 0;
      if (!ensureSurface(w,h)) return 0;
      drawDemo(w,h);
      var pixels = new Uint8Array(w*h*4);
      var ok = _ckState.surface.readPixels({width:w, height:h, colorType:_ckState.CanvasKit.ColorType.RGBA_8888, alphaType:_ckState.CanvasKit.AlphaType.Unpremul}, pixels, 4*w);
      if (!ok) return 0;
      var need = pixels.byteLength;
      if (bufSize < need) return 0;
      Module.HEAPU8.set(pixels, ptr);
      return need;
    } catch(e){ console.error('[GDExt] draw_rgba error', e); return 0; }
  };
})();

