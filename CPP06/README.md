# CPP06 - Cast in C++

Modulo sui cast.

La domanda di fondo è:

```text
quando cambio tipo a un valore, cosa sto facendo davvero?
```

In C c'è un solo cast, `(int)x`, che fa un po' di tutto. In C++ ce ne sono diversi, ognuno con un compito preciso. L'idea del modulo è scegliere quello giusto, non solo far compilare.

Cose nuove:

- `static_cast`
- `reinterpret_cast`
- `dynamic_cast`
- classi non istanziabili con soli metodi statici
- `uintptr_t`

---

# I cast del C++

## static_cast

Conversioni "normali", che hanno senso e che il compilatore può controllare a compile time.

```cpp
int i = static_cast<int>(42.5);       // 42
double d = static_cast<double>('a');  // 97.0
```

Uso tipico: tra tipi numerici. Lo uso nell'ex00.

## reinterpret_cast

Cast di basso livello. Non converte il valore: prende gli stessi bit e li legge come se fossero un altro tipo.

```text
Data* -> uintptr_t -> Data*
```

Il compilatore lascia fare quasi tutto, quindi è facile usarlo male. Lo uso nell'ex01.

## dynamic_cast

Cast su gerarchie di classi polimorfiche. Controlla a **runtime** se l'oggetto è davvero del tipo che dico.

```text
ho un Base*: punta davvero ad A, B o C?
```

Funziona solo se la base ha almeno una funzione virtuale. Lo uso nell'ex02.

## const_cast

C'è anche questo, che serve a togliere o aggiungere `const`. Nel modulo non serve, ma è bene sapere che esiste.

## Perché non il cast del C?

`(int)x` prova in ordine `static_cast`, `const_cast`, `reinterpret_cast`, finché uno funziona. Quindi non si capisce mai cosa sta facendo davvero, e il compilatore non ti avvisa se stai facendo una cosa pericolosa.

Con i cast C++ l'intenzione è scritta nel codice.

---

# Ex00 - Conversion of scalar types

## Obiettivo

Classe `ScalarConverter` con un solo metodo statico:

```cpp
static void convert(std::string const literal);
```

Riceve una stringa e la stampa convertita nei quattro tipi scalari:

```text
char
int
float
double
```

Uso:

```bash
./convert "42"
./convert "a"
./convert "42.0f"
./convert "nan"
```

Esempio di output:

```text
./convert 42
char: '*'
int: 42
float: 42.0f
double: 42.0
```

## Classe non istanziabile

Nel `.hpp` costruttori, assegnazione e distruttore sono privati:

```cpp
private:
    ScalarConverter();
    ScalarConverter(ScalarConverter &other);
    ScalarConverter &operator=(ScalarConverter &other);
    ~ScalarConverter();
```

Così nessuno può creare un oggetto `ScalarConverter`. Il subject lo chiede: la classe è solo un contenitore per la funzione.

Si usa così:

```cpp
ScalarConverter::convert(argv[1]);
```

Non così:

```cpp
ScalarConverter c;  // non compila
```

Funziona perché `convert` è `static`: appartiene alla classe, non a un oggetto, quindi non serve un oggetto per chiamarla.

## L'idea generale

Riconosco il tipo dell'input, lo converto in un `double` (il tipo più "largo" dei quattro), e da lì stampo tutti gli altri con `static_cast`.

```text
stringa -> double -> char / int / float / double
```

## Riconoscimento del char

```cpp
static int is_char(std::string literal)
{
    if (literal.length() == 1 && !std::isdigit(literal[0]))
        return (1);
    return (0);
}
```

Un solo carattere che non è una cifra:

```text
"a" -> char
"*" -> char
"7" -> numero
```

Se è un char, il valore è il suo codice ASCII:

```cpp
conv = static_cast<double>(literal[0]);
```

## Parsing numerico con strtod

Tutto il resto lo passo a `strtod`:

```cpp
conv = std::strtod(literal.c_str(), &pos);
```

`strtod` prova a leggere un `double`, e `pos` mi dice dove si è fermato. È lo stesso trucco dell'end pointer di CPP09.

Comodo: `strtod` capisce già da solo `nan`, `inf`, `-inf` e la notazione scientifica tipo `1e10`.

