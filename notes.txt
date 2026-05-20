//clsid to cała implementacja obiektu com, iid to tylko jednego interfejsu tej klasy
przede wszystkim przy pomocy  [guid]::NewGuid() wygeneruj se guid dla tego obiektu COM
zadeklaruj globalny objcount (liczy ile istnieje instancji danego obiektu COM) i lock_count (pozwala zwiększyć se ten counter żeby nie unloadowało dll nawet gdy dll nie ma obiektów - używane m.in. przez internals windowsa gdy wie że zaraz będzie inicjować dużo obiektów com i nie chce żęby dll się zwinęła do tego czasu)
potem main struct z pointerem do vtable i potem własne zmienne (koniecznie refcounter)
vtable query,add,release,inne user-made OCZYWIŚCIE JAKO FUNCTION PTRS
query daje pointer do danego interfejsu (co za tym idzie - zwiększa refcount) w obiekcie COM - bierze pointer IID. Jak daje IID_Unknown daje głównego structa
addref zwiększa counter danej jednej instancji obiektu com
release zmniejsza counter danej jednej instancji obiektu com, jak się okazuje że jest 0 to robi free na tej instancji i zmniejsza globalny objcount
fabryka do main structa która strukturą przypomina main structa:
	struct z pointerem do vtable
	refcounter
	vtable ma query,add,release,CreateInstance(wrapper na funkcje fabryczną która mallocuje kolejną instancje main structa, ona (tak jak wszystkie main structy, nie tylko fabryczne) ma doszytą STATYCZNĄ CONST deklarację vtable (tak jak wszystkie deklaracje vtable) (tak jak struktów może być wiele na raz zainicjowanych (jako osobne obiety com tego samego typu), tak wszystkie odnoszą się do jednego structa vtable który jest const), umożliwia używanie COM obj aggregates (albo przy próbie bycia wywołanym jako aggregate oddaje CLASS_E_NOAGGREGATION) i (ofc zwiększa objcounter)), i LockServer (param jako bool -  w zaleznosci od boola (1 = zwieksz), zwiększa/zmiejsza lock_count danej dll) OCZYWIŚCIE JAKO FUNCTION PTRS
	DllGetClassObject czyli funkcje fabryczną na structa fabryki dla main structa zwracający konkretny interfejs i ofc zwieksza refcount DANEGO OBIEKTU COM
funkcja CanUnloadNow która sprawdza objcount i lock_count i jeśli obydwa to 0 to zwraca S_OK (internals windowsa se same ją unloadują)
funkcja register server co rejestruje pod HKEY_CLASSES_ROOT CLSID {<gid>} nazwe obiektu (human readable), pod \InprocServer32 jako zwykły null terminated string path do dll-ki trzymającej obiekt COM. 
pod inprocserver jest też string o nazwie ThreadingModel - opcje to: Apartment, Free, Both, Neutral. Jak się nie bawisz w ochronę przed środowiskami multi-threaded to dawaj apartment
funkcja unregister server co wywala cały HKEY_CLASSES_ROOT CLSID {<gid>}
