# Dynamic C++ array


A simple class for replicating the standard library "std::Vector", but with a much smaller header size, and less functions.

The class stores an array, managed with realloc, free, and placement new, and manages to dynamically resize without hastle and strife.
The class also preforms deconstructing, and deallocating clipped and uneeded memory, and constructing objects allocated on resize.
