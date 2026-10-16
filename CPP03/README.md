# CPP03 - Ereditarietà e la famiglia ClapTrap

Modulo sull'ereditarietà: parto da una classe base, `ClapTrap`, e costruisco classi derivate che riusano la parte comune e aggiungono la propria.

Cose nuove:

- ereditarietà
- `protected`
- ordine di costruzione e distruzione
- override di metodi
- ereditarietà multipla
- diamond problem ed ereditarietà virtuale

La gerarchia completa a fine modulo è questa:

```text
        ClapTrap
        /      \
   ScavTrap   FragTrap
        \      /
       DiamondTrap
```

---

# Ereditarietà

## Cos'è

Quando scrivo:

```cpp
class ScavTrap : public ClapTrap
```

sto dicendo che `ScavTrap` **è un** `ClapTrap`, con qualcosa in più.

Quindi `ScavTrap` si ritrova gratis:

- gli attributi di `ClapTrap` (`_name`, `_hitPoints`, ...)
- i metodi di `ClapTrap` (`attack`, `takeDamage`, `beRepaired`, ...)

e può aggiungere roba sua, tipo `guardGate()`.

Il vantaggio è che il codice comune lo scrivo una volta sola, nella base.

Il `public` davanti a `ClapTrap` vuol dire che quello che era pubblico nella base resta pubblico anche nella derivata. È il caso normale, e quello che uso in tutto il modulo.

## private, protected, public

In CPP00 avevamo `private` e `public`. Ora entra il terzo:

| Accesso     | Classe stessa | Derivate | Fuori |
| ----------- | ------------- | -------- | ----- |
| `private`   | sì            | no       | no    |
| `protected` | sì            | sì       | no    |
| `public`    | sì            | sì       | sì    |

In ex00 gli attributi di `ClapTrap` sono `private`.

Da ex01 diventano `protected`, perché `ScavTrap` deve poter cambiare `_hitPoints`, `_energyPoints` ecc. nel suo costruttore. Con `private` non potrebbe toccarli, neanche essendo una derivata.

## Ordine di costruzione e distruzione

Una derivata contiene dentro di sé una base. Quindi:

- costruzione: prima la base, poi la derivata
- distruzione: prima la derivata, poi la base

```cpp
ScavTrap::ScavTrap(std::string name) : ClapTrap(name)
{
    _hitPoints = 100;
    ...
}
```

Ordine:

1. `ClapTrap(name)` costruisce la parte base, con i valori di ClapTrap (10, 10, 0)
2. il corpo del costruttore `ScavTrap` sovrascrive le statistiche con le sue (100, 50, 20)

Mentalmente è come costruire una casa: prima le fondamenta, poi i piani sopra. Per demolirla, si parte dall'alto.

I log nei costruttori e distruttori servono proprio a vedere questo ordine:

```text
ClapTrap Carlotta constructor has been called.
ScavTrap Carlotta constructor has been called
...
ScavTrap Carlotta destructor called
ClapTrap Carlotta destructor has been called.
```

## Override

`ScavTrap` ridefinisce `attack()`:

```cpp
void attack(const std::string &target);
```

Stessa firma della base, comportamento diverso (in questo caso, il messaggio dice `ScavTrap` invece di `ClapTrap`).

Se chiamo `attack()` su uno `ScavTrap`, viene usata la versione di `ScavTrap`.

Nota: senza `virtual` questo funziona solo se chiamo il metodo direttamente sull'oggetto `ScavTrap`. Se lo chiamassi tramite un puntatore a `ClapTrap`, userebbe la versione di `ClapTrap`. Questo è il tema di CPP04.

---

# Ex00 - Aaaaand... OPEN!

## Obiettivo

Implementare la classe `ClapTrap` con nome, punti vita, energia, danno d'attacco e tre azioni base:

```cpp
void attack(const std::string &target);
void takeDamage(unsigned int amount);
void beRepaired(unsigned int amount);
```

Valori iniziali:

```text
_hitPoints = 10
_energyPoints = 10
_attackDamage = 0
```

## Azioni

`attack()` e `beRepaired()` funzionano solo se il robot ha ancora energia e vita:

```cpp
if (_energyPoints > 0 && _hitPoints > 0)
```

Ognuna delle due consuma 1 energy point.

`attack()` stampa un messaggio e basta: non toglie davvero vita a nessuno, perché il target è solo una stringa.

