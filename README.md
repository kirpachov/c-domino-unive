# Linear domino
by Oleksandr Kirpachov

Hi! I'm Oleksandr Kirpachov and this is my first
project at Ca' Foscari university of Venice.

This project is about a Linear domino game.

Project is included in [Consegna.pdf](Consegna.pdf). Sorry, it's in Italian.

Basically it's a solo version of Domino.

Shortly, the rules are:
- You're assigned `x` number of dominoes.
- Initially, there are no dominoes on the table.
- Player can put dominoes down as long as the domino he's putting down has
at least one number that equals to one of the two numbers in the previous domino.
- When the user can't anymore put dominoes down, the game eds.
- The score the user has is made by the sum of all the numbers have been put on the table.

## Special dominoes
All specials can be placed on the empty table.
Their effect will depend on the first domino you put on them.

### [0|0]
Can be placed everywhere. E.g: 
```
[1|2][0|0][5|6]
```
A domino `[11|11]` <b>has</b> effect on this.

### [11|11]
Will add 1 to all the dominoes on the table in that moment. If it's 6, will become 1. E.g:
```
[1|6][6|3][11|11] => [2|1][1|4][4|4]
```
This domino has effect on all dominoes that are on the table at the moment it was placed.
So, if you place dominoes after it, it won't affect them.
```
1) [1|6][6|3]
2) [1|6][6|3][11|11] (added [11|11])
2) [2|1][1|4][4|4] (after [11|11] effect)
3) [2|1][1|4][4|4][4|5] (placed [4|5] after [11|11]. [4|5] wasn't affected by [11|11])
```

### [12|21]
Will copy mirror the closest domino. E.g:
```
[1|2][2|3][12|21] => [1|2][2|3][3|2]
```

## How to play
First, compile the mail file with:
```bash
gcc -O2 -std=c99 --pedantic *.c -o iap
```

Then, execute it with:
```bash
./iap [--challenge]
```

## Development
For the development ideas and `TODOs`, please refer to [DEVELOPMENT.md](DEVELOPMENT.md).

## Credits
