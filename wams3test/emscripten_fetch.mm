//
//  emscripten_fetch.c
//  wams3test
//
//  Created by kg on 2020/1/13.
//  Copyright © 2020 zorro. All rights reserved.
//

#include "emscripten_fetch.h"

#import <Foundation/Foundation.h>
#include "m3_env.h"
#include "m3_exception.h"

m3ApiRawFunction(_emscripten_fetch_free)
{
    M3Result result = m3Err_none;
    return result;
}

static void runOnMainThread(void (^block)(void))
{
    if (!block) return;

    if ( [[NSThread currentThread] isMainThread] ) {
        block();
    } else {
        dispatch_async(dispatch_get_main_queue(), block);
    }
}

m3ApiRawFunction(emscripten_start_fetch)
{
    M3Result result = m3Err_none;
    char *url = NULL;
    char* requestMethod = NULL;
    NSURL *nsbaseurl = nil;
    NSURL *nsurl = nil;
    NSString *nsrequestMethod = nil;
    NSMutableURLRequest *request = nil;
    id completionHandler = nil;
    NSURLSessionDataTask *task = nil;
    
    m3ApiGetArg(u32, fetchptr)
    emscripten_fetch_t *fetch = (emscripten_fetch_t *)m3ApiOffsetToPtr(fetchptr);
    
    int onerror = fetch->__attributes.onerror;
    int onsuccess = fetch->__attributes.onsuccess;
    
    int urlptr = fetch->url;
    if (!urlptr) {
        fprintf(stderr, "error in emscripten_start_fetch: fetch->url is 0");
        return m3Err_trapAbort;
    }
    
    IM3Function onerrorFunc = NULL;
    IM3Function onsuccessFunc = NULL;
    
    IM3Module module = runtime->modules; // for now, assume it's always the first module.
    if (onerror) {
_       (m3_FindIndirectFunction(&onerrorFunc, module, onerror))
    }
    if (onsuccess) {
_       (m3_FindIndirectFunction(&onsuccessFunc, module, onsuccess))
    }
    
    url = (char *)m3ApiOffsetToPtr(urlptr);
    requestMethod = fetch->__attributes.requestMethod;
    
    //nsbaseurl = [NSURL URLWithString: @"http://10.86.98.112:8080/"];
    nsbaseurl = [NSURL URLWithString: @"http://192.168.0.106:8080/"];
    nsurl = [NSURL URLWithString: [NSString stringWithUTF8String: url] relativeToURL: nsbaseurl];
    nsrequestMethod = [NSString stringWithUTF8String: requestMethod];
    
    request = [NSMutableURLRequest requestWithURL: nsurl];
    [request setHTTPMethod: nsrequestMethod];
    
    static IM3Function mallocFunc = NULL;
    if (!mallocFunc) {
_       (m3_FindFunction(&mallocFunc, runtime, "malloc"))
    }
    
    completionHandler = ^(NSData *receivedData, NSURLResponse *response, NSError *error) {runOnMainThread(^{
        M3Result result = m3Err_none;
        
        if (error) {
            fetch->data = 0;
            fetch->numBytes = 0;
            fetch->dataOffset = 0;
            fetch->totalBytes = 0;
            fetch->readyState = 4;
            fetch->status = 500;
            
            u64 args[] = {fetchptr};
_           (m3_CallDirect(onerrorFunc, args, NULL))
        } else {
            NSHTTPURLResponse *httpResponse = (NSHTTPURLResponse *) response;
            
            u64 len = [receivedData length];
            
            u64 bytePtr;
            u64 malloc_args[] = {len};
            m3_CallDirect(mallocFunc, malloc_args, &bytePtr);
            
            printf("successfully fetch http-server filesize:%ld", len);
            char *bytes = NULL;
            bytes = (char *)m3ApiOffsetToPtr(bytePtr);
            
            memcpy(bytes, [receivedData bytes], len);
            
            fetch->data = bytePtr;
            fetch->numBytes = len;
            fetch->dataOffset = 0;
            fetch->totalBytes = len;
            
            fetch->readyState = 4;
            fetch->status = httpResponse.statusCode;
            
            if (httpResponse.statusCode >= 200 && httpResponse.statusCode < 300) {
                u64 args[] = {fetchptr};
_               (m3_CallDirect(onsuccessFunc, args, NULL))
            } else {
                u64 args[] = {fetchptr};
_               (m3_CallDirect(onerrorFunc, args, NULL))
            }
        }
        
    _catch:
        if (result) {
            fprintf(stderr, "error in dataTask completionHandler: %s.", result);
        }
    });};
    
    task = [[NSURLSession sharedSession]
            dataTaskWithRequest:request completionHandler:completionHandler];
    [task resume];

_catch:
    return result;
}
