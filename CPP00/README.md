# CPP00 - Stream, string e prime classi

La logica è la stessa di C, cambia il modo di organizzarla.

Cose nuove:

- `std::cout` / `std::cin`
- `std::string`
- classi
- oggetti
- `private` / `public`
- costruttore
- distruttore
- metodi getter/setter

---

## std::cout

In C:

```c
printf("hello\n");
```

In C++ si usano gli stream:

```cpp
std::cout << "hello" << std::endl;
```

`std::cout` = output standard.

`<<` manda qualcosa nello stream.

`std::endl` manda newline e fa flush.

Quindi:

```cpp
std::cout << "ciao" << std::endl;
```
manda "ciao" sul terminale + vai a capo

## std::cin

Input standard.

```cpp
std::cin >> value;
```

Legge fino al primo separatore (spazio / newline).

## std::getline

Per leggere la riga intera:

```cpp
std::getline(std::cin, str);
```

Nel phonebook serve perché i campi possono avere spazi.

Esempio:

```text
First Name: Maria Luisa
```

Con `std::cin >> str` leggerebbe solo `Maria`. `getline` legge tutta la riga.

Quando uso `std::cin >>` e `getline` insieme, rimane newline nel buffer. Nel codice succede in ex01, `searchContact()` dopo aver letto l'indice:

```cpp
std::cin >> index;
std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
```

`ignore` serve per buttare via il resto della riga.

## std::string

In C sono array di char:

```c
char *str;
char str[100];
```

In C++ c'è `std::string`, che è una classe. Quindi un oggetto (istanza della classe) con metodi (funzioni appartenenti alla classe).

Esempi dal modulo:

```cpp
std::string str = argv[j];
str.length();
str.substr(0, 9);
```

`length()` mi dà la lunghezza. Ed è già "disponibile" quando creo l'oggetto "string".

`substr(0, 9)` prende 9 caratteri a partire dall'indice 0, cioè dall'indice 0 all'8.
Nel phonebook l'ho usato per accorciare i campi troppo lunghi:

```cpp
str.substr(0, 9) + "."
```

Se il nome è più lungo di 10 caratteri, tengo i primi 9 e aggiungo `.`.

---

# Classi

## Cos'è una classe

Una classe è un tipo creato da me. Come `int` è un tipo già esistente, posso creare un tipo mio:

```cpp
class Contact
{
};
```
E' tipo un blueprint, e da lì posso creare oggetti di quel tipo:

```cpp
Contact contact;
```

Quindi:

```text
classe = stampo / modello
oggetto = cosa reale creata da quello stampo
```

Esempio:

```text
classe Contact = idea generale di contatto
oggetto contact = un contatto concreto
```

Nel modulo:

```cpp
class Contact
class PhoneBook
```

- `Contact` rappresenta un singolo contatto
- `PhoneBook` rappresenta la rubrica

## Dati + comportamenti

```cpp
class Contact
{
    private:
        std::string firstName;
        std::string lastName;

    public:
        void setFirstName(std::string str);
        std::string getFirstName();
};
```

Dentro la classe ci sono:

- attributi / membri dati: `firstName`, `lastName`, ecc.
- metodi / funzioni membro: `setFirstName`, `getFirstName`, ecc.

## Oggetto

Quando scrivo:

```cpp
PhoneBook phonebook;
```

sto creando un oggetto di tipo PhoneBook.

Questo oggetto ha:

- i suoi dati interni
- i suoi metodi

Quindi posso fare:

```cpp
phonebook.addContact();
phonebook.searchContact();
```

Il punto `.` significa: chiama questo metodo su questo oggetto.

---

# private e public

## Perché esistono

La classe ha zone con accesso diverso.

```cpp
private:
    // roba interna

public:
    // roba accessibile da fuori
```

## private

`private` significa: accessibile solo dall'interno della classe.

In `Contact.hpp`:

```cpp
private:
    std::string firstName;
    std::string lastName;
    std::string nickname;
    std::string phoneNumber;
    std::string secret;
```

Quindi da fuori NON posso fare:

```cpp
contact.firstName = "Susanna";
```

Perché `firstName` è privato.

