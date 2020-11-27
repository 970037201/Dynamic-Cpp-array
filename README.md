# Dynamic C++ array


A simple class for replicating the standard library "std::vector", but with a much smaller header size, and less functionality.

The class stores an array, managed with realloc, free, and placement new, and manages to dynamically resize without hastle and strife.
The class also preforms deconstructing, and deallocating clipped and uneeded memory, and constructing objects allocated on resize.
The class is a small alternative toward std::vector, if you dont want to add 30kb on your executable size - or whatever it is nowdays
The class is easy enough to read and interpret that you could understand how dynamic arrays work - if you want to port it to another
language, or educate yourself.
