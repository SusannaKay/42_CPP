# CPP05 - Eccezioni, Bureaucrat e Form

Modulo sulle eccezioni, ambientato in una burocrazia infinita. Molto realistico.

Cose nuove:

- `try` / `throw` / `catch`
- eccezioni custom che derivano da `std::exception`
- classi annidate
- membri `const`
- classi astratte usate per davvero
- una factory semplice con `Intern`

---

# Eccezioni

## Perché

In C, per segnalare un errore, una funzione ritorna un valore speciale (`-1`, `NULL`) e chi la chiama deve ricordarsi di controllarlo.

Con le eccezioni l'errore "salta" fuori dalla funzione, e se nessuno lo gestisce il programma termina. Non si può ignorare per sbaglio.

Serve soprattutto nei costruttori: un costruttore non ha valore di ritorno, quindi l'unico modo per dire "questo oggetto non si può creare" è lanciare un'eccezione.

## Sintassi

Lancio:

```cpp
throw GradeTooHighException();
```

Gestione:

```cpp
try
{
    Bureaucrat b("Carlotta", 0);  // codice che può fallire
}
catch (std::exception& e)
{
    std::cout << e.what() << std::endl;
}
```

Quando parte il `throw`, l'esecuzione esce subito dal blocco `try` e salta al primo `catch` compatibile. Quello che c'era dopo nel `try` non viene eseguito.

Il `catch` prende l'eccezione per reference (`std::exception&`): così posso catturare anche le classi derivate, e la chiamata a `what()` usa la versione giusta (è virtuale, come in CPP04).

## Eccezioni custom

Nel modulo le eccezioni sono classi annidate dentro la classe che le usa, e derivano da `std::exception`:

```cpp
class Bureaucrat
{
    ...
    class GradeTooHighException : public std::exception
    {
        virtual const char *what() const throw();
    };
};
```

Fuori dalla classe si chiamano con il nome completo: `Bureaucrat::GradeTooHighException`.

Ridefinisco solo `what()`, che ritorna il messaggio:

```cpp
const char *Bureaucrat::GradeTooHighException::what() const throw()
{
    return ("Grade too high.");
}
```

`throw()` dopo la firma vuol dire "questa funzione non lancia eccezioni". È la sintassi C++98, deve combaciare con quella di `std::exception::what()`.

---

# I gradi

Punto controintuitivo:

```text
1   = grado più alto
150 = grado più basso
```

Quindi:

- incrementare il grado = diminuire il numero
- decrementare il grado = aumentare il numero

```text
grade 42 increment -> grade 41
grade 42 decrement -> grade 43
```

Fuori range:

- sotto 1: `GradeTooHighException`
- sopra 150: `GradeTooLowException`

Per firmare o eseguire un form:

```text
grade burocrate <= grade richiesto -> ok
altrimenti exception
```

Numero più piccolo = più potere.

---

# Ex00 - Mommy, when I grow up, I want to be a bureaucrat!

## Obiettivo

Classe `Bureaucrat` con un nome costante e un grado da 1 a 150.

```cpp
std::string const _name;
int _grade;
```

## Costruttore

Valida subito il grado:

```cpp
if (grade > 150)
    throw Bureaucrat::GradeTooLowException();
else if (grade < 1)
    throw Bureaucrat::GradeTooHighException();
```

Se il grado non è valido, l'oggetto non viene creato.

## increment / decrement

`increment()` migliora il grado, quindi `_grade--`.

`decrement()` lo peggiora, quindi `_grade++`.

Il controllo va fatto **prima** di modificare:

```cpp
void Bureaucrat::increment()
{
    if (_grade <= 1)
        throw Bureaucrat::GradeTooHighException();
    _grade--;
}
```

Così, se lancio, il grado resta quello di prima.

## Membri const

`_name` è `const`: si può inizializzare solo nella initialization list e non si può più cambiare.

Conseguenza: `operator=` non può copiare il nome. Copia solo il grado.

```cpp
Bureaucrat &Bureaucrat::operator=(const Bureaucrat &other)
{
    if (this != &other)
        _grade = other.getGrade();
    return (*this);
}
```

## operator<<

```cpp
std::ostream &operator<<(std::ostream &out, Bureaucrat const &obj);
```

Output:

```text
name, bureaucrat grade X.
```

---

# Ex01 - Form up, maggots!

## Obiettivo

Aggiungere la classe `Form`, che un `Bureaucrat` può firmare se il suo grado è abbastanza alto.

```cpp
std::string const _name;
int const _grade;      // grado per firmare
int const _exec;       // grado per eseguire
bool _isSigned;        // parte da false
```

Anche qui i gradi devono essere tra 1 e 150, sennò il costruttore lancia.

Tutto `const` tranne `_isSigned`: quindi l'`operator=` di `Form` non può copiare praticamente niente. C'è perché la forma canonica lo vuole, ma è vuoto.

## beSigned() e signForm()

Due funzioni, una per lato:

```cpp
void Form::beSigned(Bureaucrat &other);      // il form viene firmato
void Bureaucrat::signForm(Form &form);        // il burocrate prova a firmare
```

`beSigned()` fa il controllo e lancia se il grado non basta:

```cpp
if (other.getGrade() <= _grade)
    _isSigned = true;
else
    throw Form::GradeTooLowException();
```

`signForm()` chiama `beSigned()` dentro un `try` e stampa il risultato:

```text
Carlotta signed Modulo27
Carlotta couldn't sign Modulo27 because Grade too low.
```

Quindi l'eccezione nasce in `Form` e viene gestita in `Bureaucrat`.

## Dipendenza circolare

