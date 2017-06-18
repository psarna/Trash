all: simulator

simulator: simulator.cc actor.h
	g++ -Wall -o simulator simulator.cc
