#include <stdio.h>
#include <math.h>

#define RECURS_LEVEL_MAX  10
#define N_MAX             10

// pointer to function of one variable
typedef double (* Func1vFp) (double) ;

// example functions of one variable
// polynomial a[0] + a[1]x + ... + a[n]x^n
double f_poly(double x){
    return 2* pow(x, 5) -4* pow(x,4) + 3.5* pow(x,2) +1.35*x -6.25;

}

double f_rat(double x) {
    return 1/((x-0.5)*(x-0.5) +0.01);
}

double f_exp(double x) {
    return (2*x*pow(2.71828, (-1.5)*x)) -1;
}

double f_trig(double x) {
    return x*tan(x);
}

// Quadratures

// rectangle rule, leftpoint
double quad_rect_left(Func1vFp f1, double a, double b, int n) {
    double h = (b-a)/n;
    double sum=0;
    for(int i=0;i<n;i++){
        sum += f1(a + i*h);

    }
    return h*sum;
}

// rectangle rule, rightpoint
double quad_rect_right(Func1vFp f1, double a, double b, int n) {
    double h = (b-a)/n;
    double sum=0;
    for(int i=0;i<n;i++) {
        sum += f1(b-i * h);
    }
    return h*sum;
}

// rectangle rule, midpoint
double quad_rect_mid(Func1vFp f1, double a, double b, int n) {
    double h = (b-a)/n;
    double sum=0;
    for(int i=0;i<n;i++) {
        sum += f1(i * h + a+ h/2);
    }
    return h * sum;
}

// trapezoidal rule
double quad_trap(Func1vFp func, double a, double b, int n) {
    double h = (b-a)/(2*n);
    double sum=0;
    for(int i=0;i<n;i++){

        sum+= (func(a + 2*i*h) + func(a+ 2*(i+1)*h));

    }
    return h*sum;

}

// Simpson's rule
double quad_simpson(Func1vFp f, double a, double b, int n) {
    double h = (b-a)/(6*n);
    double sum=0;
    double temp_a, temp_b, temp_c;
    for(int i=0;i<n;i++){
        temp_a= a + 6*i*h;
        temp_b = a + 6*(i+1)*h;
        temp_c = (temp_a+temp_b)/2;
        sum += (f(temp_a) + 4*f(temp_c) + f(temp_b));

    }
    return h*sum;
}


// pointer to quadrature function
typedef double (*QuadratureFp)(Func1vFp , double , double,int);

// array of pointers to integrand functions
Func1vFp func_tab[] = { f_poly, f_rat, f_trig, f_exp };

// array of pointers to quadrature functions
QuadratureFp quad_tab[] = {
        quad_rect_left, quad_rect_right, quad_rect_mid, quad_trap, quad_simpson };

// calls 'quad_no' quadrature function for 'fun_no' integrand function
// on interval [a, b] and n subintervals
double quad_select(int fun_no, int quad_no, double a, double b, int n) {
    Func1vFp fun = func_tab[fun_no];
    QuadratureFp quad = quad_tab[quad_no];
    return quad(fun, a, b, n);
}

// adaptive algorithm
double recurs(Func1vFp f, double a, double b, double S, double delta, QuadratureFp quad, int level) {
    double c = (a+b)/2;
    double S1 = quad(f, a, c, 1);
    double S2 = quad(f, c, b, 1);
    if(fabs(S-(S1+S2)) <= delta && level < RECURS_LEVEL_MAX){
        return S1+S2;
    }
    if(level > RECURS_LEVEL_MAX){
        return NAN;
    }else{
        double S_left =  recurs(f,a,c, S1,  delta/2, quad, level+1 );
        double S_right = recurs(f,(a+b)/2,b, S2, delta/2, quad, level+1);
        return S_left + S_right;
    }

}

// initialization for adaptive algorithm
double init_recurs(Func1vFp f, double a, double b, double delta, QuadratureFp quad) {
    double S = quad(f, a, b, 1);
    return recurs(f, a, b, S,delta, quad, 0);
}

// double integrals

// pointer to function of two variables
typedef double (*Func2vFp) (double , double );

double func2v_2(double x, double y) {
    return 2 - x*x - y*y*y;
}

// sample functions to define the normal domain

double lower_bound2(double x) {
    return 0.7*exp(-2*x*x);
}
double upper_bound2(double x) {
    return sin(10*x);
}

