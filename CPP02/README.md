# CPP02 - Orthodox Canonical Form, fixed-point e overload

Il modulo `CPP02` introduce tre concetti:

1. **Orthodox Canonical Form**
2. **Numeri fixed-point**
3. **Overload degli operatori**

L’idea centrale è costruire una classe `Fixed` che rappresenti un numero a virgola fissa, cioè un numero decimale gestito internamente tramite un intero. Il subject linka ad alcuni articoli estremamente matematici. Se hai passato 2 ore e ancora non li hai capiti, siamo in due. La buona notizia però è che non servono per completare il modulo, la brutta è che quelle due ore le hai perse. 

---

# Orthodox Canonical Form

Da adesso in poi, la classe dovremo scriverla in Orthodox Canonical Form, pena: l'evaluation si ferma subito.
Ogni OCF deve avere sempre:

```cpp
ClassName();
ClassName(const ClassName &other); 
ClassName &operator=(const ClassName &other);
~ClassName();
```

Quindi:

| Funzione | Scopo |
| --- | --- |
| Costruttore di default | Crea un oggetto vuoto/default |
| Costruttore di copia | Crea un oggetto copiandone un altro |
| Operatore di assegnazione | Copia i valori da un oggetto già esistente |
| Distruttore | Libera eventuali risorse |

Esempio concettuale:

```cpp
Fixed a;
Fixed b(a);
Fixed c;

c = b;
```

Qui succedono cose diverse:

```cpp
Fixed b(a);
```

chiama il costruttore di copia, quindi `ClassName(const ClassName &other)`.

```cpp
c = b;
```

chiama l’operatore di assegnazione, quindi `ClassName &operator=(const ClassName &other)`.

Regola veloce: se l'oggetto a sinistra sta nascendo adesso, è il costruttore di copia. Se esisteva già, è l'operatore di assegnazione.

Occhio che anche questa è una copia, non un'assegnazione:

```cpp
Fixed d = a;
```

`d` sta nascendo, quindi viene chiamato il costruttore di copia, anche se c'è un `=`.

---

# Fixed-point numbers

Un numero fixed-point è un numero che conserva una parte frazionaria usando un intero. Che vuol dire? In sostanza se abbiamo un numero con la virgola, possiamo decidere di salvarlo come un numero intero. 

Immaginiamo di avere 1,50€ ed abbiamo un computer che salva solo numeri interi. La cosa più sensata ( mica tanto ma almeno è intuitiva ) è salvare quel numero come se fossero tutti centesimi. Quindi 150. Noi sapremo allora che, per convertirlo a numero con la virgola, dobbiamo semplicemente dividere per 100. 

La stessa cosa avviene in questi esercizi, solo che dobbiamo ragionare in binario ( base 2) e non in base 10.

Perchè i float non vanno bene? 
Perchè si perde precisione su numeri molto grandi o molto piccoli. Nei fixed point la precisione è sempre la stessa, dipende esclusivamente da quanti bit (frazionari) decidi di riservare alle cifre dopo la virgola.


Nel progetto, la classe ha:

```cpp
private:
    int                 _rawBits;
    static const int    _fractionalBits = 8;
```

`_rawBits` contiene il valore grezzo.

`_fractionalBits` è `static const`: è uguale per tutti gli oggetti `Fixed`, quindi esiste una volta sola per la classe e non una per oggetto.

Quindi `_fractionalBits` indica quanti bit sono dedicati alla parte decimale.

Con 8 fractional bits, il fattore di scala è:

```cpp
1 << 8
```

cioè:

```text
256
```

Quindi il numero reale viene salvato così:

```text
valore_reale * 256
```

E recuperato così:

```text
valore_grezzo / 256
```

Esempio:

```text
5.5
```

internamente diventa:

```text
5.5 * 256 = 1408
```

Quando lo riconverti:

```text
1408 / 256 = 5.5
```

---

# Ex00 - My First Class in Orthodox Canonical Form

## Obiettivo

Creare una classe `Fixed` in Orthodox Canonical Form.

In questo esercizio la classe non deve ancora fare le conversioni numeriche.

Serve soprattutto per capire:

- costruttore
- costruttore di copia
- operatore `=`
- distruttore
- getter/setter del raw value

## Funzioni principali

