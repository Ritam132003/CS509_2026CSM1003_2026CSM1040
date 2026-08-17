#include <math.h>
#include "../Include/gradient_descent.h"

static double evaluate_polynomial(int degree, const double *c, double x)
{
    double result = c[degree];

    for (int i = degree - 1; i >= 0; --i)
        result = result * x + c[i];

    return result;
}

static double evaluate_derivative(int degree, const double *c, double x)
{
    if (degree == 0)
        return 0.0;

    double result = (double)degree * c[degree];

    for (int i = degree - 1; i >= 1; --i)
        result = result * x + (double)i * c[i];

    return result;
}

GDResult gradient_descent(
    int degree,
    const double *coefficients,
    double initial_x,
    double learning_rate,
    double tolerance,
    int max_iterations)
{
    GDResult result;
    double x = initial_x;
    int iterations = 0;
    int converged = 0;

    for (iterations = 0; iterations < max_iterations; ++iterations) {
        double derivative = evaluate_derivative(degree, coefficients, x);

        if (fabs(derivative) <= tolerance) {
            converged = 1;
            break;
        }

        x -= learning_rate * derivative;

        if (!isfinite(x))
            break;
    }

    result.degree = degree;
    result.final_x = x;
    result.final_fx = evaluate_polynomial(degree, coefficients, x);
    result.iterations = iterations;
    result.converged = converged;

    return result;
}
