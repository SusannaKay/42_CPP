# CPP Module 02

## Obiettivo del modulo

Il modulo `CPP02` introduce tre concetti:

1. **Orthodox Canonical Form**
2. **Numeri fixed-point**
3. **Overload degli operatori**

L’idea centrale è costruire una classe `Fixed` che rappresenti un numero a virgola fissa, cioè un numero decimale gestito internamente tramite un intero. Il subject linka ad alcuni articoli estremamente matematici. Se hai passato 2 ore e ancora non li hai capiti, siamo in due. La buona notizia però è che non servono per completare il modulo, la brutta è che quelle due ore le hai perse. 

---

## 1. Orthodox Canonical Form

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

chiama il costruttore di copia, quindi 'ClassName(const ClassName &other)'.

```cpp
c = b;
```

chiama l’operatore di assegnazione, quindi 'ClassName &operator=(const ClassName &other)'.

---

## 2. Fixed-point numbers

Un numero fixed-point è un numero che conserva una parte frazionaria usando un intero. Che vuol dire? In sostanza se abbiamo un numero con la virgola, possiamo decidere di salvarlo come un numero intero. 

Immaginiamo di avere 1,50€ ed abbiamo un computer che salva solo numeri interi. La cosa più sensata ( mica tanto ma almeno è intuitiva ) è salvare quel numero come se fossero tutti centesimi. Quindi 150. Noi sapremo allora che, per convertirlo a numero con la virgola, dobbiamo semplicemente dividere per 100. 

La stessa cosa avviene in questi esercizi, solo che dobbiamo ragionare in binario ( base 2) e non in base 10.

Perchè i float non vanno bene? 
Perchè si perde precisione su numeri molto grandi o molto piccoli. Nei fixed point la precisione è sempre la stessa, dipende esclusivamente da quanti bit (frazionari) decidi di riservare alle cifre dopo la virgola.


Nel progetto, la classe ha:

```cpp
private:
    int                 _value;
    static const int    _fractionalBits = 8;
```

`_value` contiene il valore grezzo.

`_fractionalBits` indica quanti bit sono dedicati alla parte decimale.

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

## 3. Exercise 00 — Prima classe Fixed

### Obiettivo

Creare una classe `Fixed` in Orthodox Canonical Form.

In questo esercizio la classe non deve ancora fare le conversioni numeriche.

Serve soprattutto per capire:

- costruttore
- costruttore di copia
- operatore `=`
- distruttore
- getter/setter del raw value

### Funzioni principali

```cpp
int getRawBits(void) const;
void setRawBits(int const raw);
```

### `getRawBits`

Restituisce il valore interno `_value`.

```cpp
return (_value);
```

### `setRawBits`

Modifica il valore interno `_value`.

```cpp
_value = raw;
```

---

## 4. Exercise 01 — Conversione int/float

### Obiettivo

Rendere la classe `Fixed` davvero utilizzabile.

Si aggiungono:

```cpp
Fixed(const int n);
Fixed(const float n);
float toFloat(void) const;
int toInt(void) const;
```

### Costruttore da int

Il costruttore si prende un int e lo shifta di n fractional bit per salvarsi il rawBit.

```cpp
_value = n << _fractionalBits;
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

### Costruttore da float

Un float deve essere moltiplicato per 256 e arrotondato.

```cpp
_value = roundf(n * (1 << _fractionalBits));
```

Esempio:

```cpp
Fixed a(42.42f);
```

Internamente viene salvato un intero che rappresenta 42.42.

Serve `roundf` perché il risultato può avere decimali.

### `toFloat`

Converte il valore fixed-point in float.

```cpp
return ((float)_value / (1 << _fractionalBits));
```

Esempio:

```cpp
_value = 10860;
```

Risultato:

```text
10860 / 256 = 42.421875
```

### `toInt`

Converte il valore fixed-point in int.

```cpp
return (_value >> _fractionalBits);
```

È come dividere per 256, ma usando i bit.

### Overload di `<<`

Per stampare direttamente un oggetto `Fixed`:

```cpp
std::cout << fixed;
```

si implementa:

```cpp
std::ostream &operator<<(std::ostream &out, const Fixed &fixed);
```

Dentro, normalmente si stampa:

```cpp
out << fixed.toFloat();
return (out);
```

---

## 5. Exercise 02 — Operator Overload

### Obiettivo

Rendere `Fixed` simile a un tipo numerico vero. Quindi dobbiamo "riscrivere" i comportamenti di alcuni operatori. 

Perchè? 
perchè di base gli operatori sanno operare su classi precise standard ( tipo int, float, double ecc ), dobbiamo dire al programma che quando legge quegli operatori, si deve andare sostanzialmente a prendere il comportamento delle funzioni che scriviamo io.

Si aggiungono operatori:

- confronto
- aritmetici
- incremento/decremento
- min/max

### Operatori di confronto

```cpp
>
<
>=
<=
==
!=
```

Questi confrontano il valore interno `_value`.

Esempio:

```cpp
bool Fixed::operator>(const Fixed &other) const
{
    return (_value > other._value);
}
```
quindi gli stiamo dicendo che quando chiediamo se a > b, lui deve confrontare il _value dei due oggetti. 
b è "other". 

Non serve convertire in float: se entrambi usano la stessa scala, confrontare il raw value è corretto.

### Operatori aritmetici

```cpp
+
-
*
/
```

Somma:

```cpp
return Fixed(this->toFloat() + other.toFloat());
```

Sottrazione:

```cpp
return Fixed(this->toFloat() - other.toFloat());
```

Moltiplicazione:

```cpp
return Fixed(this->toFloat() * other.toFloat());
```

Divisione:

```cpp
return Fixed(this->toFloat() / other.toFloat());
```

Versione semplice e leggibile: converti in float, fai l’operazione, ritorni un nuovo `Fixed`.

---

## 6. Incremento e decremento

### Pre-incremento

```cpp
Fixed &Fixed::operator++()
{
    _value++;
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
    _value++;
    return (tmp);
}
```

Usato così:

```cpp
a++;
```

Prima salva una copia, poi incrementa, poi restituisce il vecchio valore.

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

## 7. min e max

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

---