```cpp
int getRawBits(void) const;
void setRawBits(int const raw);
```

### `getRawBits`

Restituisce il valore interno `_rawBits`.

```cpp
return (_rawBits);
```

È `const` perché legge e basta: non modifica l'oggetto.

### `setRawBits`

Modifica il valore interno `_rawBits`.

```cpp
_rawBits = raw;
```

---

# Ex01 - Towards a more useful fixed-point number class

## Obiettivo

Rendere la classe `Fixed` davvero utilizzabile.

Si aggiungono:

```cpp
Fixed(const int n);
Fixed(const float n);
float toFloat(void) const;
int toInt(void) const;
```

## Costruttore da int

Il costruttore si prende un int e lo shifta di n fractional bit per salvarsi il rawBit.

```cpp
_rawBits = n << _fractionalBits;
```

Esempio:

```cpp
Fixed a(10);
```

Internamente:

```text
10 << 8 = 2560
```

È come fare:

```text
10 * 256
```

## Costruttore da float

Un float deve essere moltiplicato per 256 e arrotondato.

Non posso fare lo shift come per l'int: `<<` sui float non esiste.

```cpp
_rawBits = roundf(n * (1 << _fractionalBits));
```

Esempio:

```cpp
Fixed a(42.42f);
```

Internamente viene salvato un intero che rappresenta 42.42.

Serve `roundf` perché il risultato può avere decimali.

Infatti `42.42 * 256 = 10859.52`, che arrotondato diventa `10860`. Quel `.52` perso è la precisione che il fixed-point con 8 bit non riesce a tenere.

## `toFloat`

Converte il valore fixed-point in float.

```cpp
return ((float)_rawBits / (1 << _fractionalBits));
```

Il cast a float va fatto prima della divisione, sennò è una divisione tra interi e perdo la parte decimale.

Esempio:

```cpp
_rawBits = 10860;
```

Risultato:

```text
10860 / 256 = 42.421875
```

## `toInt`

Converte il valore fixed-point in int.

```cpp
return (_rawBits >> _fractionalBits);
```

È come dividere per 256, ma usando i bit.

## Overload di `<<`

Per stampare direttamente un oggetto `Fixed`:

```cpp
std::cout << fixed;
```

si implementa:

```cpp
std::ostream &operator<<(std::ostream &out, const Fixed &fixed);
```

Non può essere un metodo della classe, perché a sinistra di `<<` c'è `std::cout`, non un `Fixed`. Quindi è una funzione esterna che riceve lo stream e l'oggetto.

Dentro si stampa:

```cpp
out << fixed.toFloat();
return (out);
```

Ritorna lo stream per permettere di concatenare: `std::cout << a << b << std::endl;`.

---

# Ex02 - Now we're talking

## Obiettivo

Rendere `Fixed` simile a un tipo numerico vero. Quindi dobbiamo "riscrivere" i comportamenti di alcuni operatori. 

Perchè? 
perchè di base gli operatori sanno operare su classi precise standard ( tipo int, float, double ecc ), dobbiamo dire al programma che quando legge quegli operatori, si deve andare sostanzialmente a prendere il comportamento delle funzioni che scriviamo noi.

Si aggiungono operatori:

- confronto
- aritmetici
- incremento/decremento
- min/max

## Operatori di confronto

```cpp
>
<
>=
<=
==
!=
```

Questi confrontano il valore interno `_rawBits`.

Esempio:

```cpp
bool Fixed::operator>(const Fixed &other) const
{
    return (this->getRawBits() > other.getRawBits());
}
```

Quindi gli stiamo dicendo che quando chiediamo se `a > b`, lui deve confrontare il `_rawBits` dei due oggetti.
`a` è `this`, `b` è `other`.

Non serve convertire in float: se entrambi usano la stessa scala, confrontare il raw value è corretto.

## Operatori aritmetici

```cpp
+
-
*
/
```

Si potrebbe fare la versione facile: converto in float, faccio l'operazione, ricreo un `Fixed`. Funziona, ma passa per i float, che è proprio quello che il fixed-point vuole evitare.

Io ho lavorato direttamente sui raw bits.

### Somma e sottrazione

Se entrambi sono scalati per 256, posso sommare direttamente i raw:

