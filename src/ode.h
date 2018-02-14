#define __HD_ODE__ "#include <stdio.h>\n#include <stdarg.h>\n#include <R.h>\n#include <Rinternals.h>\n#include <Rmath.h>\n#include <R_ext/Rdynload.h>\n#define JAC_Rprintf Rprintf\n#define JAC0_Rprintf if (_jac_counter_val() == 0) Rprintf\n#define ODE_Rprintf Rprintf\n#define ODE0_Rprintf if (_dadt_counter_val() == 0) Rprintf\n#define LHS_Rprintf Rprintf\n#define R_pow Rx_pow\n#define R_pow_di Rx_pow_di\ntypedef void (*t_dydt)(int *neq, double t, double *A, double *DADT);\ntypedef void (*t_calc_jac)(int *neq, double t, double *A, double *JAC, unsigned int __NROWPD__);\ntypedef void (*t_calc_lhs)(int cSub, double t, double *A, double *lhs);\ntypedef void (*t_update_inis)(int cSub, double *);\ntypedef void (*t_dydt_lsoda_dum)(int *neq, double *t, double *A, double *DADT);\ntypedef void (*t_jdum_lsoda)(int *neq, double *t, double *A,int *ml, int *mu, double *JAC, int *nrowpd);\ntypedef int (*t_dydt_liblsoda)(double t, double *y, double *ydot, void *data);\n\ntypedef struct {\n  // These options should not change based on an individual solve\n  int badSolve;\n  double ATOL;          //absolute error\n  double RTOL;          //relative error\n  double H0;\n  double HMIN;\n  int mxstep;\n  int MXORDN;\n  int MXORDS;\n  //\n  int do_transit_abs;\n  int nlhs;\n  int neq;\n  int stiff;\n  int ncov;\n  char modNamePtr[1000];\n  int *par_cov;\n  double *inits;\n  double *scale;\n  int do_par_cov;\n  // approx fun options\n  double f1;\n  double f2;\n  int kind;\n  int is_locf;\n  int cores;\n  int extraCmt;\n  double hmax2; // Determined by diff\n  double *rtol2;\n  double *atol2;\n  int nDisplayProgress;\n  int ncoresRV;\n  int isChol;\n  int *svar;\n  int abort;\n} rx_solving_options;\n\n\ntypedef struct {\n  long slvr_counter;\n  long dadt_counter;\n  long jac_counter;\n  double *InfusionRate;\n  int *BadDose;\n  int nBadDose;\n  double HMAX; // Determined by diff\n  double tlast;\n  double podo;\n  double *par_ptr;\n  double *dose;\n  double *solve;\n  double *lhs;\n  int  *evid;\n  int *rc;\n  double *cov_ptr;\n  int n_all_times;\n  int ixds;\n  int ndoses;\n  double *all_times;\n  int *idose;\n  int idosen;\n  int id;\n  int sim;\n  double ylow;\n  double yhigh;\n} rx_solving_options_ind;\n\ntypedef struct {\n  rx_solving_options_ind *subjects;\n  int nsub;\n  int nsim;\n  int nobs;\n  int add_cov;\n  int matrix;\n  int *stateIgnore;\n} rx_solve;\n\ntypedef void (*t_set_solve)(rx_solve *);\ntypedef rx_solve *(*t_get_solve)();\n\n\nrx_solve *getRxSolve_();\nrx_solve *getRxSolve(SEXP ptr);\n\nvoid getSolvingOptionsPtr(double ATOL,          //absolute error\n                          double RTOL,          //relative error\n                          double H0,\n                          double HMIN,\n                          int mxstep,\n                          int MXORDN,\n                          int MXORDS,\n                          // Approx options\n                          int do_transit_abs,\n                          int nlhs,\n                          int neq,\n                          int stiff,\n                          double f1,\n                          double f2,\n                          int kind,\n                          int is_locf,\n                          int cores,\n                          int ncov,\n                          int *par_cov,\n                          int do_par_cov,\n                          double *inits,\n			  double *scale,\n			  const char *modNamePtr,\n			  double hmax2,\n                          double *atol2,\n                          double *rtol2,\n                          int nDisplayProgress,\n                          int ncoresRV,\n                          int isChol,\n                          int *svar);\nvoid getSolvingOptionsIndPtr(double *InfusionRate,\n                             int *BadDose,\n                             double HMAX, // Determined by diff\n                             double *par_ptr,\n                             double *dose,\n                             int *idose,\n                             double *solve,\n                             double *lhs,\n                             int *evid,\n                             int *rc,\n                             double *cov_ptr,\n                             int n_all_times,\n                             double *all_times,\n                             int id,\n                             int sim,\n                             rx_solving_options_ind *o);\nvoid rxSolveData(rx_solving_options_ind *subjects,\n                 int nsub, int nsim, int *stateIgnore, int nobs, int add_cov, int matrix);\nvoid par_solve(rx_solve *rx);\n\nrx_solving_options *getRxOp(rx_solve *rx);\n\nSEXP RxODE_df(SEXP sd, int doDose);\nSEXP RxODE_par_df(SEXP sd);\n\nrx_solving_options_ind *rxOptionsIniEnsure(int mx);\n\nvoid rxUpdateFuns(SEXP trans);\n\n// Types for par pointers.r\ntypedef void (*RxODE_update_par_ptr)(double t, rx_solve *rx, unsigned int id);\ntypedef double (*RxODE_transit3P)(double t, rx_solve *rx, unsigned int id, double n, double mtt);\ntypedef double (*RxODE_fn) (double x);\ntypedef double (*RxODE_fn2) (double x, double y);\ntypedef double (*RxODE_fn2i) (double x, int i);\ntypedef double (*RxODE_transit4P)(double t, rx_solve *rx, unsigned int id, double n, double mtt, double bio);\ntypedef double (*RxODE_vec) (int val, rx_solve *rx, unsigned int id);\ntypedef long (*RxODE_cnt) (rx_solve *rx, unsigned int id);\ntypedef void (*RxODE_inc) (rx_solve *rx, unsigned int id);\ntypedef double (*RxODE_val) (rx_solve *rx, unsigned int id);\ntypedef void (*RxODE_assign_ptr)(SEXP);\ntypedef void (*RxODE_ode_solver_old_c)(int *neq,double *theta,double *time,int *evid,int *ntime,double *inits,double *dose,double *ret,double *atol,double *rtol,int *stiff,int *transit_abs,int *nlhs,double *lhs,int *rc);\ntypedef double (*RxODE_solveLinB)(rx_solve *rx, unsigned int id, double t, int linCmt, int diff1, int diff2, double A, double alpha, double B, double beta, double C, double gamma, double ka, double tlag);\n\nRxODE_solveLinB solveLinB    = NULL;\nRxODE_assign_ptr _assign_ptr = NULL;\n\ntypedef void (*_rxRmModelLibType)(const char *inp);\n_rxRmModelLibType _rxRmModelLib = NULL;\n\ntypedef SEXP (*_rxGetModelLibType)(const char *s);\n_rxGetModelLibType _rxGetModelLib = NULL;\n\nRxODE_ode_solver_old_c _old_c = NULL;\n\nRxODE_fn2i Rx_pow_di = NULL;\nRxODE_fn2 Rx_pow = NULL;\nRxODE_fn2 sign_exp = NULL;\n\nRxODE_fn _as_zero = NULL;\nRxODE_fn _safe_log = NULL;\nRxODE_fn safe_zero = NULL;\nRxODE_fn abs_log = NULL;\nRxODE_fn abs_log1p = NULL;\nRxODE_fn factorial = NULL;\nRxODE_transit4P _transit4P = NULL;\nRxODE_transit3P _transit3P =NULL;\nRxODE_val podo = NULL;\nRxODE_val tlast = NULL;\nRxODE_inc _dadt_counter_inc=NULL;\nRxODE_inc _jac_counter_inc =NULL;\nRxODE_cnt _dadt_counter_val = NULL;\nRxODE_cnt _jac_counter_val = NULL;\nRxODE_update_par_ptr _update_par_ptr=NULL;\nRxODE_vec _InfusionRate= NULL;\n\ntypedef  SEXP (*_rx_asgn) (SEXP objectSEXP);\n_rx_asgn _RxODE_rxAssignPtr =NULL;\n\nint _rxIsCurrentC(SEXP obj){\n  static int(*fun)(SEXP)= NULL;\n  if (fun==NULL) fun = (int(*)(SEXP))R_GetCCallable(\"RxODE\",\"rxIsCurrentC\");\n  return fun(obj);\n}\n\ndouble _sum(double *p, long double *pld, int m, int type, int n, ...){\n  va_list valist;\n  va_start(valist, n);\n  for (unsigned int i = 0; i < n; i++){\n    p[i] = va_arg(valist, double);\n  }\n  va_end(valist);\n  static double (*fun)(double *, int, long double *, int, int)=NULL;\n  if (fun == NULL) fun = (double(*)(double *, int, long double *, int, int)) R_GetCCallable(\"PreciseSums\",\"PreciseSums_sum_r\");\n  return fun(p, n, pld, m, type);\n}\n\n\nextern double _prod(double *input, double *p, int type, int n, ...){\n  va_list valist;\n  va_start(valist, n);\n  for (unsigned int i = 0; i < n; i++){\n    input[i] = va_arg(valist, double);\n  }\n  va_end(valist);\n  static double (*fun)(double*, double*, int, int)=NULL;\n  if (fun == NULL) fun = (double(*)(double*, double*, int, int)) R_GetCCallable(\"PreciseSums\",\"PreciseSums_prod_r\");\n  return fun(input, p, n, type);\n}\n\nextern double _sign(unsigned int n, ...){\n  va_list valist;\n  va_start(valist, n);\n  double s = 1;\n  for (unsigned int i = 0; i < n; i++){\n    s = sign(va_arg(valist, double))*s;\n    if (s == 0){\n      break;\n    }\n  }\n  va_end(valist);\n  return s;\n}\n\nextern double _max(unsigned int n, ...){\n  va_list valist;\n  va_start(valist, n);\n  double mx = NA_REAL;\n  double tmp = 0;\n  if (n >= 1){\n    mx = va_arg(valist, double);\n    for (unsigned int i = 1; i < n; i++){\n      tmp = va_arg(valist, double);\n      if (tmp>mx) mx=tmp;\n    }\n    va_end(valist);\n  }\n  return mx;\n}\n\nextern double _min(unsigned int n, ...){\n  va_list valist;\n  va_start(valist, n);\n  double mn = NA_REAL;\n  double tmp = 0;\n  if (n >= 1){\n    mn = va_arg(valist, double);\n    for (unsigned int i = 1; i < n; i++){\n      tmp = va_arg(valist, double);\n      if (tmp<mn) mn=tmp;\n    }\n    va_end(valist);\n  }\n  return mn;\n}\n\nrx_solve *_solveData = NULL;\n\nint _prodType(){\n  // Type 3 = Logify\n  return 3;\n}\nint _sumType(){\n  // Type 1 = PairwiseSum\n  return 1;\n}\n\nextern void __ODE_SOLVER_SOLVEDATA__ (rx_solve *solve){\n  _solveData = solve;\n}\n\nextern rx_solve *__ODE_SOLVER_GET_SOLVEDATA__(){\n  return _solveData;\n}\n\nSEXP __MODEL_VARS__();\nextern void __ODE_SOLVER__(int *neq,\n			   double *theta,      //order:\n			   double *time,\n			   int *evid,\n			   int *ntime,\n			   double *inits,\n			   double *dose,\n			   double *ret,\n			   double *atol,\n			   double *rtol,\n			   int *stiff,\n			   int *transit_abs,\n			   int *nlhs,\n			   double *lhs,\n			   int *rc){\n  // Backward compatible ode solver for 0.5* C interface\n  _assign_ptr(__MODEL_VARS__());\n  _old_c(neq, theta, time, evid, ntime, inits, dose, ret, atol, rtol, stiff, transit_abs, nlhs, lhs, rc);\n}\n\nstatic R_NativePrimitiveArgType __ODE_SOLVER__rx_t[] = {\n  //*neq, *theta, *time,  *evid, *ntime, *inits,   *dose,   *ret,     *atol,  *rtol,   *stiff, *transit_abs, *nlhs, *lhs, *rc\n  INTSXP,REALSXP, REALSXP, INTSXP, INTSXP, REALSXP, REALSXP, REALSXP, REALSXP, REALSXP, INTSXP, INTSXP, INTSXP, REALSXP, INTSXP\n};\n\n"
#define __HD_SOLVE1__ "\nextern void __DYDT_LSODA__(int *neq, double *t, double *A, double *DADT)\n{\n  __DYDT__(neq, *t, A, DADT);\n}\n\nextern int __DYDT_LIBLSODA__(double t, double *y, double *ydot, void *data)\n{\n  int *neq = (int*)(data);\n  __DYDT__(neq, t, y, ydot);\n  return(0);\n}\n\nextern void __CALC_JAC_LSODA__(int *neq, double *t, double *A,int *ml, int *mu, double *JAC, int *nrowpd){\n  // Update all covariate parameters\n  __CALC_JAC__(neq, *t, A, JAC, *nrowpd);\n}\n\n//Initilize the dll to match RxODE's calls\nvoid __R_INIT__ (DllInfo *info){\n  // Get C callables on load; Otherwise it isn't thread safe\n  solveLinB = (RxODE_solveLinB) R_GetCCallable(\"RxODE\",\"RxODE_solveLinB\");\n  _assign_ptr=(RxODE_assign_ptr) R_GetCCallable(\"RxODE\",\"RxODE_assign_fn_pointers\");\n  _rxRmModelLib=(_rxRmModelLibType) R_GetCCallable(\"RxODE\",\"rxRmModelLib\");\n  _rxGetModelLib=(_rxGetModelLibType) R_GetCCallable(\"RxODE\",\"rxGetModelLib\");\n  _old_c = (RxODE_ode_solver_old_c) R_GetCCallable(\"RxODE\",\"rxSolveOldC\");\n  Rx_pow_di=(RxODE_fn2i) R_GetCCallable(\"RxODE\",\"RxODE_pow_di\");\n  Rx_pow = (RxODE_fn2) R_GetCCallable(\"RxODE\",\"RxODE_pow\");\n  sign_exp = (RxODE_fn2) R_GetCCallable(\"RxODE\",\"RxODE_sign_exp\");\n  _as_zero = (RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_as_zero\");\n  _safe_log=(RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_safe_log\");\n  safe_zero=(RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_safe_zero\");\n  abs_log = (RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_abs_log\");\n  abs_log1p=(RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_abs_log1p\");\n  factorial = (RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_factorial\");\n  _transit4P= (RxODE_transit4P) R_GetCCallable(\"RxODE\",\"RxODE_transit4P\");\n  _transit3P=(RxODE_transit3P) R_GetCCallable(\"RxODE\",\"RxODE_transit3P\");\n  podo = (RxODE_val) R_GetCCallable(\"RxODE\",\"RxODE_podoP\");\n  tlast = (RxODE_val) R_GetCCallable(\"RxODE\",\"RxODE_tlastP\");\n  _dadt_counter_inc=(RxODE_inc) R_GetCCallable(\"RxODE\",\"RxODE_dadt_counter_incP\");\n  _jac_counter_inc=(RxODE_inc) R_GetCCallable(\"RxODE\",\"RxODE_jac_counter_incP\");\n  _dadt_counter_val= (RxODE_cnt) R_GetCCallable(\"RxODE\",\"RxODE_dadt_counter_valP\");\n  _jac_counter_val=(RxODE_cnt) R_GetCCallable(\"RxODE\",\"RxODE_jac_counter_valP\");\n  _update_par_ptr=(RxODE_update_par_ptr) R_GetCCallable(\"RxODE\",\"RxODE_update_par_ptrP\");\n  _InfusionRate=(RxODE_vec) R_GetCCallable(\"RxODE\",\"RxODE_InfusionRateP\");\n  _RxODE_rxAssignPtr=(_rx_asgn)R_GetCCallable(\"RxODE\",\"_RxODE_rxAssignPtr\");\n  // Register the outside functions\n  R_RegisterCCallable(__LIB_STR__,__ODE_SOLVER_STR__,       (DL_FUNC) __ODE_SOLVER__);\n  R_RegisterCCallable(__LIB_STR__,\"__INIS__\", (DL_FUNC) __INIS__);\n  R_RegisterCCallable(__LIB_STR__,\"__INIS__\", (DL_FUNC) __INIS__);\n  R_RegisterCCallable(__LIB_STR__,\"__DYDT__\", (DL_FUNC) __DYDT__);\n  R_RegisterCCallable(__LIB_STR__,\"__CALC_LHS__\", (DL_FUNC) __CALC_LHS__);\n  R_RegisterCCallable(__LIB_STR__,\"__CALC_JAC__\", (DL_FUNC) __CALC_JAC__);\n  R_RegisterCCallable(__LIB_STR__,\"__DYDT_LSODA__\", (DL_FUNC) __DYDT_LSODA__);\n  R_RegisterCCallable(__LIB_STR__,\"__CALC_JAC_LSODA__\", (DL_FUNC) __CALC_JAC_LSODA__);\n  R_RegisterCCallable(__LIB_STR__,\"__ODE_SOLVER_SOLVEDATA__\", (DL_FUNC) __ODE_SOLVER_SOLVEDATA__);\n  R_RegisterCCallable(__LIB_STR__,\"__ODE_SOLVER_GET_SOLVEDATA__\", (DL_FUNC) __ODE_SOLVER_GET_SOLVEDATA__);\n  R_RegisterCCallable(__LIB_STR__,\"__DYDT_LIBLSODA__\", (DL_FUNC) __DYDT_LIBLSODA__);\n  \n  static const R_CMethodDef cMethods[] = {\n    {__ODE_SOLVER_STR__, (DL_FUNC) &__ODE_SOLVER__, 15, __ODE_SOLVER__rx_t},\n    {NULL, NULL, 0, NULL}\n  };\n  \n  R_CallMethodDef callMethods[]  = {\n    {__MODEL_VARS_STR__, (DL_FUNC) &__MODEL_VARS__, 0},\n    {NULL, NULL, 0}\n  };\n  R_registerRoutines(info, cMethods, callMethods, NULL, NULL);\n  R_useDynamicSymbols(info,FALSE);\n}\n\nvoid __R_UNLOAD__ (DllInfo *info){\n  // Free resources required for single subject solve.\n  SEXP _mv = PROTECT(_rxGetModelLib(__MODEL_VARS_STR__));\n  if (!isNull(_mv)){\n    _rxRmModelLib(_"
#define __HD_SOLVE2__ "_MODEL_VARS_STR__);\n  }\n  UNPROTECT(1);\n}"
