#!/bin/sh 

for i in {1..100}; do
	echo run $i  
	./plague.bin < coronavirusStats.txt >> log.txt 
done  
