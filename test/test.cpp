// ****************************************************************************
// * Author:      Hikawa Rin
// * Create Time: 2020-1-8 15:00
// * Last Change: 2020-1-8 15:25
// * Description: frame for testing the simple generation algorithm.
// * 
// ****************************************************************************
// This file is the entrance of test program.
// In this file population will be tested by set different objective solver
#include "../src/sga.cpp"
#include "./quadratic_solver.cpp"
#include "./Record.cpp"

#define SingleQuadratic

int main(){
    IComputeAble* solver;
    std::vector<std::vector<double> > cons;

    #ifdef SingleQuadratic
        solver = new SingleParameterQuadraticSolver();
        cons = {{-50, 50}};
    #endif

    cPopulation pop(
        12,     // popsize
        cons,  // constrain
        0.5,   // crossover_rate
        0.3,   // mutattion_rate
        2,     // scale_rate
        solver // solver
    );

    for (int i = 0; i < 100; i++){
        auto inds = pop.Report();
        Record(inds, i);
        pop.Evolution();
    }
    return 0;
}