```text
a * 256 + b * 256 = (a + b) * 256
```

Quindi:

```cpp
Fixed temp;
temp.setRawBits(this->getRawBits() + other.getRawBits());
return (temp);
```

Stessa cosa per la sottrazione.

### Moltiplicazione

Qui la scala si moltiplica due volte:

```text
(a * 256) * (b * 256) = (a * b) * 256 * 256
```

Quindi dopo la moltiplicazione devo togliere un 256, cioè shiftare a destra di 8:

```cpp
long long n = static_cast<long long>(this->getRawBits()) * other.getRawBits();
Fixed temp;
temp.setRawBits(n >> temp._fractionalBits);
return (temp);
```

Il cast a `long long` va fatto **prima** di moltiplicare. Se scrivo `long long n = a * b;` la moltiplicazione avviene comunque tra due `int`, va in overflow, e solo dopo il risultato (già sbagliato) viene messo nel `long long`.

### Divisione

Al contrario: dividendo, la scala si cancella.

```text
(a * 256) / (b * 256) = a / b      <- niente più 256
```

Quindi devo rimettere il 256, e devo farlo **prima** di dividere, sennò la divisione tra interi butta via la parte decimale:

```cpp
long long n = (static_cast<long long>(this->getRawBits()) << temp._fractionalBits) / other.getRawBits();
temp.setRawBits(n);
```

Esempio con `5.05 / 2`:

```text
raw 5.05 = 1293, raw 2 = 512

sbagliato: (1293 / 512) << 8  = 2 << 8 = 512      -> 2.0
giusto:    (1293 << 8) / 512  = 331008 / 512 = 646 -> 2.52344
```

Nella prima versione del codice c'era proprio quella sbagliata. Il main del subject usa solo la moltiplicazione, quindi con i test del subject non salta fuori: la divisione va testata a parte.

La divisione per zero non è gestita: il subject dice che è accettabile che il programma crashi.

## Incremento e decremento

`++` aumenta il raw di 1, non il numero di 1.

Quindi `a++` aumenta il valore di `1 / 256 = 0.00390625`, cioè il passo più piccolo rappresentabile (il subject lo chiama epsilon).

### Pre-incremento

```cpp
Fixed &Fixed::operator++()
{
    _rawBits++;
    return (*this);
}
```

Usato così:

```cpp
++a;
```

Modifica prima, poi restituisce l’oggetto modificato.

### Post-incremento

```cpp
Fixed Fixed::operator++(int)
{
    Fixed tmp(*this);
    _rawBits++;
    return (tmp);
}
```

Usato così:

```cpp
a++;
```

Prima salva una copia, poi incrementa, poi restituisce il vecchio valore.

Il parametro `int` non serve a niente: è solo il modo in cui il C++ distingue `a++` da `++a`.

Il pre-incremento ritorna una reference (`Fixed &`), il post-incremento una copia (`Fixed`), perché deve restituire il valore di prima e l'oggetto ormai è già cambiato.

La differenza è questa:

```cpp
std::cout << ++a;
```

stampa il valore già incrementato.

```cpp
std::cout << a++;
```

stampa il valore precedente.

---

## min e max

Servono quattro funzioni:

```cpp
static Fixed &min(Fixed &a, Fixed &b);
static const Fixed &min(const Fixed &a, const Fixed &b);

static Fixed &max(Fixed &a, Fixed &b);
static const Fixed &max(const Fixed &a, const Fixed &b);
```

Perché due versioni?

Una serve per oggetti modificabili:

```cpp
Fixed a;
Fixed b;
Fixed::min(a, b);
```

L’altra serve per oggetti const:

```cpp
const Fixed a;
const Fixed b;
Fixed::min(a, b);
```

Esempio:

```cpp
Fixed &Fixed::min(Fixed &a, Fixed &b)
{
    if (a < b)
        return (a);
    return (b);
}
```

Sono `static`: appartengono alla classe e non a un oggetto, quindi si chiamano con `Fixed::min(a, b)` e non con `a.min(b)`.

---

# Ex03 - BSP

## Obiettivo

Scrivere una funzione che dice se un punto sta dentro un triangolo:

```cpp
bool bsp(Point const a, Point const b, Point const c, Point const point);
```

`a`, `b`, `c` sono i vertici, `point` è il punto da controllare.

