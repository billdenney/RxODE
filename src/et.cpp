#include <Rcpp.h>
using namespace Rcpp;

bool rxIs(const RObject &obj, std::string cls);
Environment RxODEenv();

RObject evCur;

Function loadNamespace2("loadNamespace", R_BaseNamespace);
Environment unitsPkg = loadNamespace2("units");
//(x, value, ..., mode = units_options("set_units_mode"))
NumericVector setUnits(NumericVector obj, std::string unit){
  Function f = as<Function>(unitsPkg["set_units"]);
  if (unit == ""){
    obj.attr("class") = R_NilValue;
    obj.attr("units") = R_NilValue;
    return obj;
  } else {
    return as<NumericVector>(f(_["x"] = obj, _["value"] = unit, _["mode"] = "standard"));
  }
}

extern "C" void getWh(int evid, int *wh, int *cmt, int *wh100, int *whI, int *wh0);

//[[Rcpp::export]]
RObject etUpdate(RObject obj,
		 RObject arg = R_NilValue,
		 RObject value = R_NilValue){
  if (rxIs(obj,"rxEt")){
    evCur = obj;
    if (rxIs(value, "NULL")){
      CharacterVector cls = clone(as<CharacterVector>(obj.attr("class")));
      List e = clone(as<List>(cls.attr(".RxODE.lst")));
      if (rxIs(arg, "character")){
	CharacterVector carg = as<CharacterVector>(arg);
	std::string sarg = as<std::string>(carg[0]);
	if (sarg == "env"){
	  return as<RObject>(e);
	} else if (e.containsElementNamed(sarg.c_str())){
	  return as<RObject>(e[sarg]);
	}
	List lst = as<List>(obj);
	if (lst.containsElementNamed(sarg.c_str())){
	  return(as<RObject>(lst[sarg]));
	}
      }
    } else {
      // Assign
    }
  }
  return R_NilValue;
}

List etEmpty(CharacterVector units){
  CharacterVector cls = CharacterVector::create("rxEt","data.frame");
  List e;
  e["units"] = clone(units);
  Function parse2("parse", R_BaseNamespace);
  Function eval2("eval", R_BaseNamespace);
  // eventTable style methods
  std::string getUnits= "function() .Call(`_RxODE_et_`, list(getUnits=TRUE) ,list('last'))";
  e["get.units"] = eval2(_["expr"]   = parse2(_["text"]=getUnits),
			 _["envir"]  = e);
  
  e["getUnits"] = eval2(_["expr"]   = parse2(_["text"]=getUnits),
			_["envir"]  = e);
  e["get_units"] = eval2(_["expr"]   = parse2(_["text"]=getUnits),
			 _["envir"]  = e);

  std::string addDosing= "function (dose, nbr.doses = 1L, dosing.interval = 24, \n    dosing.to = 1L, rate = NULL, amount.units = NA_character_, \n    start.time = 0, do.sampling = FALSE, time.units = NA_character_, \n    ...) \n{\n    .lst <- list(dose = dose, nbr.doses = nbr.doses, start.time = start.time, \n        do.sampling = do.sampling)\n    if (!is.na(amount.units)) \n        .lst$amount.units <- amount.units\n    if (!is.na(time.units)) \n        .lst$time.units <- time.units\n    if (dosing.to != 1) \n        .lst$dosing.to <- dosing.to\n    if (!is.null(rate)) \n        .lst$rate <- rate\n    .lst$dosing.interval <- dosing.interval\n    invisible(.Call(`_RxODE_et_`, .lst, list('last')))\n}";

  e["add.dosing"] = eval2(_["expr"] = parse2(_["text"] = addDosing),
			  _["envir"]  = e);
  e["add_dosing"] = eval2(_["expr"] = parse2(_["text"] = addDosing),
			  _["envir"]  = e);
  e["addDosing"] = eval2(_["expr"] = parse2(_["text"] = addDosing),
			 _["envir"]  = e);

  std::string addSampling="function(time, time.units = NA) {\n  .lst <- list();\n  .lst$time<-time;\n  if(!is.na(time.units)) .lst$time.units<- time.units\n  invisible(.Call(`_RxODE_et_`, .lst, list('last')));\n}";

  e["add.sampling"] = eval2(_["expr"] = parse2(_["text"] = addSampling),
			    _["envir"]  = e);
  e["add_sampling"] = eval2(_["expr"] = parse2(_["text"] = addSampling),
			    _["envir"]  = e);
  e["addSampling"] = eval2(_["expr"] = parse2(_["text"] = addSampling),
			   _["envir"]  = e);
  
  e["clear.sampling"] = eval2(_["expr"] = parse2(_["text"] = "function() invisible(.Call(`_RxODE_et_`, list(clearSampling=TRUE),list('last')))"),
			      _["envir"]  = e);

  e["clear_sampling"] = eval2(_["expr"] = parse2(_["text"] = "function() invisible(.Call(`_RxODE_et_`, list(clearSampling=TRUE),list('last')))"),
			      _["envir"]  = e);

  e["clearSampling"] = eval2(_["expr"] = parse2(_["text"] = "function() invisible(.Call(`_RxODE_et_`, list(clearSampling=TRUE),list('last')))"),
			     _["envir"]  = e);


  e["clear.dosing"] = eval2(_["expr"] = parse2(_["text"] = "function() invisible(.Call(`_RxODE_et_`, list(clearDosing=TRUE),list('last')))"),
			    _["envir"]  = e);

  e["clear_dosing"] = eval2(_["expr"] = parse2(_["text"] = "function() invisible(.Call(`_RxODE_et_`, list(clearDosing=TRUE),list('last')))"),
			    _["envir"]  = e);

  e["clearDosing"] = eval2(_["expr"] = parse2(_["text"] = "function() invisible(.Call(`_RxODE_et_`, list(clearDosing=TRUE),list('last')))"),
			   _["envir"]  = e);

  std::string importET = "function(data) invisible(.Call(`_RxODE_et_`, list(data=data),list('import')))";

  e["import.EventTable"] = eval2(_["expr"] = parse2(_["text"] = importET),
				 _["envir"]  = e);

  e["importEventTable"] = eval2(_["expr"] = parse2(_["text"] = importET),
				_["envir"]  = e);

  e["copy"] = eval2(_["expr"] = parse2(_["text"] = "function() .Call(`_RxODE_et_`, list(copy=TRUE),list('last'))"),
		    _["envir"]  = e);
  
  e["get.EventTable"] = eval2(_["expr"] = parse2(_["text"] = "function() .Call(`_RxODE_et_`, list(get.EventTable=TRUE),list('last'))"),
			      _["envir"]  = e);
  e["getEventTable"] = eval2(_["expr"] = parse2(_["text"] = "function() .Call(`_RxODE_et_`, list(get.EventTable=TRUE),list('last'))"),
			     _["envir"]  = e);
  e["get.obs.rec"] = eval2(_["expr"] = parse2(_["text"] = "function() .Call(`_RxODE_et_`, list(get.obs.rec=TRUE),list('last'))"),
			   _["envir"]  = e);

  e["get.nobs"] = eval2(_["expr"] = parse2(_["text"] = "function() .Call(`_RxODE_et_`, list(get.nobs=TRUE),list('last'))"),
			_["envir"]  = e);


  e["get.dosing"] = eval2(_["expr"] = parse2(_["text"] = "function() .Call(`_RxODE_et_`, list(get.dosing=TRUE),list('last'))"),
			  _["envir"]  = e);
  e["getDosing"] = eval2(_["expr"] = parse2(_["text"] = "function() .Call(`_RxODE_et_`, list(get.dosing=TRUE),list('last'))"),
			 _["envir"]  = e);
  
  e["get.sampling"] = eval2(_["expr"] = parse2(_["text"] = "function() .Call(`_RxODE_et_`, list(get.sampling=TRUE),list('last'))"),
			    _["envir"]  = e);

  e["getSampling"] = eval2(_["expr"] = parse2(_["text"] = "function() .Call(`_RxODE_et_`, list(get.sampling=TRUE),list('last'))"),
			   _["envir"]  = e);
  e["nobs"] = 0;
  e["ndose"] = 0;
  e["show"] = LogicalVector::create(_["id"] = false, _["low"] = false,_["time"] = true, _["high"] = false,
				    _["cmt"] =false, _["amt"]=false, _["rate"] = false,
				    _["ii"] = false, _["addl"] = false,
				    _["evid"] = true, _["ss"] = false);
  e["IDs"] = IntegerVector::create(1);

  e["canResize"] = true;

  // Return an empty data frame.
  List lst(11);
  CharacterVector nme(11);
  nme[0] = "id";
  lst[0] = IntegerVector(0);
      
  nme[2] = "time";
  lst[2] = NumericVector(0);
      
  nme[1] = "low";
  lst[1] = NumericVector(0);
      
  nme[3] = "high";
  lst[3] = NumericVector(0);
      
  nme[4] = "cmt";
  lst[4] = CharacterVector(0);
      
  nme[5] = "amt";
  lst[5] = NumericVector(0);

  nme[6] = "rate";
  lst[6] = NumericVector(0);
      
  nme[7] = "ii";
  lst[7] = NumericVector(0);
      
  nme[8] = "addl";
  lst[8] = IntegerVector(0);
      
  nme[9] = "evid";
  lst[9] = IntegerVector(0);
      
  nme[10] = "ss";
  lst[10] = IntegerVector(0);

  e.attr("class") = "rxHidden";

  cls.attr(".RxODE.lst") = e;

  lst.attr("names") = nme;
  lst.attr("class") = cls;
  lst.attr("row.names") = IntegerVector::create(NA_INTEGER, 0);
  if (!CharacterVector::is_na(units[1])){
    lst["low"]  = setUnits(lst["low"],  as<std::string>(units[1]));
    lst["time"] = setUnits(lst["time"], as<std::string>(units[1]));
    lst["high"] = setUnits(lst["high"], as<std::string>(units[1]));
    lst["ii"]  = setUnits(lst["ii"],  as<std::string>(units[1]));
  } else {
    lst["low"]  = setUnits(lst["low"], "");
    lst["ii"]  = setUnits(lst["ii"], "");
    lst["time"] = setUnits(lst["time"],"");
    lst["high"] = setUnits(lst["high"], "");
  }
  if (!CharacterVector::is_na(units[0])){
    lst["amt"] = setUnits(lst["amt"], as<std::string>(units[0]));
  } else {
    lst["amt"] = setUnits(lst["amt"], "");
  }
  return lst;
}

