#include <stdint.h>
#include "CustomAlloc.h"

#define HEAP_SIZE 2*1024*1024

#define BLOCK_SIZE 0x1000
#define BLOCK_SIZE_MASK 0xFFFFF000

/*
В данном решении выделяется статически 2Мб. памяти и работа идет с этим буфером.
В других реализациях можно вызвать heap_init(void* buf, size_t size) и передать нужный буфер и его размер.
*/

static uint8_t memory[HEAP_SIZE];

typedef struct segment {
	int is_free;
	int size; // в блоках
	struct segment* next;
	struct segment* prev;
} segment_t; // сегмент - как непрерывный кусок памяти

static segment_t* segments = nullptr;
static segment_t* old_free_segment = nullptr;

void heap_init(void* buf, size_t size)
{
	segments = (segment_t*)buf;
	segments->is_free = 1;
	segments->size = size / BLOCK_SIZE;
	segments->next = 0;
	segments->prev = 0;
	old_free_segment = 0;
}

// находит сегмент свободной памяти, но никаких действий над ним не производит
// поиск производится от переданного элемента и до конца списка
static segment_t* search_free(segment_t* s, int size)
{
	while (s) {
		if (s->is_free && s->size >= size) return s;
		s = s->next;
	}
	return s;
}

// переводит размер в байтах в количество блоков
static int get_num_block(size_t size)
{
	if (size % BLOCK_SIZE) size += BLOCK_SIZE;
	return size / BLOCK_SIZE;
}

// отрезает от указанного сегмента, указанный размер
// возвращает указатель на новый сегмент памяти
static segment_t* cut_segment(segment_t* s, int size)
{
	uintptr_t addr = (uintptr_t)s;
	addr += (s->size - size) * BLOCK_SIZE;
	segment_t* result = (segment_t*)addr;
	s->size -= size;
	result->size = size;
	result->prev = s;
	result->next = s->next;
	if (s->next) s->next->prev = result;
	s->next = result;
	result->is_free = s->is_free;
	return result;
}

// объединяет два соседних сегмента в один(первый поглощает второй)
// возвращает указатель на новый сегмент
static segment_t* merge_segment(segment_t* s, segment_t* old)
{
	if (old_free_segment == old) old_free_segment = s;
	s->size += old->size;
	s->next = old->next;
	if (old->next) old->next->prev = s;
	return s;
}

static void* segment_to_ptr(segment_t* s)
{
	return (char*)s + sizeof(segment_t);
}

static segment_t* ptr_to_segment(void* ptr)
{
	return (segment_t*)((char*)ptr - sizeof(segment_t));
}

static void* _memcpy(void* dest, const void* src, size_t bytes)
{
	size_t i;
	char* cdest = (char*)dest;
	const char* csrc = (char*)src;
	for (i = 0; i < bytes; ++i)
		*cdest++ = *csrc++;

	return dest;
}


// возвращает 0 если нету памяти
void* _malloc(size_t size)
{
	
	if (segments == nullptr) {
		heap_init(memory, HEAP_SIZE);
	};

	int s = get_num_block(size + sizeof(segment_t));
	segment_t* it = search_free(old_free_segment, s);
	if (!it) it = search_free(segments, s);
	if (!it) {
		return nullptr;
	}

	it->is_free = 0; // бронируем участок памяти

	// отрежем лишнюю память(на конце может остаться маленький кусочек памяти)
	if (it->size > s + get_num_block(sizeof(segment_t))) {
		segment_t* n = cut_segment(it, it->size - s);
		n->is_free = 1;
		old_free_segment = n;
	}
	return segment_to_ptr(it);
}

void _free(void* ptr)
{
	if (!ptr) return;
	segment_t* s = ptr_to_segment(ptr);
	s->is_free = 1;

	if (s->next && s->next->is_free) merge_segment(s, s->next);
	if (s->prev && s->prev->is_free) merge_segment(s->prev, s);
}

void* _realloc(void* ptr, size_t size)
{
	if (segments == nullptr) {
		heap_init(memory, HEAP_SIZE);
	};

	if (!size) {
		_free(ptr);
		return nullptr;
	}

	segment_t* s = ptr_to_segment(ptr);
	int b = get_num_block(size + sizeof(segment_t));
	if (s->size == b) return ptr; // ничего делать не надо, размер не изменился
	else if (s->size > b) return ptr; // FIXME: надо бы освободить часть памяти
	else { // if (s->size < b)
		if (s->next && s->next->is_free && s->size + s->next->size >= b) {
			merge_segment(s, s->next);
			if (s->size > b + get_num_block(sizeof(segment_t))) {
				segment_t* n = cut_segment(s, s->size - b);
				n->is_free = 1;
			}
			return ptr;
		}
		else {
			void* pdst = _malloc(size);
		     _memcpy(pdst, ptr, size);
			_free(ptr);
			return pdst;
		}
	}
}
