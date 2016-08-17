Muscatupa - Multi-Scale Turing Patterns
---

There are many visual patterns that occur naturally: zebra stripes, leopard spots, tribal tattoos... Ah well, maybe not the last one. Anyway, it turns out that some of these patterns can be described mathematically and computer-simulated. Starting with a grid of pixels of random values, and applying a simple set of rules over and over again, you see unexpected patterns emerge. Maybe you know Conway's game of life, that's the same kind of idea. Right now we're interested in Turing patterns, or reaction-diffusion patterns, which have been first described by Alan Turing. We owe him a lot, don't we?

Jonathan McCabe published an [algorithm](http://www.jonathanmccabe.com/Cyclic_Symmetric_Multi-Scale_Turing_Patterns.pdf) to generate "Cyclic Symmetric Multi-Scale Turing Patterns". Multi-scale because the patterns exist and evolve at different levels and at different timesteps (e.g. small wobbly mazes wobbling along with big wobbly mazes), and cyclic-symmetric because there's some circular averaging involved in the process, which gives a kind of mandala feeling to the generated pictures.

His published [pictures](https://www.flickr.com/photos/jonathanmccabe/albums/72157620365106838) and the [videos](https://www.youtube.com/watch?v=ihXZ2d2_b3U) that can be created with his algorithm are amazing. Think of a Windows Media Player visualization who quit her job and started doing LSD and listening to aggrotech. McCabe was nice enough to give lots of additional details about his algorithm to Jason Rampe, who published them in [this blog post](https://softologyblog.wordpress.com/2011/07/05/multi-scale-turing-patterns/). Originally, I became aware of McCabe's work thanks to [that post](http://www.wblut.com/2011/07/13/mccabeism-turning-noise-into-a-thing-of-beauty/) by Frederik Vanhoutte.

Some developers have already put their implementation online, see for instance [this](http://www.wblut.com/constructs/McCabeism/) and [that](http://www.openprocessing.org/sketch/31195). I wanted to write my own so I can really understand the algorithm and play with it to discover different kinds of patterns (hopefully). At first I tried in Python, because I'm learning it so I thought it would be a good exercise. It was, but the generation was also really, really slow. Now I'm working with C because it's my favorite language and it's much faster for this application.

The source code is on [GitHub](https://github.com/xdlg/Muscatupa). It uses the [gifsave89 library](http://www.forkosh.com/gifsave89.html) to generate gifs, static or animated. I just took that one because the API is really simple, but I might pick another library later. The documentation is generated with Doxygen. To understand the algorithm, **everything** that you need to know is explained in the abovementioned [blog post](https://softologyblog.wordpress.com/2011/07/05/multi-scale-turing-patterns/).

As of today, the program runs and it's quite fast, so I got that going for me, which is nice. Generated animated gifs can get huge, though. Three issues that I'm already aware of, and that I'm going to work on:
1. When increasing the image size, the number of patterns, or the size of certain variables, there's always a moment when the program crashes with a "Segmentation fault". So I'm doing something wrong, maybe not freeing memory when I should.
2. The patterns seem to stabilize after a while, but maybe I just have to play with the patterns' parameters.
3. It works only with square images, perhaps that's a feature of the algorithm that I don't understand at the moment.

Here's a sample. Left to right, the pictures were generated with 1, 2 and 4 scales (i.e. Turing patterns evolving within the same picture).

![](https://github.com/xdlg/Muscatupa/blob/master/img/sample.gif)
