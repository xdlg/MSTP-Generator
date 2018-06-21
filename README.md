BlindQuarter - Multi-Scale Turing Patterns
===

Description
---

There are many visual patterns that occur naturally: zebra stripes, leopard spots, tribal tattoos... Ah well, maybe not the last one. Anyway, it turns out that some of these patterns can be described mathematically and computer-simulated. Starting with a grid of pixels of random values, and applying a simple set of rules over and over again, you see unexpected patterns emerge. Maybe you know Conway's game of life, that's the same kind of idea. Right now we're interested in Turing patterns, or reaction-diffusion patterns, which have been first described by Alan Turing.

History
---

Jonathan McCabe published an [algorithm](http://www.jonathanmccabe.com/Cyclic_Symmetric_Multi-Scale_Turing_Patterns.pdf) to generate "Cyclic Symmetric Multi-Scale Turing Patterns". Multi-scale because the patterns exist and evolve at different levels and at different timesteps (e.g. small wobbly mazes wobbling along with big wobbly mazes), and cyclic-symmetric because there's some circular averaging involved in the process, which gives a kind of mandala feeling to the generated pictures.

His published [pictures](https://www.flickr.com/photos/jonathanmccabe/albums/72157620365106838) and the [videos](https://www.youtube.com/watch?v=ihXZ2d2_b3U) that can be created with his algorithm are amazing. Think of a Windows Media Player visualization who quit her job and started doing LSD and listening to aggrotech. McCabe was nice enough to give lots of additional details about his algorithm to Jason Rampe, who published them in [this blog post](https://softologyblog.wordpress.com/2011/07/05/multi-scale-turing-patterns/). Originally, I became aware of McCabe's work thanks to [that post](http://www.wblut.com/2011/07/13/mccabeism-turning-noise-into-a-thing-of-beauty/) by Frederik Vanhoutte.

Some developers have already put their implementation online, see for instance [this](https://www.wblut.com/constructs/McCabeism/) and [that](http://www.openprocessing.org/sketch/31195). I wanted to write my own so I can really understand the algorithm and play with it to discover different kinds of patterns (hopefully). At first I tried in Python, because I'm learning it so I thought it would be a good exercise. It was, but the generation was also really, really slow. Then I worked with C because that's the language I know best. At that time, the program generated images using the [gifsave89 library](http://www.forkosh.com/gifsave89.html).

Because I wasn't satisfied with generating GIFs, I switched to [SDL](https://www.libsdl.org/) for displaying the generated images and adapted the rest of the program to C++. I don't have a lot of experience with C++ and I mostly learn as I go, so the code is really C with some C++ features that were useful to me.

Documentation
---

The documentation is generated with Doxygen. To understand the main algorithm, **everything** that you need to know is explained in the abovementioned [blog post](https://softologyblog.wordpress.com/2011/07/05/multi-scale-turing-patterns/), although I combined some generation steps to streamline the code a bit.

Make & run
---

You need SDL to compile, and you might have to adapt the Makefile to your installation path (values SDL_LIB and SDL_INCLUDE). After `make all`, `make run` starts the generation with standard parameters. The command line arguments are the image width, height, and the colormap. Just run the executable without arguments to show usage info. The other pattern parameters are hard-coded for the moment.

Once the SDL window is open, the generation starts with a single pattern. The numpad keys '+' and '-' add and remove a pattern, respectively. Clicking in the window resets the image.

![](https://github.com/xdlg/Muscatupa/blob/master/img/sample2.png)
