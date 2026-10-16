# 42_CPP

I moduli C++ del cursus di 42, da CPP00 a CPP09, con i miei appunti di studio.

Ogni modulo ha il suo README: non è la soluzione spiegata riga per riga, ma quello che mi è servito per capire i concetti, le scelte che ho fatto nel codice e gli errori che ho fatto (o che stavo per fare).

---

## Moduli

| Modulo | Argomento | Esercizi |
| ------ | --------- | -------- |
| [CPP00](CPP00/README.md) | Stream, `std::string`, prime classi | megaphone, PhoneBook |
| [CPP01](CPP01/README.md) | Memoria, puntatori, reference, file stream | Zombie, Weapon, sed, Harl |
| [CPP02](CPP02/README.md) | Orthodox Canonical Form, fixed-point, overload degli operatori | Fixed, BSP |
| [CPP03](CPP03/README.md) | Ereditarietà, ereditarietà multipla e virtuale | ClapTrap, ScavTrap, FragTrap, DiamondTrap |
| [CPP04](CPP04/README.md) | Polimorfismo, classi astratte, deep copy | Animal, Brain |
| [CPP05](CPP05/README.md) | Eccezioni | Bureaucrat, Form, Intern |
| [CPP06](CPP06/README.md) | Cast del C++ | ScalarConverter, Serializer, identify |
| [CPP07](CPP07/README.md) | Template | whatever, iter, Array |
| [CPP08](CPP08/README.md) | Container STL, iteratori, algoritmi | easyfind, Span, MutantStack |
| [CPP09](CPP09/README.md) | STL applicata, complessità, Ford-Johnson | Bitcoin Exchange, RPN, PmergeMe |

Note:

- CPP04 ex03 (Materia) era opzionale e non l'ho svolto (tempus fugit).

---

## In breve

```text
CPP00-01  dal C al C++: classi, stack/heap, reference
CPP02     la forma canonica, che da qui in poi è obbligatoria
CPP03-04  ereditarietà e polimorfismo
CPP05     eccezioni
CPP06     cast
CPP07     template
CPP08-09  STL: container, iteratori, algoritmi
```

Ogni modulo si appoggia su quelli prima. Il `Brain` di CPP04 è la stessa deep copy dell'`Array` di CPP07, l'end pointer di `strtod` torna in CPP06 e CPP09, i puntatori a funzione di Harl (CPP01) tornano nell'Intern di CPP05.

---

## Come sono fatti i README

Più o meno tutti seguono lo stesso schema:

1. **concetti del modulo**: la teoria che serve prima di aprire gli esercizi
2. **un capitolo per esercizio**: obiettivo, scelte nel codice, error handling
3. **checklist finale**: concetti da saper spiegare, errori tipici, mini riassunto

La checklist è la parte da rileggere prima della correzione.

---

## Compilare ed eseguire

Regole del subject: C++98, compilato con `c++ -Wall -Wextra -Werror -std=c++98`. Niente STL fino a CPP08.

Ogni esercizio ha il suo `Makefile`:

```bash
cd CPP09/ex02
make
./PmergeMe 3 5 9 7 4
```

Regole disponibili: `make`, `make clean`, `make fclean`, `make re`.

Alcuni esercizi vogliono argomenti o file:

```bash
./megaphone "shhhhh... I think the students are asleep..."   # CPP00/ex00
./sed file.txt cerca sostituisci                             # CPP01/ex04
./convert 42.0f                                              # CPP06/ex00
./btc input.txt                                              # CPP09/ex00, da lanciare nella cartella con data.csv
./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"                            # CPP09/ex01
```
