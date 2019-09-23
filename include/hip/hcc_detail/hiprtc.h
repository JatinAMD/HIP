/*
Copyright (c) 2015 - present Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#pragma once

#include <cstddef>
#include <string>

enum hiprtcResult {
    HIPRTC_SUCCESS = 0,
    HIPRTC_ERROR_OUT_OF_MEMORY = 1,
    HIPRTC_ERROR_PROGRAM_CREATION_FAILURE = 2,
    HIPRTC_ERROR_INVALID_INPUT = 3,
    HIPRTC_ERROR_INVALID_PROGRAM = 4,
    HIPRTC_ERROR_INVALID_OPTION = 5,
    HIPRTC_ERROR_COMPILATION = 6,
    HIPRTC_ERROR_BUILTIN_OPERATION_FAILURE = 7,
    HIPRTC_ERROR_NO_NAME_EXPRESSIONS_AFTER_COMPILATION = 8,
    HIPRTC_ERROR_NO_LOWERED_NAMES_BEFORE_COMPILATION = 9,
    HIPRTC_ERROR_NAME_EXPRESSION_NOT_VALID = 10,
    HIPRTC_ERROR_INTERNAL_ERROR = 11
};

const char* hiprtcGetErrorString(hiprtcResult result);

inline
hiprtcResult hiprtcVersion(int* major, int* minor) noexcept
{   // TODO: NVRTC versioning is somewhat unclear.
    if (!major || !minor) return HIPRTC_ERROR_INVALID_INPUT;

    // TODO: this should be generic / set by the build infrastructure.
    *major = 9;
    *minor = 0;

    return HIPRTC_SUCCESS;
}

struct _hiprtcProgram;
using hiprtcProgram = _hiprtcProgram*;

hiprtcResult hiprtcAddNameExpression(hiprtcProgram prog,
                                     const char* name_expression);

hiprtcResult hiprtcCompileProgram(hiprtcProgram prog, int numOptions,
                                  const char** options);

hiprtcResult hiprtcCreateProgram(hiprtcProgram* prog, const char* src,
                                 const char* name, int numHeaders,
                                 const char** headers,
                                 const char** includeNames);

hiprtcResult hiprtcDestroyProgram(hiprtcProgram* prog);

hiprtcResult hiprtcGetLoweredName(hiprtcProgram prog,
                                  const char* name_expression,
                                  const char** lowered_name);

hiprtcResult hiprtcGetProgramLog(hiprtcProgram prog, char* log);

hiprtcResult hiprtcGetProgramLogSize(hiprtcProgram prog,
                                     std::size_t* logSizeRet);

hiprtcResult hiprtcGetCode(hiprtcProgram prog, char* code);

hiprtcResult hiprtcGetCodeSize(hiprtcProgram prog, std::size_t* codeSizeRet);

namespace hip_impl
{
    std::string demangle(const char* mangled_expression);
}

#if defined(HIPRTC_GET_TYPE_NAME)
    #include <typeinfo>

    #if defined(_WIN32)
        #include <dbghelp.h>

        template<typename>
        hiprtcResult hiprtcGetTypeName(std::string*) = delete;
    #else
        #include "bundleio/bundleio.hpp"
        template<typename T>
        inline
        hiprtcResult hiprtcGetTypeName(std::string* result)
        {
            if (!result) return HIPRTC_ERROR_INVALID_INPUT;

            *result = cxxdemangle(typeid(T).name());

            return (result->empty()) ? HIPRTC_ERROR_INTERNAL_ERROR :
                                       HIPRTC_SUCCESS;
        }
    #endif
#endif
