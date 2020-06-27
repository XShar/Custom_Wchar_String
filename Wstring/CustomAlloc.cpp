#include "CustomAlloc.h"

#define HEAP_SIZE 2*1024*1024
#define BLOCK_SIZE 0x1000

typedef unsigned char  uint8_t;
typedef unsigned int   uintptr_t;

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

void HeapInit(void* buf, size_t size)
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
static segment_t* SearchFree(segment_t* s, int size)
{
	while (s) {
		if (s->is_free && s->size >= size) return s;
		s = s->next;
	}
	return s;
}

// переводит размер в байтах в количество блоков
static int GetNumBlock(size_t size)
{
	if (size % BLOCK_SIZE) size += BLOCK_SIZE;
	return size / BLOCK_SIZE;
}

// отрезает от указанного сегмента, указанный размер
// возвращает указатель на новый сегмент памяти
static segment_t* CutSegment(segment_t* s, int size)
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
static segment_t* MergeSegment(segment_t* s, segment_t* old)
{
	if (old_free_segment == old) old_free_segment = s;
	s->size += old->size;
	s->next = old->next;
	if (old->next) old->next->prev = s;
	return s;
}

static void* SegmentToPtr(segment_t* s)
{
	return (char*)s + sizeof(segment_t);
}

static segment_t* PtrToSegment(void* ptr)
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


// возвращает nullptr если нету памяти
void* _malloc(size_t size)
{
	
	if (segments == nullptr) {
		HeapInit(memory, HEAP_SIZE);
	};

	int s = GetNumBlock(size + sizeof(segment_t));
	segment_t* it = SearchFree(old_free_segment, s);
	if (!it) it = SearchFree(segments, s);
	if (!it) {
		return nullptr;
	}

	it->is_free = 0; // бронируем участок памяти

	// отрежем лишнюю память(на конце может остаться маленький кусочек памяти)
	if (it->size > s + GetNumBlock(sizeof(segment_t))) {
		segment_t* n = CutSegment(it, it->size - s);
		n->is_free = 1;
		old_free_segment = n;
	}
	return SegmentToPtr(it);
}

void _free(void* ptr)
{
	if (!ptr) return;
	segment_t* s = PtrToSegment(ptr);
	s->is_free = 1;

	if (s->next && s->next->is_free) MergeSegment(s, s->next);
	if (s->prev && s->prev->is_free) MergeSegment(s->prev, s);
}

void* _realloc(void* ptr, size_t size)
{
	if (segments == nullptr) {
		HeapInit(memory, HEAP_SIZE);
	};

	if (!size) {
		_free(ptr);
		return nullptr;
	}

	segment_t* s = PtrToSegment(ptr);
	int b = GetNumBlock(size + sizeof(segment_t));
	if (s->size == b) return ptr; // ничего делать не надо, размер не изменился
	else if (s->size > b) return ptr; // FIXME: надо бы освободить часть памяти
	else { // if (s->size < b)
		if (s->next && s->next->is_free && s->size + s->next->size >= b) {
			MergeSegment(s, s->next);
			if (s->size > b + GetNumBlock(sizeof(segment_t))) {
				segment_t* n = CutSegment(s, s->size - b);
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
