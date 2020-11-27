#pragma once
#include <memory>

/*A small class for managing dynamic arrays with objects, and simple types if you want to.

Object rules: (For constructing dynamic arrays with)
	-Smart array will use default constructor. for use in reserve(), and in extention, many of the class functions.
	-Smart array will use copy operator. (Operator= (const yourtype& item)), for use in push(), operator=(copy), and make_smart()
	-Smart array will compare elements (Operator== (const yourtype& comparing)), for use in find() function
	-Smart array will call deconstructor. (Safe with basic types, for use in reserve())

Array safety Notices:
	-Do not preform memory reallocation/management/manipulation outside of access with the return of the data() function
	-Do not expect pop to remove elements when your array size is zero.
	-Do not expect peek to return valid element if array size is zero, it might even corrupt heap
	-Do not expect operator[] to return valid object in array if index is outside array bounds, it might even corrupt heap

Other Notices:
	-If you notice any problems with object management with array, contact your debugger/brain before you contact me.
	-This is not a python program, nor a scratch program, brush your teeth, dont do drugs.
	-No license, feel free to use, dont blame me for your mistakes, use at your own liking
*/

template<class T>
class smart_arr {
	size_t width;
	T* arr;
public:
		//Constructors
	inline constexpr smart_arr() : arr(0), width(0) {};//default constructor, creates array of size 0, and does not allocate memory
	inline smart_arr(size_t length) : arr(0), width(0) { reserve(length); }//creates array of width: length, and default constructs values
	inline smart_arr(const smart_arr<T>& construct) : arr(0), width(0) { this->operator=(construct); }//providing constructor for copy
	inline smart_arr(smart_arr<T>&& construct) noexcept : arr(0), width(0) { this->operator=(construct); }//providing constructor for move
	inline smart_arr(const T* construct, size_t length) : arr(0), width(0) { make_smart(construct, length); }//providing constructor for copy
		//Deconstructors
	inline ~smart_arr() { reserve(0); }//resizes the array to length of zero, deconstructing objects and free-ing memory

		//Allocators: Resizes, copies, and moves arrays
	void operator=(const smart_arr<T>& construct) {//copies elements from
		reserve(construct.width);
		if (construct.arr && arr) {
			for (size_t ptr = 0; ptr < width; ++ptr) {
				arr[ptr] = construct.arr[ptr];
			}
		}
	}
	void operator=(smart_arr<T>&& construct) noexcept {//moves values from construct to this, as object is passed by reference
		reserve(0);
		arr = construct.arr;
		width = construct.width;
		construct.arr = 0;
	}
	void make_smart(const T* construct, size_t length) {//creates array containing raw array of values, matching construct.
		reserve(0);
		if (construct) {
			reserve(length);
			for (size_t ptr = 0; ptr < width; ++ptr) {
				arr[ptr] = construct[ptr];
			}
		}
	}
	void reserve(size_t length) {//changes size of array, deconstructing clipped elements, and constructing added elements. 
		if (arr) {
			for (size_t ptr = length; ++ptr < width;) {
				arr[ptr].~T();
			}
			if (!length) {
				free(arr);
			}
		}
		if (length) {
			T* temp = (T*)realloc(arr, length * sizeof(T));
			if (temp) {
				arr = temp;
				if (width < length) {//Placement new here: (constructs elements)
					temp = new(temp) T[length - width];
				}
				width = length;
			}
		}
	}

		//Element access, for stack-like use.
	inline void push(const T& instance) {//adds value to array - slow for multiple pushes (use reserve() and operator[])
		reserve(width + 1);
		arr[width - 1] = instance;
	}
	inline T& peek(void) { return arr[width - 1]; }//Access top element
	inline void pop(void) { reserve(width ? (width - 1) : 0); } //removes top of array. (reserve() faster for multiple)

		//Element access - for array and referencing use.
	inline T& operator[](size_t ptr) { return arr[ptr]; }//access element at index by reference
	inline const T& operator[](size_t ptr) const { return arr[ptr]; }//access element at index by const reference
	inline const T* data(void) { return arr; }//Get ray array pointer - Not recommended to alter / remove data in any way!

		//Multi element removal
	//writes over elements beyond index + length to index position, deconstructs and copies during move. Does not remove trailing elements.
	void remove(size_t index, size_t length) {
		for (size_t ptr = index; ptr < (width > length ? width - length : 0); ++ptr) {
			arr[ptr] = arr[ptr + length];
		}
	}
	void remove_resize(size_t index, size_t length) {//Does the same as remove() but also resizes array to fit removal of elements
		remove(index, length);
		if (index < width) {
			reserve(index + length > width ? index : width - length);
		}
	}

		//States of array
	inline size_t size(void) const { return width; }//returns count of elements
	inline operator bool(void) const { return arr; }//returns allocation status of array
	bool operator==(const smart_arr<T>& other) const {//returns equality (operator==) with another array of same type
		if (!(bool(arr) ^ bool(other.arr))) {
			if (!arr) {
				return 1;
			}
			else if (width == other.width) {
				for (size_t ptr = 0; ptr < width; ++ptr) {
					if (arr[ptr] != other.arr[ptr]) {
						return 0;
					}
				}
				return 1;
			}
		}
		return 0;
	}

		//Existence of elements in array
	size_t find(const T& item) const {//returns index of matching element, or count of elements otherwise.
		for (size_t ptr = arr ? 0 : width; ptr < width; ++ptr) {
			if (arr[ptr] == item) {
				return ptr;
			}
		}
		return width;
	}
	inline bool contains(const T& item) const { return find(item) != width; }//returns true if array contains element, false otherwise
};