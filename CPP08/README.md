# CPP08 - Templated containers, iterators, algorithms

Primo modulo sulla STL. Da qui in poi si possono usare i container e gli algoritmi della libreria standard, e il punto è capire quale usare e perché. CPP09 riparte esattamente da qui.

Cose nuove:

- container
- iteratori
- algoritmi
- template
- container adapter

---

# Container

Container = strutture dati della STL (Standard Template Library).

Memorizzano gruppi di elementi in modo generico.

Il tipo contenuto lo decide il template.

Esempi:

```cpp
std::vector<int>
std::list<std::string>
std::map<std::string, int>
```

nota:

```text
std::vector<int>
```

= un vector che contiene int.

```text
std::list<std::string>
```

= una list che contiene stringhe.

Il container:

- gestisce memoria automaticamente
- offre funzioni standard tipo `insert`, `erase`, `size`, `begin`, `end`
- usa gli iteratori per scorrere gli elementi

## Tipi di container

### Sequence containers

Elementi in sequenza.

Esempi:

- `vector`
- `list`
- `deque`

### Associative containers

Elementi ordinati per chiave.

Esempi:

- `map`
- `set`
- `multimap`

### Adapter containers

Interfacce limitate costruite sopra altri container.

Esempi:

- `stack`
- `queue`

---

# Vector

```cpp
std::vector<int>
```

Array dinamico. Memoria contigua.

Come un array C, ma ridimensionabile.

Quando finisce lo spazio:

1. alloca un array più grande
2. copia/sposta gli elementi
3. libera il vecchio array

Pro:

- accesso per indice veloce: `v[i]`
- memoria contigua
- molto cache friendly

Contro:

- inserire in mezzo costa perché deve spostare elementi
- alcune operazioni possono invalidare iteratori e puntatori, soprattutto quando rialloca

---

# List

```cpp
std::list<int>
```

Lista doppiamente concatenata.

```text
[node] <-> [node] <-> [node]
```

Ogni nodo contiene:

- valore
- puntatore al precedente
- puntatore al successivo

Memoria non contigua.

nota:

- accesso random lento
- niente `list[i]`
- insert/erase sono veloci se ho già l'iteratore nella posizione giusta

---

# Deque

```cpp
std::deque<int>
```

Double ended queue è una via di mezzo tra vector e list.

Rispetto al vector fa molto bene:

- `push_back()`
- `push_front()`

Ha accesso rapido tramite indice.

Mentalmente può essere vista come blocchi di memoria:

```text
[1 2 3] --> [4 5 6] --> [7 8]
```

Cresce davanti e dietro senza spostare tutto ogni volta.

---

# Map

```cpp
std::map<std::string, int>
```

Fa associazione:

```text
chiave -> valore
```

Chiavi ordinate automaticamente.

Esempio:

```cpp
std::map<std::string, int> ages;
ages["Susanna"] = 42;
std::cout << ages["Susanna"];
```

Stampa:

```text
42
```


```cpp
ages["x"]
```

Se `x` non esiste, `operator[]` crea l'elemento.

Per controllare se esiste un elemento senza crearlo è meglio usare:

```cpp
ages.find("x")
```

---

# Set

```cpp
std::set<int>
```

Come una map senza valore associato.

Solo chiavi uniche e ordinate.

Utile quando voglio sapere se un valore esiste, ma non mi serve associare quel valore a qualcos'altro.

---

# Stack

```cpp
std::stack<int>
```

`std::stack` è un container adapter.

Non autonomo come `vector` o `list`: wrapper sopra un altro container.

Comportamento:

```text
Last In, First Out
```

ultimo dentro = primo fuori.

Tipo pila di piatti.

Operazioni principali:

- `push` -> metti un elemento sopra lo stack
- `top` -> leggi l'elemento in cima
- `pop` -> elimini l'elemento in cima
- `empty` -> controlli se è vuoto
- `size` -> controlli quanti elementi ha

Default interno: `deque`.

Quando scrivo:

```cpp
std::stack<int>
```

internamente fa una cosa tipo

```cpp
std::stack<int, std::deque<int> >
```

Lo stack usa deque perché ha bisogno soprattutto di `push_back`, `pop_back` e `back`, e deque gestisce bene la crescita dinamica.

