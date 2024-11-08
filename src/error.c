#include "../include/error.h"


const char* result_code_to_str(ResultCode code) {
    switch (code) {
        OK: 
            return "OK";
        ERROR_UNABLE_TO_ALLOCATE:
            return "Unable to allocate the requested memory";
        default: 
            return "Unknown ResultCode";
    }
}