Controlli:

```cpp
if (pos == literal)
```

Non ha letto niente: input non valido.

```cpp
if (*pos != '\0')
{
    if (!(*pos == 'f' && *(pos + 1) == '\0'))
        // input non valido
}
```

Se avanza qualcosa, l'unica cosa ammessa è una `f` finale (il suffisso dei float), e dopo la `f` deve finire la stringa.

```text
"42.0f"  -> ok
"42abc"  -> invalido
"42fabc" -> invalido
```

## Pseudo-literals

Casi speciali:

```text
nan   nanf
-inf  -inff
+inf  +inff
```

Esistono per `float` e `double`, ma non per `char` e `int`: un intero non può valere infinito.

Li riconosco con `is_impossible()` e in quel caso stampo `impossible` per char e int:

```text
char: impossible
int: impossible
float: nanf
double: nan
```

## Stampa del char

Tre casi:

```cpp
if (conv < 0 || conv > 127)
    // impossible
else if (!std::isprint(static_cast<char>(conv)))
    // Not Displayable
else
    // stampo il carattere tra apici
```

Esempi:

```text
42  -> '*'
10  -> Not Displayable (è il newline)
300 -> impossible
```

## Stampa dell'int

Prima di fare il cast controllo il range:

```cpp
if (conv < INT_MIN || conv > INT_MAX)
    std::cout << "int: impossible" << std::endl;
```

Fare `static_cast<int>` su un double fuori range è comportamento indefinito, quindi il controllo va fatto prima.

## Stampa di float e double

`std::cout` stampa `42.0` come `42`. Il subject vuole vedere `42.0f` e `42.0`, quindi se il numero è intero aggiungo `.0` a mano.

Il controllo:

```cpp
static bool isRound(double conv)
{
    return (conv == std::floor(conv) && std::fabs(conv) < 1000000);
}
```

- `conv == std::floor(conv)`: il numero non ha parte decimale
- `< 1000000`: sopra questa soglia `cout` passa alla notazione scientifica (`1e+07`), e aggiungere `.0` darebbe `1e+07.0`, che non ha senso

## Error handling ex00

- numero di argomenti diverso da 1
- stringa vuota
- input non numerico, tipo `abc`
- numero con roba dopo, tipo `42abc`
- `f` non finale, tipo `42fabc`
- char non stampabile
- valori fuori range per char e int
- pseudo-literals

---

# Ex01 - Serialization

## Obiettivo

Classe `Serializer`, anche lei non istanziabile, con due metodi statici:

```cpp
static uintptr_t serialize(Data* ptr);
static Data* deserialize(uintptr_t raw);
```

Nonostante il nome, non è serializzazione su file: è la conversione di un puntatore in un numero e ritorno.

```text
Data* -> uintptr_t -> stesso Data*
```

`Data` è una struct qualsiasi, purché non vuota:

```cpp
struct Data
{
    int n;
};
```

## uintptr_t

È un tipo intero abbastanza grande da contenere un indirizzo di memoria. Sta in `<stdint.h>`.

Non posso usare un `int`: su un sistema a 64 bit un puntatore è 8 byte e un `int` 4, quindi l'indirizzo non ci starebbe.

## reinterpret_cast

```cpp
uintptr_t Serializer::serialize(Data* ptr)
{
    return (reinterpret_cast<uintptr_t>(ptr));
}

Data* Serializer::deserialize(uintptr_t raw)
{
    return (reinterpret_cast<Data *>(raw));
}
```

Qui `static_cast` non compilerebbe: puntatore e intero non hanno una conversione "sensata" tra loro. `reinterpret_cast` invece dice al compilatore: prendi questi bit e trattali come un altro tipo, so cosa sto facendo.

## Test nel main

```cpp
Data* data = new Data;
data->n = 42;

uintptr_t serialized = Serializer::serialize(data);
Data* deserialized = Serializer::deserialize(serialized);
```

Da verificare:

- `deserialized == data`, cioè è lo stesso indirizzo
- `deserialized->n == 42`

## Cosa NON succede

- non salvo niente su disco
- non copio l'oggetto
- non trasformo `Data` in stringa

Se faccio `delete data`, anche `deserialized` punta a memoria liberata: è lo stesso oggetto.

---

