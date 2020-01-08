// ****************************************************************************
// * Author:      Hikawa Rin
// * Create Time: 2020-1-7 15:30
// * Last Change: 2020-1-8 14:46
// * Description: complete implement code of population.
// * 
// ****************************************************************************
// This file implement the simple generation algorithm.
// See the header comment on "sga.hpp" for additional information.
#ifndef SGA_SGA_CPP
#define SGA_SGA_CPP

#include "sga.hpp"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <ctime>

#define Elite
// gain seed for random device
std::random_device rd;                  
// use rd() to constructe mersenne_twister_engine
std::mt19937 gen(rd());                  

// Init population
//
// After constract public parameter, it must set the range of random number 
// generator and compute scale value. At now all public parameteres are 
// initialed. 
//
// And all first generation individuales also be constracted at here. Use 
// random number constract genes, it`s ok there may have some individual 
// with same genes. 
//
// This function make a guarantee that all member are initialized.
cPopulation::cPopulation(
    int popsize, 
    svector<svector<double> > constrain, 
    double crossover_rate,
    double mutation_rate,
    int scale_rate,
    IComputeAble* solver)
:popsize(popsize), 
 constrain(constrain),
 crossover_rate(crossover_rate),
 mutation_rate(mutation_rate)
{
    // Set optimization problem solver
    this->SetSolver(solver);

    // Init random number generator
    std::uniform_real_distribution<double> Dis(0.0, 1.0);
    this->dis = Dis;

    // Compute the scale number
    this->scale = std::pow(10.0, scale_rate);

    // Compute bits size
    svector<int> bits;
    for (auto &con : constrain){
        double low = con[0], high = con[1];
        low  = (low  < 0)? -low : low;
        high = (high < 0)? -high: high;
        double range = (low < high)? high: low;
        range = range * scale;
        
        int bit = 2;
        while (range > 1){
            bit++;
            range /= 2;
        }

        bits.push_back(bit);
    } // for (auto con : constrain)

    // Init individual
    for (int i = 0; i < this->popsize; i++){
        sIndividual ind;

        for (int i = 0; i < bits.size(); i++){
            svector<int> v;
            ind.genes.push_back(v);
            // random init gene serials
            for (int j = 0; j < bits[i]; j++){
                if (this->dis(gen) > 0.5){
                    ind.genes[i].push_back(1);
                }else{
                    ind.genes[i].push_back(0);
                }
            }
        } // for (int i = 0; i < bits.size(); i++)
        ind.objective = 0.0;
        ind.constrain_violence = 0.0;
        
        this->inds.push_back(ind);
    } // for (int i = 0; i < this->popsize; i++)

    // Decode gene
    this->_decode();

    // Compute objective
    for (int i = 0; i < this->inds.size(); i++) {
        this->solver->Compute(this->inds[i]);
    }
} // cPopulation::cPopulation

// Compete parameter value with the formula below:
//
//  Positive: $ if gene[0]==0: value = \sum_{i}^{n} 2^{n-i}*gene[i] $
//  Negative: $ if gene[0]==1: value = -(1 + \sum_{i}^{n} 2^{n-i}*(!gene[i])) $
//  value = value / scale
//
// every individual must be decode at the very beginning of each generation
// return 0 or 1
// - 0: decode success
// - 1: decode failure
int cPopulation::_decode(){
    for (int ind = 0; ind < this->inds.size(); ind++) {
        this->inds[ind].values.clear();
        for (int i = 0; i < this->inds[ind].genes.size(); i++){
            double value = 0.0;

            if (this->inds[ind].genes[i][0] == 0){
                // Is positive number
                for (int j = 1; j < this->inds[ind].genes[i].size(); j++){
                    value = value * 2 + this->inds[ind].genes[i][j];
                }
            }else{
                // Is negative number
                for (int j = 1; j < this->inds[ind].genes[i].size(); j++){
                    int g = (this->inds[ind].genes[i][j] == 0)? 1: 0;
                    value = value * 2 + g;
                }
                value = -value - 1;
            }
            value = value / this->scale;
            // lower than low constrain
            value = (value < this->constrain[i][0])?this->constrain[i][0]:value;
            // higher than high constrain
            value = (value > this->constrain[i][1])?this->constrain[i][1]:value;
            this->inds[ind].values.push_back(value);
        } // for (int i = 0; i < ind.genes.size(); i++)
    } // for (auto ind : this->inds)

    return 0;
} // int cPopulation::_decode()

