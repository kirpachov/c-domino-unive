# Sviluppo
Di seguito idee iniziali sullo sviluppo.

## Layout
Bisogna permettere all'utente di visualizzare:
- Tavolo da gioco con tutti i domino che vi sono stati messi
- Punteggio attuale (in alto a dx)
- Domino che il giocatore ha in mano:
  - In grigio quelli che non si possono mettere.
  - In bianco quelli che si possono mettere invece.
  - In verde la mossa consigliata.

## Log
Dovrà essere presente un sistema di log.
Un file `game.log` verrà scritto mentre si gioca.

I log dovranno chiaramente avere un livello di logging,
che può essere impostato da configurazione.

Ciò permetterà:
- Scrivere codice "commentato", dove i commenti saranno i log stessi
- Debug semplificato nel momento in cui vi sono presenti bug