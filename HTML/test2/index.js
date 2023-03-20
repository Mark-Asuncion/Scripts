window.onload = function() {
  invoke_at_startup();  
}
function invoke_at_startup(){
    let txt = document.getElementById("tosend");
    txt.innerHTML = "this is index";    
}

function send() {
    location.href = "index1.html";
}

