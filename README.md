# Five words. Twenty five letters

## Premise

See `https://www.youtube.com/watch?v=_-AfhLQfb6w&t=1s`

## Based on

There's an awesome solution at `https://gitlab.com/bpaassen/five_clique`
This is just an optimization of that same aproach. The main speedup comes from
generating the graph with only edges that will be considered during traversal,
instead of skipping them repeatedly during traversal.
Oh, and by using C++ instead of python.

## Setup

Clone words\_alpha.txt from https://github.com/dwyl/english-words

I had to use `dos2unix words_alpha.txt` to convert line endings since I'm running linux.

## Running

You should be able just run the `run_everything.sh` bash script.
The first time you run it will compile the C++ programs. If you
run it again you'll be able to see how fast this runs without taking
into account C++ compile times.

It outputs the cliques to stdout. So I recommend running:

```
time ./run_everything.sh > cliques.txt
```

On my machine the timing is:

```
real	0m58.758s
user	2m40.111s
sys	0m0.174s
```

