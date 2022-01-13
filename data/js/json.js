function openUrl(url) 
{
    var request = new XMLHttpRequest();
    request.open('GET', url, true);
    request.send(null);
}


function refreshData() 
{
    var request = new XMLHttpRequest();
    request.open('GET', '/json', true);
    request.onreadystatechange = function() 
    {
      if (request.readyState == 4) 
      {
        var data = JSON.parse(request.responseText);
        document.getElementById('result').innerHTML = data.result;
      }

      request.send(null);
    }

    setInterval(refreshData, 500);
}