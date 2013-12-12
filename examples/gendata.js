require("viclib")();
(function(){
  var fs, cols, dir, str, i$, t, j$, l, name, y, x;
  fs = require('fs');
  cols = ['0xFF0000', '0x00FF00', '0x0000FF'];
  dir = "/Users/vhsmaia/3d_multilayer_viewer/example/";
  str = "";
  for (i$ = 0; i$ < 100; ++i$) {
    t = i$;
    if (t > 0) {
      str += "\n";
    }
    for (j$ = 0; j$ < 3; ++j$) {
      l = j$;
      name = "layer" + l + "time" + t + ".dat";
      str += name + ";" + cols[l] + "\n";
      fs.writeFileSync(dir + name, "-0.5 -0.5 0.1 0.1\n" + (fn$()).join("\n"));
    }
  }
  log(str);
  function fn$(){
    var i$, results$ = [];
    for (i$ = 0; i$ <= 10; ++i$) {
      y = i$;
      results$.push((fn$()).join(" "));
    }
    return results$;
    function fn$(){
      var i$, results$ = [];
      for (i$ = 0; i$ <= 10; ++i$) {
        x = i$;
        results$.push(((l + 1) * 0.17 + (t / 10 + 1) * 0.04) - (Math.sqrt(Math.pow(x - 5, 2) + Math.pow(y - 5, 2)) * 0.004 * (t / 10 + 4)).toFixed(2));
      }
      return results$;
    }
  }
}).call(this);
