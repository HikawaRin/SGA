// ****************************************************************************
// * Author:      Hikawa Rin
// * Create Time: 2020-1-7 12:00
// * Last Change: 2020-1-8 10:38
// * Description: add Report public method to population to export inds.
// * 
// ****************************************************************************
// This file define the simple generation algorithm.
// The simple generation algorithm can used to handle optimization problems
// , which has only ONE optimazation object.
//
// The basic idea is encode every real number parameter of optimization problem 
// into a binary gene serial as follows:
//
//   [0, 1, 0, 1, 1, 0, ...]
//
// Due to the simple idea that a binary serial can present a positive number in 
// [0, 2^n] or a real number in [-2^(n-1), 2^(n-1)-1], so the length(bits) of 
// every parameter can be decide by the range of parameter. To present double 
// value we can large by 10^n and tanslate it in decode().
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
// constrain_violence to every individual, which violence constrain will be 
// punished in select step to avoid select them first. 
#ifndef SGA_SGA_HPP
#define SGA_SGA_HPP

#include <random>
#include <vector>
#include <string>
#define svector std::vector

// An instance of this struct represent a set of parameters of optimization
// problem.
//
// The values will be filled by decode() function based on genes, which defined 
// in population class. The objective value and constrain_violence(if has) 
// should be compute before select.
//
// constrain_violence present if the solution violence the constrain and how it 
// violented. It can be compute by $\sum_{i}^{n}(values[i]-constrain[i])$. 
// Because the constrain of a optimization problem is fixed, it will be stroed 
// in population class.
struct sIndividual
{
    svector<double> values;              // the value of parameters
    svector<svector<int> > genes;        // the genes of individual
    double objective;                       // the objective value
    int constrain_violence;              // all violence add up
};

// An interface for compute the objective value of individual.
//
// this interface only contain one method Compute(individual), this method will 
// compute the objective value of the individual and write to 
// individual.objective
//
// The return value of Compute method should be 0 or 1:
//  - 0: Compute objective value success
//  - 1: something wrong occur
class IComputeAble
{
public:
    virtual int Compute(sIndividual &ind)=0;
};

// An instance of this class represent a enviroment of evolution, which means
// ALL indicidual will be managed by this class and the process of evolution 
// will occur in there.
//
// scale present the precision of value, it will used in _decode() when 
// compute parameter value.
//
// call ```double num = dis(gen);``` can get a random number in [0.0, 1.0]
class cPopulation
{
public:
    int popsize;                         // the number of individuals
    svector<svector<double> > constrain; // the constrain of each parameter
    double crossover_rate;               // the percent of occur crossover
    double mutation_rate;                // the percent of occur mutate
    double scale;                        // the scale value

    cPopulation(
        int popsize, 
        svector<svector<double> > constrain,
        double crossover_rate,
        double mutation_rate,
        int scale_rate,
        IComputeAble* solver);
    int Evolution();
    std::vector<sIndividual> Report();
    void SetSolver(IComputeAble* solver);
private:
    std::vector<sIndividual> inds;             // all individuals
    IComputeAble* solver;                      // objective solver
    std::uniform_real_distribution<double> dis;// gain random number

    int _decode();
    int _select();
    int _crossover();
    int _mutate();
};

bool Compare(sIndividual a, sIndividual b);
void FLog(std::string s);

#endif // SGA_SGA_HPP