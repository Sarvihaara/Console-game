Console game in C++ inspired by Match3. To try out the game, run `match.exe`.

# Rules

Each instance of the game randomly generates a block of bricks of different colours. The goal is to eliminate all possible matches.
A match is any 3 bricks of the same colour, either in horizontal or vertical line.

The player controls a grey cursor and can move it in any direction by typing a direction or a combination of directions from WASD (w - up, s - down, a - left, d - right) and Enter. 
For example, typing `dddss` and hitting Enter will move the white cursor to a position which is three bricks right and two bricks down from the current position.

The player can type `o` + Enter to select the brick the cursor is currently at. The selected brick will be coloured white. If the cursor is then moved to a brick which is one brick up, left, right or down from the
selected brick, typing `o` + Enter will replace the brick the cursor is currently at with the selected brick. This replacement only works if the second selected brick is one non-diagonal position away from the first selected brick, and will not be performed for any brick further than that.

Moving throughout the block and switching the bricks this way, the player aims to arrange matches of three bricks of same colour either horizontally or vertically. When three such bricks are so arranged, the match is eliminated and the bricks are removed from the block.

The game is over when there are no possible matches left, at which point the player can start a new game (`d` for a new game, `n` to quit the game). The player can quit the game at any time with `q` + Enter.

As displayed in the demo, for faster gameplay it is possible to combine directions and `o` to move the cursor to desired positions, select the desired two bricks and switch them at the same time, all in one command. For example, `ddowo` moves the cursor two bricks right, selects the first brick, moves the cursor one brick up, and selects the second brick, switching it with the brick below.

# Demo

![Demo](./demo.gif)
