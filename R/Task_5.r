library(datasets)
data(iris)

row_n = 11
df = iris[1:row_n, 1:3]

# a) without any feature
first_res = 0

first= (df[, 1]/df[, 2]-df[,3])^2

custom_indexes = rep(1:3, row_n/3)
rest = row_n%%3
if (rest != 0)
  custom_indexes <- c(custom_indexes, 1:rest)

second= (rep(df[1,1],row_n)+df[1, custom_indexes])

first_res = sum(second*first)
print(first_res)

# b) using loop
res_func = 0
for (i in 1:row_n){
  res_func = res_func + (((df[i, 1] / df[i, 2] - df[i, 3]) ^ 2) * (df[1, 1]+df[1, (i-1) %% 3+1]))
}

print(res_func)