Questa cosa serve a proteggere i dati. Quindi i metodi che operano all'interno della classe possono modificare questi campi, ma se io chiamo un metodo della classe all'esterno ( tipo sul main ) non posso farlo a meno che non imposto un metodo pubblico setter/getter.

## public

`public` significa: accessibile da fuori.

In `Contact.hpp`:

```cpp
public:
    void setFirstName(std::string str);
    std::string getFirstName();
```

Così da fuori posso modificare o leggere il dato private:

```cpp
contact.setFirstName("Susanna");
std::cout << contact.getFirstName();
```

## Incapsulamento

Significa che l'oggetto tiene i suoi dati protetti e decide come farli leggere/modificare attraverso dei metodi pubblici ( quello che ho scritto prima ).

Nel modulo il caso è semplice:

```cpp
void Contact::setFirstName(std::string str)
{
    firstName = str;
}
```

Però il concetto è:

```text
fuori dalla classe non modifico firstName direttamente
chiamo setFirstName()
```

Così se un giorno volessi aggiungere un controllo, lo farei nel setter.

Esempio mentale:

```cpp
void Contact::setPhoneNumber(std::string str)
{
    // potrei controllare che siano solo numeri
    phoneNumber = str;
}
```
---

# Header e cpp

Le classi sono divise tra `.hpp` e `.cpp`.

## .hpp

Nel `.hpp` dichiaro la classe. E' come l'header file di C

Esempio:

```cpp
class Contact
{
    private:
        std::string firstName;

    public:
        Contact();
        ~Contact();
        void setFirstName(std::string str);
};
```

Qui sto dicendo:

```text
creo una classe di nome Contact con
1 attributo stringa di nome firstName
3 metodi pubblici
```
dentro all'hpp posso includere anche le librerie che mi servono per le funzioni della classe stessa.

## .cpp

Nel `.cpp` scrivo il corpo delle funzioni.

Esempio:

```cpp
void Contact::setFirstName(std::string str)
{
    firstName = str;
}
```

`Contact::` significa: questa funzione appartiene alla classe Contact

Senza `Contact::`, sarebbe una funzione normale globale, non un metodo della classe.

## Include guard ( come in C )

Evitano che lo stesso header venga incluso due volte nello stesso file.


```cpp
#ifndef CONTACT_HPP
# define CONTACT_HPP
...
#endif
```
---

# Costruttore e distruttore

## Costruttore

Il costruttore è una funzione speciale chiamata quando nasce un oggetto. Ha lo stesso nome della classe. Non ha tipo di ritorno.

Esempio:

```cpp
Contact::Contact()
{
}
```

Quando scrivo:

```cpp
Contact c;
```

viene chiamato automaticamente:

```cpp
Contact::Contact()
```

Nel `PhoneBook` il costruttore serve davvero:

```cpp
PhoneBook::PhoneBook()
{
    index = 0;
    count = 0;
}
```

Perché `index` e `count` sono int e vanno inizializzati


## Distruttore

Il distruttore è chiamato quando l'oggetto viene distrutto.

```cpp
Contact::~Contact()
{
}
```

Quando l'oggetto esce dallo scope, il distruttore viene chiamato automaticamente.

Nel main:

- all'inizio viene chiamato `PhoneBook::PhoneBook()`
- quando il programma finisce, viene chiamato `PhoneBook::~PhoneBook()` in automatico

## Costruttore di default

Sono costruttori che non prendono parametri:

```cpp
Contact();
PhoneBook();
```

---

# Ex00 - Megaphone

## Obiettivo

Scrivere un programma che prende gli argomenti da terminale e li stampa tutti in maiuscolo, usando un approccio C++ invece delle funzioni stile C.

Se non ci sono argomenti:

```text
* LOUD AND UNBEARABLE FEEDBACK NOISE *
```

## Conversione a std::string

Nel loop:

```cpp
std::string str = argv[j];
```

`argv[j]` è una stringa C. La salvo in una `std::string` per usare i metodi C++ tipo `length()`.

## toupper

```cpp
str[i] = std::toupper(str[i]);
std::cout << str[i];
```

