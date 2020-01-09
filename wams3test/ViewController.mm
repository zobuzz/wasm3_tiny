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
#include "tiny_api_wasi.h"
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

    M3Result repl_call  (IM3Runtime runtime, const char* name)
    {
        M3Result result = m3Err_none;

        IM3Function func;
        result = m3_FindFunction (&func, runtime, name);
        if (result) return result;

        result = m3_CallWithArgs (func, 0, nil);
        if (result) return result;

        return result;
}
    
    M3Result repl_load  (IM3Runtime runtime, const char* fn)
    {
        M3Result result = m3Err_none;
        
        //NSString* path = [[NSBundle mainBundle] pathForResource:@"wasm3_tiny_data/example.wasm"
        //NSString* path = [[NSBundle mainBundle] pathForResource:@"wasm3_tiny_data/Tiny3D.wasm"
        NSString* path = [[NSBundle mainBundle] pathForResource:@"wasm3_tiny_data/tiny3d_hooters.wasm"
        ofType:@""];
        
        NSURL *fileUrl = [NSURL fileURLWithPath:path];
        NSData *fileData = [NSData dataWithContentsOfURL:fileUrl];
        
        u8* wasm = (u8*)fileData.bytes;
        u32 fsize = fileData.length;

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
    
    result = tiny_LinkWASI(runtime->modules);
    if (result) FATAL("tiny_LinkWASI: %s", result);
    
    result = repl_call(runtime, "_start");


    printf("Finish Wasm3 Init!!");
}


@end