List etSort(List curEt){
  std::vector<double> time = as<std::vector<double>>(curEt["time"]);
  std::vector<int> id = as<std::vector<int>>(curEt["id"]);
  std::vector<int> evid = as<std::vector<int>>(curEt["evid"]);
  std::vector<int> idx(id.size());
  std::iota(idx.begin(),idx.end(),0);
  std::sort(idx.begin(),idx.end(),
	    [id,time,evid](int a, int b){
	      if (id[a] == id[b]){
		if (time[a] == time[b]){
		  if (evid[a] == evid[b]){
		    return a < b;
		  }
		  return evid[a] < evid[b];
		}
		return time[a] < time[b];
	      }
	      return id[a] < id[b];
	    });
  List newEt(curEt.size());
  int i, j, newSize = time.size();
  IntegerVector tmpI, tmpI2;
  CharacterVector tmpC, tmpC2;
  NumericVector tmpN, tmpN2;
  for (j = newEt.size(); j--;){
    for (i = newSize;i--;){
      if (rxIs(curEt[j], "numeric")) {
	if (i == newSize-1) newEt[j] = NumericVector(newSize);
	tmpN=newEt[j];
	tmpN2 = curEt[j];
	tmpN[i] = tmpN2[idx[i]];
      } else if (rxIs(curEt[j], "integer")) {
	if (i == newSize-1) newEt[j] = IntegerVector(newSize);
	tmpI=newEt[j];
	tmpI2 = curEt[j];
	tmpI[i] = tmpI2[idx[i]];
      } else if (rxIs(curEt[j], "character")){
	if (i == newSize-1) newEt[j] = CharacterVector(newSize);
	tmpC=newEt[j];
	tmpC2 = curEt[j];
	tmpC[i] = tmpC2[idx[i]];
      } 
    }
  }
  newEt.attr("class") = clone(as<CharacterVector>(curEt.attr("class")));
  newEt.attr("names") = curEt.attr("names");
  return newEt;
}


List etAddWindow(List windowLst, IntegerVector IDs, RObject cmt, bool turnOnShowCmt, List curEt){
  std::vector<double> time = as<std::vector<double>>(curEt["time"]);
  std::vector<double> low = as<std::vector<double>>(curEt["low"]);
  std::vector<double> high = as<std::vector<double>>(curEt["high"]);
  std::vector<int> id = as<std::vector<int>>(curEt["id"]);
  int oldSize =id.size();
  std::vector<int> idx(oldSize+windowLst.size()*IDs.size());
  std::vector<int> evid = as<std::vector<int>>(curEt["evid"]);
  std::iota(idx.begin(),idx.end(),0);
  double c = 0;
  CharacterVector cls = clone(as<CharacterVector>(curEt.attr("class")));
  List eOld = cls.attr(".RxODE.lst");
  List e = clone(eOld);
  CharacterVector units = e["units"];
  int nobs=0;
  for (int j = IDs.size(); j--;){
    for (int i = windowLst.size(); i--;){
      NumericVector cur = as<NumericVector>(windowLst[i]);
      if (rxIs(cur, "units")){
	if (!CharacterVector::is_na(units["time"])){
	  cur = setUnits(cur, as<std::string>(units["time"]));
	} else {
	  cur = setUnits(cur, "");
	}
      }
      if (cur.size() != 2)
	stop("Windows need to be a list of observation windows, each of 2 elements e.g. list(c(0,2), c(2,7)).");
      if (cur[0]> cur[1])
	stop("Windows need to be ordered list(c(2,0)) is invalid.");
      id.push_back(IDs[j]);
      low.push_back(cur[0]);
      high.push_back(cur[1]);
      c = Rf_runif(cur[0], cur[1]);
      time.push_back(c);
      evid.push_back(0);
      nobs++;
    }
  }
  std::sort(idx.begin(),idx.end(),
	    [id,time,evid](int a, int b){
	      if (id[a] == id[b]){
		if (time[a] == time[b]){
		  if (evid[a] == evid[b]){
		    return a < b;
		  }
		  return evid[a] < evid[b];
		}
		return time[a] < time[b];
	      }
	      return id[a] < id[b];
	    });
  List lst(curEt.size());
  IntegerVector tmpI = as<IntegerVector>(curEt["id"]), tmpI2;
  NumericVector tmpN, tmpN2;
  CharacterVector tmpC, tmpC2;
  lst.attr("names") = curEt.attr("names");
  int i, j;
  // nme[0] = "id";
  lst[0] = IntegerVector(id.size());
      
  // nme[1] = "time";
  lst[2] = NumericVector(id.size());
      
  // nme[2] = "low";
  lst[1] = NumericVector(id.size());
      
  // nme[3] = "high";
  lst[3] = NumericVector(id.size());
      
  // nme[4] = "cmt";
  bool isCmtInt=false;
  if (!rxIs(cmt, "integer")){
    lst[4] = CharacterVector(id.size());
  } else { 
    lst[4] = IntegerVector(id.size());
    isCmtInt=true;
  }
  // nme[5] = "amt";
  lst[5] = NumericVector(id.size());

  // nme[6] = "rate";
  lst[6] = NumericVector(id.size());
      
  // nme[7] = "ii";
  lst[7] = NumericVector(id.size());
      
  // nme[8] = "addl";
  lst[8] = IntegerVector(id.size());
  
  // nme[9] = "evid";
  lst[9] = IntegerVector(id.size());
      
  // nme[10] = "ss";
  lst[10] = IntegerVector(id.size());
  for (i = idx.size(); i--;){
    tmpI = as<IntegerVector>(lst[0]); // id
    tmpI[i] = id[idx[i]];
    
    tmpI = as<IntegerVector>(lst[9]); // evid
    tmpI[i] = evid[idx[i]];

    tmpN = as<NumericVector>(lst[2]); // time
    tmpN[i] = time[idx[i]];

    // low
    tmpN = as<NumericVector>(lst[1]);
    tmpN[i] = low[idx[i]];

    // hi
    tmpN = as<NumericVector>(lst[3]);
    tmpN[i] = high[idx[i]];
    if (idx[i] >= oldSize){
      if (isCmtInt){
	tmpI = as<IntegerVector>(lst[4]);
	tmpI[i] = as<int>(cmt);
      } else {
	tmpC = as<CharacterVector>(lst[4]);
	tmpC2 = as<CharacterVector>(cmt);
	tmpC[i] = tmpC2[0];
      }
      
      // nme[5] = "amt";
      tmpN = as<NumericVector>(lst[5]);
      tmpN[i] = NA_REAL;

      // nme[6] = "rate";
      tmpN = as<NumericVector>(lst[6]);
      tmpN[i] = NA_REAL;
      
      // nme[7] = "ii";
      tmpN = as<NumericVector>(lst[7]);
      tmpN[i] = NA_REAL;
      
      // nme[8] = "addl";
      tmpI = as<IntegerVector>(lst[8]); // id
      tmpI[i] = NA_REAL;
  
      // nme[10] = "ss";
      tmpI = as<IntegerVector>(lst[10]); // id
      tmpI[i] = NA_REAL;
    } else {
      for (j = 11; j--;){
	if (rxIs(curEt[j], "numeric")){
	  tmpN = as<NumericVector>(lst[j]);
	  tmpN2 = as<NumericVector>(curEt[j]);
	  tmpN[i] = tmpN2[idx[i]];
	} else if (rxIs(curEt[j], "integer")) {
	  tmpI = as<IntegerVector>(lst[j]);
	  tmpI2 = as<IntegerVector>(curEt[j]);
	  tmpI[i] = tmpI2[idx[i]];
	} else if (rxIs(curEt[j], "character")){
	  // Char
	  tmpC = as<CharacterVector>(lst[j]);
	  tmpC2 = as<CharacterVector>(curEt[j]);
	  tmpC[i] = tmpC2[idx[i]];	
	}
      }
    }
  }
  e["nobs"] = as<int>(e["nobs"]) + nobs;
  LogicalVector show = e["show"];
  if (turnOnShowCmt){
    show["cmt"] = true;
  }
  std::vector<double> uIds = as<std::vector<double>>(eOld["IDs"]);
  for (i = IDs.size(); i--;){
    if (std::find(uIds.begin(), uIds.end(), IDs[i]) == uIds.end()){
      uIds.push_back(IDs[i]);
    }
  }
  if ((int)IDs.size() == (int)uIds.size() && as<bool>(eOld["canResize"])){
    e["canResize"] = true;
  } else {
    e["canResize"] = false;
  }
  e["IDs"] = wrap(uIds);
  if (uIds.size() > 1){
    show["id"] = true;
  }
  show["low"] = true;
  show["high"] = true;
  e["show"] = show;
  
  e.attr("class") = "rxHidden";
  cls.attr(".RxODE.lst") = e;
  lst.attr("class") = cls;
  int len = as<int>(e["nobs"]) +as<int>(e["ndose"]);
  lst.attr("row.names") = IntegerVector::create(NA_INTEGER, -len);
  return lst;
}

List etAddTimes(NumericVector newTimes, IntegerVector IDs, RObject cmt, bool turnOnShowCmt, List curEt){
  CharacterVector cls = clone(as<CharacterVector>(curEt.attr("class")));
  List eOld = cls.attr(".RxODE.lst");
  List e = clone(eOld);

  std::vector<double> time = as<std::vector<double>>(curEt["time"]);
  std::vector<int> id = as<std::vector<int>>(curEt["id"]);
  int oldSize =id.size();
  std::vector<int> idx(oldSize+newTimes.size()*IDs.size());
  std::vector<int> evid = as<std::vector<int>>(curEt["evid"]);
  std::iota(idx.begin(),idx.end(),0);
  int nobs = 0;

  for (int j = IDs.size(); j--;){
    for (int i = newTimes.size(); i--;){
      id.push_back(IDs[j]);
      time.push_back(newTimes[i]);
      evid.push_back(0);
      nobs++;
    }
  }
  std::sort(idx.begin(),idx.end(),
	    [id,time,evid](int a, int b){
	      if (id[a] == id[b]){
		if (time[a] == time[b]){
		  if (evid[a] == evid[b]){
		    return a < b;
		  }
		  return evid[a] < evid[b];
		}
		return time[a] < time[b];
	      }
	      return id[a] < id[b];
	    });

  List lst(curEt.size());
  IntegerVector tmpI = as<IntegerVector>(curEt["id"]), tmpI2;
  NumericVector tmpN, tmpN2;
  CharacterVector tmpC, tmpC2;
  lst.attr("names") = curEt.attr("names");
  int i, j;
  // nme[0] = "id";
  lst[0] = IntegerVector(id.size());
      
  // nme[1] = "time";
  lst[2] = NumericVector(id.size());
      
  // nme[2] = "low";
  lst[1] = NumericVector(id.size());
      
  // nme[3] = "high";
  lst[3] = NumericVector(id.size());
      
  // nme[4] = "cmt";
  bool isCmtInt = false;
  if (rxIs(cmt, "integer")){
    lst[4] = IntegerVector(id.size());
    isCmtInt=true;
  } else {
    lst[4] = CharacterVector(id.size());
  }
      
  // nme[5] = "amt";
  lst[5] = NumericVector(id.size());

  // nme[6] = "rate";
  lst[6] = NumericVector(id.size());
      
  // nme[7] = "ii";
  lst[7] = NumericVector(id.size());
      
  // nme[8] = "addl";
  lst[8] = IntegerVector(id.size());
  
  // nme[9] = "evid";
  lst[9] = IntegerVector(id.size());
      
  // nme[10] = "ss";
  lst[10] = IntegerVector(id.size());
  for (i = idx.size(); i--;){
    tmpI = as<IntegerVector>(lst[0]); // id
    tmpI[i] = id[idx[i]];
    
    tmpI = as<IntegerVector>(lst[9]); // evid
    tmpI[i] = evid[idx[i]];

    tmpN = as<NumericVector>(lst[2]); // time
    tmpN[i] = time[idx[i]];
    if (idx[i] >= oldSize){
      // low
      tmpN = as<NumericVector>(lst[1]);
      tmpN[i] = NA_REAL;
      
      // hi
      tmpN = as<NumericVector>(lst[3]);
      tmpN[i] = NA_REAL;

      if (isCmtInt){
	tmpI = as<IntegerVector>(lst[4]);
	tmpI[i] = as<int>(cmt);
      } else {
	tmpC = as<CharacterVector>(lst[4]);
	tmpC2 = as<CharacterVector>(cmt);
	tmpC[i] = tmpC2[0];
      }

      // nme[5] = "amt";
      tmpN = as<NumericVector>(lst[5]);
      tmpN[i] = NA_REAL;

      // nme[6] = "rate";
      tmpN = as<NumericVector>(lst[6]);
      tmpN[i] = NA_REAL;
      
      // nme[7] = "ii";
      tmpN = as<NumericVector>(lst[7]);
      tmpN[i] = NA_REAL;
      
      // nme[8] = "addl";
      tmpI = as<IntegerVector>(lst[8]); // id
      tmpI[i] = NA_REAL;
  
      // nme[10] = "ss";
      tmpI = as<IntegerVector>(lst[10]); // id
      tmpI[i] = NA_REAL;
    } else {
      // low
      for (j = 11; j--;){
	if (rxIs(curEt[j], "numeric")){
	  tmpN = as<NumericVector>(lst[j]);
	  tmpN2 = as<NumericVector>(curEt[j]);
	  tmpN[i] = tmpN2[idx[i]];
	} else if (rxIs(curEt[j], "integer")) {
	  tmpI = as<IntegerVector>(lst[j]);
	  tmpI2 = as<IntegerVector>(curEt[j]);
	  tmpI[i] = tmpI2[idx[i]];
	} else if (rxIs(curEt[j], "character")){
	  // Char
	  tmpC = as<CharacterVector>(lst[j]);
	  tmpC2 = as<CharacterVector>(curEt[j]);
	  tmpC[i] = tmpC2[idx[i]];	
	}
      }
    }
  }
  e["nobs"] = as<int>(e["nobs"]) + nobs;
  LogicalVector show = e["show"];
  if (turnOnShowCmt){
    show["cmt"] = true;
  }
  std::vector<double> uIds = as<std::vector<double>>(eOld["IDs"]);
  for (i = IDs.size(); i--;){
    if (std::find(uIds.begin(), uIds.end(), IDs[i]) == uIds.end()){
      uIds.push_back(IDs[i]);
    }
  }
  if (uIds.size() > 1){
    show["id"] = true;
  }
  if ((int)IDs.size() == (int)uIds.size() && as<bool>(eOld["canResize"])){
    e["canResize"] = true;
  } else {
    e["canResize"] = false;
  }
  e["IDs"] = wrap(uIds);
  e["show"] = show;
  e.attr("names") = eOld.attr("names");
  e.attr("class") = "rxHidden";//eOld.attr("class");
  cls.attr(".RxODE.lst") = e;
  lst.attr("class") = cls;
  int len = as<int>(e["nobs"]) +as<int>(e["ndose"]);
  lst.attr("row.names") = IntegerVector::create(NA_INTEGER, -len);
  return lst;
}

