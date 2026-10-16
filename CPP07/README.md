# CPP07 - Template

Modulo sui template.

Idea base:

```text
scrivo una volta, uso con tipi diversi
```

Senza template, per avere una `max` per `int`, una per `double` e una per `std::string` dovrei scrivere tre funzioni identiche che cambiano solo nel tipo. Con un template la scrivo una volta e il compilatore genera lui le versioni che servono.

Cose nuove:

- function template
- class template
- file `.tpp`
- deduzione del tipo
- funzioni passate come parametro

---

# Function template

## Forma

```cpp
template <typename T>
T max(T a, T b)
{
    if (a > b)
        return a;
    return b;
}
```

`T` è un segnaposto per un tipo. Non esiste finché qualcuno non chiama la funzione.

## Deduzione del tipo

Il compilatore capisce `T` da solo, guardando gli argomenti:

```cpp
max(2, 3);                                  // T = int
max(2.5, 3.1);                              // T = double
max(std::string("a"), std::string("b"));    // T = std::string
```

Per ogni tipo diverso genera una versione della funzione. Questo si chiama istanziazione.

Quindi il template in sé non è codice: è una ricetta. Il codice vero nasce quando lo uso.

## Il tipo deve supportare le operazioni

Il template non controlla niente in anticipo. Se dentro uso `a > b`, `T` deve avere `operator>`. Se non ce l'ha, l'errore arriva solo quando provo a usarlo con quel tipo.

- se uso `<` o `>`, `T` deve essere confrontabile
- se uso `std::cout << value`, `T` deve essere stampabile
- se faccio `T c = b;`, `T` deve essere copiabile

---

# Class template

Stessa idea, applicata a una classe:

```cpp
template <typename T>
class Array
{
    T *_array;
    unsigned int _size;
};
```

Qui però il tipo non si può dedurre, va scritto:

```cpp
Array<int> numbers(750);
Array<std::string> strings(10);
```

`Array<int>` e `Array<std::string>` per il compilatore sono due classi diverse, generate dallo stesso stampo.

## Template negli header

Le funzioni template vanno scritte negli header, non nei `.cpp`.

Perché? Il compilatore genera il codice per un tipo quando vede l'uso, tipo `Array<int>`. In quel momento deve avere sotto mano anche le definizioni delle funzioni. Se stanno in un `.cpp` compilato a parte, non le vede e il linker non trova niente.

Per non avere un header enorme, lo schema comune è:

- dichiarazioni in `Array.hpp`
- implementazioni in `Array.tpp`
- `#include "Array.tpp"` in fondo ad `Array.hpp`

```cpp
#ifndef ARRAY_HPP
# define ARRAY_HPP
...
#include "Array.tpp"
#endif
```

Il `.tpp` non si compila da solo e non va nel Makefile: è come se il suo contenuto fosse scritto dentro l'header.

---

# Ex00 - Start with a few functions

## Obiettivo

Tre function template in `whatever.hpp`:

- `swap`: scambia due valori
- `min`: ritorna il più piccolo
- `max`: ritorna il più grande

Se i due valori sono uguali, `min` e `max` devono ritornare il secondo.

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

Qui le reference sono obbligatorie. Se prendesse `T a, T b` per valore, scambierebbe due copie locali e le variabili originali resterebbero uguali.

## min e max

```cpp
template <typename T>
T min(T &a, T &b)
{
    if (a < b)
        return a;
    return b;
}
```

Con `a < b` stretto, se sono uguali la condizione è falsa e ritorna `b`, cioè il secondo, come chiede il subject.

`max` uguale con `>`.

Con `std::string` il confronto è lessicografico, cioè in ordine alfabetico:

```text
"chaine1" < "chaine2"
```

## Perché ::swap

Nel main:

```cpp
::swap(a, b);
::min(a, b);
::max(a, b);
```

`::` davanti vuol dire "namespace globale". Esistono già `std::swap`, `std::min` e `std::max`, e con `std::string` il compilatore potrebbe trovare quelle (le cerca anche nel namespace dei tipi degli argomenti). Con `::` sono sicura di chiamare le mie.

---

# Ex01 - Iter

## Obiettivo

Function template `iter` che:

1. riceve un array
2. riceve la sua lunghezza
3. riceve una funzione
4. applica la funzione a ogni elemento

```cpp
template <typename T, typename F>
void iter(T *array, int length, F func)
{
    for (int i = 0; i < length; i++)
        func(array[i]);
}
```

Due parametri template:

- `T`: tipo degli elementi
- `F`: tipo della funzione

## La lunghezza

`iter` riceve un puntatore al primo elemento, non l'array intero. Da un puntatore non si può sapere quanti elementi ci sono dopo, quindi la lunghezza va passata a parte. Esattamente come in C.

## Funzione come parametro

Anche la funzione da applicare può essere un template:

```cpp
template <typename T>
void print(T item)
{
    std::cout << item << std::endl;
}
```

Ma quando la passo a `iter` devo dire quale versione voglio, perché da sola non ha un tipo:

```cpp
iter(array, 4, print<int>);
```

Significa: applica `print<int>` a ogni elemento.

## const

Nel main gli array sono `const`:

```cpp
const int array[4] = {1, 2, 3, 4};
const std::string ciccio[3] = {...};
```

