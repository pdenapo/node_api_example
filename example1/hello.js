var addon = require('bindings')('hello');

const my_string="Confusión"
console.log(my_string)
console.log(addon.hello(my_string))

