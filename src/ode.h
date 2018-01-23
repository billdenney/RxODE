#define __HD_ODE__ "#include <stdio.h>\n#include <stdarg.h>\n#include <R.h>\n#include <Rinternals.h>\n#include <Rmath.h>\n#include <R_ext/Rdynload.h>\n#define JAC_Rprintf Rprintf\n#define JAC0_Rprintf if (_jac_counter_val() == 0) Rprintf\n#define ODE_Rprintf Rprintf\n#define ODE0_Rprintf if (_dadt_counter_val() == 0) Rprintf\n#define LHS_Rprintf Rprintf\n#define max(a,b) (((a)>(b))?(a):(b))\n#define min(a,b) (((a)<(b))?(a):(b))\n#define R_pow Rx_pow\n#define R_pow_di Rx_pow_di\ntypedef void (*t_dydt)(int *neq, double t, double *A, double *DADT);\ntypedef void (*t_calc_jac)(int *neq, double t, double *A, double *JAC, unsigned int __NROWPD__);\ntypedef void (*t_calc_lhs)(int cSub, double t, double *A, double *lhs);\ntypedef void (*t_update_inis)(int cSub, double *);\ntypedef void (*t_dydt_lsoda_dum)(int *neq, double *t, double *A, double *DADT);\ntypedef void (*t_jdum_lsoda)(int *neq, double *t, double *A,int *ml, int *mu, double *JAC, int *nrowpd);\n\n\ntypedef struct {\n  // These options should not change based on an individual solve\n  int badSolve;\n  double ATOL;          //absolute error\n  double RTOL;          //relative error\n  double H0;\n  double HMIN;\n  int global_jt;\n  int global_mf;\n  int global_debug;\n  int mxstep;\n  int MXORDN;\n  int MXORDS;\n  //\n  int do_transit_abs;\n  int nlhs;\n  int neq;\n  int stiff;\n  int ncov;\n  SEXP stateNames;\n  SEXP lhsNames;\n  SEXP paramNames;\n  int *par_cov;\n  double *inits;\n  double *scale;\n  int do_par_cov;\n  t_dydt dydt;\n  t_calc_jac calc_jac;\n  t_calc_lhs calc_lhs;\n  t_update_inis update_inis;\n  t_dydt_lsoda_dum dydt_lsoda_dum;\n  t_jdum_lsoda jdum_lsoda;\n  // approx fun options\n  double f1;\n  double f2;\n  int kind;\n  int is_locf;\n  int cores;\n  int extraCmt;\n} rx_solving_options;\n\n\ntypedef struct {\n  long slvr_counter;\n  long dadt_counter;\n  long jac_counter;\n  double *InfusionRate;\n  int *BadDose;\n  int nBadDose;\n  double HMAX; // Determined by diff\n  double tlast;\n  double podo;\n  double *par_ptr;\n  double *dose;\n  double *solve;\n  double *lhs;\n  int  *evid;\n  int *rc;\n  double *cov_ptr;\n  int n_all_times;\n  int ixds;\n  int ndoses;\n  double *all_times;\n  int *idose;\n  int idosen;\n  int id;\n  int sim;\n  double ylow;\n  double yhigh;\n} rx_solving_options_ind;\n\ntypedef struct {\n  rx_solving_options_ind *subjects;\n  int nsub;\n  int nsim;\n  int nobs;\n  int add_cov;\n  int matrix;\n  int *stateIgnore;\n  SEXP op;\n} rx_solve;\n\ntypedef void (*t_set_solve)(rx_solve *);\ntypedef rx_solve *(*t_get_solve)();\n\n\nrx_solve *getRxSolve_(SEXP ptr);\nvoid rxSolveDataFree(SEXP ptr);\nint rxUpdateResiduals_(SEXP md);\nrx_solve *getRxSolve(SEXP ptr);\nvoid freeRxSolve(SEXP ptr);\n\nSEXP getSolvingOptionsPtr(double ATOL,          //absolute error\n                          double RTOL,          //relative error\n                          double H0,\n                          double HMIN,\n                          int global_jt,\n                          int global_mf,\n                          int global_debug,\n                          int mxstep,\n                          int MXORDN,\n                          int MXORDS,\n                          // Approx options\n                          int do_transit_abs,\n                          int nlhs,\n                          int neq,\n                          int stiff,\n                          double f1,\n                          double f2,\n                          int kind,\n                          int is_locf,\n                          int cores,\n                          int ncov,\n                          int *par_cov,\n                          int do_par_cov,\n                          double *inits,\n			  double *scale,\n                          SEXP stateNames,\n                          SEXP lhsNames,\n                          SEXP paramNames,\n                          SEXP dydt,\n                          SEXP calc_jac,\n                          SEXP calc_lhs,\n                          SEXP update_inis,\n                          SEXP dydt_lsoda_dum,\n                          SEXP jdum_lsoda);\nvoid getSolvingOptionsIndPtr(double *InfusionRate,\n                             int *BadDose,\n                             double HMAX, // Determined by diff\n                             double *par_ptr,\n                             double *dose,\n                             int *idose,\n                             double *solve,\n                             double *lhs,\n                             int *evid,\n                             int *rc,\n                             double *cov_ptr,\n                             int n_all_times,\n                             double *all_times,\n                             int id,\n                             int sim,\n                             rx_solving_options_ind *o);\nSEXP rxSolveData(rx_solving_options_ind *subjects,\n                 int nsub,\n                 int nsim,\n                 int *stateIgnore,\n                 int nobs,\n                 int add_cov,\n                 int matrix,\n                 SEXP op);\nvoid par_solve(rx_solve *rx, SEXP sd, int ini_updateR);\n\nrx_solving_options *getRxOp(rx_solve *rx);\nSEXP RxODE_df(SEXP sd, int doDose);\nSEXP RxODE_par_df(SEXP sd);\n\nrx_solving_options_ind *rxOptionsIniEnsure(int mx);\n\n// Types for par pointers.r\ntypedef void (*RxODE_update_par_ptr)(double t, rx_solve *rx, unsigned int id);\ntypedef double (*RxODE_transit3P)(double t, rx_solve *rx, unsigned int id, double n, double mtt);\ntypedef double (*RxODE_fn) (double x);\ntypedef double (*RxODE_fn2) (double x, double y);\ntypedef double (*RxODE_fn2i) (double x, int i);\ntypedef double (*RxODE_transit4P)(double t, rx_solve *rx, unsigned int id, double n, double mtt, double bio);\ntypedef double (*RxODE_vec) (int val, rx_solve *rx, unsigned int id);\ntypedef long (*RxODE_cnt) (rx_solve *rx, unsigned int id);\ntypedef void (*RxODE_inc) (rx_solve *rx, unsigned int id);\ntypedef double (*RxODE_val) (rx_solve *rx, unsigned int id);\ntypedef SEXP (*RxODE_ode_solver) (SEXP sexp_theta, SEXP sexp_inits, SEXP sexp_lhs, SEXP sexp_time, SEXP sexp_evid,SEXP sexp_dose, SEXP sexp_pcov, SEXP sexp_cov, SEXP sexp_locf, SEXP sexp_atol, SEXP sexp_rtol, SEXP sexp_hmin, SEXP sexp_hmax, SEXP sexp_h0, SEXP sexp_mxordn, SEXP sexp_mxords, SEXP sexp_mx,SEXP sexp_stiff, SEXP sexp_transit_abs, SEXP sexp_object, SEXP sexp_extra_args, SEXP sexp_matrix, SEXP sexp_add_cov);\ntypedef void (*RxODE_assign_ptr)(SEXP);\ntypedef SEXP (*RxODE_assign_fn_xpointers)(void (*fun_dydt)(int *, double, double *, double *),\n					  void (*fun_calc_lhs)(int, double, double *, double *),\n					  void (*fun_calc_jac)(int *, double, double *, double *, unsigned int),\n					  void (*fun_update_inis)(int, double *_ini_sexp),\n					  void (*fun_dydt_lsoda_dum)(int *, double *, double *, double *),\n					  void (*fun_jdum_lsoda)(int *, double *, double *,int *, int *, double *, int *),\n					  void (*fun_set_solve) (rx_solve *solve),\n					  rx_solve *(*fun_get_solve)(),\n					  int fun_jt,int fun_mf, int fun_debug);\n\ntypedef void (*RxODE_ode_solver_old_c)(int *neq,double *theta,double *time,int *evid,int *ntime,double *inits,double *dose,double *ret,double *atol,double *rtol,int *stiff,int *transit_abs,int *nlhs,double *lhs,int *rc);\ntypedef double (*RxODE_solveLinB)(rx_solve *rx, unsigned int id, double t, int linCmt, int diff1, int diff2, double A, double alpha, double B, double beta, double C, double gamma, double ka, double tlag);\ntypedef double (*RxODE_sum_prod)(double *input, int n);\n\ndouble solveLinB(rx_solve *rx, unsigned int id, double t, int linCmt, int diff1, int diff2, double A, double alpha, double B, double beta, double C, double gamma, double ka, double tlag){\n  static RxODE_solveLinB fun = NULL;\n  if (fun == NULL) fun = (RxODE_solveLinB) R_GetCCallable(\"RxODE\",\"RxODE_solveLinB\");\n  return fun(rx, id, t, linCmt, diff1, diff2, A, alpha, B, beta, C, gamma, ka, tlag);\n}\n\nvoid _assign_ptr(SEXP x){\n  static RxODE_assign_ptr fun = NULL;\n  if (fun == NULL) fun = (RxODE_assign_ptr) R_GetCCallable(\"RxODE\",\"RxODE_assign_fn_pointers\");\n  fun(x);\n} \n\ndouble _sum1(double *input, int n){\n  static RxODE_sum_prod fun = NULL;\n  if (fun == NULL) fun = (RxODE_sum_prod) R_GetCCallable(\"RxODE\",\"RxODE_sum\");\n  return fun(input, n);\n}\n\ndouble _prod1(double *input, int n){\n  static RxODE_sum_prod fun = NULL;\n  if (fun == NULL) fun = (RxODE_sum_prod) R_GetCCallable(\"RxODE\",\"RxODE_prod\");\n  return fun(input, n);\n}\n\ntypedef void (*_rxRmModelLibType)(const char *inp);\nvoid _rxRmModelLib(const char *inp){\n  static _rxRmModelLibType fun = NULL;\n  if (fun == NULL) fun = (_rxRmModelLibType) R_GetCCallable(\"RxODE\",\"rxRmModelLib\");\n  fun(inp);\n}\n\ntypedef SEXP (*_rxGetModelLibType)(const char *s);\nSEXP _rxGetModelLib(const char *inp){\n  static _rxGetModelLibType fun = NULL;\n  if (fun == NULL) fun = (_rxGetModelLibType) R_GetCCallable(\"RxODE\",\"rxGetModelLib\");\n  return fun(inp);\n}\n\nvoid _old_c(int *neq,double *theta,double *time,int *evid,int *ntime,double *inits,double *dose,double *ret,double *atol,double *rtol,int *stiff,int *transit_abs,int *nlhs,double *lhs,int *rc){\n  static RxODE_ode_solver_old_c fun = NULL;\n  if (fun == NULL) fun = (RxODE_ode_solver_old_c) R_GetCCallable(\"RxODE\",\"rxSolveOldC\");\n  return fun(neq, theta, time, evid, ntime, inits, dose, ret, atol, rtol, stiff, \n	     transit_abs, nlhs, lhs, rc);\n}\n\nSEXP _assign_fn_xpointers(void (*fun_dydt)(int *, double, double *, double *),\n			  void (*fun_calc_lhs)(int, double, double *, double *),\n			  void (*fun_calc_jac)(int *, double, double *, double *, unsigned int),\n			  void (*fun_update_inis)(int, double *_ini_sexp),\n			  void (*fun_dydt_lsoda_dum)(int *, double *, double *, double *),\n			  void (*fun_jdum_lsoda)(int *, double *, double *,int *, int *, double *, int *),\n			  void (*fun_set_solve) (rx_solve *solve),\n			  rx_solve *(*fun_get_solve)(),\n			  int fun_jt,int fun_mf, int fun_debug){\n  static RxODE_assign_fn_xpointers fun = NULL;\n  if (fun == NULL) fun = (RxODE_assign_fn_xpointers) R_GetCCallable(\"RxODE\",\"RxODE_get_fn_pointers\");\n  return fun(fun_dydt, fun_calc_lhs, fun_calc_jac, fun_update_inis, fun_dydt_lsoda_dum, fun_jdum_lsoda, \n	     fun_set_solve, fun_get_solve, fun_jt, fun_mf, fun_debug);\n}\n\ndouble Rx_pow_di(double x, double y){\n  static RxODE_fn2i fun = NULL;\n  if (fun == NULL) fun = (RxODE_fn2i) R_GetCCallable(\"RxODE\",\"RxODE_pow_di\");\n  return fun(x, y);\n}\n\ndouble Rx_pow(double x, double y){\n  static RxODE_fn2 fun = NULL;\n  if (fun == NULL) fun = (RxODE_fn2) R_GetCCallable(\"RxODE\",\"RxODE_pow\");\n  return fun(x, y);\n}\n\ndouble sign_exp(double x, double y){\n  static RxODE_fn2 fun = NULL;\n  if (fun == NULL) fun = (RxODE_fn2) R_GetCCallable(\"RxODE\",\"RxODE_sign_exp\");\n  return fun(x, y);\n}\n\ndouble _as_zero(double x){\n  static RxODE_fn fun = NULL;\n  if (fun == NULL) fun = (RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_as_zero\");\n  return fun(x);\n}\n\ndouble _safe_log(double x){\n  static RxODE_fn fun = NULL;\n  if (fun == NULL) fun = (RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_safe_log\");\n  return fun(x);\n}\n\ndouble safe_zero(double x){\n  static RxODE_fn fun = NULL;\n  if (fun == NULL) fun = (RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_safe_zero\");\n  return fun(x);\n}\n\ndouble abs_log(double x){\n  static RxODE_fn fun = NULL;\n  if (fun == NULL) fun = (RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_abs_log\");\n  return fun(x);\n}\n\ndouble abs_log1p(double x){\n  static RxODE_fn fun = NULL;\n  if (fun == NULL) fun = (RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_abs_log1p\");\n  return fun(x);\n}\n\ndouble factorial(double x){\n  static RxODE_fn fun = NULL;\n  if (fun == NULL) fun = (RxODE_fn) R_GetCCallable(\"RxODE\",\"RxODE_factorial\");\n  return fun(x);\n}\n\ndouble _transit4P(double t, rx_solve *rx, unsigned int id, double n, double mtt, double bio){\n  static RxODE_transit4P fun = NULL;\n  if (fun == NULL) fun = (RxODE_transit4P) R_GetCCallable(\"RxODE\",\"RxODE_transit4P\");\n  return fun(t, rx, id, n, mtt, bio);\n}\n\ndouble _transit3P(double t, rx_solve *rx, unsigned int id, double n, double mtt){\n  static RxODE_transit3P fun = NULL;\n  if (fun == NULL) fun = (RxODE_transit3P) R_GetCCallable(\"RxODE\",\"RxODE_transit3P\");\n  return fun(t, rx, id, n, mtt);\n}\n\ndouble podo(rx_solve *rx, unsigned int id){\n  static RxODE_val fun = NULL;\n  if (fun == NULL) fun = (RxODE_val) R_GetCCallable(\"RxODE\",\"RxODE_podoP\");\n  return fun(rx, id);\n}\n\ndouble tlast(rx_solve *rx, unsigned int id){\n  static RxODE_val fun = NULL;\n  if (fun == NULL) fun = (RxODE_val) R_GetCCallable(\"RxODE\",\"RxODE_tlastP\");\n  return fun(rx, id);\n}\n\nvoid _dadt_counter_inc(rx_solve *rx, unsigned int id){\n  static RxODE_inc fun = NULL;\n  if (fun == NULL) fun = (RxODE_inc) R_GetCCallable(\"RxODE\",\"RxODE_dadt_counter_incP\");\n  fun(rx, id);\n}\n\nvoid _jac_counter_inc(rx_solve *rx, unsigned int id){\n  static RxODE_inc fun = NULL;\n  if (fun == NULL) fun = (RxODE_inc) R_GetCCallable(\"RxODE\",\"RxODE_jac_counter_incP\");\n  fun(rx, id);\n}\n\nlong _dadt_counter_val(rx_solve *rx, unsigned int id){\n  static RxODE_cnt fun = NULL;\n  if (fun == NULL) fun = (RxODE_cnt) R_GetCCallable(\"RxODE\",\"RxODE_dadt_counter_valP\");\n  return fun(rx, id);\n}\n\nlong _jac_counter_val(rx_solve *rx, unsigned int id){\n  static RxODE_cnt fun = NULL;\n  if (fun == NULL) fun = (RxODE_cnt) R_GetCCallable(\"RxODE\",\"RxODE_jac_counter_valP\");\n  return fun(rx, id);\n}\n\nvoid _update_par_ptr(double t, rx_solve *rx, unsigned int id){\n  static RxODE_update_par_ptr fun = NULL;\n  if (fun == NULL) fun = (RxODE_update_par_ptr) R_GetCCallable(\"RxODE\",\"RxODE_update_par_ptrP\");\n  return fun(t, rx, id);\n}\n\ndouble _par_ptr(int val, rx_solve *rx, unsigned int id){\n  static RxODE_vec fun = NULL ;\n  if (fun == NULL) fun = (RxODE_vec) R_GetCCallable(\"RxODE\",\"RxODE_par_ptrP\");\n  return fun(val, rx, id);\n}\n\ndouble _InfusionRate(int val, rx_solve *rx, unsigned int id){\n  static RxODE_vec fun = NULL ;\n  if (fun == NULL) fun = (RxODE_vec) R_GetCCallable(\"RxODE\",\"RxODE_InfusionRateP\");\n  return fun(val, rx, id);\n}\n\ndouble _sum(int n, ...){\n  va_list valist;\n  va_start(valist, n);\n  double *p = Calloc(n, double);\n  for (unsigned int i = 0; i < n; i++){\n    p[i] = va_arg(valist, double);\n  }\n  va_end(valist);\n  double s = _sum1(p, n);\n  Free(p);\n  return s;\n}\n\ndouble _prod(int n, ...){\n  va_list valist;\n  va_start(valist, n);\n  double *p = Calloc(n, double);\n  for (unsigned int i = 0; i < n; i++){\n    p[i] = va_arg(valist, double);\n  }\n  va_end(valist);\n  double s = _prod1(p, n);\n  Free(p);\n  return s;\n}\n\nextern double _sign(unsigned int n, ...){\n  va_list valist;\n  va_start(valist, n);\n  double s = 1;\n  for (unsigned int i = 0; i < n; i++){\n    s = sign(va_arg(valist, double))*s;\n    if (s == 0){\n      break;\n    }\n  }\n  va_end(valist);\n  return s;\n}\n\nSEXP _rxModels;\nSEXP __MODEL_VARS__0();\nextern SEXP __MODEL_VARS__(){\n  SEXP _mv = _rxGetModelLib(__ODE_SOLVER_PTR_STR__);\n  if (isNull(_mv)){\n    _mv = __MODEL_VARS__0();\n    _assign_ptr(_mv);\n    return _mv;\n  } else {\n    return _mv;\n  }\n}\n\nrx_solve *_solveData = NULL;\nextern SEXP __ODE_SOLVER_XPTR__ ();\n\nextern void __ODE_SOLVER_SOLVEDATA__ (rx_solve *solve){\n  _solveData = solve;\n}\n\nextern rx_solve *__ODE_SOLVER_GET_SOLVEDATA__(){\n  return _solveData;\n}\n\nextern void __ODE_SOLVER_PTR__();\nSEXP __MODEL_VARS__();\nextern void __ODE_SOLVER__(int *neq,\n			   double *theta,      //order:\n			   double *time,\n			   int *evid,\n			   int *ntime,\n			   double *inits,\n			   double *dose,\n			   double *ret,\n			   double *atol,\n			   double *rtol,\n			   int *stiff,\n			   int *transit_abs,\n			   int *nlhs,\n			   double *lhs,\n			   int *rc){\n  __ODE_SOLVER_PTR__();\n  // Backward compatible ode solver for 0.5* C interface\n  _old_c(neq, theta, time, evid, ntime, inits, dose, ret, atol, rtol, stiff, transit_abs, nlhs, lhs, rc);\n}\n\nstatic R_NativePrimitiveArgType __ODE_SOLVER__rx_t[] = {\n  //*neq, *theta, *time,  *evid, *ntime, *inits,   *dose,   *ret,     *atol,  *rtol,   *stiff, *transit_abs, *nlhs, *lhs, *rc\n  INTSXP,REALSXP, REALSXP, INTSXP, INTSXP, REALSXP, REALSXP, REALSXP, REALSXP, REALSXP, INTSXP, INTSXP, INTSXP, REALSXP, INTSXP\n};\n\n"
#define __HD_SOLVE1__ "\nextern void __DYDT_LSODA__(int *neq, double *t, double *A, double *DADT)\n{\n  __DYDT__(neq, *t, A, DADT);\n}\n\nextern void __CALC_JAC_LSODA__(int *neq, double *t, double *A,int *ml, int *mu, double *JAC, int *nrowpd){\n  // Update all covariate parameters\n  __CALC_JAC__(neq, *t, A, JAC, *nrowpd);\n}\n\n\nextern SEXP __ODE_SOLVER_XPTR__  (){\n  return _assign_fn_xpointers(__DYDT__ , __CALC_LHS__ , __CALC_JAC__, __INIS__, __DYDT_LSODA__, __CALC_JAC_LSODA__,\n                              __ODE_SOLVER_SOLVEDATA__, __ODE_SOLVER_GET_SOLVEDATA__, __JT__ , __MF__,\n#ifdef __DEBUG__\n                              1\n#else\n                              0\n#endif\n                              );\n}\n\nextern void __ODE_SOLVER_PTR__  (){\n  _assign_ptr(__MODEL_VARS__());\n}\n//Initilize the dll to match RxODE's calls\nvoid __R_INIT__ (DllInfo *info){\n  // Register the outside functions\n  R_RegisterCCallable(__LIB_STR__,__ODE_SOLVER_STR__,       (DL_FUNC) __ODE_SOLVER__);\n  R_RegisterCCallable(__LIB_STR__,\"__ODE_SOLVER_XPTR__\",   (DL_FUNC) __ODE_SOLVER_XPTR__);\n  R_RegisterCCallable(__LIB_STR__,\"__ODE_SOLVER_PTR__\",   (DL_FUNC) __ODE_SOLVER_PTR__);\n  R_RegisterCCallable(__LIB_STR__,\"__INIS__\", (DL_FUNC) __INIS__);\n  R_RegisterCCallable(__LIB_STR__,\"__INIS__\", (DL_FUNC) __INIS__);\n  R_RegisterCCallable(__LIB_STR__,\"__DYDT__\", (DL_FUNC) __DYDT__);\n  R_RegisterCCallable(__LIB_STR__,\"__CALC_LHS__\", (DL_FUNC) __CALC_LHS__);\n  R_RegisterCCallable(__LIB_STR__,\"__CALC_JAC__\", (DL_FUNC) __CALC_JAC__);\n  R_RegisterCCallable(__LIB_STR__,\"__DYDT_LSODA__\", (DL_FUNC) __DYDT_LSODA__);\n  R_RegisterCCallable(__LIB_STR__,\"__ODE_SOLVER_SOLVEDATA__\", (DL_FUNC) __ODE_SOLVER_SOLVEDATA__);\n  R_RegisterCCallable(__LIB_STR__,\"__ODE_SOLVER_GET_SOLVEDATA__\", (DL_FUNC) __ODE_SOLVER_GET_SOLVEDATA__);\n  \n  static const R_CMethodDef cMethods[] = {\n    {__ODE_SOLVER_STR__, (DL_FUNC) &__ODE_SOLVER__, 15, __ODE_SOLVER__rx_t},\n    {NULL, NULL, 0, NULL}\n  };\n  \n  R_CallMethodDef callMethods[]  = {\n    {__ODE_SOLVER_XPTR_STR__, (DL_FUNC) &__ODE_SOLVER_XPTR__, 0},\n    {__ODE_SOLVER_PTR_STR__, (DL_FUNC) &__ODE_SOLVER_PTR__, 0},\n    {__MODEL_VARS_STR__, (DL_FUNC) &__MODEL_VARS__, 0},\n    {NULL, NULL, 0}\n  };\n  R_registerRoutines(info, cMethods, callMethods, NULL, NULL);\n  R_useDynamicSymbols(info,FALSE);\n}\n\nvoid __R_UNLOAD__ (DllInfo *info){\n  // Free resources required for single subject solve.\n  SEXP _mv = _rxGetModelLib(__ODE_SOLVER_PTR_STR__);\n  if (!isNull(_mv)){\n    _rxRmModelLib(__ODE_SOLVER_PTR_STR__);\n  }\n}"
#define __HD_SOLVE2__ ""
