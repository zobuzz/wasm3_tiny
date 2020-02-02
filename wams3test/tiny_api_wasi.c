//
//  tiny_api_wasi.c
//  wams3test
//
//  Created by Xingwei Zhu on 2020/1/8.
//  Copyright © 2020 zorro. All rights reserved.
//

#include "tiny_api_wasi.h"

#include "m3_api_defs.h"
#include "m3_env.h"
#include "m3_exception.h"
#include "emscripten_fetch.h"

#include <time.h>
#include <unistd.h>


static
M3Result  SuppressLookupFailure (M3Result i_result)
{
    if (i_result == m3Err_functionLookupFailed)
        return m3Err_none;
    else
        return i_result;
}

m3ApiRawFunction(tiny_sum)
{
    m3ApiReturnType (int32_t)

    m3ApiGetArg     (int32_t, value_1)
    m3ApiGetArg     (int32_t, value_2)

    u32 result = value_1 + value_2;
    m3ApiReturn(result);
}

m3ApiRawFunction(tiny_log)
{
    m3ApiReturnType (int32_t)
    
    m3ApiGetArg     (int32_t, value_1)
    
    printf("[tiny_log] print a tiny log %d \n", value_1);
    
    u32 result = 0;
    m3ApiReturn(result);
}

m3ApiRawFunction(emscripten_get_now)
{
    m3ApiReturnType (double_t)
    
    f64 result = 0.0;
    
    struct timespec tp;
    if (!clock_gettime(CLOCK_MONOTONIC, &tp)) {
        result = tp.tv_sec * 1000.0 + tp.tv_nsec / 1000000.0;
    }
    
    printf("[tiny_log] call emscripten_get_now %lf\n", result);
    m3ApiReturn(result)
}

m3ApiRawFunction(__syscall4)
{
    m3ApiReturnType (int32_t)
    
    m3ApiGetArg     (int32_t, which) // should be 4.
    m3ApiGetArgMem  (u32*,    args)
    
    int fd = args[0];
    char *buf = (char *)m3ApiOffsetToPtr(args[1]);
    size_t nbyte = args[2];
    
    m3ApiReturn(write(fd, buf, nbyte));
}

typedef struct AnimationFrameCallback
{
    IM3Function function;
    i32 userData;
} AnimationFrameCallback;

AnimationFrameCallback __raf;

void runAnimationFrame(f64 timeStamp)
{
    if (__raf.function) {
        AnimationFrameCallback raf = __raf;
        memset(&__raf, 0, sizeof(__raf));
        
        M3Result result = m3Err_none;
        
        u64 r;
        u64 args[2] = {timeStamp, raf.userData};
        size_t count = sizeof(args);
_       (m3_CallDirect(raf.function, args, &r))
        if (r) {
            __raf = raf;
        }
        
    _catch:
        if (result)
            fprintf (stderr, "error running runAnimationFrame: %s\n", result);
    }
}

