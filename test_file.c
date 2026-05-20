#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <windows.h>



//0ba1cd58-89db-4102-bc74-472b7a65c0ba
IID clsid = {
0x0ba1cd58,
0x89db,
0x4102,
{0xbc,0x74,0x47,0x2b,0x7a,0x65,0xc0,0xba}
};

//cf7bcbdb-3f73-4f97-8d9d-42e9feac394b
IID F_iid = {
0xcf7bcbdb,
0x3f73,
0x4f97,
{0x8d,0x9d,0x42,0xe9,0xfe,0xac,0x39,0x4b}
};

//b2dd4312-08e5-425d-9e76-30a76a1d4e1d 

IID main_iid = {
 0xb2dd4312, 
 0x08e5,
 0x425d,
 {0x9e,0x76,0x30,0xa7,0x6a,0x1d,0x4e,0x1d}
};

//cdbd4fda-a0fc-4a08-8c1b-4b8b8087ec5f 

IID off_iid = {
0xcdbd4fda,
0xa0fc,
0x4a08,
{0x8c,0x1b,0x4b,0x8b,0x80,0x87,0xec,0x5f}
};




typedef HRESULT (*main_interface_query_ft)(void*, REFIID,  void**);
typedef HRESULT (*main_interface_add_ft)(void*);
typedef HRESULT (*main_interface_release_ft)(void*);
typedef void (*main_interface_hello_ft)(void*,char*);


typedef HRESULT (*off_interface_query_ft)(void*, REFIID,  void**);
typedef HRESULT (*off_interface_add_ft)(void*);
typedef HRESULT (*off_interface_release_ft)(void*);
typedef void (*off_interface_hello_ft)(void*);


typedef HRESULT (*F_CreateInstance)(void* this, IUnknown *pUnkOuter,REFIID riid,void **ppvObject);
typedef HRESULT (*F_LockServer)(void* this,BOOL fLock);



typedef  const struct main_interface_vtable{
	main_interface_query_ft  main_interface_query_f;
	main_interface_add_ft  main_interface_add_f;
	main_interface_release_ft  main_interface_release_f;
	main_interface_hello_ft  main_interface_hello_f;
} main_interface_vtable_t;

typedef  const struct off_interface_vtable{
	off_interface_query_ft off_interface_query_f;
	off_interface_add_ft 	off_interface_add_f;
	off_interface_release_ft off_interface_release_f;
	off_interface_hello_ft off_interface_hello_f;
} off_interface_vtable_t;



typedef struct off_interface{
const off_interface_vtable_t* off_interface_vtable;
}off_interface_t;


typedef struct com_1{
const main_interface_vtable_t* main_interface_vtable;

off_interface_t off_interface;

uint16_t refcount;
} com_1_t;


int main(int argc, char const *argv[])
{
	if(CoInitialize(NULL)!=S_OK){
		printf("CoInitialize failed\n");
		return 1;
	}

LPVOID out;

HRESULT o = CoCreateInstance(&clsid, NULL, CLSCTX_INPROC_SERVER, &off_iid, &out);

if(o!=S_OK){
	printf("CoCreateInstance failed with code %08X\n",o);
		return 1;
}
	(((off_interface_t*)out)->off_interface_vtable)->off_interface_add_f(out);
	(((off_interface_t*)out)->off_interface_vtable)->off_interface_add_f(out);
	(((off_interface_t*)out)->off_interface_vtable)->off_interface_add_f(out);
	(((off_interface_t*)out)->off_interface_vtable)->off_interface_release_f(out); //just fooling around
	(((off_interface_t*)out)->off_interface_vtable)->off_interface_hello_f(out);

	return 0;
}