Utile per:

- parsing
- DFS ( depth-first search - algoritmo utilizzato per esplorare alberi e grafi )
- gestione parentesi
- undo/redo
- call stack
- simulazione della ricorsione

Esempio parentesi:

```text
(((())))
```

Ogni `(` -> push.

Ogni `)` -> pop.

Alla fine stack vuoto = stringa valida.

---

# Queue

```cpp
std::queue<int>
```

La queue è:

```text
First In, First Out
```

primo dentro = primo fuori come una coda

---

# Scelta del container

Ogni container è ottimizzato per problemi diversi.

| Situazione                    | Container |
| ----------------------------- | --------- |
| array dinamico                | `vector`  |
| tante insert/remove nel mezzo | `list`    |
| chiave -> valore              | `map`     |
| valori unici                  | `set`     |
| pila                          | `stack`   |
| coda                          | `queue`   |

---

# Iterator

Iteratore = modo universale per attraversare un container.

In C con array possiamo usare un ciclo for perché:

- esiste un indice
- la memoria è contigua
- l'array è fisso

Con STL non vale sempre.

Esempio:

## Vector

```text
[1][2][3][4]
```

Indice ok.

## List

```text
[node] <-> [node] <-> [node]
```

Niente accesso random.

## Map

Internamente: struttura ordinata ad albero.

Non ha senso fare `map[i]` come se fosse un array.

quindi

```cpp
container[i]
```

funziona solo con container indicizzabili tipo vector/deque.

La STL standardizza il movimento nei container:

- i container forniscono `begin()` ed `end()`
- gli iteratori sanno fare `++it`
- gli iteratori sanno fare `*it`

----

```text
CONTAINER -> memorizza
ITERATOR  -> naviga
ALGORITHM -> lavora sui dati
```

## Iterator

Formalmente è un tipo.

Classe che rappresenta una posizione dentro un container.

Simile a un puntatore C, ma astratto sul container: vector, list, map, set...

Esempio:

```cpp
std::vector<int>::iterator it;
```

Qui:

```text
std::vector<int>::iterator
```

è il tipo.

```text
it
```

è il nome della variabile.

Dichiarazione di una variabile che naviga dentro un `vector<int>`.

Le funzioni `begin()` ed `end()` restituiscono iteratori.

## end() non è l'ultimo elemento

`end()` non punta all'ultimo elemento.

Punta alla posizione subito dopo l'ultimo elemento.

Se faccio una ricerca e non trovo nulla, spesso gli algoritmi STL restituiscono proprio `end()`.

Esempio:

```cpp
it = std::find(v.begin(), v.end(), 42);
if (it == v.end())
    // non trovato
```

## ++it vs it++

Sugli iteratori: meglio usare

```cpp
++it
```

non

```cpp
it++
```

`it++` crea copia temporanea; `++it` incrementa diretto.

Su int cambia poco; sugli iteratori è buona abitudine.

## Invalidazione degli iteratori

Alcune operazioni invalidano iteratori/puntatori.

Esempio classico: `vector::push_back()`.

Se il vector rialloca, gli iteratori sul vecchio array non valgono più.

Bisogna fare attenzione a tenere iteratori salvati mentre si modifica il container.

---

# Algoritmi STL

Algoritmi STL: lavorano su iteratori, non direttamente sui container.

Esempio:

```cpp
std::find(container.begin(), container.end(), value);
```

Non passo il container intero.

Passo:

- inizio
- fine
- valore da cercare

Così lo stesso algoritmo funziona su container diversi.

---

# Ex00 - Easy find

## Obiettivo

Devo scrivere una funzione generica che trovi un intero dentro un container di interi, senza sapere che tipo di container sia.

Quindi deve funzionare con:

- `vector<int>`
- `list<int>`
- `deque<int>`
- altri container compatibili

La soluzione naturale è usare un template.

## Template della funzione

Nel codice:

```cpp
template <typename T>
typename T::iterator easyfind(T& container, int toFind)
```

Significa:

- `T` è il tipo del container
- `container` è il container reale che ricevo
- `toFind` è l'intero da cercare
- il ritorno è un iteratore del container

Il ritorno è:

```cpp
typename T::iterator
```

Perché non posso scrivere direttamente `std::vector<int>::iterator`.

