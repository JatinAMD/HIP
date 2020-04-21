#ifndef HIP_NVRTC_H

#include <hip/hip_runtime.h>
#include <hip/hip_runtime_api.h>
#include <nvrtc.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdlib.h>
#define hiprtcProgram nvrtcProgram

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

inline static hiprtcResult hiprtcErrorTonvrtcError(nvrtcResult e) {
    switch (e) {
        case NVRTC_SUCCESS:
            return HIPRTC_SUCCESS;
        case NVRTC_ERROR_OUT_OF_MEMORY:
            return HIPRTC_ERROR_OUT_OF_MEMORY;
        case NVRTC_ERROR_PROGRAM_CREATION_FAILURE:
            return HIPRTC_ERROR_PROGRAM_CREATION_FAILURE;
        case NVRTC_ERROR_INVALID_INPUT:
            return HIPRTC_ERROR_INVALID_INPUT;
        case NVRTC_ERROR_INVALID_PROGRAM:
            return HIPRTC_ERROR_INVALID_PROGRAM;
        case NVRTC_ERROR_INVALID_OPTION:
            return HIPRTC_ERROR_INVALID_OPTION;
        case NVRTC_ERROR_COMPILATION:
            return HIPRTC_ERROR_COMPILATION;
        case NVRTC_ERROR_BUILTIN_OPERATION_FAILURE:
            return HIPRTC_ERROR_BUILTIN_OPERATION_FAILURE;
        case NVRTC_ERROR_NO_NAME_EXPRESSIONS_AFTER_COMPILATION:
            return HIPRTC_ERROR_NO_NAME_EXPRESSIONS_AFTER_COMPILATION;
        case NVRTC_ERROR_NO_LOWERED_NAMES_BEFORE_COMPILATION:
            return HIPRTC_ERROR_NO_LOWERED_NAMES_BEFORE_COMPILATION;
        case NVRTC_ERROR_NAME_EXPRESSION_NOT_VALID:
            return HIPRTC_ERROR_NAME_EXPRESSION_NOT_VALID;
        case NVRTC_ERROR_INTERNAL_ERROR:
            return HIPRTC_ERROR_INTERNAL_ERROR;
        default:
            return HIPRTC_ERROR_INTERNAL_ERROR;
    }
}

inline static const char* hiprtcGetErrorString(nvrtcResult result) {
    return nvrtcGetErrorString(result);
}

inline static hiprtcResult hiprtcVersion(int* major, int* minor) {
    return hiprtcErrorTonvrtcError(nvrtcVersion(major, minor));
}

inline static hiprtcResult hiprtcAddNameExpression(hiprtcProgram prog,
                                                   const char* name_expression) {
    return hiprtcErrorTonvrtcError(nvrtcAddNameExpression(prog, name_expression));
}

inline static hiprtcResult hiprtcCompileProgram(hiprtcProgram prog, int numOptions,
                                                const char** options) {
    return hiprtcErrorTonvrtcError(nvrtcCompileProgram(prog, numOptions, options));
}

inline static hiprtcResult hiprtcCreateProgram(hiprtcProgram* prog, const char* src,
                                               const char* name, int numHeaders,
                                               const char** headers, const char** includeNames) {
    return hiprtcErrorTonvrtcError(
        nvrtcCreateProgram(prog, src, name, numHeaders, headers, includeNames));
}

inline static hiprtcResult hiprtcDestroyProgram(hiprtcProgram* prog) {
    return hiprtcErrorTonvrtcError(nvrtcDestroyProgram(prog));
}

inline static hiprtcResult hiprtcGetLoweredName(hiprtcProgram prog, const char* name_expression,
                                                const char** lowered_name) {
    return hiprtcErrorTonvrtcError(nvrtcGetLoweredName(prog, name_expression, lowered_name));
}

inline static hiprtcResult hiprtcGetProgramLog(hiprtcProgram prog, char* log) {
    return hiprtcErrorTonvrtcError(nvrtcGetProgramLog(prog, log));
}

inline static hiprtcResult hiprtcGetProgramLogSize(hiprtcProgram prog, size_t* logSizeRet) {
    return hiprtcErrorTonvrtcError(nvrtcGetProgramLogSize(prog, logSizeRet));
}

inline static hiprtcResult hiprtcGetCode(hiprtcProgram prog, char* code) {
    return hiprtcErrorTonvrtcError(nvrtcGetPTX(prog, code));
}

inline static hiprtcResult hiprtcGetCodeSize(hiprtcProgram prog, size_t* codeSizeRet) {
    return hiprtcErrorTonvrtcError(nvrtcGetPTXSize(prog, codeSizeRet));
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
