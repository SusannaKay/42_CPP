# CPP05 - Eccezioni, Bureaucrat, Form

- exception
- classi annidate
- membri `const`
- overload di `operator<<`
- relazioni tra classi
- classi astratte
- funzioni virtuali pure
- factory semplice con `Intern`

---

# Concetti chiave

## Eccezioni

Lancio:

```cpp
throw SomeException();
```

Gestione:

```cpp
try
{
    // codice che puo' fallire
}
catch (std::exception& e)
{
    std::cout << e.what() << std::endl;
}
```

Nel modulo le eccezioni derivano da `std::exception` e ridefiniscono il metodo what():

```cpp
const char *what() const throw();
```


## I gradi

Punto controintuitivo:

```text
1   = grado piu' alto
150 = grado piu' basso
```

Quindi:
- incrementare il grado = diminuire il numero
- decrementare il grado = aumentare il numero

```text
grade 42 increment -> grade 41
grade 42 decrement -> grade 43
```

Eccezioni:
- sotto 1: `GradeTooHighException`
- sopra 150: `GradeTooLowException`

## const nei membri

`Bureaucrat`:

```cpp
std::string const _name;
```

`Form` / `AForm`:

```cpp
std::string const _name;
int const _grade;
int const _exec;
```

- valori fissati nel costruttore
- non riassegnabili dopo
- `operator=` non può copiare tutto se ci sono membri `const`

## Collaborazione tra classi

Un form non si firma da solo.

```cpp
form.beSigned(bureaucrat);
bureaucrat.signForm(form);
bureaucrat.executeForm(form);
```

Regola mentale:

```text
se grade burocrate <= grade richiesto -> ok
altrimenti exception
```

Ricordare: numero più piccolo = grado più alto.

---

# Ex00 - Mommy, when I grow up, I want to be a bureaucrat!

## Idea

Classe `Bureaucrat`.

```cpp
std::string const _name;
int _grade;
```

Il costruttore valida subito il grado:

```text
grade < 1   -> GradeTooHighException
grade > 150 -> GradeTooLowException
```

## increment / decrement

`increment()` migliora il grado:

```cpp
_grade--;
```

`decrement()` peggiora il grado:

```cpp
_grade++;
```

Nota: controllo dei limiti prima di modificare.

## operator<<

Overload:

```cpp
std::ostream &operator<<(std::ostream &out, Bureaucrat const &obj);
```

Uso:

```cpp
std::cout << bureaucrat << std::endl;
```

Output atteso:

```text
name, bureaucrat grade X.
```

---

# Ex01 - Form up, maggots!

## Idea

Aggiungere `Form`.

```cpp
std::string const _name;
int const _grade;
int const _exec;
bool _isSigned;
```

Campi:
- `_grade`: grado per firmare
- `_exec`: grado per eseguire
- `_isSigned`: stato della firma

Anche qui gradi validi: 1..150.

## beSigned()

```cpp
void beSigned(Bureaucrat &other);
```

Controllo:

```cpp
other.getGrade() <= _grade
```

Se ok: form firmato.
Se no: `GradeTooLowException`.

## Forward declaration

Nell'header:

```cpp
class Bureaucrat;
```

Nota: dice al compilatore che `Bureaucrat` esiste.
Utile per evitare include circolari tra `Bureaucrat.hpp` e `Form.hpp`.

---

# Ex02 - No, you need form 28B, not 28C...

## Idea

`Form` diventa `AForm`, classe astratta.

```cpp
virtual void executeAction() const = 0;
virtual ~AForm() = 0;
```

Idea:
- `AForm` contiene i controlli comuni
- le derivate contengono l'azione concreta

Funzione pubblica:

```cpp
void execute(Bureaucrat const &executor) const;
```

Controlli:
1. form firmato
2. executor con grado sufficiente
3. chiamata a `executeAction()`

Schema:

```text
controlli comuni -> AForm::execute()
azione specifica -> executeAction() nelle derivate
```

## Form concrete

### ShrubberyCreationForm

```text
grade sign: 145
grade exec: 137
```

Crea file:

```text
<target>_shrubbery
```

Dentro: alberello ASCII.

### RobotomyRequestForm

```text
grade sign: 72
grade exec: 45
```

Stampa rumori tipo trapano.
Poi successo/fallimento circa 50/50 (`rand()`).

### PresidentialPardonForm

```text
grade sign: 25
grade exec: 5
```

Stampa che il target è stato perdonato da Zaphod Beeblebrox.

## FormNotSigned

`AForm::execute()` controlla:

```cpp
if (!_isSigned)
    throw FormNotSigned();
```

Nota: grado altissimo non basta. Prima serve la firma.

---

# Ex03 - At least this beats coffee-making

## Idea

Aggiungere `Intern`: piccola factory di form.

```cpp
AForm *Intern::makeForm(std::string name, std::string target);
```

Nel codice:

```cpp
std::string forms[3]
AForm *(*creators[3])(std::string)
```

Nomi riconosciuti:

```text
robotomy request
shrubbery creation
presidential pardon
```

Schema:

```text
"robotomy request"      -> new RobotomyRequestForm(target)
"shrubbery creation"    -> new ShrubberyCreationForm(target)
"presidential pardon"   -> new PresidentialPardonForm(target)
```

Se il nome non esiste: messaggio + `UnknownException`.

## Memoria

`makeForm()` restituisce un puntatore creato con `new`.
Chi lo riceve deve fare `delete`.

Nota: facile dimenticarlo nei test.
