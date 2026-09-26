# LUDO-CS

A C simulation of *LUDO-CS* — an extended variant of Ludo with blocking, mystery
cells, and colour-specific AI player behaviours. Built for SCS1301 (Data
Structures and Program Design using C), University of Colombo School of
Computing.

> **Status: Work in Progress** — core dice/movement/capture logic is implemented;
> blocking and the full mystery-cell system are not yet complete. See
> [Known Limitations](#known-limitations) below.

## Overview

The simulation runs with no user interaction — once started, four AI-controlled
players (Red, Yellow, Green, Blue) each follow a distinct strategy and the game
plays itself out to completion, printing status messages at each stage.

**Player strategies:**
- **Red** — aggressive; prioritizes capturing opponents over racing home
- **Yellow** — win-focused; only captures when needed to progress
- **Green** — blocking-focused; prioritizes forming and holding blocks
- **Blue** — moves pieces cyclically (B1→B2→B3→B4) and seeks out/avoids the
  mystery cell depending on direction of travel

## Project Structure

```
ludo-cs/
├── main.c    # Entry point — initializes players, runs the simulation loop
├── types.c   # Player and Piece structs
├── logic.c   # Game rules — dice, movement, captures, mystery cells, AI behaviour
```

### `types.c`
Defines two structures — `Player` and `Piece` — used instead of a
multi-dimensional array, since accessing a piece's properties via nested loops
would add unnecessary time complexity. Each player holds an array of 4 pieces.

### `logic.c`
Implements the core rules and per-colour AI behaviour:
- `rollDice()` — simulates the 6-sided die
- `initializePlayer()` — sets up a player's pieces, colour, and starting state
- `move()` — dispatches to the correct colour-specific move function
- `moveRed()`, `moveYellow()`, `moveGreen()`, `moveBlue()` — colour-specific
  turn logic (base→board, board movement, home straight entry)
- `attemptCapture()` / `capturePiece()` — capture detection and resolution
- `calculateDistancetoHome()` — used to pick the best piece to move/capture with
- `handleMysteryCell()` — applies mystery cell effects (teleport, energized,
  sick, skip rounds, return to base, direction change)
- `findPlayerIndexByColor()` — looks up a player by colour string

### `main.c`
Initializes all four players and drives the simulation loop, calling into
`logic.c` each round and printing status after every turn.

## Build & Run

```bash
gcc main.c types.c logic.c -o ludo
./ludo
```

## Known Limitations

- **Blocking is unimplemented.** Traditional Rule 7 and the extended blocking
  rules (CS-3 to CS-8 — forming blocks, moving them together, breaking them,
  blockade-vs-blockade capture) are not yet handled.
- **Mystery cell logic is simplified.** The spec (CS-11 to CS-15) calls for a
  piece landing on the mystery cell to teleport to one of six named locations
  (Bhawana, Kotuwa, Pita-Kotuwa, Base, X, Approach), each with its own
  follow-on effect. The current implementation moves the piece forward by a
  fixed distance for the teleport case rather than resolving all six
  destinations; the energized/sick/skip-rounds effects exist as separate cases
  but aren't yet tied to the specific named cells.
- **Rule CS-7** (a piece may only enter the home straight after capturing at
  least one opponent piece) is not implemented.
- **Blue player's cyclic ordering and direction-based mystery-cell targeting**
  need verification against the spec.
- Some functions are underdeveloped or commented out (e.g. an early version of
  the base→starting-point transition), and there's noticeable duplication
  across the four colour-specific move functions that could be generalized.

## Roadmap / TODO

- [ ] Implement blocking (formation, movement, breaking, blockade capture)
- [ ] Implement the full 6-destination mystery cell system
- [ ] Implement Rule CS-7 (capture-gated home entry)
- [ ] Verify/complete Blue player behaviour
- [ ] Reduce duplication across `moveRed`/`moveYellow`/`moveGreen`/`moveBlue`
- [ ] Match exact required output message formatting

## Design Notes

Structs were used over parallel arrays for readability and to avoid the added
time complexity of nested-loop access that a multi-dimensional array
representation would require. Player structs are generally passed by pointer
to avoid unnecessary copying.

## Notes

Developed as a take-home assignment. An accompanying report covers the design
process (including initial flowcharts for each player's behaviour), structure
justification, and a self-assessed discussion of the limitations above.
