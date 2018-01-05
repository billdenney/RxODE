#define __HD_ODE__ "#include <stdio.h>\n#include <stdarg.h>\n#include <R.h>\n#include <Rinternals.h>\n#include <Rmath.h>\n#include <R_ext/Rdynload.h>\n#define JAC_Rprintf Rprintf\n#define JAC0_Rprintf if (_jac_counter_val() == 0) Rprintf\n#define ODE_Rprintf Rprintf\n#define ODE0_Rprintf if (_dadt_counter_val() == 0) Rprintf\n#define LHS_Rprintf Rprintf\n#define max(a,b) (((a)>(b))?(a):(b))\n#define min(a,b) (((a)<(b))?(a):(b))\n#define R_pow Rx_pow\n#define R_pow_di Rx_pow_di\ntypedef void (*t_dydt)(int *neq, double t, double *A, double *DADT);\ntypedef void (*t_calc_jac)(int *neq, double t, double *A, double *JAC, unsigned int __NROWPD__);\ntypedef void (*t_calc_lhs)(int cSub, double t, double *A, double *lhs);\ntypedef void (*t_update_inis)(int cSub, double *);\ntypedef void (*t_dydt_lsoda_dum)(int *neq, double *t, double *A, double *DADT);\ntypedef void (*t_jdum_lsoda)(int *neq, double *t, double *A,int *ml, int *mu, double *JAC, int *nrowpd);\n\n\ntypedef struct {\n  // These options should not change based on an individual solve\n  int badSolve;\n  double ATOL;          //absolute error\n  double RTOL;          //relative error\n  double H0;\n  double HMIN;\n  int global_jt;\n  int global_mf;\n  int global_debug;\n  int mxstep;\n  int MXORDN;\n  int MXORDS;\n  //\n  int do_transit_abs;\n  int nlhs;\n  int neq;\n  int stiff;\n  int ncov;\n  SEXP stateNames;\n  SEXP lhsNames;\n  SEXP paramNames;\n  int *par_cov;\n  double *inits;\n  double *scale;\n  int do_par_cov;\n  t_dydt dydt;\n  t_calc_jac calc_jac;\n  t_calc_lhs calc_lhs;\n  t_update_inis update_inis;\n  t_dydt_lsoda_dum dydt_lsoda_dum;\n  t_jdum_lsoda jdum_lsoda;\n  // approx fun options\n  double f1;\n  double f2;\n  int kind;\n  int is_locf;\n  int cores;\n  int extraCmt;\n} rx_solving_options;\n\n\ntypedef struct {\n  long slvr_counter;\n  long dadt_counter;\n  long jac_counter;\n  double *InfusionRate;\n  int *BadDose;\n  int nBadDose;\n  double HMAX; // Determined by diff\n  double tlast;\n  double podo;\n  double *par_ptr;\n  double *dose;\n  double *solve;\n  double *lhs;\n  int  *evid;\n  int *rc;\n  double *cov_ptr;\n  int n_all_times;\n  int ixds;\n  int ndoses;\n  double *all_times;\n  int *idose;\n  int idosen;\n  int id;\n  int sim;\n  double ylow;\n  double yhigh;\n} rx_solving_options_ind;\n\ntypedef struct {\n  rx_solving_options_ind *subjects;\n  int nsub;\n  int nsim;\n  int nobs;\n  int add_cov;\n  int matrix;\n  int *stateIgnore;\n  SEXP op;\n} rx_solve;\n\ntypedef void (*t_set_solve)(rx_solve *);\ntypedef rx_solve *(*t_get_solve)();\n\n\nrx_solve *getRxSolve_(SEXP ptr);\nvoid rxSolveDataFree(SEXP ptr);\nint rxUpdateResiduals_(SEXP md);\nrx_solve *getRxSolve(SEXP ptr);\nvoid freeRxSolve(SEXP ptr);\n\nSEXP getSolvingOptionsPtr(double ATOL,          //absolute error\n                          double RTOL,          //relative error\n                          double H0,\n                          double HMIN,\n                          int global_jt,\n                          int global_mf,\n                          int global_debug,\n                          int mxstep,\n                          int MXORDN,\n                          int MXORDS,\n                          // Approx options\n                          int do_transit_abs,\n                          int nlhs,\n                          int neq,\n                          int stiff,\n                          double f1,\n                          double f2,\n                          int kind,\n                          int is_locf,\n                          int cores,\n                          int ncov,\n                          int *par_cov,\n                          int do_par_cov,\n                          double *inits,\n			  double *scale,\n                          SEXP stateNames,\n                          SEXP lhsNames,\n                          SEXP paramNames,\n                          SEXP dydt,\n                          SEXP calc_jac,\n                          SEXP calc_lhs,\n                          SEXP update_inis,\n                          SEXP dydt_lsoda_dum,\n                          SEXP jdum_lsoda,\n                          SEXP set_solve,\n                          SEXP get_solve);\nvoid getSolvingOptionsIndPtr(double *InfusionRate,\n                             int *BadDose,\n                             double HMAX, // Determined by diff\n                             double *par_ptr,\n                             double *dose,\n                             int *idose,\n                             double *solve,\n                             double *lhs,\n                             int *evid,\n                             int *rc,\n                             double *cov_ptr,\n                             int n_all_times,\n                             double *all_times,\n                             int id,\n                             int sim,\n                             rx_solving_options_ind *o);\nSEXP rxSolveData(rx_solving_options_ind *subjects,\n                 int nsub,\n                 int nsim,\n                 int *stateIgnore,\n                 int nobs,\n                 int add_cov,\n                 int matrix,\n                 SEXP op);\nvoid par_lsoda(SEXP sd);\nvoid par_dop(SEXP sd);\n\nrx_solving_options *getRxOp(rx_solve *rx);\nSEXP RxODE_df(SEXP sd, int doDose);\nSEXP RxODE_par_df(SEXP sd);\n\n// Types for par pointers.r\ntypedef void (*RxODE_update_par_ptr)(double t, rx_solve *rx, unsigned int id);\ntypedef double (*RxODE_transit3P)(double t, rx_solve *rx, unsigned int id, double n, double mtt);\ntypedef double (*RxODE_fn) (double x);\ntypedef double (*RxODE_fn2) (double x, double y);\ntypedef double (*RxODE_fn2i) (double x, int i);\ntypedef double (*RxODE_transit4P)(double t, rx_solve *rx, unsigned int id, double n, double mtt, double bio);\ntypedef double (*RxODE_vec) (int val, rx_solve *rx, unsigned int id);\ntypedef long (*RxODE_cnt) (rx_solve *rx, unsigned int id);\ntypedef void (*RxODE_inc) (rx_solve *rx, unsigned int id);\ntypedef double (*RxODE_val) (rx_solve *rx, unsigned int id);\ntypedef SEXP (*RxODE_ode_solver) (SEXP sexp_theta, SEXP sexp_inits, SEXP sexp_lhs, SEXP sexp_time, SEXP sexp_evid,SEXP sexp_dose, SEXP sexp_pcov, SEXP sexp_cov, SEXP sexp_locf, SEXP sexp_atol, SEXP sexp_rtol, SEXP sexp_hmin, SEXP sexp_hmax, SEXP sexp_h0, SEXP sexp_mxordn, SEXP sexp_mxords, SEXP sexp_mx,SEXP sexp_stiff, SEXP sexp_transit_abs, SEXP sexp_object, SEXP sexp_extra_args, SEXP sexp_matrix, SEXP sexp_add_cov);\ntypedef SEXP (*RxODE_assign_fn_xpointers)(void (*fun_dydt)(int *, double, double *, double *),\n					  void (*fun_calc_lhs)(int, double, double *, double *),\n					  void (*fun_calc_jac)(int *, double, double *, double *, unsigned int),\n					  void (*fun_update_inis)(int, double *_ini_sexp),\n					  void (*fun_dydt_lsoda_dum)(int *, double *, double *, double *),\n					  void (*fun_jdum_lsoda)(int *, double *, double *,int *, int *, double *, int *),\n					  void (*fun_set_solve) (rx_solve *solve),\n					  rx_solve *(*fun_get_solve)(),\n					  int fun_jt,int fun_mf, int fun_debug);\n\ntypedef void (*RxODE_ode_solver_old_c)(SEXP mv, int *neq,double *theta,double *time,int *evid,int *ntime,double *inits,double *dose,double *ret,double *atol,double *rtol,int *stiff,int *transit_abs,int *nlhs,double *lhs,int *rc);\ntypedef double (*RxODE_solveLinB)(rx_solve *rx, unsigned int id, double t, int linCmt, int diff1, int diff2, double A, double alpha, double B, double beta, double C, double gamma, double ka, double tlag);\ntypedef double (*RxODE_sum_prod)(double *input, int n);\n// Give par pointers\nRxODE_vec _par_ptr, _InfusionRate;\nRxODE_update_par_ptr _update_par_ptr;\nRxODE_cnt _dadt_counter_val, _jac_counter_val;\nRxODE_inc _dadt_counter_inc, _jac_counter_inc;\nRxODE_val podo, tlast;\nRxODE_transit4P _transit4P;\nRxODE_transit3P _transit3P;\nRxODE_fn _safe_log, safe_zero, factorial, _as_zero, abs_log, abs_log1p;\nRxODE_fn2 sign_exp, Rx_pow;\nRxODE_fn2i Rx_pow_di;\nRxODE_assign_fn_xpointers _assign_fn_xpointers;\nRxODE_ode_solver_old_c _old_c;\nRxODE_solveLinB solveLinB;\nRxODE_sum_prod _sum1, _prod1;\n\ndouble _sum(int n, ...){\n  va_list valist;\n  va_start(valist, n);\n  double *p = Calloc(n, double);\n  for (unsigned int i = 0; i < n; i++){\n    p[i] = va_arg(valist, double);\n  }\n  va_end(valist);\n  double s = _sum1(p, n);\n  Free(p);\n  return s;\n}\n\ndouble _prod(int n, ...){\n  va_list valist;\n  va_start(valist, n);\n  double *p = Calloc(n, double);\n  for (unsigned int i = 0; i < n; i++){\n    p[i] = va_arg(valist, double);\n  }\n  va_end(valist);\n  double s = _prod1(p, n);\n  Free(p);\n  return s;\n}\n\nextern double _sign(unsigned int n, ...){\n  va_list valist;\n  va_start(valist, n);\n  double s = 1;\n  for (unsigned int i = 0; i < n; i++){\n    s = sign(va_arg(valist, double))*s;\n    if (s == 0){\n      break;\n    }\n  }\n  va_end(valist);\n  return s;\n}\n\nSEXP _mv;\nextern SEXP __MODEL_VARS__(){\n  return _mv;\n}\n\nrx_solve *_solveData = NULL;\nextern SEXP __ODE_SOLVER_XPTR__ ();\n\nextern void __ODE_SOLVER_SOLVEDATA__ (rx_solve *solve){\n  _solveData = solve;\n}\n\nextern rx_solve *__ODE_SOLVER_GET_SOLVEDATA__(){\n  return _solveData;\n}\n\nSEXP __MODEL_VARS__();\nextern void __ODE_SOLVER__(int *neq,\n			   double *theta,      //order:\n			   double *time,\n			   int *evid,\n			   int *ntime,\n			   double *inits,\n			   double *dose,\n			   double *ret,\n			   double *atol,\n			   double *rtol,\n			   int *stiff,\n			   int *transit_abs,\n			   int *nlhs,\n			   double *lhs,\n			   int *rc\n			   ){\n  // Backward compatible ode solver for 0.5* C interface\n  _old_c(__MODEL_VARS__(), neq, theta, time, evid, ntime, inits, dose, ret, atol, rtol, stiff, transit_abs, nlhs, lhs, rc);\n}\n\nstatic R_NativePrimitiveArgType __ODE_SOLVER__rx_t[] = {\n  //*neq, *theta, *time,  *evid, *ntime, *inits,   *dose,   *ret,     *atol,  *rtol,   *stiff, *transit_abs, *nlhs, *lhs, *rc\n  INTSXP,REALSXP, REALSXP, INTSXP, INTSXP, REALSXP, REALSXP, REALSXP, REALSXP, REALSXP, INTSXP, INTSXP, INTSXP, REALSXP, INTSXP\n};\n\n"
#define __HD_SOLVE1__ "\nextern void __DYDT_LSODA__(int *neq, double *t, double *A, double *DADT)\n{\n  __DYDT__(neq, *t, A, DADT);\n}\n\nextern void __CALC_JAC_LSODA__(int *neq, double *t, double *A,int *ml, int *mu, double *JAC, int *nrowpd){\n  // Update all covariate parameters\n  __CALC_JAC__(neq, *t, A, JAC, *nrowpd);\n}\n\n\nextern SEXP __ODE_SOLVER_XPTR__  (){\n  return _assign_fn_xpointers(__DYDT__ , __CALC_LHS__ , __CALC_JAC__, __INIS__, __DYDT_LSODA__, __CALC_JAC_LSODA__,\n                              __ODE_SOLVER_SOLVEDATA__, __ODE_SOLVER_GET_SOLVEDATA__, __JT__ , __MF__,\n#ifdef __DEBUG__\n                              1\n#else\n                              0\n#endif\n                              );\n}\n\n\n// Single solving options setup\nrx_solving_options _s_op;\n\n//Initilize the dll to match RxODE's calls\nvoid __R_INIT__ (DllInfo *info){\n  // Get the RxODE calling interfaces\n  _InfusionRate   = (RxODE_vec) R_GetCCallable(\"RxODE\",\"RxODE_InfusionRateP\");\n  _update_par_ptr = (RxODE_update_par_ptr) R_GetCCallable(\"RxODE\",\"RxODE_update_par_ptrP\");\n  _par_ptr = (RxODE_vec) R_GetCCallable(\"RxODE\",\"RxODE_par_ptrP\");\n  _dadt_counter_val = (RxODE_cnt) R_GetCCallable(\"RxODE\",\"RxODE_dadt_counter_valP\");\n  _jac_counter_val  = (RxODE_cnt) R_GetCCallable(\"RxODE\",\"RxODE_jac_counter_valP\");\n  _dadt_counter_inc = (RxODE_inc) R_GetCCallable(\"RxODE\",\"RxODE_dadt_counter_incP\");\n  _jac_counter_inc  = (RxODE_inc) R_GetCCallable(\"RxODE\",\"RxODE_jac_counter_incP\");\n  podo  = (RxODE_val) R_GetCCallable(\"RxODE\",\"RxODE_podoP\");\n  tlast = (RxODE_val) R_GetCCallable(\"RxODE\",\"RxODE_tlastP\");\n  factorial=(RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_factorial\");\n  _transit3P = (RxODE_transit3P) R_GetCCallable(\"RxODE\",\"RxODE_transit3P\");\n  _transit4P = (RxODE_transit4P) R_GetCCallable(\"RxODE\",\"RxODE_transit4P\");\n  _safe_log =(RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_safe_log\");\n  safe_zero =(RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_safe_zero\");\n  _as_zero = (RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_as_zero\");\n  _assign_fn_xpointers=(RxODE_assign_fn_xpointers) R_GetCCallable(\"RxODE\",\"RxODE_get_fn_pointers\");\n  _old_c = (RxODE_ode_solver_old_c) R_GetCCallable(\"RxODE\",\"rxSolveOldC\");\n  _sum1   = (RxODE_sum_prod)R_GetCCallable(\"RxODE\",\"RxODE_sum\");\n  _prod1 = (RxODE_sum_prod) R_GetCCallable(\"RxODE\",\"RxODE_prod\");\n  sign_exp = (RxODE_fn2) R_GetCCallable(\"RxODE\",\"RxODE_sign_exp\");\n  Rx_pow = (RxODE_fn2) R_GetCCallable(\"RxODE\",\"RxODE_pow\");\n  Rx_pow_di = (RxODE_fn2i) R_GetCCallable(\"RxODE\",\"RxODE_pow_di\");\n  abs_log = (RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_abs_log\");\n  abs_log1p = (RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_abs_log1p\");\n  solveLinB = (RxODE_solveLinB) R_GetCCallable(\"RxODE\",\"RxODE_solveLinB\");\n  // Register the outside functions\n  R_RegisterCCallable(__LIB_STR__,__ODE_SOLVER_STR__,       (DL_FUNC) __ODE_SOLVER__);\n  R_RegisterCCallable(__LIB_STR__,\"__ODE_SOLVER_XPTR__\",   (DL_FUNC) __ODE_SOLVER_XPTR__);\n\n  /* R_registerRoutines(info, NULL, NULL, NULL, NULL); */\n  /* R_useDynamicSymbols(info,TRUE); */\n\n  static const R_CMethodDef cMethods[] = {\n    {__ODE_SOLVER_STR__, (DL_FUNC) &__ODE_SOLVER__, 15, __ODE_SOLVER__rx_t},\n    {NULL, NULL, 0, NULL}\n  };\n  \n  R_CallMethodDef callMethods[]  = {\n    {__ODE_SOLVER_XPTR_STR__, (DL_FUNC) &__ODE_SOLVER_XPTR__, 0},\n    {__MODEL_VARS_STR__, (DL_FUNC) &__MODEL_VARS__, 0},\n    {\"__INIS__\", (DL_FUNC) &__INIS__, 1},\n    {NULL, NULL, 0}\n  };\n  R_registerRoutines(info, cMethods, callMethods, NULL, NULL);\n  R_useDynamicSymbols(info,FALSE);\n  _mv = __MODEL_VARS__0();\n}\n\n\nvoid __R_UNLOAD__ (DllInfo *info){\n  // Free resources required for single subject solve.\n}"
#define __HD_SOLVE2__ ""
