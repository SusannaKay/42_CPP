# CPP03 - Ereditarieta' e ClapTrap family

Gerarchia mentale:

```text
ClapTrap
  |
  +-- ScavTrap
  |
  +-- FragTrap

DiamondTrap eredita da ScavTrap e FragTrap
```

Capire l'ereditarieta' in C++, partendo da una classe base (`ClapTrap`) e poi creando classi derivate che riusano la parte comune e aggiungono comportamenti propri.

Focus:

- costruttori e distruttori
- copy constructor
- `operator=`
- ereditarieta'
- ordine costruzione / distruzione
- override di metodi
- ereditarieta' multipla
- diamond problem

---

# Concetti chiave

## Classe base

`ClapTrap` = classe base.

Stato comune:

```cpp
std::string _name;
unsigned int _hitPoints;
unsigned int _energyPoints;
unsigned int _attackDamage;
```

Azioni comuni:

```cpp
attack()
takeDamage()
beRepaired()
```

Ogni robot ha:

- nome
- hit points
- energy points
- attack damage

`ClapTrap` è il modello base.

## Ereditarieta'

```cpp
class ScavTrap : public ClapTrap
```

`ScavTrap` è un `ClapTrap` con qualcosa in più.

Valori iniziali:

```text
ClapTrap: 10 HP, 10 energy, 0 attack
ScavTrap: 100 HP, 50 energy, 20 attack
FragTrap: 100 HP, 100 energy, 30 attack
DiamondTrap: 100 HP, 50 energy, 30 attack
```

## Ordine di costruzione

Classe derivata: prima base, poi derivata.

```cpp
ScavTrap::ScavTrap(std::string name) : ClapTrap(name)
```

Ordine:

1. `ClapTrap(name)`
2. corpo del costruttore `ScavTrap`

Distruzione al contrario:

1. distruttore derivata
2. distruttore base

I log constructor/destructor seguono questo ordine. 

## Override

`ScavTrap` ridefinisce `attack()`:

```cpp
void attack(const std::string &target);
```

Chiamata su `ScavTrap` -> versione `ScavTrap`.

Focus su relazione base/derivata e costruzione oggetti.

## Ereditarieta' multipla

In ex03:

```cpp
class DiamondTrap: public ScavTrap, public FragTrap
```

Forma:

```text
        ClapTrap
        /      \
   ScavTrap   FragTrap
        \      /
       DiamondTrap
```

Diamond problem: quante copie di `ClapTrap` finiscono dentro `DiamondTrap`?

Nel progetto si ragiona su ambiguità, nomi e metodi ereditati.

`DiamondTrap` ha anche un proprio `_name`, separato dal nome della parte `ClapTrap`:

```text
nome + "_clap_name"
```

`whoAmI()` stampa entrambi:

```cpp
DiamondTrap name: ...
ClapTrap name: ...
```

---

# Ex00 - Aaaaand... OPEN!

## Scopo

Implementare la classe `ClapTrap` con nome, punti vita, energia, danno d'attacco e tre azioni base: `attack`, `takeDamage`, `beRepaired`.

Creare `ClapTrap` con:

- nome
- hit points
- energy points
- attack damage
- `attack()`
- `takeDamage()`
- `beRepaired()`

Valori iniziali:

```text
_hitPoints = 10
_energyPoints = 10
_attackDamage = 0
```

## Azioni

`attack()` solo se:

```cpp
_energyPoints > 0 && _hitPoints > 0
```

Se attacca:

- stampa messaggio
- consuma 1 energy point

`beRepaired()`:

- controlla energia e vita
- aumenta hit points
- consuma 1 energy point

`takeDamage()`:

- sottrae hit points

## Forma canonica

Da fissare:

- constructor
- copy constructor
- assignment operator
- destructor

I log aiutano a vedere quando vengono chiamati.

---

# Ex01 - Serena, my love!

## Scopo

Creare `ScavTrap` come classe derivata da `ClapTrap`, con statistiche diverse, messaggi diversi e la funzione speciale `guardGate()`.

Aggiungere `ScavTrap`, derivata da `ClapTrap`.

Valori:

```text
100 hit points
50 energy points
20 attack damage
```

Funzione speciale:

```cpp
guardGate()
```

Stampa Gate keeper mode.

## Specializzazione

`ScavTrap` eredita la base e cambia quello che serve.

Costruttore:

```cpp
ScavTrap::ScavTrap(std::string name) : ClapTrap(name)
```

Prima viene costruita la parte `ClapTrap`, poi `ScavTrap` aggiorna le stats.

## Accesso ai membri

Per accedere direttamente ai membri della base da una derivata:

- `private`: solo classe stessa
- `protected`: classe stessa + derivate
- `public`: accessibile da fuori

---

# Ex02 - Repetitive work

## Scopo

Creare `FragTrap`, un'altra classe derivata da `ClapTrap`, con i suoi valori iniziali e la funzione speciale `highFivesGuys()`.

Aggiungere `FragTrap`, altra derivata da `ClapTrap`.

Valori iniziali:

```text
100 hit points
100 energy points
30 attack damage
```

Funzione speciale:

```cpp
highFivesGuys()
```

Stampa richiesta di high five.

## Differenza con ScavTrap

```text
ClapTrap = base comune
ScavTrap = guardiano
FragTrap = high five robot
```

Ogni derivata ha:

- proprio costruttore
- proprio distruttore
- eventuali metodi propri
- valori iniziali diversi

---

# Ex03 - Now it's weird!

## Scopo

Creare `DiamondTrap`, che eredita sia da `ScavTrap` sia da `FragTrap`, gestendo il nome proprio, il nome della parte `ClapTrap` e la funzione `whoAmI()`.

`DiamondTrap`: eredita da `ScavTrap` e `FragTrap`.

```cpp
class DiamondTrap: public ScavTrap, public FragTrap
```

Qui entrano:

- ereditarieta' multipla
- metodi da più rami
- nomi duplicati
- inizializzazione di più classi base

## Statistiche

`DiamondTrap` prende:

```text
hit points da FragTrap: 100
energy points da ScavTrap: 50
attack damage da FragTrap: 30
```

`attack()` delegato a `ScavTrap`:

```cpp
ScavTrap::attack(target);
```

## whoAmI()

Due identità:

- `_name` di `DiamondTrap`
- nome della parte `ClapTrap`: `name + "_clap_name"`

`whoAmI()` mostra questa doppia identità.
