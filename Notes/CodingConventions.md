--------------------------
*SWAN Coding Conventions*
==========================

--------------------------
Indentation:
==========================
Indentation should be done with __**tabs**__ of width __**4**__.

--------------------------
Coding style:
==========================
For code, put the opening brackets for functions, ifs, else ifs, etc. on the same line, like so:
```cpp
if(condition) {
    // Do something
} else if(otherCondition) {
    // Do something else
} else {
    // Explode, crash, burn, die, etc.
}  

void foo(){
    // Function body
}
```
**However**, if the statement can be a short one-liner, put everything on the same line, like so:
```cpp
if(condition) foo();  

void bar() { return true; }
```

--------------------------
Naming conventions:
--------------------------

#### Use camelCase when naming:
* a method;
```cpp
class MyClass {
    void aMethod();
    void anotherMethod();
    void foo();
};
```
* a member variable;
```cpp
class MyClass {
    int number;
    const char* aCharArray;
};
```
* a local variable
```cpp
void bla() {
// ...
    int weight;
    float appliedForce;
// ...
}
```
--------------------------
#### Use CamelCase when naming:
* a function or variable inside a namespace
```cpp
namespace MyNamespace {
    int MyInteger;

    void MySpecialFunction();
    void MyOtherFunction();
}
```
* a class
```cpp
class MyClass {
    // code goes here
};
```
* a global variable
```cpp
int ImportantInteger;
float VeryImportantFloat;
```
--------------------------

#### Use UPPER_CASE when naming:
* a macro
```cpp
#define CODE_WORKS_CORRECTLY 0
#define CONCATENATE(str1, str2) //do the thing here
```
--------------------------
General tips:
--------------------------
* If a **function**, **variable** or **type** is to be used only for **debugging**, prefix it with `"dbg_"`.

* Use `DEBUG_OUT()`, `DEBUG_VAR()`, etc. if the purpose of something is only for **debugging**.

* When writing an **importer**, be it for a mesh, texture, sound, etc., prefer to write it as a free function.  
***__DO NOT__*** give the resource class the ability to import itself, *that is not its job*.

* If a class is going to be instantiated only once (i.e. it's a *singleton*), prefer to write it as a **namespace** instead.  
Put any *private* functions inside of a nested `detail` namespace.
--------------------------
