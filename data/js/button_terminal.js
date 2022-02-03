

function click_terminal(value) {

    var request = new XMLHttpRequest();
    
    console.log(value);
    
  var GET_Server =
    "click_terminal?" +
    "name_click=" +
    value;

  request.open("GET", GET_Server, true);
  request.send();
}

