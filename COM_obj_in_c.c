#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <windows.h>

//2613b4b0-f7dc-4aaa-8609-3ab240cd68a6


IID gid = {
0x2613b4b0,
0xf7dc,
0x4aaa,
{0x86,0x09,0x3a,0xb2,0x40,0xcd,0x68,0xa6}
};

//cb294039-0e08-4e2d-9e27-c731ec320cf7
IID F_gid = {
0xcb294039,
0x0e08,
0x4e2d,
{0x9e,0x27,0xc7,0x31,0xec,0x32,0x0c,0xf7}
};


 typedef ULONG (*fun_add_release)(void*); 
 typedef HRESULT (*fun_hello)(void*,char*); 
 typedef HRESULT (*fun_query)(void*,REFIID, void**); 

typedef HRESULT (*F_CreateInstance)(void* this, IUnknown *pUnkOuter,REFIID riid,void **ppvObject);
typedef HRESULT (*F_LockServer)(void* this,BOOL fLock);


 struct vtable{
 fun_query QueryInterface;
 fun_add_release AddRef;
 fun_add_release Release;
 fun_hello HelloWorld;
};

struct F_vtable{
fun_query QueryInterface;
fun_add_release AddRef;
fun_add_release Release;
 F_CreateInstance CreateInstance;
 F_LockServer LockServer;

};

struct F_COMTest{
struct F_vtable lpVtbl;
LONG counter;
LONG lock_counter;
};

struct COMTest{
struct vtable lpVtbl;
LONG counter;
};

struct COMTest* factory();
HRESULT HelloWorld_f(void* this,char* who){
	printf("hello %s\n",who);
	return S_OK;
}



ULONG addref_f(void* this){
	return InterlockedIncrement(&((struct COMTest*)this)->counter);
}
ULONG release_f(void* this){
	LONG a = InterlockedDecrement(&((struct COMTest*)this)->counter);
	if(a==0){
		free(this);
	}
	return a;
}

HRESULT query_f(void* this, REFIID qGUID, void** out){
if (out == NULL){
		out = NULL;
		return  E_POINTER;
	}
	if(IsEqualIID(qGUID,&gid) || IsEqualIID(&IID_IUnknown, qGUID) ){
		*out = this;
		addref_f(this);
		return S_OK;
	}else{
		*out = NULL;
		return E_NOINTERFACE;
	}

}

HRESULT F_LockServer_f(void* this,BOOL fLock){
if(fLock){
(((struct F_COMTest*)this)->lock_counter)++;
return S_OK;
}
(((struct F_COMTest*)this)->lock_counter)--;
return S_OK;

}

ULONG F_release_f(void* this){
	LONG a = InterlockedDecrement(&((struct F_COMTest*)this)->counter);
	if(a==0){
		free(this);
	}
	return a;
}

ULONG F_addref_f(void* this){
	return InterlockedIncrement(&((struct F_COMTest*)this)->counter);
}

HRESULT F_query_f(void* this, REFIID qGUID, void** out){
if (out == NULL){
		out = NULL;
		return  E_POINTER;
	}
	if(IsEqualIID(qGUID,&F_gid) || IsEqualIID(&IID_IUnknown, qGUID) ){
		*out = this;
		addref_f(this);
		return S_OK;
	}else{
		*out = NULL;
		return E_NOINTERFACE;
	}

}
HRESULT F_CreateInstance_f (void* this,IUnknown *pUnkOuter,REFIID riid,void **ppvObject){
if(pUnkOuter != NULL){
	MessageBox(NULL,"wywolano mnie jako part of aggregate, ten ficzer nie jest wspierany\r\n","ctr-alt-f3",0);
	return CLASS_E_NOAGGREGATION;
}

struct COMTest* temp = factory();
query_f(temp, riid, ppvObject);
temp->lpVtbl.Release(this);
return S_OK;
}


const static struct vtable vtable = {query_f,addref_f,release_f,HelloWorld_f};
const static struct F_vtable F_vtable = {F_query_f,F_addref_f,F_release_f,F_CreateInstance_f,F_LockServer_f};


 HRESULT DllGetClassObject(
   REFCLSID rclsid,
   REFIID   riid,
   LPVOID   *ppv){

 	struct F_COMTest *temp = malloc(sizeof(struct F_COMTest));
 	temp->counter = 1;
	temp->lpVtbl = F_vtable;
	ppv = (void*)temp;

	return S_OK;
 }

HRESULT DllRegisterServer(){ 
	const char path[] = "CLSID\\{2613b4b0-f7dc-4aaa-8609-3ab240cd68a6}";
HKEY key;
DWORD dwDisposition;

LONG res = RegCreateKeyExA(HKEY_CLASSES_ROOT,path,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&key,&dwDisposition);

if (res != ERROR_SUCCESS) {
        MessageBox(NULL,"1 etap rejestracji mnie nie powiodla sie\r\n","ctr-alt-f3",0);
        RegCloseKey(key);return S_FALSE;
    }

if(dwDisposition ==REG_OPENED_EXISTING_KEY){
	MessageBox(NULL,"1 etap rejestracji: probowałem się zarejestrowac na juz istniejacym GID, wychodzę stad\r\n","ctr-alt-f3",0);
	//RegCloseKey(key);return S_FALSE;	
}

const char name[] = "ctr-alt-f3 example COM object for doin' nothing";
LSTATUS status = RegSetValueExA(key,NULL,0,REG_SZ,name,48);

if(status !=ERROR_SUCCESS){
	MessageBox(NULL,"nie moglem sie zarejestrowac, uprawnienia?\r\n","ctr-alt-f3",0);
	RegCloseKey(key);return S_FALSE;	
}


RegCloseKey(key);


const char path2[] = "CLSID\\{2613b4b0-f7dc-4aaa-8609-3ab240cd68a6}\\InprocServer32";
res = RegCreateKeyExA(HKEY_CLASSES_ROOT,path2,0,NULL,REG_OPTION_NON_VOLATILE,KEY_WRITE,NULL,&key,&dwDisposition);

if (res != ERROR_SUCCESS) {
        MessageBox(NULL,"2 etap: rejestracja mnie nie powiodla sie\r\n","ctr-alt-f3",0);
        RegCloseKey(key);return S_FALSE;
    }
const char path3[] = "C:\\Users\\arch\\Desktop\\linux\\windows\\obiektCOM\\COM_obj.dll";
status = RegSetValueExA(key,NULL,0,REG_SZ,path3,58);

if(status !=ERROR_SUCCESS){
	MessageBox(NULL,"2 etap: nie moglem sie zarejestrowac, uprawnienia?\r\n","ctr-alt-f3",0);
	RegCloseKey(key);return S_FALSE;	
}
RegCloseKey(key);

return S_OK;
}

HRESULT DllUnregisterServer(){

LSTATUS status = RegDeleteTreeA(HKEY_CLASSES_ROOT,"CLSID\\{2613b4b0-f7dc-4aaa-8609-3ab240cd68a6}");
if(status!=ERROR_SUCCESS){
	MessageBox(NULL,"derejestracja obiektu COM sie nie udala\r\n","ctr-alt-f3",0);
	return E_FAIL;
}

}

struct COMTest* factory(){
	struct COMTest *temp = malloc(sizeof(struct COMTest));
	temp->counter = 1;
	temp->lpVtbl = vtable;
	return temp;
}

struct F_COMTest* F_factory(){
	struct F_COMTest *temp = malloc(sizeof(struct F_COMTest));
	temp->counter = 1;
	temp->lpVtbl = F_vtable;
	return temp;
}



