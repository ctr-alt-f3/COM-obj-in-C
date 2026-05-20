#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <windows.h>


//0ba1cd58-89db-4102-bc74472b7a65c0ba
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


//CONTAINING_RECORD(this,com_1_t, off_interface)

static uint16_t lock_counter;
static uint16_t objcounter;

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

//const static off_interface_vtable_t off_interface_vtable{}
//const static main_interface_vtable_t main_interface_vtable{}


typedef struct off_interface{
const off_interface_vtable_t* off_interface_vtable;
}off_interface_t;


typedef struct com_1{
const main_interface_vtable_t* main_interface_vtable;

off_interface_t off_interface;

uint16_t refcount;
} com_1_t;


HRESULT main_interface_query_f(void* this, REFIID qIIDptr,  void** out){
	if(IsEqualIID(&main_iid,qIIDptr) || IsEqualIID(&IID_IUnknown, qIIDptr)){
		*out = this;
		((struct com_1*)this)->refcount++;
		return S_OK;
	}
	if(IsEqualIID(&off_iid,qIIDptr)){
		*out = &((struct com_1*)this)->off_interface;
		((struct com_1*)this)->refcount++;
		return S_OK;

	}

*out = NULL;
return E_NOINTERFACE;
}


 HRESULT main_interface_release_f(void* this){
 	((struct com_1*)this)->refcount--;
 	if(((struct com_1*)this)->refcount ==0){
 		free(this);
 		objcounter--;
 	}
 	return S_OK;
 }
 HRESULT main_interface_add_f(void* this){
((struct com_1*)this)->refcount++;
return S_OK;
 }

 
void main_interface_hello_f(void* this,char* who){
	char out[32];
	snprintf(out,32,"hello %s\n",who);
	MessageBox(NULL,out,"przywitanie",0);
	return ;
}

const static main_interface_vtable_t main_interface_vtable = {main_interface_query_f,main_interface_release_f, main_interface_add_f, main_interface_hello_f};



HRESULT off_interface_query_f(void* this, REFIID qIIDptr,  void** out){
	com_1_t* this_M = CONTAINING_RECORD(this,com_1_t,off_interface);
	
	if(IsEqualIID(&main_iid,qIIDptr) || IsEqualIID(&IID_IUnknown, qIIDptr)){
		*out = this_M;
		((struct com_1*)this_M)->refcount++;
		return S_OK;
	}
	if(IsEqualIID(&off_iid,qIIDptr)){
		*out = this;
		((struct com_1*)this_M)->refcount++;
		return S_OK;
	}

*out = NULL;
return E_NOINTERFACE;
}

HRESULT off_interface_release_f(void* this){
	com_1_t* this_M = CONTAINING_RECORD(this,com_1_t,off_interface);

 	(this_M)->refcount--;
 	if(this_M->refcount ==0){
 		free(this_M);
 		objcounter--;
 	}
 	return S_OK;
 }

 HRESULT off_interface_add_f(void* this){
 	com_1_t* this_M = CONTAINING_RECORD(this,com_1_t,off_interface);
(this_M)->refcount++;
return S_OK;
 }

void off_interface_hello_f(void* this){
	char out[4];
	snprintf(out,4,"%d",(CONTAINING_RECORD(this,com_1_t,off_interface)->refcount));
	MessageBox(NULL,out,"refcount",0);
	return;
}

const static off_interface_vtable_t off_interface_vtable = {off_interface_query_f,off_interface_release_f, off_interface_add_f, off_interface_hello_f};


typedef struct F_interface{
const struct F_vtable* F_vtable;
LONG refcount; 
}F_interface_t;


HRESULT F_interface_query_f(void* this, REFIID qIIDptr,  void** out){
	if(IsEqualIID(&F_iid,qIIDptr) || IsEqualIID(&IID_IUnknown, qIIDptr)){
		*out = this;
		((F_interface_t*)this)->refcount++;
		return S_OK;
	}
	
*out = NULL;
return E_NOINTERFACE;
}


 HRESULT F_interface_release_f(void* this){
 	((struct com_1*)this)->refcount--;
 	if(((struct com_1*)this)->refcount ==0){
 		free(this);
 		objcounter--;
 	}
 	return S_OK;
 }
 HRESULT F_interface_add_f(void* this){
((struct com_1*)this)->refcount++;
return S_OK;
 }


