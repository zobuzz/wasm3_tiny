//
//  tiny_api_wasi.h
//  wams3test
//
//  Created by Xingwei Zhu on 2020/1/8.
//  Copyright © 2020 zorro. All rights reserved.
//

#ifndef tiny_api_wasi_h
#define tiny_api_wasi_h

#include "m3_core.h"

# if defined(__cplusplus)
extern "C" {
# endif

    M3Result    tiny_LinkWASI     (IM3Module io_module);

    void runAnimationFrame(f64 timeStamp);

#if defined(__cplusplus)
}
# endif

#endif /* tiny_api_wasi_h */
