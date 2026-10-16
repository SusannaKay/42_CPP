# CPP06 - Cast in C++

Modulo sui cast.
Domanda di fondo:

```text
quando cambio tipo a un valore, cosa sto facendo davvero?
```

Cast principali:
- `static_cast`
- `reinterpret_cast`
- `dynamic_cast`

Idea: scegliere il cast giusto, non solo far compilare.

---

# Cast: panoramica veloce

## static_cast

Conversioni normali, controllabili a compile time.

```cpp
int i = static_cast<int>(42.5);
double d = static_cast<double>('a');
```

Uso tipico:
- tipi numerici
- conversioni esplicite ma sensate
- ex00

## reinterpret_cast

Cast basso livello.
Non converte matematicamente il valore: reinterpreta la rappresentazione.

```text
Data* -> uintptr_t -> Data*
```

Nota:
- potente
- facile da usare male
- il compilatore lascia fare più cose
- ex01

## dynamic_cast

Cast su gerarchie polimorfiche.
Serve per controllare a runtime il tipo reale.

```text
Base* p punta davvero ad A, B o C?
```

Richiede almeno una funzione virtuale nella base.
Compare in ex02.

---

# Ex00 - ScalarConverter

## Idea

Classe `ScalarConverter` con funzione statica:

```cpp
static void convert(std::string literal);
```

Input: stringa.
Output: conversione in quattro tipi scalari:
- `char`
- `int`
- `float`
- `double`

Uso:

```bash
./convert "42"
./convert "a"
./convert "42.0f"
./convert "nan"
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

Nota: utility class.
Si usa così:

```cpp
ScalarConverter::convert(argv[1]);
```

Non così:

```cpp
ScalarConverter c;
```

## Riconoscimento del char

Helper:

```cpp
static int is_char(std::string literal)
```

Regola:
- lunghezza 1
- non cifra

```text
"a" -> char
"*" -> char
"7" -> numero
```

Se è char:

```cpp
conv = static_cast<double>(literal[0]);
```

Poi stampa gli altri tipi partendo dal valore ASCII.

## Parsing numerico con strtod

Uso:

```cpp
conv = std::strtod(literal.c_str(), &pos);
```

`strtod` prova a leggere un `double`.
`pos` indica dove si è fermata la conversione.

Esempio:

```text
"42abc"
```

Legge `42`, si ferma su `a`.
Quindi va controllato cosa resta.

```cpp
if (pos == literal)
```

Nota: significa che non è stato letto nessun numero.

Suffisso `f` da gestire per:

```text
42.0f
nanf
+inff
```

## Pseudo-literals

Casi speciali:

```text
nan
nanf
-inf
+inf
-inff
+inff
```

Validi per `float` e `double`.
Non validi per `char` e `int`.

Output concettuale:

```text
char: impossible
int: impossible
float: nanf
double: nan
```

## Stampa del char

Casi:
1. impossibile
2. fuori range ASCII
3. non stampabile

Controlli:

```cpp
conv < 0 || conv > 127
std::isprint(static_cast<char>(conv))
```

Esempio:

```text
10 -> newline -> non displayable
```

## Stampa di float e double

Se il valore è intero, aggiunta di `.0`:

```text
42 -> 42.0f
42 -> 42.0
```

Nota: output vicino a quello richiesto dal subject.

## Punti ex00

- utility class
- funzione statica
- parsing da stringa
- `strtod` + puntatore di fine conversione
- `static_cast` per tipi scalari
- `nan`, `inf`, `nanf`, `inff`
- char non stampabili
- range `int` (`INT_MIN`, `INT_MAX`)

---

# Ex01 - Serializer

## Idea

Non è serializzazione su file.
È conversione puntatore -> intero -> puntatore.

```cpp
static uintptr_t serialize(Data* ptr);
static Data* deserialize(uintptr_t raw);
```

`Data`:

```cpp
struct Data
{
    int n;
};
```

## uintptr_t

Tipo intero abbastanza grande per contenere un indirizzo.

```text
Data* -> indirizzo in memoria -> uintptr_t
```

Nota:
- non copia l'oggetto
- non salva dati
- rappresenta l'indirizzo come numero

## reinterpret_cast

```cpp
return (reinterpret_cast<uintptr_t>(ptr));
```

Poi:

```cpp
return (reinterpret_cast<Data *>(raw));
```

Situazione tipica da `reinterpret_cast`: basso livello, rappresentazione compatibile perché so cosa sto facendo.

## Test nel main

```cpp
Data* data = new Data;
data->n = 42;
```

Poi:

```cpp
uintptr_t serialized = Serializer::serialize(data);
Data* deserialized = Serializer::deserialize(serialized);
```

Da verificare:

```text
deserialized->n == 42
```

E soprattutto: `deserialized` punta allo stesso oggetto originale.

## Cosa NON succede

- non salvo su disco
- non creo deep copy
- non trasformo `Data` in stringa

Solo:

```text
puntatore -> numero -> stesso puntatore
```

## Punti ex01

- `uintptr_t`
- `reinterpret_cast`
- oggetto vs indirizzo dell'oggetto
- conversione puntatore/intero/puntatore
- lifetime dell'oggetto puntato

---

# Ex02 - Identify real type

## Idea

Base polimorfica:

```cpp
class Base
{
public:
    virtual ~Base();
};
```

Derivate:

```cpp
class A : public Base {};
class B : public Base {};
class C : public Base {};
```

Scopo:
1. generare `A`, `B` o `C`
2. restituire come `Base*`
3. riconoscere a runtime il tipo reale

## Distruttore virtuale

```cpp
virtual ~Base();
```

Nota:
- rende la classe polimorfica
- abilita l'uso corretto di `dynamic_cast`
- permette `delete p;` tramite `Base*`

## generate()

```cpp
Base *generate(void);
```

Usa:

```cpp
rand() % 3
```

Crea:

```cpp
new A()
new B()
new C()
```

Ritorna sempre `Base*`.
Fuori si sa solo:

```text
ho un Base*
```

Tipo reale: `A`, `B` o `C`.

## identify(Base *p)

Cast su puntatore:

```cpp
dynamic_cast<A *>(p)
```

Risultati:
- successo: puntatore valido
- fallimento: `NULL`

```cpp
if (dynamic_cast<A *>(p))
    std::cout << "A\n";
```

## identify(Base &p)

Cast su riferimento.
Con riferimenti non esiste `NULL`.

Se fallisce:
- `dynamic_cast` lancia `std::bad_cast`
- servono blocchi `try/catch`

```cpp
try
{
    A &a = dynamic_cast<A &>(p);
    std::cout << "A\n";
}
catch (const std::exception &e)
{
    ...
}
```

Schema:

```text
puntatore   -> fallimento = NULL
riferimento -> fallimento = eccezione
```

## Test nel main

```cpp
p = generate();
identify(*p);
identify(p);
delete p;
```

Testa entrambe le versioni:
- riferimento
- puntatore

Nota: l'output stampa anche la classe generata, così si controlla se `identify` indovina.