com_1_t* factory(){
com_1_t* temp = malloc(sizeof(com_1_t));
temp->main_interface_vtable = &main_interface_vtable;
temp->off_interface.off_interface_vtable = &off_interface_vtable;
temp->refcount = 1;
return temp;

}

HRESULT F_CreateInstance_f (void* this,IUnknown *pUnkOuter,REFIID riid,void **ppvObject){
if(pUnkOuter != NULL){
	MessageBox(NULL,"wywolano mnie jako part of aggregate, ten ficzer nie jest wspierany\r\n","ctr-alt-f3",0);
	return CLASS_E_NOAGGREGATION;
}

 com_1_t* temp = factory();
main_interface_query_f(temp, riid, ppvObject);
temp->main_interface_vtable->main_interface_release_f(temp);
return S_OK;
}

HRESULT F_LockServer_f(void* this,BOOL fLock){
if(fLock){
lock_counter++;
return S_OK;
}
lock_counter--;
return S_OK;

}

typedef struct F_vtable{
off_interface_query_ft QueryInterface;
off_interface_release_ft AddRef;
off_interface_release_ft Release;
F_CreateInstance CreateInstance;
F_LockServer LockServer;

}F_vtable_t;


const static F_vtable_t F_interface_vtable = {F_interface_query_f,F_interface_release_f, F_interface_add_f, F_CreateInstance_f,F_LockServer_f};


 HRESULT DllGetClassObject(
   REFCLSID rclsid,
   REFIID   riid,
   LPVOID   *ppv){

 	F_interface_t *temp = malloc(sizeof(F_interface_t));
 	temp->refcount = 1;
	temp->F_vtable = &F_interface_vtable;
	*ppv = (void*)temp;

	return S_OK;
 }



HRESULT DllRegisterServer(){ 
	const char path[] = "CLSID\\{0ba1cd58-89db-4102-bc74472b7a65c0ba}";
HKEY key;
DWORD dwDisposition;

LONG res = RegCreateKeyExA(HKEY_CLASSES_ROOT,path,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&key,&dwDisposition);

if (res != ERROR_SUCCESS) {
        MessageBox(NULL,"1 etap rejestracji mnie nie powiodla sie\r\n","ctr-alt-f3",0);
        RegCloseKey(key);return S_FALSE;
    }

if(dwDisposition ==REG_OPENED_EXISTING_KEY){
	MessageBox(NULL,"1 etap rejestracji: zarejestrowalem sie na juz istniejacym clsid\r\n","ctr-alt-f3",0);
}

const char name[] = "ctr-alt-f3 example COM object for doin' things";
LSTATUS status = RegSetValueExA(key,NULL,0,REG_SZ,name,48);

if(status !=ERROR_SUCCESS){
	MessageBox(NULL,"nie moglem sie zarejestrowac, uprawnienia?\r\n","ctr-alt-f3",0);
	RegCloseKey(key);return S_FALSE;	
}


RegCloseKey(key);


const char path2[] = "CLSID\\{0ba1cd58-89db-4102-bc74472b7a65c0ba}\\InprocServer32";
res = RegCreateKeyExA(HKEY_CLASSES_ROOT,path2,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&key,&dwDisposition);

if (res != ERROR_SUCCESS) {
        MessageBox(NULL,"2 etap: rejestracja mnie nie powiodla sie\r\n","ctr-alt-f3",0);
        RegCloseKey(key);return S_FALSE;
    }
const char path3[] = "C:\\Users\\arch\\Desktop\\linux\\windows\\obiektCOM\\COM_multi_interface.dll";
status = RegSetValueExA(key,NULL,0,REG_SZ,path3,58);

if(status !=ERROR_SUCCESS){
	MessageBox(NULL,"2 etap: nie moglem sie zarejestrowac, uprawnienia?\r\n","ctr-alt-f3",0);
	RegCloseKey(key);return S_FALSE;	
}
RegCloseKey(key);

return S_OK;
}

HRESULT DllUnregisterServer(){

LSTATUS status = RegDeleteTreeA(HKEY_CLASSES_ROOT,"CLSID\\{0ba1cd58-89db-4102-bc74472b7a65c0ba}");
if(status!=ERROR_SUCCESS){
	MessageBox(NULL,"derejestracja obiektu COM sie nie udala\r\n","ctr-alt-f3",0);
	return E_FAIL;
}

}