`toupper` converte il carattere in maiuscolo.
Il programma stampa carattere per carattere, non aspetta di avere tutta la stringa trasformata.

Esempio:

```bash
./megaphone hello world
```

Output:

```text
HELLOWORLD
```

Non aggiunge spazi tra gli argomenti.

---

# Ex01 - My Awesome PhoneBook

## Obiettivo

Creare una rubrica anni '80 con due classi, `PhoneBook` e `Contact`, capace di salvare massimo 8 contatti, cercarli e mostrarli in tabella.

È una rubrica interattiva, niente allocazione dinamica: il subject la vieta.

Comandi:

```text
ADD
SEARCH
EXIT
```

Il main ha un loop infinito:

```cpp
while (true)
```

Dentro legge un comando con:

```cpp
std::getline(std::cin, str);
```

Poi:

```cpp
if (str == "ADD")
    phonebook.addContact();

if (str == "SEARCH")
    phonebook.searchContact();

if (str == "EXIT")
    break;
```

Nota: sono tre `if`, non `else if`. In questo caso va bene perché le stringhe sono esclusive.

## EOF

Nel main:

```cpp
if (std::cin.eof())
{
    std::cout << "\nEOF detected. Exiting.\n";
    break;
}
```

EOF = fine input. Può succedere con `Ctrl+D`.
Senza questo controllo `getline` continua a fallire e il loop gira all'infinito.

---

## Classe Contact

Dati:

```cpp
std::string firstName;
std::string lastName;
std::string nickname;
std::string phoneNumber;
std::string secret;
```

Sono `private`, quindi fuori dalla classe non li posso toccare direttamente.

### Setter

Setter = metodo che modifica un campo.

Esempio:

```cpp
void Contact::setFirstName(std::string str)
{
    firstName = str;
}
```

Uso:

```cpp
contacts[index].setFirstName(str);
```

Quindi sto dicendo:

```text
sul contatto in posizione index, imposta firstName con str
```

### Getter

Getter = metodo che legge un campo.

Esempio:

```cpp
std::string Contact::getFirstName()
{
    return (firstName);
}
```

Uso:

```cpp
contacts[i].getFirstName()
```

Getter e setter sono pubblici, per questo posso usarli sui dati privati.

---

## Classe PhoneBook

### Cosa contiene

In `PhoneBook.hpp`:

```cpp
Contact contacts[8];
int index;
int count;
```

- `contacts[8]`: array fisso di 8 contatti
- `index`: prossima posizione dove salvare
- `count`: quanti contatti sono effettivamente salvati

`contacts` è un array di oggetti `Contact`, non di puntatori.

Quindi quando creo `PhoneBook`, creo anche 8 oggetti `Contact` dentro di lui.

### Index e count

`index` indica dove scrivere il prossimo contatto.

`count` indica quanti contatti posso mostrare.

All'inizio:

```text
index = 0
count = 0
```

Dopo 1 ADD:

```text
index = 1
count = 1
```

Dopo 8 ADD:

```text
index = 0
count = 8
```

Dopo 9 ADD:

```text
index = 1
count = 8
```

Perché il nono sovrascrive il primo ( da subject superati gli 8 contatti, quelli successivi devono sovrascrivere i primi ).

### Rubrica circolare

Nel codice:

```cpp
index++;
if (index == 8)
    index = 0;
if (count < 8)
    count++;
```

Quindi:

- `index` gira da 0 a 7
- quando arriva a 8 torna a 0
- `count` cresce solo fino a 8

---

## ADD

`ADD` chiama:

```cpp
phonebook.addContact();
```

Dentro `addContact()` vengono chiesti i campi uno alla volta.

Funzione helper:

```cpp
ask_field("First Name", 0);
ask_field("Phone Number", 1);
```

Flag:

```text
0 -> campo generico
1 -> campo numerico
```

### is_valid

```cpp
static bool is_valid(const std::string& s, int digits_flag)
```

Controlli:

- stringa non vuota
- stringa non composta solo da spazi
- se `digits_flag == 1`, solo cifre

Per gli spazi:

```cpp
isspace(static_cast<unsigned char>(s[i]))
```

Per le cifre:

