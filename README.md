Multi-Scale Turing Patterns Generator
===

[Turing patterns](https://en.wikipedia.org/wiki/Turing_pattern) are a mathematical model for natural patterns such as stripes and spots. The model explains how these patterns can emerge from random initial conditions thanks to the interaction of two substances, called activator and inhibitor in the following. [Jonathan McCabe](http://jonathanmccabe.com/) published an [article](http://www.jonathanmccabe.com/Cyclic_Symmetric_Multi-Scale_Turing_Patterns.pdf) describing an algorithm to generate more complex Turing patterns using several layers, or scales, of activator/inhibitor couples. Very useful implementation details were given later on [Softology's blog](https://softologyblog.wordpress.com/2011/07/05/multi-scale-turing-patterns/).

Conceptually, the idea is similar to that of Conway's [Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life): create an image with random pixel values, then repeatedly apply some simple rule on each pixel that changes its value based on the value of its neighbors. If the rule is chosen wisely, interesting and unexpected patterns emerge.

This project is a C++ implementation of a multi-scale Turing patterns generator based on McCabe's algorithm. As the algorithm requires updating each pixel individually, and this update gets more computationally expensive whith each additional scale, my goal is to make the implementation quite efficient. I am also more interested in generating videos than static images, which is why the project uses [libav/FFmpeg](https://libav.org/). Finally, I want to understand how the different parameters of the algorithm (e.g. activator, inhibitor, number of scales) influence the generated patterns, and explore the creative possibilities given by symmetries and color palettes.


![](https://github.com/xdlg/MSTP-Generator/blob/master/img/sample2.png)


# Examples

This section explores how the algorithm parameters influence the generated images.

## One scale

A pattern based on a single scale seems to always end up stabilizing. It reaches a stable form quicker if the increment is larger. In general, the increment can be seen as the evolution speed of a scale.

If the activator and inhibitor radii are large, the end pattern has wide shapes. With an activator radius smaller than the inhibitor radius, the shapes tend to look like mazes. Image size 854x480, activator radius 100, inhibitor radius 200:

![](https://github.com/xdlg/MSTP-Generator/blob/master/img/examples/example00.png)

Smaller radii give narrower, more detailed shapes. Image size 854x480, activator radius 25, inhibitor radius 50:

![](https://github.com/xdlg/MSTP-Generator/blob/master/img/examples/example01.png)

An activator radius larger than the inhibitor radius produces more regular patterns without mazes. Image size 854x480, activator radius 200, inhibitor radius 100:

![](https://github.com/xdlg/MSTP-Generator/blob/master/img/examples/example02.png)

Again, smaller radii generate narrower shapes. Image size 854x480, activator radius 50, inhibitor radius 25:

![](https://github.com/xdlg/MSTP-Generator/blob/master/img/examples/example03.png)

An activator radius much smaller than the inhibitor radius gives a sort of cow pattern. Image size 854x480, activator radius 10, inhibitor radius 200:

![](https://github.com/xdlg/MSTP-Generator/blob/master/img/examples/example04.png)

If the activator radius is much larger than the inhibitor radius, we get striped patterns with horizontal/vertical zones. Image size 854x480, activator radius 200, inhibitor radius 10:

![](https://github.com/xdlg/MSTP-Generator/blob/master/img/examples/example05.png)

Finally, if both radii are equal, they cancel each other out and the image does not evolve at all.
