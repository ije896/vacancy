/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   choppy_jpg;
    const int            choppy_jpgSize = 123222;

    extern const char*   droplet_jpg;
    const int            droplet_jpgSize = 3041;

    extern const char*   ripples_jpg;
    const int            ripples_jpgSize = 122636;

    extern const char*   surface_jpg;
    const int            surface_jpgSize = 87197;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 4;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}