List etImportEventTable(List inData){
  CharacterVector lName = as<CharacterVector>(inData.attr("names"));
  int i, idCol = -1, evidCol=-1, timeCol=-1, amtCol=-1, cmtCol=-1,
    ssCol=-1, rateCol=-1, addlCol=-1, iiCol=-1, j;
  std::string tmpS;
  for (i = lName.size(); i--;){
    tmpS = as<std::string>(lName[i]);
    std::transform(tmpS.begin(), tmpS.end(), tmpS.begin(), ::tolower);
    lName[i] = tmpS;
    if (tmpS == "id") idCol=i;
    else if (tmpS == "evid") evidCol=i;
    else if (tmpS == "time") timeCol=i;
    else if (tmpS == "amt") amtCol=i;
    else if (tmpS == "cmt" || tmpS == "ytype") cmtCol=i;
    else if (tmpS == "ss")   ssCol=i;
    else if (tmpS == "rate") rateCol=i;
    else if (tmpS == "addl") addlCol=i;
    else if (tmpS == "ii")   iiCol=i;
  }
  if (evidCol == -1) stop("Need evid column.");
  IntegerVector oldEvid=as<IntegerVector>(inData[evidCol]);
  std::vector<int> evid;
  
  IntegerVector oldId;
  if (idCol == -1){
    oldId=IntegerVector(oldEvid.size(), 1);
  } else {
    oldId=as<IntegerVector>(inData[idCol]);
  }
  std::vector<int> id;
  
  std::vector<double> low;

  NumericVector oldTime;
  if (timeCol == -1){
    stop("Need a time column");
  } else {
    oldTime=as<NumericVector>(inData[timeCol]);
  }
  std::vector<double> time;
  std::vector<double> high;
  
  std::vector<double> rate;
  NumericVector oldRate;
  if (rateCol == -1){
    oldRate = NumericVector(oldEvid.size(), 0.0);
  } else {
    oldRate = as<NumericVector>(inData[rateCol]);
  }
  
  std::vector<double> ii;
  NumericVector oldIi;
  if (iiCol == -1){
    oldIi = NumericVector(oldEvid.size(), 0.0);
  } else {
    oldIi = as<NumericVector>(inData[iiCol]);
  }
  
  std::vector<int> addl;
  IntegerVector oldAddl;
  if (addlCol == -1){
    oldAddl = IntegerVector(oldEvid.size(), 0);
  } else {
    oldAddl = as<IntegerVector>(inData[addlCol]);
  }
  
  std::vector<double> ss;
  IntegerVector oldSs;
  if (ssCol == -1){
    oldSs = IntegerVector(oldEvid.size(), 0);
  } else {
    oldSs = as<IntegerVector>(inData[ssCol]);
  }
  
  std::vector<double> amt;
  NumericVector oldAmt;
  if (amtCol == -1){
    oldAmt = NumericVector(oldEvid.size(), 0);
  } else {
    oldAmt = as<NumericVector>(inData[amtCol]);
  }
  
  std::vector<int> cmt;
  IntegerVector oldCmt;
  if (cmtCol == -1){
    oldCmt = IntegerVector(oldEvid.size(), 0);
  } else {
    oldCmt = as<IntegerVector>(inData[cmtCol]);
  }
  int wh, cmtI, wh100, whI, wh0, ndose=0, nobs=0;
  List lst = etEmpty(CharacterVector::create(_["dosing"]=NA_STRING, _["time"]=NA_STRING));
  CharacterVector cls = lst.attr("class");
  List e = cls.attr(".RxODE.lst");
  LogicalVector show = e["show"];
  show["id"] = true;
  show["amt"] = true;
  std::vector<int> uIds;
  for (int i = 0; i < oldEvid.size(); i++){
    if (oldEvid[i] == 0){
      id.push_back(oldId[i]);
      if (std::find(uIds.begin(), uIds.end(), oldId[i]) == uIds.end()){
	uIds.push_back(oldId[i]);
      }
      low.push_back(NA_REAL);
      if (ISNA(oldTime[i])){
	time.push_back(oldTime[i]);
	high.push_back(NA_REAL);
	cmt.push_back(oldCmt[i]);
	if (oldCmt[i] > 1) show["cmt"] = true;
	amt.push_back(0);
	rate.push_back(0);
	ii.push_back(0);
	addl.push_back(0);
	evid.push_back(2);
	ss.push_back(0);
	ndose++;
      } else {
	time.push_back(oldTime[i]);
	high.push_back(NA_REAL);
	cmt.push_back(oldCmt[i]);
	if (oldCmt[i] > 1) show["cmt"] = true;
	amt.push_back(0);
	rate.push_back(0);
	ii.push_back(0);
	addl.push_back(0);
	evid.push_back(2);
	ss.push_back(0);
	nobs++;
      }
    } else if (oldEvid[i] <= 4){
      id.push_back(oldId[i]);
      if (std::find(uIds.begin(), uIds.end(), oldId[i]) == uIds.end()){
	uIds.push_back(oldId[i]);
      }
      low.push_back(NA_REAL);
      time.push_back(oldTime[i]);
      high.push_back(NA_REAL);
      cmt.push_back(oldCmt[i]);
      if (oldCmt[i] > 1) show["cmt"] = true;
      amt.push_back(oldAmt[i]);
      rate.push_back(oldRate[i]);
      if (oldRate[i] > 0) show["rate"] = true;
      ii.push_back(oldIi[i]);
      if (oldIi[i] > 0) show["ii"] = true;
      addl.push_back(oldAddl[i]);
      if (oldAddl[i] > 0) show["addl"] = true;
      evid.push_back(oldEvid[i]);
      ss.push_back(oldSs[i]);
      if (oldSs[i] > 0) show["ss"] = true;
      ndose++;
    } else {
      // Convert evid
      getWh(oldEvid[i], &wh, &cmtI, &wh100, &whI, &wh0);
      cmtI++;
      if (cmtI != 1) show["cmt"] = true;
      if (oldIi[i] > 0) show["ii"] = true;
      switch (whI){
      case 6:
	break;
      case 8:
	// 8 = Duration is modeled, AMT=dose; Rate = AMT/(Modeled Duration) NONMEM RATE=-2
	id.push_back(oldId[i]);
	if (std::find(uIds.begin(), uIds.end(), oldId[i]) == uIds.end()){
	  uIds.push_back(oldId[i]);
	}
	low.push_back(NA_REAL);
	time.push_back(oldTime[i]);
	high.push_back(NA_REAL);
	cmt.push_back(cmtI);
	amt.push_back(oldAmt[i]);
	rate.push_back(-2.0);
	ii.push_back(oldIi[i]);
	addl.push_back(0);
	evid.push_back(1);
	if (whI == 10){
	  ss.push_back(1);
	  show["ss"] = true;
	} else if (whI == 20){
	  ss.push_back(2);
	  show["ss"] = true;
	} else {
	  ss.push_back(0);
	}
	show["rate"] = true;
	ndose++;
	break;
      case 7:
	break;
      case 9:
	// 9 = Rate is modeled, AMT=dose; Duration = AMT/(Modeled Rate) NONMEM RATE=-1
	id.push_back(oldId[i]);
	if (std::find(uIds.begin(), uIds.end(), oldId[i]) == uIds.end()){
	  uIds.push_back(oldId[i]);
	}
	low.push_back(NA_REAL);
	time.push_back(oldTime[i]);
	high.push_back(NA_REAL);
	cmt.push_back(cmtI);
	amt.push_back(oldAmt[i]);
	rate.push_back(-1.0);
	ii.push_back(oldIi[i]);
	addl.push_back(0);
	evid.push_back(1);
	if (whI == 10){
	  ss.push_back(1);
	  show["ss"] = true;
	} else if (whI == 20){
	  ss.push_back(2);
	  show["ss"] = true;
	} else {
	  ss.push_back(0);
	}
	show["rate"] = true;
	ndose++;
	break;
      case 1:
	if (oldAmt[i] > 0){
	  for (j = i; j < oldEvid.size(); j++){
	    if (oldEvid[i] == oldEvid[j] && oldAmt[i] == -oldAmt[j]){
	      double dur = oldTime[j] - oldTime[i];
	      id.push_back(oldId[i]);
	      if (std::find(uIds.begin(), uIds.end(), oldId[i]) == uIds.end()){
		uIds.push_back(oldId[i]);
	      }
	      low.push_back(NA_REAL);
	      time.push_back(oldTime[i]);
	      high.push_back(NA_REAL);
	      cmt.push_back(cmtI);
	      amt.push_back(oldAmt[i] * dur);
	      rate.push_back(oldAmt[i]);
	      ii.push_back(oldIi[i]);
	      addl.push_back(0);
	      evid.push_back(1);
	      if (whI == 10){
		ss.push_back(1);
		show["ss"] = true;
	      } else if (whI == 20){
		ss.push_back(2);
		show["ss"] = true;
	      } else {
		ss.push_back(0);
	      }
	      show["rate"] = true;
	      ndose++;
	      break;
	    }
	  }
	}// else {
	break;
	// }
      case 0:
	// No infusion
	id.push_back(oldId[i]);
	if (std::find(uIds.begin(), uIds.end(), oldId[i]) == uIds.end()){
	  uIds.push_back(oldId[i]);
	}
	low.push_back(NA_REAL);
	time.push_back(oldTime[i]);
	high.push_back(NA_REAL);
	cmt.push_back(cmtI);
	amt.push_back(oldAmt[i]);
	rate.push_back(0);
	ii.push_back(oldIi[i]);
	addl.push_back(0);
	evid.push_back(1);
	if (whI == 10){
	  ss.push_back(1);
	  show["ss"] = true;
	} else if (whI == 20){
	  ss.push_back(2);
	  show["ss"] = true;
	} else {
	  ss.push_back(0);
	}
	ndose++;
	break;
      }
    }
  }

  if (uIds.size() > 1){
    show["id"] = true;
  }
  
  // nme[0] = "id";
  lst[0] = wrap(id);
      
  // nme[2] = "time";
  lst[2] = wrap(time);
      
  // nme[1] = "low";
  lst[1] = wrap(low);
      
  // nme[3] = "high";
  lst[3] = wrap(high);
      
  // nme[4] = "cmt";
  lst[4] = wrap(cmt);
      
  // nme[5] = "amt";
  lst[5] = wrap(amt);

  // nme[6] = "rate";
  lst[6] = wrap(rate);
      
  // nme[7] = "ii";
  lst[7] = wrap(ii);
      
  // nme[8] = "addl";
  lst[8] = wrap(addl);
      
  // nme[9] = "evid";
  lst[9] = wrap(evid);
      
  // nme[10] = "ss";
  lst[10] = wrap(ss);
  
  e["ndose"] = ndose;
  e["nobs"] = nobs;
  e["show"]  = show;
  e["canResize"] = false;
  e["IDs"] = wrap(uIds);
  lst = etSort(lst);
  cls.attr(".RxODE.lst") = e;
  lst.attr("class") = cls;
  lst.attr("row.names") = IntegerVector::create(NA_INTEGER, -(nobs+ndose));  
  return lst;
}