// rectangle rule (leftpoint) - double integral over rectangular domain
double dbl_integr(Func2vFp f, double x1, double x2, int nx, double y1, double y2, int ny) {
    double sum=0;
    double h1 = (x2-x1)/nx;
    double h2 = (y2-y1)/ny;
    for(int i=0;i<nx;i++){
        for(int j=0;j<ny;j++){
            sum +=f(x1 + i*h1, y1 + j*h2);
        }
    }
    return sum*h1*h2;

}

// rectangle rule (midpoint) - double integral over normal domain with respect to the x-axis
double dbl_integr_normal_1(Func2vFp f, double x1, double x2, int nx, double hy,
                           Func1vFp fg, Func1vFp fh) {
    double dx = (x2 - x1) / nx;
    double x, y, sum = 0.0;
    int ny ;
    for (int i = 0; i < nx; i++) {
        x = x1 +(i+0.5 ) * dx;

        ny =ceil(((fh(x) - fg(x)) / hy));
        double h2 = (fh(x) - fg(x)) /ny;

        for (int j = 0; j < ny; j++) {
            y =  fg(x) + (j+0.5) * h2;
            sum += f(x, y) * h2;
        }
    }
    return sum*dx;
}

// rectangle rule (leftpoint) - double integral over multiple normal
// domains with respect to the x-axis
double dbl_integr_normal_n(Func2vFp f, double x1, double x2, int nx, double y1, double y2,
                           int ny, Func1vFp fg, Func1vFp fh)  {
    double hx = (x2 - x1) / nx;
    double hy = (y2 - y1) / ny;
    double sum = 0.0;
    double x ,y, gi, hi;
    x = x1;
    for (int i =0; i < nx; i++) {
        gi = fg(x);
        hi = fh(x);
        double up_bound = fmin(hi, y2);
        double down_bound = fmax(gi, y1);

        int current_ny = ceil((up_bound-down_bound)/hy);
        double current_hy = (up_bound-down_bound)/current_ny;
        y = fmax(y1, gi);
        for(int j=0; j < current_ny; j++){
            sum += current_hy * f(x,y);
            y += current_hy;
        }
        x += hx;
    }


    return sum * hx;
}


// multiple quadratures

typedef double (*FuncNvFp)(const double*, int);
typedef int (*BoundNvFp)(const double*, int);

// sample function of three variables
double func3v(const double v[], int n) {
    return v[0] - v[1] + 2*v[2];
}

// sample predicate in 3D
int bound3v(const double v[], int n) { // a cylinder
    return v[0] > 0 && v[0] < 0.5 && v[1]*v[1] + (v[2]-1)*(v[2]-1) < 1;
}

// sample function of n variables
double funcNv(const double v[], int n) {
    double fv = 1.;
    for (int i = 1; i < n; ++i) {
        fv += sin(i*v[i]);
    }
    return fv;
}
// sample n-dimensional predicate (n-dim hypersphere)
int boundNv(const double v[], int n) {
    double r = 0.0;
    for (int i = 0; i < n; ++i) {
        r += (v[i] - 1) * (v[i] - 1);

    }
    return r <= 1.;
}

// multiple integrals over a cuboid with predicate (if boundary != NULL)
// rectangular rule (rightpoint)

void calculate_integral(FuncNvFp f, BoundNvFp boundary, double *sum, double variable_lim[][2], double dx[],double x[],
                        double position, const int intervals[], int level, int variable_no);

void recur_quad_rect_mid(double *p_sum, FuncNvFp f, int variable_no, double t_variable[],
                         double variable_lim[][2], const int tn[], int level,double position,  BoundNvFp boundary);

double trpl_quad_rect(FuncNvFp f, double variable_lim[][2], const int tn[], BoundNvFp boundary) {
    double sum = 0;
    int variable_no=3;
    double dx[variable_no];
    for(int i=0;i<variable_no;i++){
        dx[i] = (variable_lim[i][1] - variable_lim[i][0])/tn[i];
    }
    recur_quad_rect_mid(&sum, f, variable_no, dx, variable_lim, tn, 0, 1, boundary);


    return sum;
}


// multiple integrals over a n-dim hypercuboid with predicate (if boundary != NULL)
// rectangular rule (midpoint)

