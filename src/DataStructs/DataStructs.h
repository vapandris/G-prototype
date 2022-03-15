#ifndef DATASTRUCTS_HG
#define DATASTRUCTS_HG

#include <inttypes.h>
#include <aio.h>

/* opaque datatype, representing an unordered array */
typedef struct DS_UnArray DS_UnArray;

/* Creates an unordered array to store data.
 * The size of that data is the size given as parameter.
 * The starting capacity is the capacity given as paramter. It can double when needed. */
DS_UnArray* DS_UnArray_Create(size_t size, int16_t capacity);


/* Free up the memory used by the unordered array self given as parameter. */
void		DS_UnArray_Destroy(DS_UnArray* self);


/* Push a copy of the elem given as paramter to the end of the array.
 * The size of elem is assumed by the size given to the Create function when self was created.
 * If the number of elements in the array exceeds the current capacity,
 * the capacity of the array is doubled along with the memory allocated.
 * If the memory allocation fails, an error is sent. */
void		DS_UnArray_Push(DS_UnArray* self, void* elem);

/* Remove the indexth elem in the array.
 * If the given index is invalid, an error is sent. */
void		DS_UnArray_Remove(DS_UnArray* self, int16_t index);

/* Get the data stored at the indexth item in the array. 
 * If the given index is invalid, an error is sent.
 * The size of returned data is assumed by the size given to the Create function when self was created. */
void*		DS_UnArray_Get(const DS_UnArray* self, int16_t index);

/* Get the numbers of elements stored in the array */
int16_t		DS_UnArray_GetSize(const DS_UnArray* self);


/* opaque datatype, representing a Map, where the type/size of value is user defined, key is string.
 * Key is unique string, but you can assign many other values to it. */
typedef struct DS_StrMap DS_StrMap;

/* Function pointer, that takes key (aka a string) as argument.
 * Calculates an integer value, so we can bind the key to a location to store at.
 * This return value is later is restricted to the Map's capacity. */
typedef int32_t (*MappingFunction)(const char* key);


/* Function that calculates an integer to a string. */
int32_t DS_StrMap_MappingFunction(const char* string);


/* Create a Map, that can store keys (strings), and assign values to these keys.
 * valueSize is the size of the datatype stored as the values.
 * mapFn is a function poniter, so we can generate an integer to the keys.
 * capacity is approximately the number of keys stored in the Map. 
 * We resolve collisions with linking. */
DS_StrMap*		DS_StrMap_Create(size_t valueSize, int16_t capacity, MappingFunction mapFn);

/* Free up the memory used by the Map given as parameter. */
void		DS_StrMap_Destroy(DS_StrMap* self);


/* Append value to the list connected to the key, if the given key is not in the map, create it. */
void		DS_StrMap_Add(DS_StrMap* self, const char* key, void* value);


/* Get the list of values connected to the key given as argument. */
DS_UnArray*		DS_StrMap_GetValues(DS_StrMap* self, const char* key);


#endif
