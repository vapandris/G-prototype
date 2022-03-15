#include "DataStructs.h"

#include "../Log/Log.h"

#include <stdlib.h>
#include <string.h>

struct DS_UnArray
{
	void* array;
	size_t  dataSize;

	int16_t capacity;
	int16_t size;
};



DS_UnArray* DS_UnArray_Create(size_t size, int16_t capacity)
{
	DS_UnArray* result = malloc(sizeof *result);
	if(result == NULL)
		Log_Error("DS_UnArray allocation failed.");

	result->array = malloc((sizeof *result->array) * capacity);

	if(result->array == NULL)
		Log_Error("DS_UnArray->array allocation failed.");

	result->capacity = capacity;
	result->size = 0;

	return result;
}


void DS_UnArray_Destroy(DS_UnArray* self)
{
	free(self->array);
	free(self);
}


void DS_UnArray_Push(DS_UnArray* self, void* elem)
{
	int16_t size = self->size++;
	if(self->capacity == self->size) {
		 self->capacity *= 2;

		 self->array = realloc(self->array, self->capacity);

		 if(self->array == NULL)
			 Log_Error("DS_UnArray->array reallocation failed.");
	}
	
	memcpy(
		&self->array[size],
		elem,
		self->dataSize
	);
	
}


void DS_UnArray_Remove(DS_UnArray* self, int16_t index)
{
	--self->size;
	
	memcpy(
		&self->array[index],
		&self->array[self->size],
		self->dataSize
	);
}


void* DS_UnArray_Get(const DS_UnArray* self, int16_t index)
{
	if(self->size >= index)
		Log_Error("DS_UnArray overindex: %d >= %d.", self->size, index);

	return &self->array[index];
}


int16_t	DS_UnArray_GetSize(const DS_UnArray* self)
{
	return self->size;
}


typedef struct DS_StrMapSlot DS_StrMapSlot;

/* Here we store one key-values pair.
 * It's also a linked list, in case the mapping function maps two different keys to the same spot. */
struct DS_StrMapSlot
{
	const char* key;
	DS_UnArray* values;
	DS_StrMapSlot* next;
};


/* The Map, really is just a fixed-sized array, storing DS_StrMapSlots along with
 * the capacity, so we can restrict the return value of the mapping function,
 * and the mapping function we use to assign a location in the slots array to a key
 * We also need the value sizes, so we know how big data do we store in the slots' UnArrays.*/
struct DS_StrMap
{
	const int16_t capacity;
	const MappingFunction mapFn;
	DS_StrMapSlot* slots;

	const size_t valueSize;
	// const int16_t maxStrLen;
};


/* Return the slot, where the stored key equals to the key given as paramter.
 * If it doesn't exist return NULL. */
static DS_StrMapSlot* DS_StrMap_GetSlot(DS_StrMap* self, const char* key);


/* Calculates the index, where the given key should fit */
static int32_t DS_StrMap_GetIndex(const DS_StrMap* self, const char* key);


DS_StrMap* DS_StrMap_Create(size_t valueSize, int16_t capacity, MappingFunction mapFn)
{
	DS_StrMap* result = malloc(sizeof *result);
	if(result == NULL)
		Log_Error("DS_StrMap allocation failed!");

	result->slots = malloc((sizeof *result->slots) * capacity);
	if(result->slots == NULL)
		Log_Error("DS_StrMap/slots allocation failed");

	/* Init the constant values with pointers. */
	int16_t* cap = (int16_t*)&result->capacity;
	MappingFunction* fn = (MappingFunction*)&result->mapFn;
	size_t* valSize = (size_t*)&result->valueSize;
	*cap = capacity;
	*fn = mapFn;
	*valSize = valueSize;

	/* Init the slots array */
	int i;
	for(i = 0; i < result->capacity; ++i) {
		DS_StrMapSlot* slot = &result->slots[i];
		slot->key = NULL;
		slot->values = NULL;
		slot->next = NULL;
	}

	return result;
}

void DS_StrMap_Destroy(DS_StrMap* self)
{
	int i;
	for(i = 0; i < self->capacity; ++i) {
		DS_StrMapSlot* current = &self->slots[i];
		free((char*)current->key);
		DS_UnArray_Destroy(current->values);
		current = current->next;
		while(current != NULL) {
			free((char*)current->key);
			DS_UnArray_Destroy(current->values);

			DS_StrMapSlot* next = current->next;
			free(current);
			current = next;
		}
	}
}


void DS_StrMap_Add(DS_StrMap* self, const char* key, void* value)
{
	DS_StrMapSlot* slot = DS_StrMap_GetSlot(self, key);
	int32_t index = DS_StrMap_GetIndex(self, key);

	// the given key is not yet in the map.
	if(slot == NULL) {
		slot = &self->slots[index];

		while(slot->next != NULL)
			slot = slot->next;

		/* copy & allocate key. */
		slot->key = malloc((sizeof *slot->key) * (strlen(key) + 1));
		strcpy((char*)slot->key, key);

		/* create the UnArray. */
		slot->values = DS_UnArray_Create(self->valueSize, 2);
		
		slot->next = NULL;
	}
	DS_UnArray_Push(slot->values, value);
}


DS_UnArray* DS_StrMap_GetValues(DS_StrMap* self, const char* key)
{
	if(key == NULL)
		Log_Error("DS_StrMap_GetValues paramter key is NULL.");

	DS_StrMapSlot* slot = DS_StrMap_GetSlot(self, key);
	if(slot == NULL)
		return NULL;

	return slot->values;
}


int32_t DS_StrMap_MappingFunction(const char* string)
{
	if(string == NULL)
		Log_Error("DS_StrMap_MappingFunction paramter string is NULL");

	int32_t result = 0;

	int i;
	for(i = 0; i < strlen(string); ++i)
		result += string[i] * 17 * i;

	return result;
}


/* static functions */
static DS_StrMapSlot* DS_StrMap_GetSlot(DS_StrMap* self, const char* key)
{
	int32_t index = DS_StrMap_GetIndex(self, key);
	DS_StrMapSlot* current = &self->slots[index];

	while(current != NULL && key != NULL && strcmp(current->key, key) != 0)
		current = current->next;

	return current;
}


static int32_t DS_StrMap_GetIndex(const DS_StrMap* self, const char* key)
{
	return (*self->mapFn)(key) % self->capacity;
}
