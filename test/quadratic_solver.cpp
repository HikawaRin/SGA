// ****************************************************************************
// * Author:      Hikawa Rin
// * Create Time: 2020-1-7 15:13
// * Last Change: 2020-1-7 15:30
// * Description: add test example of quadratic problem.
// * 
// ****************************************************************************
// This file implement a quadratic problem solver for test algorithm.
// 
// This solver implement the IComputeAble interface. See comment in 
// "src/sga.hpp" for more interface information.
#ifndef SGA_TEST_QUADRATICSOLVER_CPP
#define SGA_TEST_QUADRATICSOLVER_CPP

#include "../src/sga.hpp"

// An instance of this class will compute objective value by quadratic formula
//
//      $ y=x^2 $
//
class SingleParameterQuadraticSolver : public IComputeAble
{
public:
    int Compute(sIndividual &ind)
    {
        double objective = 0.0;
        double parameter = ind.values[0];

        objective = parameter * parameter;// y=x^2

        ind.objective = objective;
        
        return 0;                         // Compute success
    }
};


#endif // SGA_TEST_QUADRATICSOLVER_CPP