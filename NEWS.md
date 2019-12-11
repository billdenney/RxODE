# RxODE xxxx
## New features
* Added ability to prune branching with `rxPrune`. This converts
  `if`/`else` or `ifelse` to single line statements without any
  `if`/`then` branching within them.
  
* Added ability to take more complex conditional expressions, including:
  - `ifelse(expr, yes, no)`
  - `x = (x==1)*1 + (!(x==1))*2`
  - `if (logic){ expr} else if (logic) {expr} else {}`.  The preferred
    syntax is still only `if`/`else` and the corresponding parsed code
    reflects this preference.
    - Note `ifelse` is not allowed as an ODE compartment or a variable.
	
* Switched to `symengine` instead of using `sympy`
  - Remove dependence on python.
  - Since symengine is C-based and doesn't require the python
    interface it is much faster than `sympy`, though some functions in
    `sympy` are no longer accessible.
	
* Added new ODE solving method "indLin", or inductive linearization.
  When the full model is a linear ODE system this becomes simply the
  matrix exponential solution.  Currently this requires a different
  setup.

* Added arbitrary function definition to RxODE using `rxFun`
  - Requires function, arguments and corresponding C-code
  - Derivatives (if required) can be added to the derivative table
    `rxD`.  When taking deviates without a derivative function, RxODE
    will use numerical differences.

* Will error if RxODE does not know of a function that you are trying
  to use; This could be a breaking change.  Currently:
  - C's functions from `math.h` are supported
  - R's function returning and taking doubles are supported
  - Other functions can be added using `rxFun` and `rxD`

* Added `NA`, `NaN`, `Inf` and `+Inf` handling to a RxODE model.  Can
  be useful to diagnose problems in models and provide alternate
  solutions. In addition, added R-like functions `is.nan`, `is.na`,
  `is.finite` and `is.infinite` which can be called within the RxODE
  block.

* Allowed the following data variables to be accessed (but not
  assigned or used as a state):
  - `cmt`
  - `dvid`
  - `addl`
  - `ss`
  - `amt`
  - `rate`

* Kept `evid` and `ii` as restricted items since they are not part of
  the covariate table and are restricted in use.
  
* Added the following random number generators; They are thread safe
  (based on `threefry` `sitmo` and c++11) and your simulations with
  them will depend on the number of cores used in your simulation (Be
  careful about reproduciblility with large number of threads; Also
  use parallel-solve type of RxODE simulations to avoid the [birthday
  problem](https://www.johndcook.com/blog/2016/01/29/random-number-generator-seed-mistakes/))
  
  These are:
  - `rxnorm()` and `rxnormV()` (low discrepancy normal)
  - `rxcauchy()`
  - `rxchisq()`
  - `rxexp()`
  - `rxf()`
  - `rxgamma()`
  - `rxbeta()`
  - `rxgeom()`
  - `rxpois()`
  - `rxt()`
  - `rxunif()`
  - `rxweibull()`
  
* Updated to [TimSort 1.2.0](https://github.com/timsort/cpp-TimSort/releases/tag/v1.2.0)
  
## Bug fixes:
 - Occasionally RxODE misidentified dual `lhs`/`param` values.  An
   additional check is performed so that this does not happen.
 - Transit compartment models using the `transit` ODE or variable are
   now allowed.  Also check for more internally parsed items (see
   Issue #145).
   

# RxODE v0.9.1-8
* Added the ability to have an input parameter to be assigned to a new
  value (Issue #135)
* Added LINPACK authors as contributors
* Added a `NEWS.md` file to track changes to the package