Se il punto è su un lato o su un vertice, il subject vuole `false`.

## Classe Point

```cpp
class Point {
    private:
    Fixed const _x;
    Fixed const _y;
    ...
};
```

Le coordinate sono `Fixed`, quindi riuso la classe dell'ex02.

E sono `const`: una volta creato, un punto non si sposta.

Problema: con membri `const`, l'operatore di assegnazione non può copiare niente, perché non posso riassegnare `_x` e `_y`.

Il subject vuole comunque la forma canonica, quindi `operator=` c'è, ma non fa niente:

```cpp
Point &Point::operator=(Point const &other){
    (void)other;
    return *this;
}
```

Il costruttore di copia invece funziona, perché i `const` si possono inizializzare nella initialization list:

```cpp
Point::Point(Point const &other): _x(other._x), _y(other._y){}
```

## L'idea geometrica

Per ogni lato del triangolo mi chiedo: il punto sta a sinistra o a destra di questo lato?

Se sta dalla stessa parte di tutti e tre i lati, è dentro.

Per sapere da che parte sta, uso il prodotto vettoriale 2D:

```text
(b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x)
```

Il segno del risultato dice tutto:

- positivo: il punto è a sinistra del lato `a -> b`
- negativo: è a destra
- zero: è esattamente sulla retta del lato

Nel codice:

```cpp
static Fixed check_point(Point const a, Point const b, Point const p)
{
    return ((b.getx() - a.getx()) * (p.gety() - a.gety())
          - (b.gety() - a.gety()) * (p.getx() - a.getx()));
}
```

Lo calcolo per i tre lati, `a -> b`, `b -> c`, `c -> a`:

```cpp
Fixed d1(check_point(a, b, point));
Fixed d2(check_point(b, c, point));
Fixed d3(check_point(c, a, point));
```

Poi:

```cpp
if (d1 < 0 && d2 < 0 && d3 < 0)
    return (true);
if (d1 > 0 && d2 > 0 && d3 > 0)
    return (true);
return (false);
```

Controllo entrambi i casi (tutti positivi o tutti negativi) perché dipende da come sono stati passati i vertici: in senso orario o antiorario.

Il bordo esce gratis: se il punto è su un lato, uno dei tre valori è `0`, e `0` non è né `< 0` né `> 0`, quindi ritorna `false`.

Qui si vede perché serviva l'ex02: tutto il calcolo usa gli operatori `-`, `*`, `<`, `>` di `Fixed`.

## Test nel main

Triangolo `(0,0)`, `(10,0)`, `(0,10)`:

```text
(1,1)   -> dentro -> 1
(10,10) -> fuori  -> 0
(0,5)   -> bordo  -> 0
```

---

# Checklist finale modulo

## Concetti da sapere spiegare

- le quattro funzioni della Orthodox Canonical Form
- differenza tra costruttore di copia e operatore di assegnazione
- perché `operator=` controlla `this != &other` e ritorna `*this`
- cos'è un numero fixed-point e perché non usare i float
- perché `_fractionalBits` è `static const`
- conversione int -> fixed (shift) e float -> fixed (moltiplicazione + `roundf`)
- perché `operator<<` è una funzione esterna e ritorna lo stream
- come funzionano `+`, `-`, `*`, `/` sui raw bits
- differenza tra pre e post incremento
- perché `min` e `max` hanno una versione const e una no, e perché sono `static`
- come funziona il test del prodotto vettoriale in `bsp`

## Errori tipici da evitare

- dimenticare la forma canonica: in correzione è un fail immediato
- fare `toFloat` con una divisione tra interi
- fare lo shift su un float
- moltiplicare in `int` e mettere il risultato in `long long` solo dopo
- dividere i raw bits e rimettere la scala dopo la divisione
- in `bsp` considerare dentro i punti sul bordo
- in `Point` provare a riassegnare membri `const` in `operator=`

## Mini riassunto mentale

```text
ex00 -> forma canonica + raw bits
ex01 -> int << 8, float * 256 + roundf, ritorno con >> 8 o / 256
ex02 -> operatori sui raw bits
        + e - diretti, * poi >> 8, << 8 poi /
ex03 -> Point con Fixed const + segno del prodotto vettoriale
```
