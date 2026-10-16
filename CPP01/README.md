# CPP01 - Memoria, puntatori, reference e file

- stack
- heap
- `new` / `delete`
- `new[]` / `delete[]`
- puntatori
- reference
- file stream
- puntatori a funzioni membro
- `switch` e fallthrough

Creare oggetti non basta. In questo modulo il punto è capire dove vivono, chi li possiede, quando muoiono e quale sintassi usare per manipolare memoria, reference, file e chiamate a metodi.

---

# Stack e heap

## Stack

Oggetto locale:

```cpp
Zombie zombie("Carlotta");
```

Vive nello stack quindi:

- distruzione automatica a fine scope
- niente `delete` ( analogo di free in C )

## Heap

Oggetto dinamico:

```cpp
Zombie *heapZombie = newZombie("heapCarlotta");
```

Da liberare esplicitamente con delete ( o delete[] se si tratta di array ) sennò leak.

```cpp
delete heapZombie;
```

---

# Ex00 - BraiiiiiiinnnzzzZ

## Obiettivo

Creare una classe `Zombie`, farla annunciare e confrontare due modi di crearla: una funzione che ritorna uno zombie nell' heap e una funzione che crea uno zombie temporaneo nello stack.

Funzioni:

```cpp
Zombie* newZombie(std::string name);
void randomChump(std::string name);
```

## newZombie

```cpp
Zombie* newZombie(std::string name)
{
    return new Zombie(name);
}
```

Crea nello heap e ritorna un puntatore. Chi riceve il puntatore ovviamente deve fare `delete`.

Nel `main`:

```cpp
heapZombie = newZombie("heapCarlotta");
heapZombie->Announce();
delete heapZombie;
```

## randomChump

```cpp
void randomChump(std::string name)
{
    Zombie chump(name);
    chump.Announce();
}
```

Zombie locale alla funzione.

- nasce
- annuncia
- muore automaticamente a fine funzione

## Distruttore

Il distruttore stampa:

```cpp
std::cout << name << ": Destroyed" << std::endl;
```

L'idea è usare i log per vedere il ciclo di vita degli oggetti.

---

# Ex01 - Moar brainz!

## Obiettivo

Creare una horde, cioè un array dinamico di `Zombie`, con una sola allocazione e poi liberarlo correttamente.

Funzione:

```cpp
Zombie* zombieHorde(int N, std::string name);
```

Allocazione:

```cpp
Zombie *horde = new Zombie[N];
```

Crea `N` zombie nell'heap. Poi nomi assegnati con `setName()`.

## Costruttore di default

Con `new Zombie[N]` C++ crea prima `N` oggetti usando:

```cpp
Zombie::Zombie()
```

Solo dopo si assegna il nome a ogni elemento.

## delete[]

Nel `main`:

```cpp
delete[] horde;
```
Così viene chiamato il distruttore per ogni zombie.

## Nomi

Uso di `std::stringstream`:

```cpp
conv << name << i;
horde[i].setName(conv.str());
```

Esempio:

```text
zombie 0
zombie 1
zombie 2
```

---

# Ex02 - HI THIS IS BRAIN

## Obiettivo

Stampare indirizzi e valori di una stringa, di un puntatore alla stringa e di una reference alla stringa. Serve a vedere che reference e puntatore possono riferirsi allo stesso dato, ma con sintassi diversa.

Nel codice:

```cpp
std::string str = "HI THIS IS BRAIN";
std::string* stringPTR = &str;
std::string& stringREF = str;
```

Tutti puntano / si riferiscono alla stessa stringa.

## Puntatore

```cpp
std::string* stringPTR = &str;
```

- contiene un indirizzo
- dereferenziazione con `*stringPTR`
- può essere cambiato per puntare altrove

## Reference

```cpp
std::string& stringREF = str;
```

- alias di `str`
- non nuovo oggetto
- uso diretto, senza `*`

```cpp
stringREF
```

## Output

Il programma stampa:

- indirizzo di `str`
- valore di `stringPTR` = indirizzo di `str`
- indirizzo di `stringREF` = ancora indirizzo di `str`
- valore di `str`
- valore puntato da `stringPTR`
- valore letto tramite `stringREF`

Stesso dato, sintassi diversa.

---

# Ex03 - Unnecessary violence

## Obiettivo

Creare `Weapon`, `HumanA` e `HumanB` per capire quando una classe deve contenere una reference e quando invece ha più senso usare un puntatore.

Classi:

```text
Weapon
HumanA
HumanB
```

`Weapon` contiene:

```cpp
std::string type;
```

Metodi:

```cpp
const std::string& getType() const;
void setType(std::string str);
```

## HumanA

```cpp
HumanA::HumanA(std::string str, Weapon& w): name(str), weapon(w)
```

`HumanA` riceve una `Weapon&` nel costruttore. HumanA deve sempre avere un'arma, quindi il campo non può essere NULL ( perciò non si può usare un puntatore ). La reference va inizializzata nella initialization list quindi non sarà mai vuota.

## HumanB

```cpp
HumanB::HumanB(std::string str): name(str), weapon(NULL)
```

HumanB parte senza arma. Qui il puntatore ha senso: può essere `NULL` all'inizio.

Poi:

```cpp
void HumanB::setWeapon(Weapon& w)
{
    weapon = &w;
}
```

## Cambio arma

Nel `main`:

```cpp
club.setType("some other type of club");
```

Poi `attack()` stampa il nuovo tipo.