`Form` usa `Bureaucrat` e `Bureaucrat` usa `Form`. Se ognuno include l'header dell'altro, si crea un giro infinito di include.

Soluzione: la forward declaration.

```cpp
class Bureaucrat;
```

Dice al compilatore "questa classe esiste, i dettagli arrivano dopo". Basta per dichiarare reference e puntatori a `Bureaucrat`, senza includere tutto il suo header.

---

# Ex02 - No, you need form 28B, not 28C...

## Obiettivo

`Form` diventa `AForm`, una classe astratta, e nascono tre form concreti che fanno davvero qualcosa quando vengono eseguiti.

## AForm

```cpp
class AForm
{
    protected:
    virtual void executeAction() const = 0;

    public:
    virtual ~AForm() = 0;
    void execute(Bureaucrat const &executor) const;
};
```

L'idea è dividere il lavoro:

```text
controlli comuni -> AForm::execute()
azione specifica -> executeAction() nelle derivate
```

`execute()` è uguale per tutti i form:

```cpp
void AForm::execute(Bureaucrat const &executor) const
{
    if (!_isSigned)
        throw FormNotSigned();
    if (executor.getGrade() > _exec)
        throw GradeTooLowException();
    executeAction();
}
```

1. il form deve essere firmato
2. l'executor deve avere un grado sufficiente
3. solo allora parte l'azione vera

Così i controlli li scrivo una volta sola, e le derivate implementano solo la loro azione.

`executeAction()` è `protected`: le derivate la implementano, ma da fuori non si può chiamare direttamente saltando i controlli.

## Distruttore virtuale puro

```cpp
virtual ~AForm() = 0;
```

Rende la classe astratta anche se non avesse altre funzioni pure. Particolarità: va comunque implementato nel `.cpp`, perché il distruttore della base viene sempre chiamato quando si distrugge una derivata.

## Form concrete

### ShrubberyCreationForm

```text
grade sign: 145
grade exec: 137
```

Crea un file `<target>_shrubbery` con dentro un alberello ASCII.

### RobotomyRequestForm

```text
grade sign: 72
grade exec: 45
```

Stampa rumori di trapano, poi il target viene robotomizzato con successo il 50% delle volte:

```cpp
if (rand() % 2 == 0)
```

`rand()` da solo dà sempre la stessa sequenza a ogni esecuzione. Per avere davvero un 50/50 serve chiamare una volta, all'inizio del main:

```cpp
srand(time(NULL));
```

che usa l'ora attuale come seme.

### PresidentialPardonForm

```text
grade sign: 25
grade exec: 5
```

Stampa che il target è stato perdonato da Zaphod Beeblebrox.

## executeForm()

Come `signForm()`, ma per l'esecuzione:

```cpp
void Bureaucrat::executeForm(AForm const & form) const;
```

Chiama `form.execute(*this)` in un `try` e stampa se ce l'ha fatta o perché no.

---

# Ex03 - At least this beats coffee-making

## Obiettivo

Aggiungere `Intern`, che riceve il nome di un form e un target e crea il form giusto.

```cpp
AForm *Intern::makeForm(std::string name, std::string target);
```

È una factory: una funzione che crea oggetti di tipi diversi in base a un parametro, e li restituisce tutti come puntatore alla base.

## Niente if/else infiniti

Il subject chiede di evitare la foresta di `if/else if/else`. Uso due array paralleli: i nomi e le funzioni che creano i form.

```cpp
std::string forms[3] = {
    "robotomy request",
    "shrubbery creation",
    "presidential pardon",
};
AForm *(*creators[3])(std::string) = {
    createRobotomy,
    createShrubbery,
    createPresidential,
};
```

`creators` è un array di 3 puntatori a funzione che prendono una `std::string` e ritornano un `AForm *`. Stessa idea di Harl in CPP01, ma con funzioni normali invece che metodi.

Poi:

```cpp
for (int i = 0; i < 3; i++)
{
    if (name == forms[i])
        return creators[i](target);
}
```

Se il nome non esiste, stampa un messaggio e lancia `UnknownException`.

## Memoria

`makeForm()` restituisce un puntatore creato con `new`.

Chi lo riceve deve fare `delete`. Ed è facile dimenticarlo nei test, soprattutto quando c'è un'eccezione in mezzo.

---

# Checklist finale modulo

## Concetti da sapere spiegare

- come funzionano `try`, `throw`, `catch`
- perché i costruttori segnalano gli errori con le eccezioni
- perché si cattura per reference
- come si crea un'eccezione custom e cosa vuol dire `throw()` dopo `what()`
- perché grado 1 è il più alto
- perché `operator=` non può copiare i membri `const`
- cos'è una forward declaration e quando serve
- differenza tra `beSigned()` e `signForm()`
- perché `execute()` sta nella base e `executeAction()` nelle derivate
- cos'è un distruttore virtuale puro e perché va comunque implementato
- cos'è una factory

## Errori tipici da evitare

- incrementare il grado aumentando il numero
- modificare il grado e poi controllare i limiti
- dimenticare di validare i gradi anche nel costruttore di `Form`
- catturare l'eccezione per valore
- eseguire un form senza controllare che sia firmato
- usare `rand()` senza `srand()`: robotomy sempre con lo stesso risultato
- usare una catena di `if/else` in `Intern`
- dimenticare il `delete` sui form creati da `Intern`

## Mini riassunto mentale

```text
ex00 -> Bureaucrat, grado 1..150, eccezioni nel costruttore
ex01 -> Form firmabile, beSigned lancia, signForm cattura
ex02 -> AForm astratta: controlli in execute(), azione in executeAction()
ex03 -> Intern = factory con array di nomi + array di puntatori a funzione
```