`beRepaired()` aggiunge hit points.

`takeDamage()` toglie hit points.

## takeDamage e unsigned

Gli attributi sono `unsigned int`. Se un robot ha 10 HP e prende 20 di danno:

```text
10 - 20 = -10   <- non esiste in unsigned
```

Con un `unsigned` non esiste il negativo: il valore fa il giro e diventa un numero enorme (circa 4 miliardi). Il robot morto si ritrova immortale.

Quindi prima di sottrarre controllo:

```cpp
if (amount >= _hitPoints)
    _hitPoints = 0;
else
    _hitPoints -= amount;
```

## Forma canonica

Come da CPP02, la classe va scritta in Orthodox Canonical Form:

- costruttore (qui con il nome)
- costruttore di copia
- operatore di assegnazione
- distruttore

Ognuno stampa un log, così si vede quando viene chiamato.

---

# Ex01 - Serena, my love!

## Obiettivo

Creare `ScavTrap` come classe derivata da `ClapTrap`, con statistiche diverse, messaggi diversi e la funzione speciale `guardGate()`.

Valori:

```text
100 hit points
50 energy points
20 attack damage
```

## Costruttore

```cpp
ScavTrap::ScavTrap(std::string name) : ClapTrap(name)
{
    _name = name, _hitPoints = 100 , _energyPoints = 50, _attackDamage = 20;
}
```

Il costruttore della base va chiamato nella initialization list. Se non lo faccio, il compilatore prova a chiamare il costruttore di default di `ClapTrap`, che qui non esiste: errore di compilazione.

Dopo, nel corpo, sovrascrivo le statistiche. Posso farlo perché da questo esercizio gli attributi sono `protected`.

## Copia

Anche costruttore di copia e `operator=` delegano alla base:

```cpp
ScavTrap::ScavTrap(ScavTrap &other) : ClapTrap(other) {}

ScavTrap& ScavTrap::operator=(const ScavTrap& other)
{
    if (this != &other)
        ClapTrap::operator=(other);
    return *this;
}
```

`ScavTrap` non ha attributi suoi, quindi copiare la parte `ClapTrap` è copiare tutto.

## guardGate()

Stampa che lo `ScavTrap` è entrato in Gate keeper mode. È il metodo che la base non ha.

## attack()

Ridefinito per stampare `ScavTrap` invece di `ClapTrap`. È il primo override del modulo.

---

# Ex02 - Repetitive work

## Obiettivo

Creare `FragTrap`, un'altra classe derivata da `ClapTrap`, con i suoi valori iniziali e la funzione speciale `highFivesGuys()`.

Valori iniziali:

```text
100 hit points
100 energy points
30 attack damage
```

## Struttura

È praticamente lo `ScavTrap` dell'esercizio prima con altri numeri, infatti l'esercizio si chiama "Repetitive work".

Ogni derivata ha:

- proprio costruttore, che chiama quello della base
- proprio distruttore
- eventuali metodi propri
- valori iniziali diversi

```text
ClapTrap = base comune
ScavTrap = guardiano, guardGate()
FragTrap = high five robot, highFivesGuys()
```

`highFivesGuys()` stampa una richiesta di high five. Profondissimo.

---

# Ex03 - Now it's weird!

## Obiettivo

Creare `DiamondTrap`, che eredita sia da `ScavTrap` sia da `FragTrap`.

```cpp
class DiamondTrap: public ScavTrap, public FragTrap
```

Deve avere:

- un suo `_name`
- il nome della parte `ClapTrap` uguale a `name + "_clap_name"`
- le statistiche prese un po' da uno e un po' dall'altro
- `attack()` di `ScavTrap`
- `whoAmI()`, che stampa entrambi i nomi

## Il diamond problem

Ereditando da due classi che derivano entrambe da `ClapTrap`, di default `DiamondTrap` si ritrova con **due** `ClapTrap` dentro:

```text
DiamondTrap
├── ScavTrap
│   └── ClapTrap   <- copia 1
└── FragTrap
    └── ClapTrap   <- copia 2
```

Quindi due `_name`, due `_hitPoints`, ecc.

Se scrivo `_hitPoints`, il compilatore non sa quale dei due intendo: errore di ambiguità.

## Ereditarietà virtuale

La soluzione è dire a `ScavTrap` e `FragTrap` che la base `ClapTrap` va condivisa:

