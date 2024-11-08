#include "../include/error.h"


const char* result_code_to_str(ResultCode code) {
    switch (code) {
        OK: 
            return "OK";
        ERROR_UNABLE_TO_ALLOCATE:
            return "Unable to allocate the requested memory";
        ERROR_DATA_IS_TOO_LARGE:
            return "The provided Data is too large";
        default: 
            return "Unknown ResultCode";
    }
}