List etAddDose(NumericVector curTime, RObject cmt,  double amt, double rate, double ii,
	       int addl, int curEvid, int ss,
	       IntegerVector IDs, bool turnOnShowCmt, bool doSampling, List curEt){
  std::vector<double> time = as<std::vector<double>>(curEt["time"]);
  std::vector<int> id = as<std::vector<int>>(curEt["id"]);
  std::vector<int> evid = as<std::vector<int>>(curEt["evid"]);
  std::vector<double> low = as<std::vector<double>>(curEt["low"]);
  std::vector<double> high = as<std::vector<double>>(curEt["high"]);
  int oldSize = id.size();
  int i, j;
  double a, b, c;
  int ndose=0, nobs = 0;
  for (j = IDs.size(); j--;){
    if (curTime.size() == 1){
      id.push_back(j+1);
      evid.push_back(curEvid);
      time.push_back(curTime[0]);
      low.push_back(NA_REAL);
      high.push_back(NA_REAL);
      ndose++;
      if (doSampling){
	id.push_back(IDs[j]);
	evid.push_back(0);
	time.push_back(curTime[0]);
	low.push_back(NA_REAL);
	high.push_back(NA_REAL);
	nobs++;
	for (i = addl; i--;){
	  id.push_back(IDs[j]);
	  evid.push_back(0);
	  low.push_back(NA_REAL);
	  high.push_back(NA_REAL);
	  time.push_back(curTime[0] + (i+1)*ii);
	  nobs++;
	}
      }
    } else if (curTime.size() == 2) {
      if (curTime[0] < curTime[1]){
	if (doSampling){
	  stop("do.sampling is not supported with dose windows");
	}
	id.push_back(j+1);
	evid.push_back(curEvid);
	low.push_back(curTime[0]);
	high.push_back(curTime[1]);
	c = Rf_runif(curTime[0], curTime[1]);
	time.push_back(c);
	ndose++;
	for (i = addl; i--;){
	  id.push_back(j+1);
	  evid.push_back(curEvid);
	  a = curTime[0]+ (i+1)*ii;
	  b = curTime[1]+ (i+1)*ii;
	  low.push_back(a);
	  high.push_back(b);
	  c = Rf_runif(a, b);
	  time.push_back(c);
	  ndose++;
	}
      } else {
	stop("For dosing window you need to specify window in order, e.g. et(time=c(0,2),amt=3).");
      }
    } else {
      stop("Time windows must only be 2 elements for dosing.");
    }
    
  }
  std::vector<int> idx(time.size());
  std::iota(idx.begin(),idx.end(),0);
  std::sort(idx.begin(),idx.end(),
	    [id,time,evid](int a, int b){
	      if (id[a] == id[b]){
		if (time[a] == time[b]){
		  if (evid[a] == evid[b]){
		    return a < b;
		  }
		  return evid[a] < evid[b];
		}
		return time[a] < time[b];
	      }
	      return id[a] < id[b];
	    });

  List lst(curEt.size());
  IntegerVector tmpI = as<IntegerVector>(curEt["id"]), tmpI2;
  NumericVector tmpN, tmpN2;
  CharacterVector tmpC, tmpC2;
  lst.attr("names") = curEt.attr("names");
  // nme[0] = "id";
  lst[0] = IntegerVector(id.size());
      
  // nme[1] = "time";
  lst[2] = NumericVector(id.size());
      
  // nme[2] = "low";
  lst[1] = NumericVector(id.size());
      
  // nme[3] = "high";
  lst[3] = NumericVector(id.size());
      
  // nme[4] = "cmt";
  bool isCmtInt=false;
  if (rxIs(cmt,"integer")){
    lst[4] = IntegerVector(id.size());
    isCmtInt=true;
  } else {
    lst[4] = CharacterVector(id.size());
  }
      
  // nme[5] = "amt";
  lst[5] = NumericVector(id.size());

  // nme[6] = "rate";
  lst[6] = NumericVector(id.size());
      
  // nme[7] = "ii";
  lst[7] = NumericVector(id.size());
      
  // nme[8] = "addl";
  lst[8] = IntegerVector(id.size());
  
  // nme[9] = "evid";
  lst[9] = IntegerVector(id.size());
      
  // nme[10] = "ss";
  lst[10] = IntegerVector(id.size());
  for (i = idx.size(); i--;){
    tmpI = as<IntegerVector>(lst[0]); // id
    tmpI[i] = id[idx[i]];
    
    tmpI = as<IntegerVector>(lst[9]); // evid
    tmpI[i] = evid[idx[i]];

    tmpN = as<NumericVector>(lst[1]); // low
    tmpN[i] = low[idx[i]];

    tmpN = as<NumericVector>(lst[2]); // time
    tmpN[i] = time[idx[i]];
    
    tmpN = as<NumericVector>(lst[3]); // high
    tmpN[i] = high[idx[i]];

    if (idx[i] >= oldSize){
      if (isCmtInt){
	tmpI = as<IntegerVector>(lst[4]);
	tmpI[i] = as<int>(cmt);
      } else {
	tmpC = as<CharacterVector>(lst[4]);
	tmpC2 = as<CharacterVector>(cmt);
	tmpC[i] = tmpC2[0];
      }

      // nme[5] = "amt";
      tmpN = as<NumericVector>(lst[5]);
      tmpN[i] = amt;

      // nme[6] = "rate";
      tmpN = as<NumericVector>(lst[6]);
      tmpN[i] = rate;
      
      // nme[7] = "ii";
      tmpN = as<NumericVector>(lst[7]);
      tmpN[i] = ii;
      
      // nme[8] = "addl";
      tmpI = as<IntegerVector>(lst[8]); // id
      tmpI[i] = addl;
  
      // nme[10] = "ss";
      tmpI = as<IntegerVector>(lst[10]); // id
      tmpI[i] = ss;
    } else {
      // low
      for (j = 11; j--;){
	if (rxIs(curEt[j], "numeric")){
	  tmpN = as<NumericVector>(lst[j]);
	  tmpN2 = as<NumericVector>(curEt[j]);
	  tmpN[i] = tmpN2[idx[i]];
	} else if (rxIs(curEt[j], "integer")) {
	  tmpI = as<IntegerVector>(lst[j]);
	  tmpI2 = as<IntegerVector>(curEt[j]);
	  tmpI[i] = tmpI2[idx[i]];
	} else if (rxIs(curEt[j], "character")){
	  // Char
	  tmpC = as<CharacterVector>(lst[j]);
	  tmpC2 = as<CharacterVector>(curEt[j]);
	  tmpC[i] = tmpC2[idx[i]];	
	}
      }
    }
  }
  CharacterVector cls = clone(as<CharacterVector>(curEt.attr("class")));
  List eOld = cls.attr(".RxODE.lst");
  List e = clone(eOld);
  LogicalVector show = e["show"];
  if (turnOnShowCmt){
    show["cmt"] = true;
  }
  std::vector<double> uIds = as<std::vector<double>>(eOld["IDs"]);
  for (i = IDs.size(); i--;){
    if (std::find(uIds.begin(), uIds.end(), IDs[i]) == uIds.end()){
      uIds.push_back(IDs[i]);
    }
  }
  if ((int)IDs.size() == (int)uIds.size() && as<bool>(eOld["canResize"])){
    e["canResize"] = true;
  } else {
    e["canResize"] = false;
  }
  e["IDs"] = wrap(uIds);
  if (uIds.size() > 1){
    show["id"] = true;
  }
  show["amt"] = true;
  if (rate != 0){
    show["rate"] = true;
  }
  if (ss != 0){
    show["ss"] = true;
    show["ii"] = true;
  }
  e["ndose"] = as<int>(e["ndose"])+ndose;
  e["nobs"] = as<int>(e["nobs"])+nobs;
  if (curTime.size() == 2){
    show["low"] = true;
    show["high"] = true;
    if (addl != 0){
    }
  } else {
    if (ii != 0){
      show["ii"] = true;
    }
    if (addl != 0){
      show["addl"] = true;
      show["ii"] = true;
    }  
  }
  e["show"] = show;
  e.attr("names") = eOld.attr("names");
  cls.attr(".RxODE.lst") = e;
  lst.attr("class") = cls;
  int len = as<int>(e["nobs"]) +as<int>(e["ndose"]);
  lst.attr("row.names") = IntegerVector::create(NA_INTEGER, -len);
  return lst;
}

