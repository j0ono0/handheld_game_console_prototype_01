# Handheld game console and Arduino game

The motivation and goal of this project was primarily as an excuse to try and design a PCB. Choosing a handheld game console made an appealing form on several points:

- Has enough wiring to be an interesting, but achievable, as a first PCB design
- Makes a useful progression from a breadboard prototype
- Results in an artifact that has broad appeal and is fun showing off to tech, and non-tech, friends alike  

To fully realise success I needed a game to run on the hardware. To Achive this requirement I chose to appropriate the key game mechanic of Sokoban (designed in 1981 by Hiroyuki Imabayashi, and first published in December 1982 [[wikipedia](https://en.wikipedia.org/wiki/Sokoban)]).

## Hardware

Having a [Teensy 4](https://www.pjrc.com/store/teensy40.html) and [TFT screen](https://www.pjrc.com/store/display_ili9341_touch.html) on hand encouraged some initial design constraints which proved to be generous and made for fast firmware development without the need for much attention to optimisation.

The initial wiring was tested on a bread-board with microcontroller and screen communicating via SPI. Several input buttons were multiplexed, not due to lack of gpio pins, but simple to try out a technique new to me. At this point the hardware worked reliably but was somewhat unwieldly to press buttons and move around my desk whist developing the game. 

The screen module includes touch input and an SD card reader. In the spirit of making a fully-featured development device I decided to enable these additional components. Once connected on the bread-board the hardware ceased to work reliably. Testing individually each did work correctly making for a hopeful outlook. I decided to forge ahead and include these components after ponder failure/fix scenarios:

- Bread-board/wiring issue - Issue will be fixed with PCB wiring
- Faulty hardware - I can cut traces on the PCB effectively 'removing' components
- Firmware error - Not initialising components or changing code/libraries should resolve errors

With a somewhat working bread-board prototype I moved onto designing a PCB using [KiCad](https://www.kicad.org/) with fabrication done by PCBWay.  

Once assembled the reliability issues vanished. I assume controlling x3 SPI devices was pushing the capibilities of my bread-board. Power and portability was resolved with a small battery bank making for a fun prototype to show to others whilst out-and-about plus a convenient development device at my desk.


## Firmware

Goals for this section of the project were less defined than the hardware - Something fun to show off and achievable to create. I have a 'secret' ongoing project of creating a roguelike and would very much like it to run on a handheld console. With several attempts of this game genre for desktop under my belt - all of them failures of varying degrees - I decided to set some further constraints around what a 'minimum viable product' might look like. The result is my homage to Sokoban (see intro for details). The code is total overkill for the task at hand but with the thought it might be the ground work for a more complex roguelike game.

The project utilises the Arduino environment which streamlines access to libraries, interfacing with hardware, and compiling/uploading. Whilst Arduino is essentially C++, I've attempted to primarily write in C compatible code as practice for developing for more restricted hardware devices. 

In the course of processing graphics for embedding into into the game I wrote a hand-full of Python scripts. These are a work-in-progress as I experiment and streamline how I create graphical assets (sprites, level maps, and background tiles) and process them for embedding into the code.

At the time of writing this the game is fully functional with 3 levels. The user interface includes buttons to reset, skip levels, plus display a demo art screen. The latter as a talking point for where the graphics may go when showing off the project.
