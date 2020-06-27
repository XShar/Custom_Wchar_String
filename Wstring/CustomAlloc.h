#pragma once

void HeapInit(void* buf, size_t size);
void* _malloc(size_t size);
void _free(void* ptr);
void* _realloc(void* ptr, size_t size);