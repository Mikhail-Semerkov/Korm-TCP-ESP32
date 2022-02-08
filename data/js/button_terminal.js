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

function time_terminal()
{
    var Data = new Date();
    var Hour = Data.getHours();
    var Minutes = Data.getMinutes();
  var Seconds = Data.getSeconds();
  if (Hour.toString().length == 1)
  {
    Hour = "0" + Hour;
  }

    if (Minutes.toString().length == 1)
  {
    Minutes = "0" + Minutes;
  }

    if (Seconds.toString().length == 1)
  {
    Seconds = "0" + Seconds;
  }
  

    var Time = Hour + ":" + Minutes + ":" + Seconds;
  
  var request = new XMLHttpRequest();
    
  var GET_Server =
    "time_terminal?" +
    "time=" +
    Time;

  console.log(Time);
  request.open("GET", GET_Server, true);
  request.send();
}


function recipe_terminal_clear()
{
  var request = new XMLHttpRequest();
  var GET_Server =
    "recipe_terminal_clear?" +
    "terminal_clear=" +
    "true";
  request.open("GET", GET_Server, true);
  request.send();
}