m3ApiRawFunction(js_html_initImageLoading)
{
//    m3ApiReturnType (int32_t)
//
//    printf("[tiny_log] call js_html_initImageLoading\n");
//
//    u32 result = 0;
//    m3ApiReturn(result);
    return m3Err_none;
}
//v(ii)
m3ApiRawFunction(emscripten_request_animation_frame_loop)
{
    m3ApiReturnType (int32_t)
    
    printf("[tiny_log] call emscripten_request_animation_frame_loop\n");
    
    M3Result result = m3Err_none;
    
    m3ApiGetArg     (i32, cb)
    m3ApiGetArg     (i32, userdata)

    IM3Function function = NULL;

    IM3Module module = runtime->modules; // for now, assume it's always the first module.
_   (m3_FindIndirectFunction(&function, module, cb));
    __raf.function = function;
    __raf.userData = userdata;
    
_catch:
    return result;
}
//type3 v(i)
m3ApiRawFunction(emscripten_throw_string)
{
    m3ApiReturnType (int32_t)

    M3Result result = m3Err_none;
    
    m3ApiGetArgMem  (char*, buf)

    printf("[tiny_log] call emscripten_throw_string arg:%s\n", buf);
    
    return m3Err_trapAbort;
}
//type8  i()
m3ApiRawFunction(emscripten_is_main_browser_thread)
{
    m3ApiReturnType (i32)
    
////    m3ApiGetArg     (int32_t, value_1)
//
//    printf("[tiny_log] call emscripten_is_main_browser_thread arg:%d\n", value_1);
    
    u32 result = 1;
    m3ApiReturn(result);
}
//v()
m3ApiRawFunction(js_html_init)
{
    return m3Err_none;
}
//v(ii)
m3ApiRawFunction(js_html_getScreenSize)
{
    m3ApiGetArgMem  (u32*,    wPtr)
    m3ApiGetArgMem  (u32*,    hPtr)
    
    *wPtr = 375;
    *hPtr = 667;
    
    return m3Err_none;
}
//i(iii)
m3ApiRawFunction(js_html_setCanvasSize)
{
    m3ApiReturnType (i32)
    m3ApiGetArg  (i32,    weight)
    m3ApiGetArg  (i32,    height)
    m3ApiGetArg  (i32,    webgl)
    
    m3ApiReturn(1)
}
//i()
m3ApiRawFunction(js_inputInit)
{
    m3ApiReturnType (i32)
    m3ApiReturn(1)
}
//v()
m3ApiRawFunction(js_inputResetStreams)
{
    return m3Err_none;
}
//v(i)
m3ApiRawFunction(js_inputSetMouseMode)
{
    m3ApiGetArg  (i32,    arg1)
    return m3Err_none;
}
//i(ii)
m3ApiRawFunction(js_inputGetKeyStream)
{
    m3ApiReturnType (i32)
    m3ApiGetArg  (i32,    arg1)
    m3ApiGetArg  (i32,    arg2)
    m3ApiReturn(0)
}
//i()
m3ApiRawFunction(js_inputGetCanvasLost)
{
    m3ApiReturnType (i32)
    m3ApiReturn(0)
}

M3Result  tiny_LinkWASI  (IM3Module module)
{
    M3Result result = m3Err_none;

    const char* wasi  = "env";

_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "sum", "i(ii)", &tiny_sum)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "tiny_log", "i(i)", &tiny_log)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "emscripten_get_now", "F()", &emscripten_get_now)));
    
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "emscripten_performance_now", "F()", &emscripten_get_now)));
    
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "__syscall4", "i(ii)", &__syscall4)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "js_html_initImageLoading", "v()", &js_html_initImageLoading)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "emscripten_request_animation_frame_loop", "v(ii)", &emscripten_request_animation_frame_loop)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "emscripten_throw_string", "v(i)", &emscripten_throw_string)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "emscripten_is_main_browser_thread", "i()", &emscripten_is_main_browser_thread)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "emscripten_start_fetch", "v(i)", &emscripten_start_fetch)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "js_html_init", "v()",   &js_html_init)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "js_html_getScreenSize",  "v(ii)", &js_html_getScreenSize)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "js_html_getCanvasSize",  "v(ii)", &js_html_getScreenSize)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "js_html_getFrameSize", "v(ii)", &js_html_getScreenSize)));

_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "js_html_setCanvasSize", "i(iii)", &js_html_setCanvasSize)));
    
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "js_inputInit", "i()", &js_inputInit)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "js_inputResetStreams", "v()", &js_inputResetStreams)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "js_inputSetMouseMode", "v(i)", &js_inputSetMouseMode)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "js_inputGetKeyStream", "i(ii)", &js_inputGetKeyStream)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "js_inputGetMouseStream", "i(ii)", &js_inputGetKeyStream)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "js_inputGetTouchStream", "i(ii)", &js_inputGetKeyStream)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "js_inputGetCanvasLost", "i()", &js_inputGetCanvasLost)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "js_inputGetFocusLost", "i()", &js_inputGetCanvasLost)));

_catch:
    return result;
}
