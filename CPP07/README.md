# CPP07 - Templates

Modulo sui template.
Idea base:

```text
scrivi una volta, usa con tipi diversi
```

Esempio:

```cpp
template <typename T>
T max(T a, T b)
{
    return (a > b ? a : b);
}
```

Se la chiamo con `int`, il compilatore genera una versione per `int`.
Se la chiamo con `std::string`, genera una versione per stringhe.

Nota: il tipo deve supportare le operazioni usate nel template.

---

# Template function

Forma:

```cpp
template <typename T>
void func(T value)
{
    ...
}
```

`T` = placeholder di tipo.
Il compilatore lo deduce dalla chiamata.

```cpp
func(42);                    // T = int
func("ciao");               // T = const char*
func(std::string("ciao"));   // T = std::string
```

Conseguenza importante:
- se dentro uso `a < b`, `T` deve avere `operator<`
- se dentro uso `std::cout << value`, `T` deve essere stampabile

---

# Template class

Stessa idea, applicata a una classe.

```cpp
template <typename T>
class Array
{
    T *_array;
};
```

Istanze possibili:

```cpp
Array<int>
Array<std::string>
Array<double>
```

Struttura uguale, tipo degli elementi diverso.

## Template negli header

In ex02:

```cpp
#include "Array.tpp"
```

alla fine di `Array.hpp`.

Nota:
- il compilatore deve vedere le definizioni template quando le usa
- se le metto solo in `.cpp`, rischio errori di linker
- schema 42 comune: dichiarazioni in `.hpp`, implementazioni in `.tpp`, `.tpp` incluso dall'header

---

# Ex00 - Whatever

## Idea

Tre funzioni template:
- `swap`
- `min`
- `max`

Nel file `whatever.hpp`:

```cpp
template <typename T>
T max(T a, T b)
```

```cpp
template <typename T>
T min(T &a, T &b)
```

```cpp
template <typename T>
void swap(T &a, T &b)
```

Idea: stesse funzioni per `int`, `std::string`, ecc.

## swap

```cpp
template <typename T>
void swap(T &a, T &b)
{
    T c = b;
    b = a;
    a = c;
}
```

Nota: riferimenti obbligatori per modificare le variabili originali.
Se fossero valori, cambierebbero solo copie locali.

```cpp
int a = 2;
int b = 3;
::swap(a, b);
```

Dopo:

```text
a = 3, b = 2
```

## min e max

`min`: valore più piccolo.
`max`: valore più grande.

Usano:

```cpp
<
>
```

Quindi `T` deve essere confrontabile.
Con `std::string` il confronto è lessicografico.

```text
"chaine1" < "chaine2"
```

## Perché `::swap`

Nel main:

```cpp
::swap(a, b);
::min(a, b);
::max(a, b);
```

`::` = namespace globale.
Serve a prendere le funzioni definite da me, non omonime in altri namespace.

## Punti ex00

- function template
- deduzione di `T`
- riferimenti in `swap`
- operatori richiesti dal tipo (`<`, `>`, assegnazione)
- `::` per namespace globale

---

# Ex01 - Iter

## Idea

Funzione template `iter`:
1. riceve un array
2. riceve la lunghezza
3. riceve una funzione/functor
4. applica la funzione a ogni elemento

```cpp
template <typename T, typename F>
void iter(T *array, int length, F func)
{
    for (int i = 0; i < length; i++)
        func(array[i]);
}
```

Template parameter:
- `T`: tipo degli elementi
- `F`: tipo della funzione/functor

## Funzione come parametro

Nel main:

```cpp
template <typename T>
void print(T item)
{
    std::cout << item << std::endl;
}
```

Uso:

```cpp
iter(array, 4, print<int>);
```

Significa:

```text
applica print<int> a ogni elemento
```

Con stringhe:

```cpp
iter(ciccio, 3, print<const std::string>);
```

## Scorrimento array

`iter` non conosce la dimensione dell'array.
La riceve:

