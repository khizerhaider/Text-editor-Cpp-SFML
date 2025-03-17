#ifndef CSTRINGMETHODS_H
#define CSTRINGMETHODS_H
#include <iostream> // For size_t 
// saperate file for managing cstring methods locally

void* customMemcpy(void* dest, const void* src, size_t length) {
    // Cast source and destination pointers to unsigned char*
    unsigned char* d = static_cast<unsigned char*>(dest);
    const unsigned char* s = static_cast<const unsigned char*>(src);

    // Check for overlap: if dest is within the src block
    if (d > s && d < s + length) {
        // Copy backward to avoid overwriting source data
        for (size_t i = length; i > 0; --i) {
            d[i - 1] = s[i - 1];
        }
    } else {
        // Copy forward
        for (size_t i = 0; i < length; ++i) {
            d[i] = s[i];
        }
    }

    return dest; // Return destination pointer/////////////////////////
}

#endif