Se passo una list, il tipo dell'iteratore è diverso.

Quindi dico: dammi l'iterator del tipo T.

## Perché serve typename?

Questa riga può sembrare strana:

```cpp
typename T::iterator
```

Serve perché nei template il compilatore non sa se `T::iterator` è:

- un tipo
- una variabile statica
- qualcos'altro

Con `typename` gli sto dicendo:

```text
fidati, T::iterator è un tipo
```

## std::find

Nel codice:

```cpp
it = std::find(container.begin(), container.end(), toFind);
```

`std::find`:

- parte da `begin()`
- arriva fino a `end()`
- confronta ogni elemento con `toFind`
- se trova, restituisce l'iteratore dell'elemento
- se non trova, restituisce `end()`

Quindi il controllo è:

```cpp
if (it != container.end())
    return it;
throw std::exception();
```

La STL non lancia eccezioni quando non trova.

Sono io, in `easyfind`, a decidere di lanciare eccezione.

## Test nel main

Nel main vengono testati due container:

- `std::vector<int>`
- `std::list<int>`

Nel vector cerco `42`, ma il vector contiene:

```text
1 3 7 9
```

Quindi non lo trova e va nel catch.

Nella list invece contiene:

```text
1 3 42 5
```

Quindi lo trova e stampa:

```text
Value 42 Found
```

Nota: la stessa funzione template lavora su container diversi.

## Concetti chiave ex00

- template function
- `typename T::iterator`
- `std::find`
- `begin()` / `end()`
- `end()` come valore di non trovato
- eccezione lanciata manualmente se non trova

---

# Ex01 - Span

## Obiettivo

Devo creare una classe `Span` che può contenere al massimo N numeri.

Poi devo calcolare:

- shortest span
- longest span

Cioè:

- la distanza minima tra due numeri
- la distanza massima tra due numeri

Esempio:

```text
6 3 17 9 11
```

Ordinati diventano:

```text
3 6 9 11 17
```

Shortest span:

```text
11 - 9 = 2
```

Longest span:

```text
17 - 3 = 14
```

## Struttura della classe

Nel codice:

```cpp
class Span
{
    unsigned int _size;
    std::vector<int> _numbers;
};
```

`_size` è la capacità massima.

`_numbers` è il container in cui salvo i numeri.

Qui ha senso usare `vector` perché:

- devo memorizzare tanti int
- devo ordinarli
- mi serve accesso per indice
- `std::sort` funziona bene con vector

## reserve

Nel costruttore:

```cpp
_numbers.reserve(_size);
```

`reserve` non aggiunge elementi.

Riserva solo capacità.

nota:

```cpp
_numbers.size()
```

rimane 0.

Ma il vector ha già spazio interno per `_size` elementi.

Nota: evita riallocazioni inutili quando poi aggiungo numeri.

Differenza importante:

```cpp
reserve(n)
```

riserva memoria, ma non cambia la size.

```cpp
resize(n)
```

cambia la size e crea davvero n elementi.

## addNumber

```cpp
void Span::addNumber(int n)
```

Aggiunge un numero se c'è ancora spazio.

Nel codice:

```cpp
if (_numbers.size() != _size)
    _numbers.push_back(n);
else
    throw SpanFull();
```

Quindi se il vector ha già raggiunto la capacità logica richiesta dal subject, lancio eccezione.

## addRange

Nel `.hpp` c'è una funzione template:

```cpp
template <typename Iterator>
void addRange(Iterator begin, Iterator end)
{
    while (begin != end)
    {
        addNumber(*begin);
        ++begin;
    }
}
```

Serve per aggiungere tanti numeri in una volta sola.

Invece di chiamare `addNumber()` a mano 10000 volte, passo un range di iteratori.

Esempio dal main:

```cpp
sp.addRange(numbers.begin(), numbers.end());
```

Quindi `Span` non sa da dove arrivano i numeri.

Sa solo:

- inizio range
- fine range
- dereference dell'iteratore

Stile STL: lavoro su range, non sulla provenienza dei dati.

## shortestSpan

Per trovare la distanza minima conviene ordinare i numeri.

Perché: la distanza minima sarà sempre tra due elementi vicini nell'ordine crescente.

Esempio:

```text
3 6 9 11 17
```

