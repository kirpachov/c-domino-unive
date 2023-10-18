# Linear domino
by Oleksandr Kirpachov

Hi! I'm Oleksandr Kirpachov and this is my first
project at Ca' Foscari university of Venice.

This project is about a Linear domino game.

Project is included in [Consegna.pdf](Consegna.pdf). Sorry, it's in Italian.

Basicly it's a solo version of Domino.

Sortly, the rules are:
- You're assigned `x` number of dominoes.
- Initially, there are no dominoes on the table.
- Player can put dominoes down as long as the domino he's putting down has
at least one number that equals to one of the two numbers in the previous domino.
- When the user can't anymore put dominoes down, the game eds.
- The score the user has is made by the sum of all the numbers have been put on the table.

## How to play
First, compile the mail file with:
```bash
gcc -O2 -std=c99 --pedantic *.c -o iap
```

Then, execute it with:
```bash
./iap [--challenge]
```
<small>--challange param will basicly make the game play by itself.</small>

## Development
For the development ideas and `TODOs`, please refer to [DEVELOPMENT.md](DEVELOPMENT.md).

## Credits
