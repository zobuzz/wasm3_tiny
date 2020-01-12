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
    
    printf("[tiny_log] call emscripten_get_now\n");
    
    f64 result = 0;
    m3ApiReturn(result);
}

m3ApiRawFunction(__syscall4)
{
    m3ApiReturnType (int32_t)
    
    m3ApiGetArg     (int32_t, value_1)
    m3ApiGetArg     (int32_t, value_2)
    printf("[tiny_log] call __syscall4\n");

    u32 result = 0;
    m3ApiReturn(result);
}

//TODO by sj
m3ApiRawFunction(js_html_initImageLoading)
{
    m3ApiReturnType (int32_t)
    
    printf("[tiny_log] call js_html_initImageLoading\n");
    
    u32 result = 0;
    m3ApiReturn(result);
}

m3ApiRawFunction(emscripten_request_animation_frame_loop)
{
    m3ApiReturnType (int32_t)
    
    printf("[tiny_log] call emscripten_request_animation_frame_loop\n");
    
    u32 result = 0;
    m3ApiReturn(result);
}
//type3 v(i)
m3ApiRawFunction(emscripten_throw_string)
{
    m3ApiReturnType (int32_t)

    m3ApiGetArg     (int32_t, value_1)

    printf("[tiny_log] call emscripten_throw_string arg:%d\n", value_1);
    
    u32 result = 0;
    m3ApiReturn(result);
}
//type8  i()
m3ApiRawFunction(emscripten_is_main_browser_thread)
{
    m3ApiReturnType (int32_t)
    
    printf("[tiny_log] call emscripten_is_main_browser_thread\n");
    
    u32 result = 1;
    m3ApiReturn(result);
}
//type3 v(i)
m3ApiRawFunction(_emscripten_start_fetch)
{
    m3ApiReturnType (int32_t)
    
    m3ApiGetArg     (int32_t, value_1)

    printf("[tiny_log] call _emscripten_start_fetch arg:%d\n", value_1);
    
    u32 result = 0;
    m3ApiReturn(result);
}



M3Result  tiny_LinkWASI  (IM3Module module)
{
    M3Result result = m3Err_none;

    const char* wasi  = "env";

_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "sum", "i(ii)", &tiny_sum)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "tiny_log", "i(i)", &tiny_log)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "emscripten_get_now", "F()", &emscripten_get_now)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "__syscall4", "i(ii)", &__syscall4)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "js_html_initImageLoading", "v()", &js_html_initImageLoading)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "emscripten_request_animation_frame_loop", "v(ii)", &emscripten_request_animation_frame_loop)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "emscripten_throw_string", "v(i)", &emscripten_throw_string)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "emscripten_is_main_browser_thread", "i()", &emscripten_is_main_browser_thread)));
_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "emscripten_start_fetch", "v(i)", &_emscripten_start_fetch)));

_catch:
    return result;
}
