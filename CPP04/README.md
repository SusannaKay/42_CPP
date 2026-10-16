# CPP04 - Polimorfismo, classi astratte e deep copy

In CPP03 abbiamo visto l'ereditarietà. Qui si va un passo oltre: cosa succede quando uso un puntatore alla classe base per un oggetto derivato.

```cpp
const Animal* animal = new Dog();
animal->makeSound();
```

Abbaia o fa un verso generico?

Dipende da una parola sola: `virtual`.

Cose nuove:

- polimorfismo
- funzioni virtuali
- distruttore virtuale
- deep copy vs shallow copy
- classi astratte e funzioni virtuali pure

---

# Polimorfismo

## Tipo statico e tipo dinamico

```cpp
Animal* a = new Cat();
```

Qui ci sono due tipi:

- tipo statico: `Animal*`, cioè quello che vede il compilatore
- tipo dinamico: `Cat`, cioè l'oggetto che c'è davvero in memoria

Polimorfismo = stessa chiamata, comportamento diverso in base al tipo dinamico.

## virtual

In `Animal`:

```cpp
virtual void makeSound() const;
```

`virtual` dice: quando chiami questo metodo tramite un puntatore o una reference, guarda cosa c'è davvero dietro, non il tipo del puntatore.

```text
a->makeSound()

con virtual    -> Cat::makeSound()     "Meow meow!"
senza virtual  -> Animal::makeSound()  verso generico
```

Senza `virtual`, il compilatore decide quale metodo chiamare al momento della compilazione, guardando solo il tipo del puntatore. Con `virtual`, la decisione viene presa a runtime.

Questo si chiama binding dinamico.

Internamente funziona con una tabella di puntatori a funzione per ogni classe (la vtable). Non serve saperla implementare, ma è utile sapere che esiste: è il "costo" del polimorfismo.

## Distruttore virtuale

In `Animal`:

```cpp
virtual ~Animal();
```

Fondamentale se cancello tramite puntatore base:

```cpp
Animal* a = new Dog();
delete a;
```

Con distruttore virtuale:

```text
Dog destructor called
Animal destructor called
```