RObject etUpdateObj(List curEt, bool update){
  List lst = clone(curEt);
  CharacterVector cls=clone(as<CharacterVector>(curEt.attr("class")));
  List e = clone(as<List>(cls.attr(".RxODE.lst")));
  CharacterVector units = e["units"];
  if (!CharacterVector::is_na(units[1])){
    lst["ii"]  = setUnits(lst["ii"],  as<std::string>(units[1]));
    lst["low"]  = setUnits(lst["low"],  as<std::string>(units[1]));
    lst["time"] = setUnits(lst["time"], as<std::string>(units[1]));
    lst["high"] = setUnits(lst["high"], as<std::string>(units[1]));
    if (units[1] == "") units[1] = NA_STRING;
  } else {
    lst["ii"]  = setUnits(lst["ii"],  "");
    lst["low"]  = setUnits(lst["low"],  "");
    lst["time"] = setUnits(lst["time"], "");
    lst["high"] = setUnits(lst["high"], "");
  }
  if (!CharacterVector::is_na(units[0])){
    lst["amt"] = setUnits(lst["amt"], as<std::string>(units[0]));
    if (units[0] == "") units[0] = NA_STRING;
  } else {
    lst["amt"] = setUnits(lst["amt"], "");
  }
  e["units"] = units;
  cls.attr(".RxODE.lst") = e;
  lst.attr("class") = cls;
  if (update){
    List cmp = as<List>(evCur);
    for (int j = lst.size(); j--;){
      cmp[j] = lst[j];
    }
    cmp.attr("class") = clone(as<CharacterVector>(lst.attr("class")));
    cmp.attr("names") = lst.attr("names");
    cmp.attr("row.names") = lst.attr("row.names");
  }
  return as<RObject>(lst);
}

RObject etCmtInt(RObject et){
  List cur = as<List>(et);
  List newEt;
  if (rxIs(cur[4], "character")){
    newEt = clone(cur);
    CharacterVector oldCmt = CharacterVector(cur[4]);
    IntegerVector newCmt = IntegerVector(oldCmt.size());
    for (int j = newCmt.size();j--;){
      if (oldCmt[j] == "(default)") newCmt[j] = 1;
      else if (oldCmt[j] == "(obs)") newCmt[j] = NA_INTEGER;
      else stop("Cannot mix named compartments and integer compartments.");
    }
    warning("Using numbered compartments is discouraged with RxODE simulations.");
    newEt[4] = newCmt;
  } else {
    newEt = cur;
  }
  return (as<RObject>(newEt));
}

RObject etSetUnit(List curEt, CharacterVector units){
  CharacterVector cls = clone(as<CharacterVector>(curEt.attr("class")));
  List eOld = cls.attr(".RxODE.lst");
  List e = clone(eOld);
  CharacterVector oldUnits = clone(as<CharacterVector>(e["units"]));
  e["units"] = units;
  List lst = clone(curEt);
  if (as<std::string>(oldUnits[1]) != as<std::string>(units[1])){
    if (!CharacterVector::is_na(units[1])){
      lst["ii"]  = setUnits(lst["ii"],  as<std::string>(units[1]));
      lst["low"]  = setUnits(lst["low"],  as<std::string>(units[1]));
      lst["time"] = setUnits(lst["time"], as<std::string>(units[1]));
      lst["high"] = setUnits(lst["high"], as<std::string>(units[1]));
    } else {
      lst["ii"]  = setUnits(lst["ii"],  "");
      lst["low"]  = setUnits(lst["low"],  "");
      lst["time"] = setUnits(lst["time"], "");
      lst["high"] = setUnits(lst["high"], "");
    }
  }
  if (as<std::string>(oldUnits[0]) != as<std::string>(units[0])){
    if (!CharacterVector::is_na(units[0])){
      lst["amt"] = setUnits(lst["amt"], as<std::string>(units[0]));
    } else {
      lst["amt"] = setUnits(lst["amt"], "");
    }
  }
  cls.attr(".RxODE.lst") = e;
  lst.attr("class") = cls;
  return as<RObject>(lst);
}

List etResizeId(List curEt, IntegerVector IDs){
  // Calculate size
  CharacterVector cls = clone(as<CharacterVector>(curEt.attr("class")));
  List eOld = cls.attr(".RxODE.lst");
  List e = clone(eOld);
  std::vector<int> oldIDs = as<std::vector<int>>(e["IDs"]);
  // Check IDs to remove
  int i;
  std::vector<int> rmIds;
  std::vector<int> newIds;
  for (i = IDs.size(); i--;){
    if (std::find(oldIDs.begin(), oldIDs.end(), IDs[i]) == oldIDs.end()){
      if (IDs[i] < 0){
	rmIds.push_back(-IDs[i]);
      } else {
	newIds.push_back(IDs[i]);
      }
    }
  }
  if (rmIds.size() == 0 && newIds.size() == 0){
    return curEt;
  }
  if (rmIds.size() > 0){
    List newEt(curEt.size());
    // Remove ids
    std::vector<int> finalIds;
    for (i = oldIDs.size(); i--;){
      if (std::find(rmIds.begin(), rmIds.end(), oldIDs[i]) == rmIds.end()){
	finalIds.push_back(oldIDs[i]);
      }
    }
    int rmId = NA_INTEGER, newId = NA_INTEGER;
    if (finalIds.size() == 0 && newIds.size() == 0){
      return etEmpty(e["units"]);
    } else if (finalIds.size() == 0 && newIds.size() > 0){
      if (as<bool>(e["canResize"])){
	newId = newIds.back();
	newIds.pop_back();
	finalIds.push_back(newId);
	rmId = rmIds.back();
	rmIds.pop_back();
      } else {
	stop("Cannot add more IDs to this event table.");
      }
    }
    int nobs = 0;
    int ndose = 0;
    std::vector<int> id = as<std::vector<int>>(curEt["id"]);
    std::vector<int> evid = as<std::vector<int>>(curEt["evid"]);
    std::vector<bool> isIn;
    for (i = 0; i < (int)id.size(); i++){
      if (std::find(rmIds.begin(), rmIds.end(), id[i]) == rmIds.end()){
	if (evid[i] == 0){
	  nobs++;
	} else {
	  ndose++;
	}
	isIn.push_back(true);
      } else {
	isIn.push_back(false);
      }
    }
    int j, newSize = nobs+ndose, k=0;
    IntegerVector tmpI, tmpI2;
    CharacterVector tmpC, tmpC2;
    NumericVector tmpN, tmpN2;
    for (j = newEt.size(); j--;){
      k=newSize-1;
      for (i = (int)id.size();i--;){
	if (rxIs(curEt[j], "numeric")) {
	  if (i == (int)id.size()-1) newEt[j] = NumericVector(newSize);
	  if (isIn[i]){
	    tmpN=newEt[j];
	    tmpN2   = curEt[j];
	    tmpN[k] = tmpN2[i];
	  }
	} else if (rxIs(curEt[j], "integer")) {
	  if (i == (int)id.size()-1) newEt[j] = IntegerVector(newSize);
	  if (isIn[i]){
	    tmpI  = newEt[j];
	    tmpI2 = curEt[j];
	    // Replace ID if needed
	    if (j == 0 && tmpI2[i] == rmId){
	      tmpI[k] = newId;
	    } else {
	      tmpI[k] = tmpI2[i];
	    }
	  }
	} else if (rxIs(curEt[j], "character")){
	  if (i == (int)id.size()-1) newEt[j] = CharacterVector(newSize);
	  if (isIn[i]){
	    tmpC=newEt[j];
	    tmpC2 = curEt[j];
	    tmpC[k] = tmpC2[i];
	  }
	}
	if (isIn[i]){
	  k--;
	}
      }
    }
    CharacterVector cls = clone(as<CharacterVector>(curEt.attr("class")));
    List e = as<List>(cls.attr(".RxODE.lst"));
    e["nobs"] = nobs;
    e["ndose"] = ndose;
    e["IDs"] = wrap(finalIds);
    cls.attr(".RxODE.lst") = e;
    newEt.attr("class") = cls;
    newEt.attr("names") = curEt.attr("names");
    newEt.attr("row.names") = IntegerVector::create(NA_INTEGER,-(nobs+ndose));
    if (newIds.size() > 0){
      // Need to add ids, call recursively.
      return etResizeId(newEt, wrap(newIds));
    } else {
      // Note that if newId is NA and no more are added, then it is a
      // simple ID replacement.  Otherwise the event table is sorted;
      // Overall there is no need to sort here.
      return newEt;
    }
  } else {
    // Add ids?
    if (as<bool>(eOld["canResize"])){
      // Enlarge data-set
      int oldMaxId = oldIDs.size();
      int maxId = oldMaxId + newIds.size();
      double c = (double)(maxId)/(double)(oldMaxId);
      int oldSize = as<int>(e["nobs"]) + as<int>(e["ndose"]);
      int newSize = (int)(oldSize*c);
      int idSize = (int)((double)(oldSize)/(double)(oldMaxId));
      // Add new ids.
      for (i = newIds.size(); i--;){
  	oldIDs.push_back(newIds[i]);
      }
      std::sort(oldIDs.begin(),oldIDs.end()); // Less expensive, then whole table doesn't need to be sorted.
      int j;
      IntegerVector tmpI, tmpI2;
      NumericVector tmpN, tmpN2;
      CharacterVector tmpC, tmpC2;
      List newEt(curEt.size());
      for (j = newEt.size(); j--;){
  	if (rxIs(curEt[j], "integer")) {
  	  tmpI = IntegerVector(newSize);
  	  tmpI2 = as<IntegerVector>(curEt[j]);
  	  std::copy(tmpI2.begin(), tmpI2.end(), tmpI.begin());
  	  if (j == 0){
  	    for (i = oldMaxId+1; i <= maxId; i++){
  	      std::fill_n(tmpI.begin() + oldSize + (i-oldMaxId-1)*idSize, idSize, oldIDs[i-1]);
  	    }
  	  } else {
  	    for (i = newSize - oldSize; i--;){
  	      tmpI[oldSize+i] = tmpI2[i % oldSize];
  	    }
  	  }
  	  newEt[j] = tmpI;
  	} else if (rxIs(curEt[j], "character")){
  	  // Char
  	  tmpC = CharacterVector(newSize);
  	  tmpC2 = as<CharacterVector>(curEt[j]);
  	  std::copy(tmpC2.begin(), tmpC2.end(), tmpC.begin());
  	  for (i = newSize - oldSize; i--;){
  	    tmpC[oldSize+i] = tmpC2[i % oldSize];
  	  }
  	  newEt[j] = tmpC;
  	} else {
  	  tmpN = NumericVector(newSize);
  	  tmpN2 = as<NumericVector>(curEt[j]);
  	  std::copy(tmpN2.begin(), tmpN2.end(), tmpN.begin());
  	  for (i = newSize - oldSize; i--;){
  	    tmpN[oldSize+i] = tmpN2[i % oldSize];
  	  }
  	  newEt[j] = tmpN;
  	}
      }
      newEt.attr("names")     = curEt.attr("names");
      bool recalcTime = false;
      tmpN = as<NumericVector>(newEt["time"]);
      NumericVector tmpN1 = as<NumericVector>(newEt["low"]);
      tmpN2 = as<NumericVector>(newEt["high"]);
      // Update new observations with recalculated windows
      for (i = newSize - oldSize; i--;){
      	if (!ISNA(tmpN1[oldSize+i]) && !ISNA(tmpN2[oldSize+i])){
      	  tmpN[oldSize+i] = Rf_runif(tmpN1[oldSize+i], tmpN2[oldSize+i]);
      	  recalcTime=true;
      	}
      }
      e["nobs"]   = (int)(as<double>(e["nobs"])*c);
      e["ndose"]  = (int)(as<double>(e["ndose"])*c);
      LogicalVector show = e["show"];
      show["id"] = true;
      e["show"] = show;
      e["IDs"] = wrap(oldIDs);
      e.attr("class")         = "rxHidden";
      cls.attr(".RxODE.lst")  = e;
      newEt.attr("class")     = cls;
      int len = as<int>(e["nobs"]) +as<int>(e["ndose"]);
      newEt.attr("row.names") = IntegerVector::create(NA_INTEGER, -len);
      if (recalcTime){
      	// Will have to sort with new times.
      	newEt = etSort(newEt);
      }
      return newEt;
    } else {
      stop("Cannot add more IDs to this event table.");
    }
  }
}