void calculate_integral(FuncNvFp f, BoundNvFp boundary, double *sum, double variable_lim[][2], double dx[],double x[],
                        double position, const int intervals[], int level, int variable_no){
    x[level] = variable_lim[level][0] + (dx[level] * position);
    for(int i=0;i<intervals[level];i++){
        if(level == variable_no-1 && (boundary == NULL || boundary(x, variable_no))){
            *sum += f(x, variable_no);
        }else if(level < variable_no-1){
            calculate_integral(f, boundary,sum, variable_lim, dx, x, position, intervals, level+1, variable_no);
        }
        x[level] += dx[level];
    }
}


void recur_quad_rect_mid(double *p_sum, FuncNvFp f, int variable_no, double t_variable[],
                         double variable_lim[][2], const int tn[], int level,double position,  BoundNvFp boundary) {
    double x[variable_no];
    *p_sum =0.0;
    for(int i=0;i<variable_no; i++){
        t_variable[i] = (variable_lim[i][1]-variable_lim[i][0]) / tn[i];
    }
    calculate_integral(f, boundary, p_sum, variable_lim, t_variable,x, position, tn, level, variable_no);
    for(int j=0;j<variable_no;j++){
        *p_sum *= t_variable[j];
    }

}

int main(void) {
    int to_do, n, nx, ny, integrand_fun_no, method_no, flag;
    int no_funcs = sizeof(func_tab) / sizeof(Func1vFp);
    int no_quads = sizeof(quad_tab) / sizeof(QuadratureFp);
    double a, b, x1, x2, y1, y2, hy, sum, delta;
    double t_variable[N_MAX], variable_lim[N_MAX][2];
    int tn[N_MAX];

    scanf("%d", &to_do);
    switch (to_do) {
        case 1: // loop over quadratures and integrands
            scanf("%lf %lf %d", &a, &b, &n);
            for(int q = 0; q < no_quads; ++q) {
                for(int f = 0; f < no_funcs; ++f) {
                    printf("%.5f ",quad_select(f, q, a, b, n));
                }
                printf("\n");
            }
            break;
        case 2: // adaptive algorithm
            scanf("%d %d",&integrand_fun_no,&method_no);
            scanf("%lf %lf %lf", &a, &b, &delta);
            printf("%.5f\n", init_recurs(func_tab[integrand_fun_no],a,b,delta,quad_tab[method_no]));
            break;
        case 3: // double integral over a rectangle
            scanf("%lf %lf %d", &x1, &x2, &nx);
            scanf("%lf %lf %d", &y1, &y2, &ny);
            printf("%.5f\n", dbl_integr(func2v_2, x1, x2, nx, y1, y2, ny));
            break;
        case 4: // double integral over normal domain
            scanf("%lf %lf %d", &x1, &x2, &nx);
            scanf("%lf", &hy);
            printf("%.5f\n", dbl_integr_normal_1(func2v_2, x1, x2, nx, hy, lower_bound2, upper_bound2));
            break;
        case 5: // double integral over multiple normal domains
            scanf("%lf %lf %d", &x1, &x2, &nx);
            scanf("%lf %lf %d", &y1, &y2, &ny);
            printf("%.5f\n",dbl_integr_normal_n(func2v_2, x1, x2, nx, y1, y2, ny, lower_bound2, upper_bound2));
            break;
        case 6: // triple integral over a cuboid
            scanf("%lf %lf %d", &variable_lim[0][0], &variable_lim[0][1], &tn[0]);
            scanf("%lf %lf %d", &variable_lim[1][0], &variable_lim[1][1], &tn[1]);
            scanf("%lf %lf %d", &variable_lim[2][0], &variable_lim[2][1], &tn[2]);
            scanf("%d", &flag);
            printf("%.5f\n", trpl_quad_rect(func3v, variable_lim, tn, flag ? bound3v : NULL));
            break;
        case 7: // multiple integral over hyper-cuboid
            scanf("%d", &n);
            if(n > N_MAX) break;
            for(int i = 0; i < n; ++i) {
                scanf("%lf %lf %d", &variable_lim[i][0], &variable_lim[i][1], &tn[i]);
            }
            scanf("%d", &flag);
            sum = 0.;
            recur_quad_rect_mid(&sum, funcNv, n, t_variable, variable_lim, tn, 0,0.5 , flag ? boundNv : NULL);
            printf("%.5f\n", sum);
            break;
        default:
            printf("Nothing to do for %d\n", to_do);
            break;
    }
    return 0;
}
