# Quantum Entanglement: Retro Edition
## A retro version of the original game "Quantum Entanglement" (PyWeek33 by Team MYCTL) made in C/SDL

This project is intended to be a "retro" re-make of the Team MYCTL entry for PyWeek 33 "Quantum Entanglement" (https://pyweek.org/e/pw33_colab/). The project is in C and uses SDL/SDL Image/SDL Mixer (https://www.libsdl.org/) for graphics and sound. Compared to the original game, the playspace is simplifed to an 8x8 grid and bother player squares must reach the goal square in their respective maps.

The aim of this re-make is to explore the puzzle concept in a bit more detail: The current implementation includes an automatic level-solver, and in future work I'd like to explore how to include proceedurally-generated puzzles (which are actually fun :) ) ... I'm also thinking about implementing a dedicated hand-held version of the game, similar to a Connect-4 game I've previously made ()

## Building:
Download the source code and run "make". Dependancies:
SDL (https://www.libsdl.org/)
SDL_Image 2.0.X (https://www.libsdl.org/projects/SDL_image/)
SDL_Mixer 2.0.X (https://www.libsdl.org/projects/SDL_mixer/)

## Credits:
Original "Quantum Entanglment" game by Team MYCTL, made in seven days in Python for PyWeek 33 (March 2022) (https://pyweek.org/e/pw33_colab/)
Artwork and music by mit-mit.