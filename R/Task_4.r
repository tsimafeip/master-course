library(data.table)
library(forcats)

# x <- data.frame("SN" = 1:2, "Age" = c(21,15), "Name" = c("John","Dora"))

nums <- c(1,2,3,4,5,6)
words <- c("red", "white", "red", "red", "yellow", "white")
conditions <- c(FALSE, TRUE,TRUE,FALSE,TRUE,FALSE)
f <- factor(words)
df <- data.table(nums, words, conditions, f, stringsAsFactors = FALSE)
df
str(df)

f_count <- fct_count(f)
f_count
border_value = 1
f_count[f_count[,2] > border_value,1]
