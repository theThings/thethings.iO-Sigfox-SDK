function negative(s) {
    a = parseInt(s,16);
    if (a >= 32767) {
        a = a - 65536;
    }
    return a;
}

function parseGPSData(s) { // Use this function in order to patch the GPS data stored as floats in the code
    a = ((s & 0x7FFFFF | 0x800000) * 1.0 / Math.pow(2,23) * Math.pow(2, ((s>>23 & 0xFF) - 127)));
    if (s >= 2147483647) { //If >= (it's a negative latitude or longitude)
      a =-a;
    }
    return a;
}

function main(params, callback){
  var size = params.data.length;
  if (params.data.substring(size-2, size) == 31) {
    var result = [
        { 
            "key": "x",
            "value": negative(params.data.substring(8, 12))
        },
        { 
            "key": "y",
            "value": negative(params.data.substring(4, 8))
        },
        { 
            "key": "z",
            "value": negative(params.data.substring(0, 4))
        },
        {
            "key": "buttonPressed",
            "value": params.data.substring(size-3, size-2)
        }
        ]
   }
    else {
      var result = [
        { 
            "key": "Temperature",
            "value": negative(params.data.substring(16, 20))
        },
        { 
            "key": "geolocalization",
            "value": 1,
            "geo" : {
                "lat" : parseGPSData(parseInt(params.data.substring(8,16),16)),
              "long" : (parseGPSData(parseInt(params.data.substring(0,8),16))),
                //"lat" : parseGPSData(parseInt(params.data.substring(0,8),16)),
                //"long" : parseGPSData(parseInt(params.data.substring(8,16),16))
            }
        }
       ]
      }
    callback(null, result) 
}