```cpp
int length
```

Poi:

```cpp
for (int i = 0; i < length; i++)
    func(array[i]);
```

Pattern:

```text
per ogni elemento -> applica funzione
```

## Const correctness

Nel main gli array sono `const`:

```cpp
const int array[4] = {1, 2, 3, 4};
const std::string ciccio[3] = {...};
```

Nota:
- `iter` deve funzionare anche con dati const
- `print` non modifica l'elemento
- se la funzione provasse a modificare elementi const, il compilatore dovrebbe bloccarla

## Punti ex01

- function template con più parametri template
- array passato come puntatore
- lunghezza passata separatamente
- funzione/functor come parametro
- const correctness

---

# Ex02 - Array

## Idea

Classe template `Array<T>`: piccolo array dinamico sicuro.

```cpp
template <typename T>
class Array
{
private:
    T *_array;
    unsigned int _size;
};
```

Contiene:
- puntatore agli elementi
- size

Uso:

```cpp
Array<int> numbers(750);
Array<std::string> strings(10);
```

## Costruttore di default

```cpp
template <typename T>
Array<T>::Array() : _array(NULL), _size(0) {}
```

Stato:

```text
_array = NULL
_size = 0
```

## Costruttore con size

```cpp
template <typename T>
Array<T>::Array(unsigned int n) : _array(new T[n]), _size(n) {}
```

Alloca `n` elementi di tipo `T`.

```cpp
Array<int> numbers(750);
```

## Copy constructor

```cpp
template <typename T>
Array<T>::Array(const Array<T> &other)
    : _array(new T[other.size()]), _size(other.size())
{
    for (unsigned int i = 0; i < _size; i++)
        _array[i] = other._array[i];
}
```

Nota: deep copy.

Non basta copiare il puntatore.
Serve:
- nuovo array
- copia elemento per elemento

Altrimenti due `Array` condividono memoria e alla distruzione arriva il doppio `delete[]`.

## Operator=

Passi:
1. controllo auto-assegnazione
2. libero vecchio array
3. copio size
4. alloco nuovo array
5. copio elementi

```cpp
if (this != &other)
{
    if (_array)
        delete[] _array;
    _size = other.size();
    _array = new T[_size];
    ...
}
```

Stesso punto: evitare shallow copy.

## Destructor

```cpp
template <typename T>
Array<T>::~Array()
{
    if (_array)
        delete[] _array;
}
```

Regola:

```text
new[] -> delete[]
```

Non `delete` semplice.

## operator[]

Header:

```cpp
T &operator[](unsigned int n) const;
```

Implementazione:

```cpp
if (n >= this->_size)
    throw std::out_of_range("Index out of range.");
return (_array[n]);
```

Accesso più sicuro di un array C.
Fuori range -> eccezione.

Indici validi:

```text
0 ... size - 1
```

## Il caso numbers[-2]

Nel main:

```cpp
numbers[-2] = 0;
```

L'operatore riceve `unsigned int`.
`-2` viene convertito in un unsigned molto grande.

Il controllo intercetta comunque:

```cpp
n >= _size
```

Nota: `unsigned` non impedisce magicamente un literal negativo passato dal chiamante.
Avviene una conversione.

## size()

```cpp
template <typename T>
unsigned int Array<T>::size() const
{
    return (_size);
}
```

Restituisce il numero di elementi.
È `const` perché non modifica l'oggetto.

## Test nel main

Schema:
1. crea `Array<int> numbers(MAX_VAL)`
2. crea array C `mirror`
3. riempie entrambi con stessi valori random
4. crea copie in uno scope
5. controlla che `numbers` sia ancora uguale a `mirror`
6. prova accessi fuori range
7. libera `mirror`

Scope di test:

```cpp
{
    Array<int> tmp = numbers;
    Array<int> test(tmp);
}
```

Nota: testa copy constructor, operator= e destructor.
Se la copia fosse shallow, qui uscirebbero problemi di memoria.