//[[Rcpp::export]]
RObject et_(List input, List et__){
  // Create or modify new event table
  double doRet = false;
  bool turnOnShowCmt = false;
  bool doUpdateObj = false;
  RObject curEt;
  if (et__.size() > 0){
    if (rxIs(et__[0],"character")){
      if (as<std::string>(et__[0]) == "last"){
	doUpdateObj=true;
	curEt = evCur;
      } else if (as<std::string>(et__[0]) == "import"){
	return etUpdateObj(etImportEventTable(as<List>(input["data"])), true);
      }
    } else if (rxIs(et__, "rxEt")) {
      curEt = as<RObject>(et__);
    }
  }
  CharacterVector inN;
  if (input.hasAttribute("names")){
    inN = input.attr("names");
  }
  int i, amtIx = -1, iiIx = -1, addlIx = -1,
    untilIx = -1, evidIx=-1, idIx=-1, cmtIx=-1, 
    amtUnitIx=-1, timeUnitIx=-1, doSamplingIdx=-1, timeIx=-1,
    rateIx = -1, nbrIx=-1, ssIx=-1;
  // Wait should be in sequences and rep
  for (i = (int)inN.size(); i--;){
    if (inN[i] == "amt" || inN[i] == "dose") amtIx=i;
    else if (inN[i] == "ii" || inN[i] == "dosing.interval" || inN[i] == "dosingInterval" || inN[i] == "dosing_interval") iiIx=i;
    else if (inN[i] == "addl") addlIx = i;
    else if (inN[i] == "until") untilIx = i;
    else if (inN[i] == "evid") evidIx = i;
    else if (inN[i] == "ID" || inN[i] == "id") idIx=i;
    else if (inN[i] == "cmt" || inN[i] == "dosing.to" || inN[i] == "dosingTo" || inN[i] =="dosing_to" ||
	     inN[i] == "dose.to" || inN[i] == "doseTo" || inN[i] == "dose_to") cmtIx=i;
    else if (inN[i] == "amount.units" || inN[i] == "amountUnits" || inN[i] == "amount_units" ||
	     inN[i] == "amt.units" || inN[i] == "amtUnits" || inN[i] == "amt_units" ||
	     inN[i] == "dose.units" || inN[i] == "doseUnits" || inN[i] == "dose_units") amtUnitIx=i;
    else if (inN[i] == "time.units" || inN[i] == "timeUnits" || inN[i] == "time_units") timeUnitIx=i;
    else if (inN[i] == "do.sampling" || inN[i] == "doSampling" || inN[i] == "do_sampling" ||
	     inN[i] == "add.sampling" || inN[i] == "addSampling" || inN[i] == "add_sampling") doSamplingIdx=i;
    else if (inN[i] == "time" || inN[i] == "start.time" || inN[i] == "startTime" || inN[i] == "start_time" ||
	     inN[i] == "start") timeIx = i;
    else if (inN[i] == "nbr.doses" || inN[i] == "nbrDoses" || inN[i] == "nbr") nbrIx=i;
    else if (inN[i] == "ss") ssIx = i;
    else if (inN[i] == "rate") rateIx = i;
    else if (inN[i] != "" &&  !doUpdateObj) stop("unused argument '%s'", (as<std::string>(inN[i])).c_str());
  }
  // missing argument name handling.
  for (i = 0; i <(int)inN.size(); i++){
    if (inN[i] == ""){
      if (rxIs(input[i], "character") && cmtIx == -1) cmtIx = i;
      if (rxIs(input[i], "numeric") && timeIx == -1) timeIx = i;
      if (rxIs(input[i], "integer") && timeIx == -1) timeIx = i;
      if (rxIs(input[i], "list") && timeIx == -1) timeIx=i;
      if (rxIs(input[i], "rxEt")) curEt = input[i];
    }
  }
  if (inN.size() == 0 && input.size() != 0){
    for (i = 0; i <(int)input.size(); i++){
      if (rxIs(input[i], "character")) cmtIx = i;
      if (rxIs(input[i], "numeric") && timeIx == -1) timeIx = i;
      if (rxIs(input[i], "integer") && timeIx == -1) timeIx = i;
      if (rxIs(input[i], "list") && timeIx == -1) timeIx=i;
      if (rxIs(input[i], "rxEt")) curEt = input[i];
    }
  }
  if (rxIs(curEt, "rxEt")){
    CharacterVector cls = clone(as<CharacterVector>(curEt.attr("class")));
    List e = cls.attr(".RxODE.lst");
    CharacterVector oldUnits = e["units"];
    CharacterVector units(2);
    bool foundUnits = false;
    if (amtUnitIx == -1){
      units[0] = oldUnits[0];
    } else  {
      CharacterVector tmpS = as<CharacterVector>(input[amtUnitIx]);
      if (tmpS.size() != 1) stop("Amount unit cannot be a vector");
      units[0] = tmpS[0];
      foundUnits = true;
    }
    if (timeUnitIx == -1){
      units[1] = oldUnits[1];
    } else  {
      CharacterVector tmpS = as<CharacterVector>(input[timeUnitIx]);
      if (tmpS.size() != 1) stop("Time unit cannot be a vector");
      units[1] = tmpS[0];
      foundUnits = true;
    }
    units.attr("names") = CharacterVector::create("dosing", "time");
    if (foundUnits){
      curEt = etSetUnit(as<List>(curEt), units);
      doRet=true;
    }
    // This is a modification to an existing event table.
    if (rxIs(input[0], "logical")){
      LogicalVector in0 = as<LogicalVector>(input[0]);
      if (in0[0]){
	CharacterVector nm = input.attr("names");
	if (nm[0] == "getUnits"){
	  return(e["units"]);
	} else if (nm[0] == "get.nobs"){
	  return e["nobs"];
	} else if (nm[0] == "get.dose"){
	  return e["nobs"];
	} else if (nm[0] == "copy"){
	  // Make sure that the object is cloned
	  return etUpdateObj(as<List>(curEt),false);
	} else if (nm[0] == "get.EventTable"){
	  e.attr("class") = R_NilValue;
	  if (as<int>(e["nobs"]) == 0 && as<int>(e["ndose"]) == 0){
	    return R_NilValue;
	  } else {
	    List ret = clone(as<List>(curEt));
	    ret.attr("class") = "data.frame";
	    return as<RObject>(ret);
	  }
	} else if (nm[0] == "get.obs.rec"){
	  List lst = as<List>(curEt);
	  IntegerVector evid = as<IntegerVector>(lst["evid"]);
	  LogicalVector ret(evid.size());
	  for (int i = evid.size(); i--;) ret[i] = (evid[i] == 0);
	  return as<RObject>(ret);
	} else if (nm[0] == "get.sampling" || nm[0] == "get.dosing" ||
		   nm[0] == "clearSampling" || nm[0] == "clearDosing"){
	  // Need to update
	  bool doDose = (nm[0] == "get.dosing" || nm[0] == "clearSampling");
	  bool updateObj = (nm[0] == "clearSampling" || nm[0] == "clearDosing");
	  int n = doDose ? as<int>(e["ndose"]) : as<int>(e["nobs"]);
	  List cmp;
	  if (n == 0){
	    if (updateObj){
	      cmp = as<List>(curEt);
	      List em =etEmpty(as<CharacterVector>(e["units"]));
	      cls = clone(as<CharacterVector>(em.attr("class")));
	      for (int j = cmp.size(); j--;){
		if (rxIs(cmp[j], "numeric")){
		  cmp[j] = NumericVector(0);
		} else if (rxIs(cmp[j], "integer")){
		  cmp[j] = IntegerVector(0);
		} else if (rxIs(cmp[j], "character")) {
		  cmp[j] = CharacterVector(0);
		}
	      }
	      cmp.attr("row.names") = IntegerVector::create(NA_INTEGER, -n);
	      cmp.attr("class") = cls;
	      return R_NilValue;
	    } else {
	      return R_NilValue;
	    }
	  } else {
	    cmp = as<List>(curEt);
	    List ret(cmp.size());
	    IntegerVector evid = as<IntegerVector>(cmp["evid"]);
	    int i, j, k;
	    for (j = ret.size(); j--;){
	      if (rxIs(cmp[j], "numeric")){
		ret[j] = NumericVector(n);
	      } else if (rxIs(cmp[j], "integer")){
		ret[j] = IntegerVector(n);
	      } else if (rxIs(cmp[j], "character")){
		ret[j] = CharacterVector(n);
	      }
	    }
	    k = 0;
	    NumericVector tmpN, tmpN2;
	    IntegerVector tmpI, tmpI2;
	    CharacterVector tmpC, tmpC2;
	    for (i = 0; i < evid.size(); i++){
	      if ((doDose && evid[i] != 0) || (!doDose && evid[i] == 0)){
		for (j = ret.size(); j--;){
		  if (rxIs(cmp[j], "numeric")){
		    tmpN = ret[j];
		    tmpN2 = cmp[j];
		    tmpN[k] = tmpN2[i];
		  } else if (rxIs(cmp[j], "integer")){
		    tmpI = ret[j];
		    tmpI2 = cmp[j];
		    tmpI[k] = tmpI2[i];
		  } else if (rxIs(cmp[j], "character")){
		    tmpC = ret[j];
		    tmpC2 = cmp[j];
		    tmpC[k] = tmpC2[i];
		  }
		}
		k++;
	      }
	    }
	    ret.attr("names") = cmp.attr("names");
	    ret.attr("row.names") = IntegerVector::create(NA_INTEGER, -n);
	    if (updateObj){
	      // This updates the object in place.
	      if (doDose){
		e["nobs"] = 0;
	      } else {
		e["ndose"] = 0;
	      }
	      cls.attr(".RxODE.lst") = e;
	      for (j = cmp.size(); j--;){
		cmp[j] = ret[j];
	      }
	      cmp.attr("row.names") = IntegerVector::create(NA_INTEGER, -n);
	      cmp.attr("class") = cls;
	      return as<RObject>(ret);
	    } else {
	      ret.attr("class") = "data.frame";
	    }
	    return as<RObject>(ret);
	  }
	} else {
	  warning("Nothing done");
	  return  as<List>(curEt);
	}
      } else {
	warning("Nothing done");
	return  as<List>(curEt);
      }
    } else {
      // We are updating the event table
      IntegerVector id; // = e["IDs"];
      if (idIx != -1){
      	id    = as<IntegerVector>(input[idIx]);
	curEt = as<RObject>(etResizeId(as<List>(curEt), id));
	doRet=true;
      } else {
      	id = as<IntegerVector>(e["IDs"]);
      }
      CharacterVector cls = clone(as<CharacterVector>(curEt.attr("class")));
      List e = cls.attr(".RxODE.lst");      
      CharacterVector cmtS;
      IntegerVector cmtI;
      RObject cmt;
      // Dose
      if (cmtIx == -1){
	List tmp = as<List>(curEt);
	if (rxIs(tmp[4], "integer")){
	  cmtI = IntegerVector(1);
	  if (amtIx == -1){
	    cmtI[0] = NA_INTEGER;
	  } else {
	    cmtI[0] = 1;
	  }
	  cmt = as<RObject>(cmtI);
	} else {
	  cmtS = CharacterVector(1);
	  if (amtIx == -1){
	    cmtS[0] = "(obs)";
	  } else {
	    cmtS[0] = "(default)";
	  }
	  cmt = as<RObject>(cmtS);
	}
      } else {
	if (rxIs(input[cmtIx], "character")){
	  cmtS = as<CharacterVector>(input[cmtIx]);
	  if (cmtS.size() == 1){
	    List tmp = as<List>(curEt);
	    if (rxIs(tmp[4], "integer")){
	      stop("Cannot mix named and integer compartments");
	    }
	    cmt = as<RObject>(cmtS);
	  } else {
	    stop("The compartment cannot be a vector.");
	  }
	} else if (rxIs(input[cmtIx], "integer") || rxIs(input[cmtIx], "numeric")){
	  cmtI = as<IntegerVector>(input[cmtIx]);
	  if (cmtI.size() == 1){
	    curEt=etCmtInt(curEt);
	    cls = clone(as<CharacterVector>(curEt.attr("class")));
	    e = cls.attr(".RxODE.lst");
	    cmt = as<RObject>(cmtI);
	  } else {
	    stop("The compartment cannot be an integer.");
	  }
	} else {
	  stop("The compartment must be an integer or a character.");
	}
	turnOnShowCmt=true;
      }
      NumericVector amt;
      bool isObs=false;
      if (amtIx == -1){
	isObs=true;
      } else {
	amt = as<NumericVector>(input[amtIx]);
	if (amt.size() != 1){
	  stop("Dose amount cannot be a vector.");
	}
	if (rxIs(amt, "units")){
	  CharacterVector cls = clone(as<CharacterVector>(curEt.attr("class")));
	  List e = clone(as<List>(cls.attr(".RxODE.lst")));
	  CharacterVector units = e["units"];
	  if (!CharacterVector::is_na(units["dosing"])){
	    amt = setUnits(amt, as<std::string>(units["dosing"]));
	  } else {
	    amt = setUnits(amt, "");
	  }
	}
	isObs = false;
      }
      if (isObs){
	IntegerVector addl;// = 0;
	if (addlIx != -1){
	  addl = as<IntegerVector>(input[addlIx]);
	  if (addl.size() != 1) stop("addl cannot be a vector.");
	  if (addl[0] != 0){
	    stop("addl needs a dose/amt.");
	  }
	} else {
	  addl = IntegerVector(1);
	  addl[0] = 0;
	}
	if (untilIx != -1){
	  stop("until needs a dose/amt.");
	}
	if (nbrIx != -1){
	  stop("nbr.doses needs a dose/amt.");
	}
	NumericVector rate;
	if (rateIx != -1){
	  rate = as<NumericVector>(input[rateIx]);
	  if (rate.size() != 1) stop("rate cannot be a vector");
	  if (rate[0] != 0.0){
	    stop("rate needs a dose/amt.");
	  }
	} else {
	  rate = NumericVector(1);
	  rate[0]=0.0;
	}
	NumericVector ii;
	if (iiIx != -1){
	  ii = as<NumericVector>(input[iiIx]);
	  if (ii.size() != 1) stop("ii cannot be a vector.");
	  if (ii[0] != 0.0){
	    stop("ii needs a dose/amt.");
	  }
	} else {
	  ii = NumericVector(1);
	  ii[0] = 0.0;
	}
	if (addl[0] > 0 && ii[0] <= 0){
	  stop("A dosing interval of zero makes no sense with multiple dose events.");
	}
	IntegerVector evid;
	if (evidIx != -1){
	  evid = as<IntegerVector>(input[evidIx]);
	  if (evid.size() != 1){
	    stop("evid cannot be a vector.");
	  }
	  if (evid[0] != 0){
	    stop("non-zero evid needs a dose/amt.");
	  }
	} else {
	  evid = IntegerVector(1);
	  evid[0]=0;
	}
	IntegerVector ss;// = 0;
	if (ssIx != -1){
	  ss = as<IntegerVector>(input[ssIx]);
	  if (ss.size() != 1) stop("ss cannot be a vector.");
	  if (ss[0] != 0){
	    stop("non-zero ss needs a dose/amt.");
	  }
	}
	if (timeIx != -1) {
	  if (rxIs(input[timeIx], "numeric") || rxIs(input[timeIx], "integer") ||
	      rxIs(input[timeIx], "units")){
	    NumericVector time = as<NumericVector>(input[timeIx]);
	    if (rxIs(time, "units")){
	      CharacterVector cls = clone(as<CharacterVector>(curEt.attr("class")));
	      List e = clone(as<List>(cls.attr(".RxODE.lst")));
	      CharacterVector units = e["units"];
	      if (!CharacterVector::is_na(units["time"])){
		time = setUnits(time, as<std::string>(units["time"]));
	      } else {
		time = setUnits(time, "");
	      }
	    }
	    return etUpdateObj(etAddTimes(time, id, cmt, turnOnShowCmt, as<List>(curEt)),
			       doUpdateObj);
	  } else if (rxIs(input[timeIx], "list")){
	    return etUpdateObj(etAddWindow(as<List>(input[timeIx]), id, cmt, turnOnShowCmt, as<List>(curEt)),
			       doUpdateObj);
	  }
	}
      } else {
	////////////////////////////////////////////////////////////////////////////////
	// Dose
	////////////////////////////////////////////////////////////////////////////////
	if (addlIx != -1 && untilIx != -1){
	  stop("Can only specify until or addl, not both.");
	}
	if (addlIx != -1 && nbrIx != -1){
	  stop("Can only specify addl or nbr.doses, not both.");
	}
	if (nbrIx != -1 && untilIx != -1){
	  stop("Can only specify nbr.doses or until, not both.");
	}
	bool doSampling = false;
	if (doSamplingIdx != -1){
	  if (rxIs(input[doSamplingIdx], "logical")){
	    LogicalVector tmpL = as<LogicalVector>(input[doSamplingIdx]);
	    if (tmpL.size() != 1) stop("do.sampling can only be a length one.");
	    doSampling = tmpL[0];
	  } else {
	    stop("do.sampling must be logical.");
	  }
	}
	NumericVector rate;
	if (rateIx != -1){
	  rate = as<NumericVector>(input[rateIx]);
	  if (rate.size() != 1) stop("rate cannot be a vector.");
	} else {
	  rate = NumericVector(1);
	  rate[0] = 0.0;
	}
	NumericVector ii;// =0.0;
	if (iiIx != -1){
	  ii = clone(as<NumericVector>(input[iiIx]));
	  if (ii.size() != 1) stop("ii cannot be a vector.");
	} else {
	  ii = NumericVector(1);
	  ii[0] = 0.0;
	}
	IntegerVector evid;// =1;
	if (evidIx != -1){
	  evid = as<IntegerVector>(input[evidIx]);
	  if (evid.size()!= 1){
	    stop("evid cannot be a vector");
	  }
	  if (evid[0] == 0){
	    stop("zero evid cannot be used with dose/amt.");
	  }
	} else {
	  evid = IntegerVector(1);
	  evid[0] = 1;
	}
	IntegerVector ss;// = 0;
	if (ssIx != -1){
	  ss = as<IntegerVector>(input[ssIx]);
	  if (ss.size() != 1) stop("ss cannot be a vector.");
	} else {
	  ss = IntegerVector(1);
	  ss[0] = 0.0;
	}
	NumericVector time;
	if (timeIx != -1){
	  time = as<NumericVector>(input[timeIx]);
	} else {
	  time = NumericVector(1);
	  time[0] = 0;
	}
	IntegerVector addl;//=0;
	if (addlIx != -1){
	  addl = clone(as<IntegerVector>(input[addlIx]));
	  if (addl.size() != 1) stop("addl cannot be a vector.");
	} else if (nbrIx != -1){
	  addl = clone(as<IntegerVector>(input[nbrIx]));
	  if (addl.size() != 1) stop("Number of doses cannot be a vector.");
	  if (addl[0] < 1){
	    stop("Number of Doses must be at least one (addl: %d)", addl[0]);
	  }
	  addl[0] = addl[0]-1;
	} else if (untilIx != -1){
	  // Need time for this
	  NumericVector until = as<NumericVector>(input[untilIx]);
	  if (until.size() != 1) stop("Until cannot be a vector.");
	  if (ii[0] < 0){
	    stop("'until' can only be used with positive inter-dose intervals (ii).");
	  }
	  if (time.size() == 1){
	    double tmp = until[0] - time[0] - ii[0];
	    if (tmp > 0){
	      addl[0] = ceil(tmp/ii[0]);
	    } else {
	      addl[0] = 0;
	    }
	  } else if (time.size() == 2){
	    double tmp = until[0] - time[1] - ii[0];
	    if (tmp > 0){
	      addl[0] = ceil(tmp/ii[0]);
	    } else {
	      addl[0] = 0;
	    }
	  }
	} else {
	  addl = IntegerVector(1);
	  addl[0]=0;
	}
	if (ii[0] > 0 && ss[0] == 0 && addl[0] == 0){
	  if (doUpdateObj && ii[0] == 24){
	    ii[0]=0;
	  } else {
	    stop("ii requires non zero additional doses (addl/until/nbr.doses) or steady state dosing (ii: %f, ss: %d; addl: %d).", ii[0], ss[0], addl[0]);
	  }
	}
	if (ss[0] < 0 || ss[0] > 2){
	  stop("ss must be 0, 1 or 2.");
	}
	if (ss[0] > 1 && time.size() > 1){
	  stop("Steady state (ss) is not supported with dosing windows.");
	}
	if (addl[0] < 0){
	  stop("Additional doses must be positive (addl=%d).", addl[0]);
	}
	return etUpdateObj(etAddDose(time, cmt, amt[0], rate[0], ii[0], addl[0], evid[0], ss[0],
				     id, turnOnShowCmt, doSampling, as<List>(curEt)),doUpdateObj);
      }
    }
  } else {
    // This is a new event table.
    // Get the amtUnit
    CharacterVector units(2);
    int foundArgs = 0;
    if (amtUnitIx == -1){
      units[0] = NA_STRING;
    } else  {
      CharacterVector tmpS = as<CharacterVector>(input[amtUnitIx]);
      if (tmpS.size() != 1) stop("Amount unit cannot be a vector");
      units[0] = tmpS[0];
      foundArgs++;
    }
    if (timeUnitIx == -1){
      units[1] = "hours";
    } else  {
      CharacterVector tmpS = as<CharacterVector>(input[timeUnitIx]);
      if (tmpS.size() != 1) stop("Time unit cannot be a vector");
      units[1] = tmpS[0];
      foundArgs++;
    }
    units.attr("names") = CharacterVector::create("dosing", "time");
    List et = etEmpty(units);
    if (input.size() == foundArgs){
      return et;
    } else {
      return et_(input, et);
    }
  }
  if (doRet) return etUpdateObj(as<List>(curEt),doUpdateObj);
  stop("Cannot figure out what type of EventTable you are trying to create.");
  // Should never get here...
  List ret(0);
  return ret;
}

