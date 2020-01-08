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


M3Result  tiny_LinkWASI  (IM3Module module)
{
    M3Result result = m3Err_none;

    const char* wasi  = "env";

_   (SuppressLookupFailure (m3_LinkRawFunction (module, wasi, "sum",       "i(ii)",   &tiny_sum)));
_catch:
    return result;
}