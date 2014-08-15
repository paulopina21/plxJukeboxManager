function millisToFormattedTime(millis)
{
  var str = ""
  var s = Math.floor(millis / 1000)
  var m = Math.floor(s / 60)
  var h = Math.floor(m / 60)

  m %= 60
  s %= 60

  if (h < 10)
    str += "0"
  str += h
  str += ":"
  if (str == "00:")
    str = ""


  if (m < 10)
    str += "0"
  str += m
  str += ":"

  if (s < 10)
    str += "0"
  str += s

  return str
}

function getRandom(min, max)
{
//  return 200;
  var result = Math.floor((Math.random()*max)+1);

  if (result < min)
    return min
  else
    return result
}

function substr(sText, iStart, iCount) {
  return sText.slice(iStart,iCount);
}
