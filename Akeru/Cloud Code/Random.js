function rev(v) {
  var s = v.replace(/^(.(..)*)$/, "0$1"); // add a leading zero if needed
  var a = s.match(/../g);             // split number in groups of two
  a.reverse();                        // reverse the groups
  var s2 = a.join("");                // join the groups back together
    return s2;
}

function negative(s) {
  a = parseInt(s,16);
    if (a >= 32767) {
      a = a - 65536;
    }
    return a;
}

function parseF(s) {
  a = ((s & 0x7FFFFF | 0x800000) * 1.0 / Math.pow(2,23) * Math.pow(2, ((s>>23 & 0xFF) - 127)));
  return a;
}

function main(params, callback){
   var s = rev(params.data);
    var result = [
    { 
            "key": "Random",
            "value": negative(s.substring(12, 16))
    }
      ]
    callback(null, result) 
}