```cpp
class ScavTrap : virtual public ClapTrap
class FragTrap : virtual public ClapTrap
```

Con `virtual`, dentro `DiamondTrap` c'è un solo `ClapTrap`:

```text
DiamondTrap
├── ScavTrap ─┐
├── FragTrap ─┤
└── ClapTrap  <- una sola copia, condivisa
```

Il `virtual` va messo su `ScavTrap` e `FragTrap`, non su `DiamondTrap`. Infatti nel codice è solo nell'ex03: negli esercizi prima non serviva.

## Chi costruisce ClapTrap?

Con l'ereditarietà virtuale cambia una regola: la base virtuale la costruisce la classe **più derivata**, cioè `DiamondTrap`.

```cpp
DiamondTrap::DiamondTrap(std::string name)
    : ClapTrap(name + "_clap_name"), ScavTrap(name), FragTrap(name)
```

Le chiamate a `ClapTrap(name)` dentro i costruttori di `ScavTrap` e `FragTrap` vengono ignorate, sennò `ClapTrap` verrebbe costruito due volte.

Ordine nei log:

```text
ClapTrap Alpha_clap_name constructor has been called.
ScavTrap Alpha constructor has been called
FragTrap Alpha constructor has been called.
DiamondTrap Alpha constructor has been called.
```

Uno solo `ClapTrap`, all'inizio.

## Statistiche

Il subject vuole:

```text
hit points:    da FragTrap -> 100
energy points: da ScavTrap -> 50
attack damage: da FragTrap -> 30
```

Il problema è che gli attributi stanno nell'unico `ClapTrap` condiviso, e i costruttori di `ScavTrap` e `FragTrap` li sovrascrivono uno dopo l'altro. Dopo `FragTrap`, l'energia sarebbe 100, non 50.

Quindi nel corpo del costruttore di `DiamondTrap` li imposto a mano:

```cpp
_hitPoints = 100;
_energyPoints = 50;
_attackDamage = 30;
```

## I due nomi

`DiamondTrap` ha un suo `_name`, che **nasconde** quello di `ClapTrap`:

```cpp
private:
    std::string _name;
```

Dentro `DiamondTrap`:

- `_name` è quello di `DiamondTrap`
- `ClapTrap::_name` è quello della base

```cpp
_name = name;
ClapTrap::_name = name + "_clap_name";
```

`whoAmI()` li stampa tutti e due:

```text
DiamondTrap name: Alpha
ClapTrap name: Alpha_clap_name
```

## attack()

```cpp
void DiamondTrap::attack(const std::string &target)
{
    ScavTrap::attack(target);
}
```

`ScavTrap::` dice esplicitamente quale versione usare. Senza, con due basi che hanno entrambe un `attack()`, sarebbe ambiguo.

---

# Checklist finale modulo

## Concetti da sapere spiegare

- cosa vuol dire che `ScavTrap` "è un" `ClapTrap`
- differenza tra `private`, `protected` e `public`
- perché da ex01 gli attributi diventano `protected`
- ordine di costruzione e distruzione tra base e derivata
- perché il costruttore della base va chiamato nella initialization list
- cos'è un override
- cos'è il diamond problem
- cosa cambia con `virtual public`
- chi costruisce la base virtuale
- perché `DiamondTrap` ha due `_name` e come si accede a ciascuno

## Errori tipici da evitare

- sottrarre danni a un `unsigned` senza controllare: underflow, robot immortale
- lasciare gli attributi `private` e non riuscire a modificarli dalla derivata
- dimenticare di chiamare il costruttore della base nella initialization list
- copiare solo la parte derivata in `operator=` e dimenticare la base
- in ex03 dimenticare `virtual` su `ScavTrap` / `FragTrap`
- in ex03 non impostare a mano le statistiche: vince l'ultimo costruttore chiamato
- chiamare `attack()` su `DiamondTrap` senza specificare quale

## Mini riassunto mentale

```text
ex00 -> ClapTrap base, forma canonica, attenzione agli unsigned
ex01 -> ScavTrap : public ClapTrap, protected, override di attack
ex02 -> FragTrap, stessa cosa con altri numeri
ex03 -> DiamondTrap : ScavTrap, FragTrap
        virtual public per avere un solo ClapTrap
        due _name, stats a mano, attack di ScavTrap
```