```cpp
isdigit(static_cast<unsigned char>(s[i]))
```

Nota sul cast a `unsigned char`: con `isdigit`/`isspace` evita problemi con char negativi.

### ask_field

```cpp
while (true)
```

continua a chiedere finché il campo non è valido.

Se arriva EOF:

```cpp
if (!std::getline(std::cin, str))
    return "EOF";
```

Nel `addContact()` controllo:

```cpp
if (str == "EOF")
    return;
```

---

## SEARCH

`SEARCH` chiama:

```cpp
phonebook.searchContact();
```

Passaggi:

1. se `count == 0`, stampa messaggio e torna
2. stampa tabella
3. chiede indice
4. valida input
5. stampa contatto completo

### Tabella

Header:

```cpp
std::setw(10)
```

`setw(10)` imposta larghezza della colonna a 10 caratteri.

Campi richiesti dalla tabella:

```text
index | first name | last name | nickname
```

### trimContact

Se il campo è più lungo di 10, va tagliato a 9 + punto.

```cpp
static std::string trimContact(std::string str)
{
    if (str.length() > 10)
        str = str.substr(0,9) + ".";
    return str;
}
```

Esempio:

```text
SuperLongName -> SuperLong.
```

### Scelta indice

Input:

```cpp
std::cin >> index;
```

Se l'utente scrive una parola:

```cpp
if (!std::cin)
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Invalid index.\n";
    return;
}
```

`std::cin.clear()` resetta lo stato di errore.

`ignore(...)` svuota il buffer fino al newline.

Senza `clear`, gli input successivi continuerebbero a fallire.

Senza `ignore`, la roba sbagliata resterebbe nel buffer.

### Indice utente vs indice array

La tabella mostra da 1 a 8:

```cpp
std::cout << std::setw(10) << i + 1
```

Ma l'array va da 0 a 7.

Quindi:

```cpp
displayContact(index - 1);
```

### displayContact

Stampa tutti i campi del contatto:

```cpp
contacts[index].getFirstName()
contacts[index].getLastName()
contacts[index].getNickname()
contacts[index].getPhoneNumber()
contacts[index].getDarkestSecret()
```

Anche `PhoneBook`, pur avendo l'array di `Contact`, non accede ai campi privati direttamente. Usa i getter.

## Error handling ex01

Casi da gestire:

- comando sconosciuto: viene ignorato e si richiede
- campo vuoto o fatto solo di spazi: si richiede
- numero di telefono con caratteri non numerici: si richiede
- `SEARCH` con rubrica vuota
- indice non numerico, tipo `abc`
- indice fuori range, tipo `0` o `9`
- EOF (`Ctrl+D`) sia nel menu sia a metà di un `ADD`

---

# Checklist finale modulo

## Concetti da sapere spiegare

- differenza tra `std::cout` / `std::cin` e `printf` / `scanf`
- differenza tra `std::cin >>` e `std::getline`
- perché serve `std::cin.ignore` dopo `std::cin >>`
- cosa fanno `std::cin.clear()` e `std::cin.eof()`
- cos'è una classe e cos'è un oggetto
- `private` vs `public`
- cos'è l'incapsulamento e a cosa servono getter e setter
- cosa vuol dire `Contact::` davanti a una funzione
- quando vengono chiamati costruttore e distruttore
- perché la rubrica è "circolare"

## Errori tipici da evitare

- usare `std::cin >>` per campi che possono contenere spazi
- mischiare `>>` e `getline` senza svuotare il buffer
- non gestire EOF: con `Ctrl+D` il programma va in loop
- non resettare lo stato di `std::cin` dopo un input sbagliato
- tagliare i campi a 10 caratteri invece che a 9 + `.`
- confondere indice mostrato all'utente (1-8) e indice dell'array (0-7)
- usare `new` nella rubrica: il subject vuole un array statico

## Mini riassunto mentale

```text
ex00 -> argv + std::string + toupper
stesso programma del C, ma con gli strumenti del C++

ex01 -> PhoneBook contiene 8 Contact
Contact = dati privati + getter/setter
PhoneBook = array circolare + ADD / SEARCH / EXIT
```
