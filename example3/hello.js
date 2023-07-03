var addon = require('bindings')('hello');

function print(s)
{
 console.log("from the print function=",s)
}

global.print=print
global.hello=addon.hello
console.log(global.hello)

console.log(addon)
const my_string="Confusión"
print(my_string)
console.log("original string=",my_string)
console.log("string returned by the hello function=",addon.hello(my_string))

console.log("string returned by the hello2 function=",addon.hello2(my_string))


