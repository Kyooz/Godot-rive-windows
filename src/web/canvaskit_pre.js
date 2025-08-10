// Pre-JS injected by Emscripten for the Web GDExtension side module.
// It dynamically loads CanvasKit (Skia WASM) from CDN and exposes readiness helpers.

(function(){
  var CK_URL_BASE = 'https://unpkg.com/canvaskit-wasm@0.38.2/bin/';
  var CK_JS = CK_URL_BASE + 'canvaskit.js';
  var CK_WASM = CK_URL_BASE + 'canvaskit.wasm';

  var _ckState = { loaded: false, loading: false, error: null, CanvasKit: null };

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

  // Expose functions on Module for EM_JS to call.
  if (typeof Module === 'undefined') { Module = {}; }
  Module._gdext_ck_load = function(){ loadCanvasKit(); };
  Module._gdext_ck_ready = function(){ return _ckState.loaded ? 1 : 0; };
  Module._gdext_ck_version = function(){ try { return _ckState.CanvasKit ? _ckState.CanvasKit.SKIA_VERSION : ''; } catch(e){ return ''; } };
})();

