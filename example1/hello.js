var addon = require('bindings')('hello');

const my_string="Confusión"
console.log("original string=",my_string)
console.log("string returned by the hello function=",addon.hello(my_string))