`HumanA` e `HumanB` non hanno copie dell'arma. Usano lo stesso oggetto `Weapon`.

---

# Ex04 - Sed is for losers

## Obiettivo

Rifare un mini comportamento tipo `sed`: leggere un file, sostituire tutte le occorrenze di una stringa con un'altra e scrivere il risultato in un nuovo file:

```text
<filename>.replace
```

Il subject vieta `std::string::replace`, quindi la sostituzione va costruita a mano con `find` e `substr`.

File stream:

```cpp
std::ifstream file(filename.c_str());
std::ofstream new_file(outname.c_str());
```

## Sostituzione

Per ogni riga:

1. cerca `s1` con `line.find(s1, found)`
2. copia la parte prima dell'occorrenza
3. aggiunge `s2`
4. avanza oltre `s1`
5. copia il resto della riga


```cpp
result += line.substr(pos, found - pos);
result += s2;
pos = found + s1.length();
```

Dopo ogni sostituzione riparto a cercare **dopo** `s1` (`found += s1.length()`), non dall'inizio. Altrimenti ritrovo sempre la stessa occorrenza e il ciclo non finisce più.

E siccome costruisco una stringa nuova (`result`) invece di modificare `line`, non ho problemi neanche se `s2` contiene `s1`, tipo `s1 = "a"` e `s2 = "aa"`.

## Error handling ex04

- numero di argomenti diverso da 3
- `s1` vuota: `find("")` trova una occorrenza ovunque, quindi va rifiutata
- file di input che non si apre
- file di output che non si crea

Nota: leggo con `getline` e riscrivo ogni riga con `std::endl`, quindi se il file originale non finisce con un newline, il `.replace` ce l'avrà comunque.

---

# Ex05 - Harl 2.0

## Obiettivo

Creare una classe `Harl` che stampa messaggi diversi in base al livello richiesto, usando puntatori a funzioni membro invece di una catena infinita di `if/else`.

Livelli:

```text
DEBUG
INFO
WARNING
ERROR
```

Array di puntatori a funzioni membro:

```cpp
void (Harl::*actions[4])(void)
```

## Puntatori a funzioni membro

```text
actions = array di 4 puntatori a metodi di Harl, senza parametri, ritorno void
```

Chiamata:

```cpp
(this->*actions[i])();
```

Schema:

1. trovare indice del livello
2. chiamare il metodo corrispondente

Nel codice:

```cpp
for (int i = 0; i < 4; i++)
{
    if (level == levels[i])
    {
        (this->*actions[i])();
        return;
    }
}
```

`this->*` serve perché un metodo ha bisogno di un oggetto su cui essere chiamato. Il puntatore da solo dice "quale metodo", `this` dice "su chi".

Se il livello non esiste, stampo un messaggio di errore.


---

# Ex06 - Harl filter

## Obiettivo

Filtrare i messaggi di `Harl`: dato un livello, stampare quel livello e tutti quelli più gravi usando uno `switch` con fallthrough voluto.

Esempio:

```text
WARNING
```

Output: `WARNING` e `ERROR`.

Livello non valido:

```text
[ Probably complaining about insignificant problems ]
```

## Logica

Array livelli:

```cpp
std::string levels[4] = {"DEBUG", "INFO", "WARNING", "ERROR"};
```

Si trova l'indice passato da argv.

Poi `switch` senza `break` tra i primi casi:

```cpp
case 0:
    h.complain(0);
case 1:
    h.complain(1);
case 2:
    h.complain(2);
case 3:
    h.complain(3);
    break;
```

Da `INFO` scende anche su `WARNING` ed `ERROR`.

Il fallthrough è voluto, quindi nel codice c'è un commento `// fall through` dopo ogni caso. Alcuni compilatori (gcc con `-Wextra`) altrimenti danno warning, e con `-Werror` non compila.

Il caso `default` stampa il messaggio di livello non valido.

---

# Checklist finale modulo

## Concetti da sapere spiegare

- differenza tra stack e heap
- quando un oggetto viene distrutto in automatico e quando serve `delete`
- `new` / `delete` vs `new[]` / `delete[]`
- perché `new Zombie[N]` richiede un costruttore di default
- differenza tra puntatore e reference
- perché `HumanA` usa una reference e `HumanB` un puntatore
- perché una reference membro va inizializzata nella initialization list
- `std::ifstream` / `std::ofstream` e perché si passa `.c_str()` in C++98
- sintassi dei puntatori a funzioni membro e `this->*`
- `switch` con fallthrough

## Errori tipici da evitare

- usare `delete` su un array allocato con `new[]` (o il contrario)
- dimenticare `delete` sugli zombie creati con `newZombie`
- usare una reference per `HumanB`, che all'inizio non ha arma
- non controllare `NULL` prima di usare il puntatore all'arma
- in ex04 ripartire a cercare dall'inizio della riga dopo ogni sostituzione
- in ex04 accettare `s1` vuota
- in ex05 usare una catena di `if/else` invece dei puntatori a metodi
- in ex06 dimenticare il `break` dopo l'ultimo caso

## Mini riassunto mentale

```text
ex00 -> stack vs heap
ex01 -> new[] / delete[]
ex02 -> puntatore e reference sullo stesso dato
ex03 -> reference se c'è sempre, puntatore se può mancare
ex04 -> file stream + find/substr
ex05 -> array di puntatori a metodi
ex06 -> switch con fallthrough
```
