#pragma once
#include "func_trait.hpp"


static void* get_val(void* addr)
{
	return *((void**)addr);
}


static void* get_vtable(void* obj)
{
	return get_val(obj);
}

static void* get_vfunc(void* obj, int idx)
{
	char* vtable = (char*)get_vtable(obj);
	char* func = vtable + idx * sizeof(void*);
	return (*(void**)func);
}

template<typename M>
static int get_vfunc_idx(M func_addr)
{
	// 48 8B 01             mov         rax, qword ptr[rcx]
	// FF 20                jmp         qword ptr[rax]

	// 48 8B 01             mov         rax, qword ptr[rcx]
	// FF 60 10             jmp         qword ptr[rax + 10h]

	// 48 8B 01             mov         rax, qword ptr[rcx]
	// FF A0 88 00 00 00    jmp         qword ptr[rax + 88h]
	void* func = *(void**)(&func_addr);

	uint8_t* code = (uint8_t*)func;
	uint8_t jmp_type = code[4];
	void* vfunc_off = code + 5;
	if (0x20 == jmp_type)
	{
		return 0;
	}
	else if (0x60 == jmp_type)
	{
		uint8_t off = *((uint8_t*)vfunc_off);
		return off / sizeof(void*);
	}
	else if (0xA0 == jmp_type) {
		uint32_t off = *((uint32_t*)vfunc_off);
		return off / sizeof(void*);
	}
	return -1;
}

template <class FucInfo, typename M>
static void* get_real_vfunc_impl(M func_addr)
{
	typedef typename FucInfo::ClsType C;

	C temp_ohj;
	int idx = get_vfunc_idx(func_addr);

	void* real = get_vfunc(&temp_ohj, idx);
	return real;
}

template<typename M>
static void* get_real_vfunc(M func_addr)
{
	return get_real_vfunc_impl <FunctionDetails<M>, M > (func_addr);
}