// Set objective solver for optimization problem
void cPopulation::SetSolver(IComputeAble* solver){
    this->solver = solver;
} // void cPopulation::SetSolver(IComputeAble* solver)

// Order individual by their objective number 
//
// return 0 or 1
// - 0: select success
// - 1: select failure
int cPopulation::_select(){
    // must strict compare and compare(x, x) should be false
    std::sort(this->inds.begin(), this->inds.end(), Compare);
    return 0;
} // int cPopulation::_select()

// Random chose 
int cPopulation::_crossover(){
    // generate index array
    svector<int> index;
    for (int j = 0; j < popsize; j++){
        index.push_back(j);
    }
    // random change individual
    // This process now only change the index array, do nothing to inds
    for (int j = 0; j < popsize / 2; j++){
        int random = dis(gen);
        index[j] = random;
        index[random] = j;
    }

    svector<sIndividual> childinds;
    // Fetch two individual generate new inds
    for (int i = 0; i < popsize / 2; i++){
        sIndividual a = this->inds[index[2 * i]];
        sIndividual b = this->inds[index[2 * i + 1]];
        // for every gene serial judge if crossover
        for (int j = 0; j < a.genes.size(); j++){
            if (dis(gen) < this->crossover_rate){
                // random chose crossover point
                int crosspoint = dis(gen) * (a.genes[j].size() - 1);
                for (; crosspoint < a.genes[j].size(); crosspoint++){
                    int temp = a.genes[j][crosspoint];
                    a.genes[j][crosspoint] = b.genes[j][crosspoint];
                    b.genes[j][crosspoint] = temp;
                }        
            }
        }
        
        childinds.push_back(a);
        childinds.push_back(b);
    }

    #ifdef Elite
        int elit = this->popsize / 4;
        for (int i = 0; i < elit; i++) {
            childinds.pop_back();
        }
        for (int i = 0; i < elit; i++) {
            childinds.push_back(this->inds[i]);
        }
    #endif

    this->inds = childinds;
    return 0;
} // int cPopulation::_crossover()

// random mutate som bit in genes
//
// return 0 or 1
// - 0: mutate success
// - 1: mutate failure
int cPopulation::_mutate(){
    for (auto &ind : this->inds){
        for (auto &gene : ind.genes){
            if (dis(gen) < this->mutation_rate){
                int mutatepoint = dis(gen) * (gene.size() - 1);
                gene[mutatepoint] = (gene[mutatepoint] == 0)? 1: 0;
            }
        }
    }

    return 0;
} // int cPopulation::_mutate()

// Evolution population one generation
//
// return 0 or 1
// - 0: evolution success
// - 1: evolution failure
int cPopulation::Evolution(){
    if (this->_select()){
        FLog("select error");
        exit(1);
    }
    if (this->_crossover()){
        FLog("crossover error");
        exit(1);
    }
    if (this->_mutate()){
        FLog("mutate error");
        exit(1);
    }
    
    this->_decode();
    for (auto &ind : this->inds){
        this->solver->Compute(ind);
    }

    return 0;
} // int cPopulation::Evolution()

// Report the inds situation
std::vector<sIndividual> cPopulation::Report(){
    return inds;
} // std::vector<sIndividual> cPopulation::Report()

// This function defined the rule of compare two individual.
//
// For handle min problem this function will return two bool value
// - True  if a.objective <= b.objective
// - False if a.objective >  b.objective
bool Compare(sIndividual a, sIndividual b){
    if (a.objective < b.objective){
        if (a.constrain_violence > b.constrain_violence){
            return false;
        }
        return true;
    }else{
        if (b.constrain_violence > a.constrain_violence) {
            return true;
        }
        return false;
    }
} // bool Compare(sIndividual a, sIndividual b)

// log information in log[time].txt
void FLog(std::string s){
    std::time_t time = std::time(nullptr);
    std::string t = std::ctime(&time);
    std::ofstream file;
    std::string path = "log" + t + ".txt";
    file.open(path, std::ios::out | std::ios::trunc);
    if (file.is_open()){
        file << s << std::endl;
    }
    file.close();
} // void Log(std::string s)
#endif // SGA_SGA_CPP