Non devo confrontare ogni numero con ogni altro numero.

Mi basta controllare:

```text
6 - 3
9 - 6
11 - 9
17 - 11
```

Quindi evito un doppio ciclo O(n²).

Nel codice:

```cpp
std::sort(_numbers.begin(), _numbers.end());
```

Poi confronto le differenze tra elementi consecutivi:

```cpp
int shortest = _numbers[1] - _numbers[0];
for (size_t i = 1; i + 1 < _numbers.size(); i++)
{
    if (_numbers[i + 1] - _numbers[i] < shortest)
        shortest = _numbers[i + 1] - _numbers[i];
}
```

Attenzione al ciclo: dentro uso `_numbers[i + 1]`, quindi devo fermarmi prima dell'ultimo elemento. La condizione è `i + 1 < size`, non `i < size`. Con `i < size`, all'ultimo giro leggo un elemento fuori dal vector, e senza un controllo tipo AddressSanitizer non te ne accorgi neanche (nella prima versione del codice c'era proprio questo errore).

Concetto importante: `std::sort` modifica il vector originale.

Quindi dopo `shortestSpan()` i numeri restano ordinati.

## longestSpan

La distanza massima è sempre tra il numero più piccolo e il più grande:

```text
max - min
```

Non serve neanche ordinare. Uso due algoritmi STL:

```cpp
int max = *std::max_element(_numbers.begin(), _numbers.end());
int min = *std::min_element(_numbers.begin(), _numbers.end());
return max - min;
```

`max_element` e `min_element` ritornano un iteratore, quindi serve `*` per leggere il valore. Costo O(n), meglio del sort che è O(n log n).

Occhio a non confondere il longest span con "la differenza più grande tra due numeri vicini". Con:

```text
3 6 9 11 17
```

il salto più grande tra vicini è `17 - 11 = 6`, ma il longest span è `17 - 3 = 14`. La prima versione del codice calcolava il salto tra vicini, e col test del subject il risultato era sbagliato.

## Error handling ex01

Ci sono due eccezioni custom:

- `SpanFull`
- `NotEnoughNumbers`

Il controllo è `size() < 2` in entrambe le funzioni: con esattamente 2 numeri lo span esiste.

`SpanFull` viene lanciata se provo ad aggiungere più numeri della capacità massima.

`NotEnoughNumbers` viene lanciata se provo a calcolare uno span con meno di due numeri.

Perché con 0 o 1 numero non esiste distanza tra due elementi.

## Test nel main

Nel main viene creato:

```cpp
Span sp(10000);
```

Poi viene creato un vector di 10000 numeri random:

```cpp
for (int i = 0; i < 10000; i++)
    numbers.push_back(rand());
```

I primi 10 vengono stampati, poi tutto il range viene aggiunto con:

```cpp
sp.addRange(numbers.begin(), numbers.end());
```

Infine stampa:

- shortest span
- longest span

Nota: verifica anche tanti numeri, non solo il test piccolo del subject.

## Concetti chiave ex01

- classe con capacità massima
- vector come storage interno
- `reserve` vs `resize`
- `addNumber`
- `addRange` con iteratori template
- `std::sort`
- shortest span tra vicini ordinati
- longest span tra minimo e massimo, con `min_element` / `max_element`
- eccezioni custom
- attenzione agli indici `i + 1`

---

# Ex02 - Mutated abomination

## Obiettivo

`std::stack` è utile, ma non è iterabile.

Cioè posso fare:

- `push`
- `pop`
- `top`

ma non posso fare:

```cpp
for (...)
```

sui suoi elementi.

L'esercizio chiede di creare un `MutantStack`, cioè uno stack che si comporta come stack ma che espone anche gli iteratori.

## Perché std::stack non è iterabile?

Perché è un container adapter.

Nasconde il container interno e ti lascia usare solo l'interfaccia da pila.

Il container interno però esiste.

Idealmente stack è fatto più o meno così:

```cpp
template <class T, class Container = std::deque<T> >
class stack
{
    protected:
        Container c;

    public:
        void push(const T& value)
        {
            c.push_back(value);
        }

        void pop()
        {
            c.pop_back();
        }

        T& top()
        {
            return c.back();
        }
};
```

La cosa importante è:

```cpp
protected:
    Container c;
```

Il container interno si chiama `c` ed è `protected`.

Quindi non posso accedere dall'esterno, ma posso accedere da una classe derivata.

## Ereditarietà da std::stack

Nel codice:

```cpp
template <typename T>
class MutantStack : public std::stack<T>
```

Quindi `MutantStack` eredita da `std::stack<T>`.

Mantiene le funzioni dello stack:

- `push`
- `pop`
- `top`
- `size`
- `empty`

ma posso aggiungere anche:

- `begin()`
- `end()`

## container_type

`std::stack` definisce un alias interno:

```cpp
container_type
```

Tipo del container usato internamente dallo stack.

Di default è `std::deque<T>`.

Quindi, se faccio:

```cpp
std::stack<int>
```

il suo `container_type` è circa:

```cpp
std::deque<int>
```

## typedef degli iteratori

Nel codice:

```cpp
typedef typename std::stack<T>::container_type::iterator iterator;
typedef typename std::stack<T>::container_type::const_iterator const_iterator;
```

Significa:

```text
creami un alias chiamato iterator che corrisponde all'iterator del container interno dello stack
```

Serve `typename` perché siamo dentro un template e il compilatore deve sapere che quello è un tipo.

## begin() ed end()

Nel codice:

```cpp
iterator begin()
{
    return (this->c.begin());
}

iterator end()
{
    return (this->c.end());
}
```

Qui `this->c` è il container interno ereditato da `std::stack`.

Non posso scrivere semplicemente `c` in modo sicuro nei template, quindi uso `this->c`.

Le versioni const:

```cpp
const_iterator begin() const
const_iterator end() const
```

servono quando il `MutantStack` è const.

## Canonical form

Nel codice ci sono:

- costruttore di default
- copy constructor
- operator=
- destructor

Il copy constructor chiama il costruttore della classe base:

```cpp
MutantStack(const MutantStack &other) : std::stack<T>(other)
```

L'operator= invece usa:

```cpp
std::stack<T>::operator=(other);
```

Quindi la parte stack viene copiata dalla classe base.

## Test nel main

Nel main:

```cpp
MutantStack<int> mstack;
mstack.push(5);
mstack.push(17);
std::cout << mstack.top() << std::endl;
mstack.pop();
std::cout << mstack.size() << std::endl;
```

Test: comportamento da stack normale.

Poi:

```cpp
MutantStack<int>::iterator it = mstack.begin();
MutantStack<int>::iterator ite = mstack.end();
```

Test: iterabilità.

Il ciclo:

```cpp
while (it != ite)
{
    std::cout << *it << std::endl;
    ++it;
}
```

stampa gli elementi attraversando il container interno.

Infine:

```cpp
std::stack<int> s(mstack);
```

Nota: `MutantStack` usabile anche come `std::stack`, perché eredita da stack.

---

# Checklist finale modulo

## Concetti da sapere spiegare

- cos'è un container e differenza tra sequence, associative e adapter
- differenze di memoria tra `vector`, `list` e `deque`
- cos'è un iteratore e perché serve
- perché `end()` punta dopo l'ultimo elemento
- perché gli algoritmi STL lavorano su iteratori e non su container
- a cosa serve `typename` in `typename T::iterator`
- differenza tra `reserve` e `resize`
- perché per lo shortest span conviene ordinare
- perché il longest span è max - min
- perché `std::stack` non è iterabile
- cos'è `container_type` e perché `this->c` è accessibile da `MutantStack`

## Errori tipici da evitare

- usare `end()` come se fosse l'ultimo elemento
- dimenticare `typename` davanti a `T::iterator`
- usare `container[i]` su container che non lo supportano, tipo `list`
- tenere iteratori salvati mentre modifico un `vector`
- confondere `reserve` con `resize`
- leggere `_numbers[i + 1]` con la condizione `i < size`
- calcolare il longest span come salto più grande tra vicini
- usare `c` invece di `this->c` dentro `MutantStack`

## Mini riassunto mentale

```text
ex00 -> template sul tipo di container + std::find
        non trovato = end() -> lancio eccezione io
ex01 -> Span = vector con capacità massima
        shortest: sort + vicini, longest: max - min
ex02 -> MutantStack : std::stack
        espongo gli iteratori del container interno c
```
