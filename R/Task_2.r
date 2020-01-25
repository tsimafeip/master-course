library(stringr)

alf = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя"
test_str <-"абfя"

get_letters_sum <- function(my_str) {
  splitted <- unlist(strsplit(my_str, "", fixed=TRUE))
  n = length(splitted)
  res = 0
  for(i in 1:n)
  {
    symbol = splitted[i]
    # print(match(c(symbol), alf))
    pos_in_alf = str_locate(pattern = splitted[i], alf)[1]
    if(is.na(pos_in_alf)){
      print(sprintf("Unrecognized symbol: %s", symbol))
      return(NA)
    } else{
      res = res + pos_in_alf
    }
  }
  return(res)
}

get_letters_sum(test_str)