Quindi `T` viene dedotto come `const int` e `const std::string`, e `iter` funziona anche su dati che non si possono modificare.

`print` va bene perché li legge e basta. Se passassi una funzione che prova a modificare gli elementi, il compilatore la bloccherebbe.

---

# Ex02 - Array

## Obiettivo

Class template `Array<T>`: un array dinamico che conosce la sua dimensione e controlla gli accessi.

```cpp
template <typename T>
class Array
{
private:
    T *_array;
    unsigned int _size;
};
```

## Costruttori

Default, array vuoto:

```cpp
Array<T>::Array() : _array(NULL), _size(0) {}
```

Con dimensione:

```cpp
Array<T>::Array(unsigned int n) : _array(new T[n]()), _size(n) {}
```

Le `()` dopo `new T[n]` sono importanti. Senza, per tipi come `int` gli elementi restano con valori casuali. Con le `()` vengono inizializzati al valore di default (`0` per gli int, stringa vuota per le string). Il subject chiede proprio che gli elementi siano inizializzati di default.

## Costruttore di copia

```cpp
Array<T>::Array(const Array<T> &other)
    : _array(new T[other.size()]), _size(other.size())
{
    for (unsigned int i = 0; i < _size; i++)
        _array[i] = other._array[i];
}
```

Deep copy, come il `Brain` di CPP04: nuovo array, poi copio elemento per elemento.

Se copiassi solo il puntatore, due `Array` condividerebbero la memoria e alla distruzione ci sarebbe un doppio `delete[]`.

## operator=

1. controllo auto-assegnazione
2. libero il vecchio array
3. copio la size
4. alloco il nuovo array
5. copio gli elementi

```cpp
if (this != &other)
{
    if (_array)
        delete[] _array;
    _size = other.size();
    _array = new T[_size];
    for (unsigned int i = 0; i < _size; i++)
        _array[i] = other._array[i];
}
return (*this);
```

Il controllo al punto 1 serve davvero: con `a = a`, senza controllo libererei l'array e poi proverei a copiarlo da memoria già liberata.

## Distruttore

```cpp
Array<T>::~Array()
{
    if (_array)
        delete[] _array;
}
```

`new[]` va con `delete[]`, mai `delete` semplice.

## operator[]

```cpp
T &operator[](unsigned int n) const;
```

```cpp
if (n >= this->_size)
    throw std::out_of_range("Index out of range.");
return (_array[n]);
```

A differenza di un array C, un accesso fuori range lancia un'eccezione invece di leggere memoria a caso.

Ritorna una reference, così posso anche scrivere: `numbers[3] = 42;`.

## Il caso numbers[-2]

Nel main:

```cpp
numbers[-2] = 0;
```

L'operatore riceve un `unsigned int`, quindi `-2` viene convertito in un numero enorme (circa 4 miliardi).

Il controllo `n >= _size` lo intercetta comunque. Non serve un controllo apposta per i negativi.

## size()

```cpp
unsigned int Array<T>::size() const
{
    return (_size);
}
```

`const` perché non modifica l'oggetto.

## Test nel main

1. crea `Array<int> numbers(750)` e un array C `mirror` con gli stessi valori random
2. in uno scope separato crea copie con il costruttore di copia e con `operator=`
3. modifica una copia e controlla che l'originale non cambi
4. a fine scope le copie vengono distrutte
5. controlla che `numbers` sia ancora uguale a `mirror`
6. prova accessi fuori range con `-2` e `750`

```cpp
{
    Array<int> tmp = numbers;    // costruttore di copia (sta nascendo)
    Array<int> test(tmp);        // costruttore di copia
    Array<int> assigned;
    assigned = numbers;          // operator=
}
```

Occhio: `Array<int> tmp = numbers;` sembra un'assegnazione, ma `tmp` sta nascendo, quindi è il costruttore di copia (come in CPP02).

Se una delle copie fosse shallow, alla chiusura dello scope ci sarebbe un doppio `delete[]` e il controllo dopo fallirebbe.

---

# Checklist finale modulo

## Concetti da sapere spiegare

- cos'è un template e cosa vuol dire istanziazione
- come il compilatore deduce `T`
- differenza tra function template e class template
- perché i template stanno negli header e cos'è il `.tpp`
- perché `swap` prende reference
- perché `min`/`max` ritornano il secondo se sono uguali
- perché uso `::swap` e non `swap`
- perché `iter` ha bisogno della lunghezza
- perché passo `print<int>` e non solo `print`
- differenza tra `new T[n]` e `new T[n]()`
- deep copy in costruttore di copia e `operator=`
- perché `numbers[-2]` viene intercettato

## Errori tipici da evitare

- mettere le definizioni dei template in un `.cpp`
- mettere il `.tpp` nel Makefile
- `swap` con parametri per valore
- `min`/`max` con `<=` / `>=`: ritornano il primo quando sono uguali
- dimenticare le `()` in `new T[n]()`
- shallow copy in `Array`
- `delete` invece di `delete[]`
- dimenticare il controllo di auto-assegnazione in `operator=`

## Mini riassunto mentale

```text
ex00 -> function template: swap (reference), min, max
ex01 -> template con due tipi: array T* + funzione F
ex02 -> class template Array<T>
        .hpp + .tpp, deep copy, operator[] con eccezione
```
