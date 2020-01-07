// ****************************************************************************
// * Author:      Hikawa Rin
// * Create Time: 2020-1-7 12:00
// * Last Change: 2020-1-7 13:02
// * Description: add description of the simple generation algorithm.
// * 
// ****************************************************************************
// This file implement the simple generation algorithm.
// The simple generation algorithm can used to handle optimization problems
// , which has only ONE optimazation object.
//
// The basic idea is encode every real number parameter of optimization problem 
// into a binary gene serial as follows:
//
//   [0, 1, 0, 1, 1, 0, ...]
//
// Due to the simple idea that a binary serial can present a positive number in 
// [0, 2^n] or a real number in [-2^(n-1), 2^(n-1)], so the length(bits) of 
// every parameter can be decide by the range of parameter.
//
// All real number parameter make up a two-dimension seriales like follows: 
//
//   [[0, 1, 0, ...],
//    [1, 0, ..],
//    [...]]
//
// the first coordinate of seriales present the index of parameters. So every 
// two-dimension serials present a complete set of input parameter for 
// optimization problem. In the generation algorithm called it a individual, 
// usually the algorithm obtain many individual to achieve the optimization 
// solution and call all individual as a population(group). 
//
// The simple generation algorithm use three step: Select, Crossover, Mutation 
// to change every individual`s gene.
// -Select:    order all individual by their object value, select better 
//          individual give up(kill) bad indvidual.
// -CrossOver: after select, chose two individual random chose cross point 
//          exchange genes after cross point to make up two new individual to 
//          fill up the individuals killed in select, keep the size of 
//          population not change.
// -Mutation:  for every individual randomed mutate some bit of gene. 
//          Such as 0->1 | 1->0.
//
// Elite strategy means select but not crossover and mutation the best 
// individual in every generation, which is a simple way to improve converge 
// speed in ALL generation algorithm.
//
// Generation algorithm can handle constrain optimization by add 
// constarin_violence to every individual, individual, which violence 
// constarin will be punished in select step to avoid select them first. 
#ifndef SGA_SGA_HPP
#define SGA_SGA_HPP

#endif // SGA_SGA_HPP