#ifndef GRADIENT_DESCENT_H
#define GRADIENT_DESCENT_H

typedef struct {
    int degree;
    double final_x;
    double final_fx;
    int iterations;
    int converged;
} GDResult;

GDResult gradient_descent(
    int degree,
    const double *coefficients,
    double initial_x,
    double learning_rate,
    double tolerance,
    int max_iterations
);

#endif