Senza, viene chiamato solo `~Animal()`. Il distruttore di `Dog` non parte mai, e se `Dog` ha memoria sua (il `Brain` dell'ex01) è leak.

Regola pratica: se una classe ha anche un solo metodo `virtual`, il distruttore deve essere `virtual`.

## Override

Le derivate ridefiniscono il metodo con la stessa firma:

```cpp
void Cat::makeSound() const
{
    std::cout << "Meow meow!" << std::endl;
}
```

Il `virtual` basta metterlo nella base: il metodo resta virtuale anche nelle derivate.

---

# Ex00 - Polymorphism

## Obiettivo

Creare una gerarchia `Animal`, `Dog`, `Cat` con `makeSound()` virtuale, e una gerarchia sbagliata `WrongAnimal` / `WrongCat` per vedere cosa succede senza `virtual`.

```text
Animal               WrongAnimal
  |-- Dog              |-- WrongCat
  |-- Cat
```

## Animal, Dog, Cat

- `Animal` ha l'attributo `type` (`protected`) e `makeSound()` virtuale
- `Dog` imposta `type = "Dog"` nel costruttore e ridefinisce il verso
- `Cat` imposta `type = "Cat"` e ridefinisce il verso

`getType()` invece non è virtuale e non serve che lo sia: è uguale per tutti, legge solo `type`.

## WrongAnimal e WrongCat

Stessa struttura, ma in `WrongAnimal`:

```cpp
void makeSound() const;
```

Non è virtuale.

Quindi:

```cpp
const WrongAnimal* wrong_cat = new WrongCat();
wrong_cat->makeSound();
```

stampa il verso di `WrongAnimal`, non quello di `WrongCat`.

`WrongAnimal` non va corretto. Serve a vedere il caso sbagliato.

## Test nel main

Il main fa tre blocchi:

1. test base del subject: `Animal`, `Dog` e `Cat` tramite `Animal*`
2. test sbagliato: `WrongCat` tramite `WrongAnimal*`
3. un array di `Animal*` con dentro cani e gatti, per vedere che ognuno fa il suo verso anche se l'array è dello stesso tipo

---

# Ex01 - I don't want to set the world on fire

## Obiettivo

Aggiungere la classe `Brain`, farla possedere a `Dog` e `Cat` tramite puntatore, gestire `new`/`delete` e dimostrare che le copie sono profonde.

```cpp
class Brain
{
    public:
    std::string ideas[100];
};
```

`Dog` e `Cat` hanno:

```cpp
Brain *ideas;
```

## Gestione della memoria

Ogni `Dog` e ogni `Cat` deve avere il proprio `Brain`:

- `new Brain` nel costruttore
- `delete` nel distruttore
- copia profonda nel costruttore di copia
- copia profonda in `operator=`

```cpp
Cat::Cat() : ideas(new(Brain)) {}

Cat::~Cat()
{
    delete ideas;
}
```

## Shallow copy

Il costruttore di copia e l'`operator=` di default copiano gli attributi uno per uno. Per un puntatore, vuol dire copiare l'indirizzo:

```text
cat1.ideas ----\
                > stesso Brain
cat2.ideas ----/
```

Problemi:

- modifico le idee di uno, cambiano anche quelle dell'altro
- quando muoiono, tutti e due fanno `delete` sullo stesso puntatore: double free

## Deep copy

```text
cat1.ideas ---> Brain A
cat2.ideas ---> Brain B con gli stessi valori
```

Costruttore di copia:

```cpp
Cat::Cat(const Cat& other) : Animal(other), ideas(new(Brain))
{
    *ideas = *other.ideas;
}
```

Alloco un `Brain` nuovo e ci copio dentro il contenuto, non l'indirizzo.

`*ideas = *other.ideas` usa l'`operator=` di `Brain`, che copia le 100 stringhe.

Operatore di assegnazione:

```cpp
Cat& Cat::operator=(const Cat& other)
{
    if (this != &other)
    {
        Animal::operator=(other);
        *ideas = *other.ideas;
    }
    return (*this);
}
```

Qui il `Brain` esiste già (l'oggetto è già stato costruito), quindi non serve allocarne uno nuovo: basta copiarci dentro il contenuto.

L'errore classico è scrivere `ideas = other.ideas;`. Compila, sembra funzionare, ma è una shallow copy: perdo il mio `Brain` (leak) e poi due oggetti fanno `delete` sullo stesso. Nella prima versione del codice c'era proprio questo errore, sia in `Dog` sia in `Cat`.

## Test nel main

```text
creo Cat a
setto idea "fish"
copio: Cat b = a
cambio l'idea di b in "pizza"
a deve avere ancora "fish"
```

Se cambiano insieme: shallow copy.

Poi il main crea un array di 100 `Animal*`, metà cani e metà gatti, e li cancella tutti tramite il puntatore base. Qui si vede perché serve il distruttore virtuale: ogni `delete` deve chiamare anche il distruttore della derivata, che libera il `Brain`.

---

# Ex02 - Abstract class

## Obiettivo

Rendere `Animal` non istanziabile, lasciandola però come base comune per `Dog` e `Cat`.

Un animale generico non esiste: esistono cani e gatti. Quindi non ha senso poter scrivere `Animal a;`.

## Funzione virtuale pura

```cpp
virtual void makeSound() const = 0;
```

`= 0` vuol dire: questo metodo non ha implementazione nella base, deve fornirla chi eredita.

Una classe con almeno una funzione virtuale pura è una **classe astratta**:

- non si può istanziare
- si può usare come tipo per puntatori e reference
- le derivate concrete devono implementare tutte le funzioni pure, sennò diventano astratte anche loro

```cpp
Animal a;               // non compila
Animal* a = new Cat();  // ok
```

Nel `main.cpp` c'è la riga:

```cpp
//Animal c;
```

Se la decommenti, la compilazione deve fallire. È la prova che `Animal` è astratta.

La base resta interfaccia comune:

- tiene `type`
- espone `getType()`
- obbliga le derivate a implementare `makeSound()`

---

# Ex03 - Interface & recap

Questo esercizio non è nella repo, quindi qui c'è solo l'idea generale.

Mette insieme tutto il modulo con un sistema di Materia (`AMateria`, `Ice`, `Cure`, `Character`, `MateriaSource`):

- interfacce, cioè classi con solo funzioni virtuali pure (`ICharacter`, `IMateriaSource`)
- polimorfismo tramite puntatori alla base
- `clone()` per copiare un oggetto senza sapere il suo tipo reale
- deep copy di un inventario di puntatori

---

# Checklist finale modulo

## Concetti da sapere spiegare

- cos'è il polimorfismo
- tipo statico vs tipo dinamico
- cosa fa `virtual` e cos'è il binding dinamico
- perché serve il distruttore virtuale
- cosa succede con `WrongAnimal` e perché
- differenza tra shallow copy e deep copy
- perché costruttore di copia e `operator=` gestiscono il `Brain` in modo diverso
- cos'è una funzione virtuale pura
- cos'è una classe astratta e perché `Animal` deve esserlo

## Errori tipici da evitare

- dimenticare `virtual` su `makeSound()`
- dimenticare il distruttore virtuale: leak quando cancello tramite `Animal*`
- copiare il puntatore al `Brain` invece del contenuto
- in `operator=` allocare un nuovo `Brain` senza liberare quello vecchio
- dimenticare `Animal::operator=(other)` e non copiare `type`
- dimenticare `delete ideas` nel distruttore di una delle due derivate
- creare un array di N elementi e riempirne N + 1

## Mini riassunto mentale

```text
ex00 -> virtual = decide il tipo reale a runtime
        WrongAnimal = cosa succede senza
ex01 -> Brain* in Dog e Cat
        new nel costruttore, delete nel distruttore
        deep copy in copia e operator=
ex02 -> makeSound() = 0, Animal astratta
```
