//
//  emscripten_fetch.h
//  wams3test
//
//  Created by kg on 2020/1/13.
//  Copyright © 2020 zorro. All rights reserved.
//

#ifndef emscripten_fetch_h
#define emscripten_fetch_h


#include <stdint.h>
#include "m3_api_defs.h"

# if defined(__cplusplus)
extern "C" {
# endif

struct emscripten_fetch_t;

// Specifies the parameters for a newly initiated fetch operation.
typedef struct emscripten_fetch_attr_t
{
    // 'POST', 'GET', etc.
    char requestMethod[32];

    // Custom data that can be tagged along the process.
    int /*void **/userData;

    int onsuccess; // void (*onsuccess)(struct emscripten_fetch_t *fetch);
    int onerror; // void (*onerror)(struct emscripten_fetch_t *fetch);
    int onprogress; // void (*onprogress)(struct emscripten_fetch_t *fetch);
    int onreadystatechange; //void (*onreadystatechange)(struct emscripten_fetch_t *fetch);

    // EMSCRIPTEN_FETCH_* attributes
    uint32_t attributes;

    // Specifies the amount of time the request can take before failing due to a timeout.
    unsigned long timeoutMSecs;

    // Indicates whether cross-site access control requests should be made using credentials.
    int /*EM_BOOL*/ withCredentials;

    // Specifies the destination path in IndexedDB where to store the downloaded content body. If this is empty, the transfer
    // is not stored to IndexedDB at all.
    // Note that this struct does not contain space to hold this string, it only carries a pointer.
    // Calling emscripten_fetch() will make an internal copy of this string.
    int /*const char **/destinationPath;

    // Specifies the authentication username to use for the request, if necessary.
    // Note that this struct does not contain space to hold this string, it only carries a pointer.
    // Calling emscripten_fetch() will make an internal copy of this string.
    int /*const char **/userName;

    // Specifies the authentication username to use for the request, if necessary.
    // Note that this struct does not contain space to hold this string, it only carries a pointer.
    // Calling emscripten_fetch() will make an internal copy of this string.
    int /*const char **/password;

    // Points to an array of strings to pass custom headers to the request. This array takes the form
    // {"key1", "value1", "key2", "value2", "key3", "value3", ..., 0 }; Note especially that the array
    // needs to be terminated with a null pointer.
    int /*const char * const **/requestHeaders;

    // Pass a custom MIME type here to force the browser to treat the received data with the given type.
    int /*const char **/overriddenMimeType;

    // If non-zero, specifies a pointer to the data that is to be passed as the body (payload) of the request
    // that is being performed. Leave as zero if no request body needs to be sent.
    // The memory pointed to by this field is provided by the user, and needs to be valid throughout the
    // duration of the fetch operation. If passing a non-zero pointer into this field, make sure to implement
    // *both* the onsuccess and onerror handlers to be notified when the fetch finishes to know when this memory
    // block can be freed. Do not pass a pointer to memory on the stack or other temporary area here.
    int /*const char **/requestData;

    // Specifies the length of the buffer pointed by 'requestData'. Leave as 0 if no request body needs to be sent.
    size_t requestDataSize;
} emscripten_fetch_attr_t;

typedef struct emscripten_fetch_t
{
    // Unique identifier for this fetch in progress.
    unsigned int id;

    // Custom data that can be tagged along the process.
    int /*void **/userData;

    // The remote URL that is being downloaded.
    int /*const char **/url;

    // In onsuccess() handler:
    //   - If the EMSCRIPTEN_FETCH_LOAD_TO_MEMORY attribute was specified for the transfer, this points to the
    //     body of the downloaded data. Otherwise this will be null.
    // In onprogress() handler:
    //   - If the EMSCRIPTEN_FETCH_STREAM_DATA attribute was specified for the transfer, this points to a partial
    //     chunk of bytes related to the transfer. Otherwise this will be null.
    // The data buffer provided here has identical lifetime with the emscripten_fetch_t object itself, and is freed by
    // calling emscripten_fetch_close() on the emscripten_fetch_t pointer.
    int /*const char **/data;

    // Specifies the length of the above data block in bytes. When the download finishes, this field will be valid even if
    // EMSCRIPTEN_FETCH_LOAD_TO_MEMORY was not specified.
    uint64_t numBytes;

    // If EMSCRIPTEN_FETCH_STREAM_DATA is being performed, this indicates the byte offset from the start of the stream
    // that the data block specifies. (for onprogress() streaming XHR transfer, the number of bytes downloaded so far before this chunk)
    uint64_t dataOffset;

    // Specifies the total number of bytes that the response body will be.
    // Note: This field may be zero, if the server does not report the Content-Length field.
    uint64_t totalBytes;

    // Specifies the readyState of the XHR request:
    // 0: UNSENT: request not sent yet
    // 1: OPENED: emscripten_fetch has been called.
    // 2: HEADERS_RECEIVED: emscripten_fetch has been called, and headers and status are available.
    // 3: LOADING: download in progress.
    // 4: DONE: download finished.
    // See https://developer.mozilla.org/en-US/docs/Web/API/XMLHttpRequest/readyState
    unsigned short readyState;

    // Specifies the status code of the response.
    unsigned short status;

    // Specifies a human-readable form of the status code.
    char statusText[64];

    uint32_t __proxyState;

    // For internal use only.
    emscripten_fetch_attr_t __attributes;
} emscripten_fetch_t;

m3ApiRawFunction(emscripten_start_fetch);

m3ApiRawFunction(_emscripten_fetch_free);

#if defined(__cplusplus)
}
# endif

#endif /* emscripten_fetch_h */
