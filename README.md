# MGPDictionary
Progetto didattico, sviluppato per il corso di MGP -Modular and Generic Programming- dei professori Davide Ancona e Giovanni Lagorio durante l'a.a. 2013/2014 presso la facoltà di Informatica - DIBRIS - UNIGE.
### Obbiettivi del progetto
L'obiettivo del progetto è l'approfondimento delle meccaniche del C++, con particolare attenzione alla Standard Template Library e alla libreria di Boost -versione 1.55-, per l'implementazione di degli iteratori e l'uso dei concept.
#### L'interfaccia
Nonostante all'interno della libreria sviluppata sia assente un'interfaccia comune per le diverse implementazione dei dizionari disponibile, sono individuabili le tre operazioni standard: *insert*, *search* e *delete*.

L'operazione di insert, descritta qui dal prototipo di funzione comune a tutte le implementazioni bool insert(const Key, const Value&, Value* = nullptr);, permette l'inserimento di nuovi elementi all'interno del dizionario. Inoltre, nel caso in cui la chiave fornita sia identica ad una presente nel dizionario il valore associato viene aggiornato.
Come nelle librerie dei più diffusi linguaggi di programmazione, l'operazione di insert fornisce anche le funzionalità di una ipotetica funzione *update*. Sfruttando il valore di ritorno booleano ed il terzo parametro opzionale, è possibile conservare i valori sovrascritti durante l'aggiornamento del dizionario. Ovviamente, l'area di memoria puntata dal terzo parametro deve essere allocata e scrivibile.
L'operazione di inserimento restituisce *true* se l'area di memoria puntata dal terzo parametro si riferisce al valore presente nel dizionario prima dell'aggiornamento, *false* altrimenti.

L'operazione di rimozione di un elemento è descritta dal seguente prototipo: bool del(const Key, Value* = nullptr);. Per rimuovere un elemento presente all'interno del dizionario, è necessario indicare la chiave ad esso associata. Come per l'operazione di insert, sono previste altre funzionalità opzionali per questa operazione: in particolare l'utente può ottenere il valore associato alla chiave prima della rimozione, sfruttando il secondo parametro opzionale. 
L'operazione di rimozione, nuovamente, restituisce *true* se l'area di memoria puntata dal secondo parametro si riferisce al valore presente nel dizionario prima della rimozione, *false* altrimenti.

L'ultima primitiva fondamentale per il dizionario è la ricerca, qui descritta dal protipo: iterator search(const Key);. Data una chiave, è restituito un iteratore: nel caso in cui un elemento sia associato con la chiave fornita sia presente nel dizionario, l'iteratore farà riferimento alla coppia *<Key, Value>*; altrimenti il puntatore coinciderà con il terminatore *end()*.
#### Comparazione con le interfacce in {Java, C#, C++, Python}


### Close Addressing
#### Metodi di Hashing
### Open Addressing
#### Metodi di Probing (...e di Hashing)
### Le feature
### I tipi
### Gli iteratori e Boost
### Le feature
### Gli iteratori e Boost
### Metodi di probing
### I concept di Boost
### GTest
### Valgring
