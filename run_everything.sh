#!/bin/bash

CC="g++"
CARGS="-O3 -pthread"

if [ ! -e word_graph ]
then 
	$CC $CARGS -o word_graph word_graph.cpp
fi

if [ ! -e five_clique ]
then
	$CC $CARGS -o five_clique five_clique.cpp
fi

gawk -f words_prep.awk words_alpha.txt | ./word_graph | ./five_clique
