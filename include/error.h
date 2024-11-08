#ifndef CHIP_8_ERROR_H
#define CHIP_8_ERROR_H

/**
 * @brief Enumeration representing result status code
 */
enum _ResultCode {
    OK,                         /* Operation was successful */
    ERROR_UNABLE_TO_ALLOCATE,   /* Operation failed, because memory cannot be allocated */
    ERROR_DATA_IS_TOO_LARGE,    /* Operation failed, because provided Data is too large */
};
typedef enum _ResultCode ResultCode;

/**
 * @brief Structure reprensenting a result with data and a status
 */
struct _Result {
    void* data;                 /* Pointer to the result data */
    ResultCode code;            /* Status code of the result */
};
typedef struct _Result Result;

/**
 * Converts a `ResultCode` to its corresponding string representation.
 *
 * @param code The `ResultCode` to be converted.
 * 
 * @return A pointer to a constant character string representing the `ResultCode`.
 * Returns "Unknown ResultCode" if the provided code does not match any known value.
 */
const char* result_code_to_str(ResultCode code);

#endif /* CHIP_8_ERROR_H */