
function require(modulePath){
	var fileContent = readFile(modulePath);	
	var script ="(function(module,exports){";
	script += fileContent;
	script+="})";

	var functor = eval (script);
	var module = {};
	var exports = {};
	functor.call(this,module,exports);
	return exports;
}


var module = require("Debug/module.js")

console.log(module.add(1,2))
console.log(module.max(3,4))