# Ex02 - Identify real type

## Obiettivo

Una classe base con tre derivate vuote:

```cpp
class Base
{
public:
    virtual ~Base();
};

class A : public Base {};
class B : public Base {};
class C : public Base {};
```

Tre funzioni:

1. `generate()`: crea a caso un `A`, un `B` o un `C` e lo ritorna come `Base*`
2. `identify(Base* p)`: stampa il tipo reale usando il puntatore
3. `identify(Base& p)`: stampa il tipo reale usando la reference

Il subject vieta `<typeinfo>` e `typeid`: si deve fare con `dynamic_cast`.

## Distruttore virtuale

```cpp
virtual ~Base();
```

Fa due cose:

- rende la classe polimorfica, e senza questo `dynamic_cast` non funziona
- permette di fare `delete p` tramite un `Base*` senza leak (CPP04)

## generate()

```cpp
switch (rand() % 3)
```

Crea `new A()`, `new B()` o `new C()` e ritorna sempre `Base*`.

Fuori da `generate()` si sa solo "ho un `Base*`", non quale delle tre classi c'è dietro. Per controllo, stampa anche la classe che ha creato.

Nel main c'è `srand(time(NULL))`, sennò `rand()` darebbe sempre lo stesso risultato.

## identify(Base *p)

Con i puntatori, se `dynamic_cast` fallisce ritorna `NULL`:

```cpp
if (dynamic_cast<A *>(p))
    std::cout << "A\n";
else if (dynamic_cast<B *>(p))
    std::cout << "B\n";
else if (dynamic_cast<C *>(p))
    std::cout << "C\n";
```

Provo i tre tipi uno dopo l'altro, il primo che non dà `NULL` è quello giusto.

## identify(Base &p)

Con le reference non esiste una "reference nulla". Quindi se `dynamic_cast` fallisce, lancia un'eccezione (`std::bad_cast`).

Il subject vieta anche di usare un puntatore dentro questa funzione, quindi non posso fare `identify(&p)`. Serve un `try` per ogni tipo:

```cpp
try
{
    A &a = dynamic_cast<A &>(p);
    (void)a;
    std::cout << "A\n";
    return;
}
catch (const std::exception &e)
{
    std::cout << e.what() << std::endl;
}
```

`(void)a` serve solo a zittire il warning di variabile non usata.

Nel `catch` stampo il messaggio dell'eccezione, quindi prima del tipo giusto si vedono i cast falliti. Utile per vedere che il meccanismo funziona, anche se l'output è un po' rumoroso.

Catturo `std::exception` e non `std::bad_cast`, perché `std::bad_cast` è dichiarata in `<typeinfo>`, che è vietata.

Schema:

```text
puntatore -> fallimento = NULL
reference -> fallimento = eccezione
```

---

# Checklist finale modulo

## Concetti da sapere spiegare

- differenza tra `static_cast`, `reinterpret_cast`, `dynamic_cast`, `const_cast`
- perché il cast stile C è meno sicuro
- come si rende una classe non istanziabile
- cosa vuol dire metodo `static`
- come funziona `strtod` con l'end pointer
- cosa sono i pseudo-literals e perché sono impossibili per int e char
- perché serve controllare il range prima di `static_cast<int>`
- cos'è `uintptr_t` e perché non basta un `int`
- perché `dynamic_cast` richiede una classe polimorfica
- differenza tra `dynamic_cast` su puntatore e su reference

## Errori tipici da evitare

- fare `static_cast<int>` di un double fuori range
- accettare qualsiasi cosa dopo la `f` finale
- stampare `.0` dopo un numero in notazione scientifica
- usare `int` invece di `uintptr_t` per un indirizzo
- dimenticare la funzione virtuale nella base: `dynamic_cast` non compila
- usare `typeid` o includere `<typeinfo>` nell'ex02
- usare un puntatore dentro `identify(Base &p)`
- dimenticare `srand`: `generate()` crea sempre la stessa classe

## Mini riassunto mentale

```text
ex00 -> static_cast
        stringa -> double -> char / int / float / double
ex01 -> reinterpret_cast
        puntatore -> uintptr_t -> stesso puntatore
ex02 -> dynamic_cast
        puntatore: NULL se sbaglio, reference: eccezione
```
