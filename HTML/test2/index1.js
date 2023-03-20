// import {readFileSync, promises as fsPromises} from 'fs';

window.onload = function() {
  invoke_at_startup();  
}

function invoke_at_startup(){
    var fs = require('fs');
    const file = fs.readFileSync("text.txt",'utf-8');
    console.log(file);
}