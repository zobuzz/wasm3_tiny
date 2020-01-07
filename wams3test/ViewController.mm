//
//  ViewController.m
//  wams3test
//
//  Created by zorro on 2020/1/7.
//  Copyright © 2020 zorro. All rights reserved.
//

#import "ViewController.h"


#include "m3.h"
#include "m3_api_wasi.h"
#include "m3_api_libc.h"
#include "m3_env.h"

#include "extra/fib32.wasm.h"

#define FATAL(msg, ...) { printf("Error: [Fatal] " msg "\n", ##__VA_ARGS__); }

extern "C"
{
    void repl_free(IM3Runtime* runtime)
    {
        if (*runtime) {
            m3_FreeRuntime (*runtime);
        }
    }
    
    M3Result repl_init(IM3Environment env, IM3Runtime* runtime)
    {
        repl_free(runtime);
        *runtime = m3_NewRuntime (env, 64*1024, NULL);
        if (*runtime == NULL) {
            return "m3_NewRuntime failed";
        }
        return m3Err_none;
    }
    
    M3Result repl_load  (IM3Runtime runtime, const char* fn)
    {
        M3Result result = m3Err_none;
        
        u8* wasm = (u8*)fib32_wasm;
        u32 fsize = fib32_wasm_len;

        IM3Module module;
        result = m3_ParseModule (runtime->environment, &module, wasm, fsize);
        if (result) return result;
        
        result = m3_LoadModule (runtime, module);
        if (result) return result;
        
        result = m3_LinkSpecTest (runtime->modules);
        
        return result;
    }
    
}


@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    M3Result result = m3Err_none;
    
    IM3Environment env = m3_NewEnvironment ();
    IM3Runtime runtime = NULL;

    result = repl_init(env, &runtime);
    result = repl_load(runtime, "");
    if (result) FATAL("repl_load: %s", result);
    
    result = m3_LinkWASI (runtime->modules);
    if (result) FATAL("m3_LinkWASI: %s", result);
    
    result = m3_LinkLibC (runtime->modules);
    if (result) FATAL("m3_LinkLibC: %s", result);

    printf("Finish Wasm3 Init!!");
}


@end