// Sequence event tables
//[[Rcpp::export]]
List etSeq_(List ets, int handleSamples=0, int waitType = 0, int reserveLen=0, bool needSort=true,
	    CharacterVector newUnits = CharacterVector::create(),
	    LogicalVector newShow = LogicalVector::create(),
	    bool isCmtIntIn = false){
  double timeDelta = 0;
  double maxTime = 0;
  double lastDose = 0;
  double lastIi = 0;
  std::vector<int> id;
  std::vector<double> time;
  std::vector<double> low;
  std::vector<double> high;
  std::vector<double> ii;
  std::vector<double> amt;
  std::vector<int> evid;
  std::vector<int> addl;
  std::vector<int> idxEts;
  std::vector<int> idxEt;
  std::vector<int> idx;
  
  NumericVector curTime, curLow, curHigh, curIi,curAmt;
  IntegerVector curEvid, curAddl, curId;
  
  int i, j, k=0, nobs = 0, ndose=0;
  List et;
  bool isCmtInt=false;
  CharacterVector units;
  LogicalVector show;
  bool gotUnits = false;
  List e;
  
  CharacterVector cls;

  if (reserveLen != 0){
    // preallocate vector
    id.reserve(reserveLen);
    time.reserve(reserveLen);
    low.reserve(reserveLen);
    high.reserve(reserveLen);
    ii.reserve(reserveLen);
    amt.reserve(reserveLen);
    evid.reserve(reserveLen);
    addl.reserve(reserveLen);
    idxEts.reserve(reserveLen);
    idxEt.reserve(reserveLen);
    idx.reserve(reserveLen);
    if (newUnits.size() == 2){
      units = newUnits;
      show = newShow;
      isCmtInt = isCmtIntIn;
      gotUnits=true;
    }
  }

  for (i = 0 ;i < ets.size(); i++){
    if (rxIs(ets[i], "rxEt")){
      List et = ets[i];
      cls = as<CharacterVector>(et.attr("class"));
      e = cls.attr(".RxODE.lst");
      if (!gotUnits){
	units = e["units"];
	show = e["show"];
	if (rxIs(et["cmt"], "integer")){
	  isCmtInt = true;
	}
	gotUnits=true;
      } else {
	if (isCmtInt && !rxIs(et["cmt"], "integer")){
	  stop("Cannot event tables with integer and character 'cmt'.");
	}
	if (!isCmtInt && rxIs(et["cmt"], "integer")){
	  stop("Cannot event tables with integer and character 'cmt'.");
	}
	LogicalVector newShow = e["show"];
	for (j = show.size(); j--;){
	  show[j] = show[j] || newShow[j];
	}
      }
      //
      // Load time, low, high and amt and convert units if needed.
      curTime = et["time"];
      if (rxIs(curTime, "units")){
	if (!CharacterVector::is_na(units["time"])){
	  curTime = setUnits(curTime, as<std::string>(units["time"]));
	} else {
	  curTime = setUnits(curTime, "");
	}
      }
      curLow=et["low"];
      if (rxIs(curLow, "units")){
	if (!CharacterVector::is_na(units["time"])){
	  curLow = setUnits(curLow, as<std::string>(units["time"]));
	} else {
	  curLow = setUnits(curLow, "");
	}
      }
      curHigh=et["high"];
      if (rxIs(curHigh, "units")){
	if (!CharacterVector::is_na(units["time"])){
	  curHigh = setUnits(curHigh, as<std::string>(units["time"]));
	} else {
	  curHigh = setUnits(curHigh, "");
	}
      }
      curIi=et["ii"];
      if (rxIs(curIi, "units")){
	if (!CharacterVector::is_na(units["time"])){
	  curIi = setUnits(curIi, as<std::string>(units["time"]));
	} else {
	  curIi = setUnits(curIi, "");
	}
      }
      curAmt = et["amt"];
      if (rxIs(curAmt, "units")){
	if (!CharacterVector::is_na(units[0])){
	  curAmt = setUnits(curAmt, as<std::string>(units[0]));
	} else {
	  curAmt = setUnits(curAmt, "");
	}
      }
      curId = et["id"];
      curEvid = et["evid"];
      curAddl = et["addl"];
      for (j = 0; j < curTime.size(); j++){
	if (handleSamples == 0 && curEvid[j]== 0) continue;
	if (curEvid[j] == 0) nobs++;
	else ndose++;
	id.push_back(curId[j]);
	addl.push_back(curAddl[j]);
	if (!ISNA(curHigh[j])){
	  if (curAddl[j] > 0){
	    lastIi = curIi[j];
	    lastDose = curHigh[j] + curAddl[j]*curIi[j];
	    maxTime = curHigh[j] + (curAddl[j]+1)*curIi[j];
	  } else {
	    lastIi = (curHigh[j] - lastDose);
	    maxTime = curHigh[j] + (curHigh[j] - lastDose); //Use last interval
	    lastDose = curHigh[j];
	  }
	  high.push_back(curHigh[j]+timeDelta);
	  time.push_back(curTime[j]+timeDelta);
	  low.push_back(curLow[j]+timeDelta);
	} else {
	  if (curAddl[j] > 0){
	    lastIi = curIi[j];
	    lastDose = curTime[j] + curAddl[j] * curIi[j];
	    double tmp = curTime[j] + (curAddl[j]+1) * curIi[j];
	    if (tmp > maxTime) maxTime = tmp;
	  } else {
	    lastIi = (curTime[j] - lastDose);
	    lastDose = curTime[j];
	    maxTime = curTime[j] + (curTime[j] - lastDose); //Use last interval
	  }
	  high.push_back(NA_REAL);
	  time.push_back(curTime[j]+timeDelta);
	  low.push_back(NA_REAL);
	}
	ii.push_back(curIi[j]);
	amt.push_back(curAmt[j]);
	evid.push_back(curEvid[j]);
	idxEts.push_back(i);
	idxEt.push_back(j);
	idx.push_back(k++);
      }
    } else if (rxIs(ets[i], "numeric") || rxIs(ets[i], "integer")){
      maxTime = as<double>(ets[i]);
      if (waitType == 0 && maxTime > lastIi){
	maxTime -= lastIi;
      }
    }
    timeDelta += maxTime;
  }
  if (needSort){
    std::sort(idx.begin(),idx.end(),
	      [id,time,evid](int a, int b){
		if (id[a] == id[b]){
		  if (time[a] == time[b]){
		    if (evid[a] == evid[b]){
		      return a < b;
		    }
		    return evid[a] < evid[b];
		  }
		  return time[a] < time[b];
		}
		return id[a] < id[b];
	      });
  }
  if (!gotUnits){
    stop("No events table found for seq/rep.");
  }
  List lst = etEmpty(units);
  // nme[0] = "id";
  lst[0] = IntegerVector(id.size());
      
  // nme[1] = "time";
  lst[2] = NumericVector(id.size());
      
  // nme[2] = "low";
  lst[1] = NumericVector(id.size());
      
  // nme[3] = "high";
  lst[3] = NumericVector(id.size());
      
  // nme[4] = "cmt";
  if (!isCmtInt){
    lst[4] = CharacterVector(id.size());
  } else {
    lst[4] = IntegerVector(id.size());
  }
  // nme[5] = "amt";
  lst[5] = NumericVector(id.size());

  // nme[6] = "rate";
  lst[6] = NumericVector(id.size());
      
  // nme[7] = "ii";
  lst[7] = NumericVector(id.size());
      
  // nme[8] = "addl";
  lst[8] = IntegerVector(id.size());
  
  // nme[9] = "evid";
  lst[9] = IntegerVector(id.size());
      
  // nme[10] = "ss";
  lst[10] = IntegerVector(id.size());

  IntegerVector tmpI, tmpI2;
  NumericVector tmpN, tmpN2;
  CharacterVector tmpC, tmpC2;
  // Now fill everything in.
  for (i = idx.size(); i--;){
    et = ets[idxEts[idx[i]]];
    j = idxEt[idx[i]];    

    tmpI = as<IntegerVector>(lst[0]); // id
    tmpI[i] = id[idx[i]];
    
    tmpN = as<NumericVector>(lst[1]); // low
    tmpN[i] = low[idx[i]];

    tmpN = as<NumericVector>(lst[2]); // time
    tmpN[i] = time[idx[i]];
    
    tmpN = as<NumericVector>(lst[3]); // high
    tmpN[i] = high[idx[i]];
    // 4 is cmt
    if (!isCmtInt){
      tmpC = as<CharacterVector>(lst[4]);
      tmpC2 = as<CharacterVector>(et[4]);
      tmpC[i] = tmpC2[j];
    } else {
      tmpI = as<IntegerVector>(lst[4]); 
      tmpI2 = as<IntegerVector>(et[4]);
      tmpI[i] = tmpI2[j];
    }

    tmpN = as<NumericVector>(lst[5]); //  amt
    tmpN[i] = amt[idx[i]];

    // 6 is rate
    tmpN = as<NumericVector>(lst[6]); // rate
    tmpN2 = as<NumericVector>(et[6]);
    tmpN[i] = tmpN2[j];    

    tmpN = as<NumericVector>(lst[7]); // ii
    tmpN[i] = ii[idx[i]];

    // 8 is addl
    tmpI = as<IntegerVector>(lst[8]); // rate
    tmpI[i] = addl[idx[i]];
    
    tmpI = as<IntegerVector>(lst[9]); // evid
    tmpI[i] = evid[idx[i]];

    tmpI = as<IntegerVector>(lst[10]); // rate
    tmpI2 = as<IntegerVector>(et[10]);
    tmpI[i] = tmpI2[j];
  }
  cls = lst.attr("class");
  e = cls.attr(".RxODE.lst");
  e["ndose"] = ndose;
  e["nobs"] = nobs;
  e["show"]  = show;
  cls.attr(".RxODE.lst") = e;
  lst.attr("class") = cls;
  lst.attr("row.names") = IntegerVector::create(NA_INTEGER, -(nobs+ndose));
  return lst;
}

// Sequence event tables
//[[Rcpp::export]]
List etRep_(RObject curEt, int times, double wait, IntegerVector ids, int handleSamples, int waitType){
  CharacterVector cls = as<CharacterVector>(curEt.attr("class"));
  List e = cls.attr(".RxODE.lst");
  int len = as<int>(e["nobs"]) +as<int>(e["ndose"]);
  IntegerVector IDs = e["IDs"];
  List seqLst(times*2);
  for (int i = times; i--;){
    seqLst[i*2] = curEt;
    seqLst[i*2+1] = wait;
  }
  return etSeq_(seqLst, handleSamples, waitType, len*times, (IDs.size() != 1), e["units"],
		e["show"], rxIs(curEt, "integer"));
}
