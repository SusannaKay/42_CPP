# CPP04 - Polimorfismo, classi astratte, deep copy

Modulo sul polimorfismo vero.
Non basta ereditare: conta cosa succede quando uso un puntatore alla classe base.

```cpp
const Animal* animal = new Dog();
animal->makeSound();
```

Scopo:
- se `makeSound()` è `virtual`, viene chiamato il metodo di `Dog`
- se non è `virtual`, viene chiamato quello di `Animal`
- tipo del puntatore != tipo reale dell'oggetto

---

# Concetti chiave

## Polimorfismo

Stessa chiamata, comportamento diverso in base all'oggetto reale.

```text
Animal* a = new Cat();
a->makeSound();
```

Nota:
- tipo statico: `Animal*`
- tipo dinamico: `Cat`
- con metodo virtuale: `Cat::makeSound()`
- senza virtual: `Animal::makeSound()`

## virtual

In `Animal`:

```cpp
virtual void makeSound() const;
```

Nota: `virtual` permette il binding dinamico.
La funzione può essere ridefinita nelle derivate.

`Dog` e `Cat` hanno il loro verso.

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
Dog destructor
Animal destructor
```

Senza, rischio di distruggere solo la parte base.
Con memoria dinamica (`Brain`) può diventare leak o comportamento indefinito.

## Classe astratta

In ex02:

```cpp
virtual void makeSound() const = 0;
```

`= 0` = funzione virtuale pura.

Effetto:
- `Animal` non è più istanziabile
- resta usabile come tipo base
- le derivate concrete devono implementare `makeSound()`

```cpp
Animal a; // non deve compilare
Animal* a = new Cat(); // ok
```

Idea: `Animal` è un concetto generale, non un animale concreto.

## Deep copy

In ex01 arrivano i `Brain`.

`Dog` e `Cat` possiedono un puntatore:

```cpp
Brain *ideas;
```

`Brain` contiene:

```cpp
std::string ideas[100];
```

Problema: copiare un `Cat` non può voler dire copiare solo il puntatore.

### Shallow copy

```text
cat1.ideas ----\
                > stesso Brain
cat2.ideas ----/
```

Rischi:
- modifico uno, cambia anche l'altro
- due `delete` sullo stesso puntatore
- double free

### Deep copy

```text
cat1.ideas ---> Brain A
cat2.ideas ---> Brain B con stessi valori
```

Nel copy constructor:

```cpp
Cat::Cat(const Cat& other) : Animal(other), ideas(new(Brain))
{
    *ideas = *other.ideas;
}
```

Nota: nuovo `Brain`, stesso contenuto. Puntatori diversi.

---

# Ex00 - Polymorphism

## Scopo

Esercizio: creare una gerarchia `Animal`, `Dog`, `Cat` con `makeSound()` virtuale, e una gerarchia sbagliata `WrongAnimal` / `WrongCat` per vedere cosa succede senza `virtual`.

Gerarchia:

```text
Animal
  |-- Dog
  |-- Cat
```

Punti:
- `Animal` ha `type` e `makeSound()` virtuale
- `Dog` imposta `type = "Dog"`
- `Cat` imposta `type = "Cat"`
- ogni derivata ridefinisce il verso

## WrongAnimal

Classi dimostrative:

```text
WrongAnimal
WrongCat
```

In `WrongAnimal`:

```cpp
void makeSound() const;
```

Non è virtuale.

Confronto:

```text
Animal + virtual makeSound     -> comportamento dinamico corretto
WrongAnimal senza virtual      -> chiamata legata al tipo del puntatore
```

Nota: `WrongAnimal` non va corretto. Serve a vedere il caso sbagliato.

---

# Ex01 - I don't want to set the world on fire

## Scopo

Esercizio: aggiungere la classe `Brain`, farla possedere a `Dog` e `Cat` tramite puntatore, gestire `new`/`delete` e dimostrare che le copie sono profonde.

Aggiungere `Brain` e farlo possedere a `Dog` e `Cat`.

```cpp
std::string ideas[100];
Brain *ideas;
```

Gestione necessaria:
- allocazione nel costruttore
- `delete` nel distruttore
- copia profonda nel copy constructor
- copia profonda in `operator=`

Nota: ogni `Dog` e ogni `Cat` deve avere il proprio `Brain`.

## Test utile

Schema mentale:

```text
creo Cat
setto idea
copio Cat
modifico idea di uno
l'altro non deve cambiare
```

Se cambiano insieme: shallow copy.

---

# Ex02 - Abstract class

## Scopo

Esercizio: rendere `Animal` non istanziabile usando una funzione virtuale pura, lasciandola però come base comune per `Dog` e `Cat`.

Rendere `Animal` astratta.

```cpp
virtual void makeSound() const = 0;
```

Nel `main.cpp`:

```cpp
//Animal c;
```

Se decommentata, la compilazione deve fallire.

Nota:
- `Dog` è un `Animal`
- `Cat` è un `Animal`
- un `Animal` generico non ha senso da istanziare

La base diventa interfaccia comune:
- tiene `type`
- espone `getType()`
- obbliga `makeSound()` nelle derivate

---

# Ex03 - Interface & recap

## Scopo

Esercizio: usare classi astratte come interfacce (`ICharacter`, `IMateriaSource`) e creare un sistema di Materia con `AMateria`, `Ice`, `Cure`, `Character` e `MateriaSource`.

Qui il subject mette insieme i concetti del modulo:

- polimorfismo tramite puntatori alla base
- classi astratte / interfacce
- `clone()` per creare copie del tipo reale
- inventario di massimo 4 Materia
- gestione memoria e deep copy

Nota: nel repository attuale non vedo ancora la cartella `ex03`, quindi questa parte resta solo come riferimento breve al subject.
