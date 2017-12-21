#pragma once

#ifdef __cplusplus
extern "C" {
#endif

    bool JaegerNet_StartClient(const char* const hostname, const char* const port);
    void JaegerNet_StopClient(void);

#ifdef __cplusplus
}
#endif
