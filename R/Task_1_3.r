library(bsts)

N = 31
# Task 1
# a)
a1 = N
an = 6.5
v1_n = nchar("prakapenka")
v1 = seq(a1, an, length=v1_n)
v1
# b)
b1=abs(13-N)+2
bn=100
v2_n = nchar("tsimafei")
ratio = (bn/b1)^(1/(v2_n-1))
v2=GeometricSequence(v2_n, b1, ratio)
v2
# c)
v3 = sample(c(v1,2), 3)
v3
#v3[1]=5
#v3

# Task 3
m = matrix(v1, nrow=12,ncol=12, byrow=if(v3[1]>10) TRUE else FALSE)
m
n=(N-1)%%12+1
my_december <- c(month.name[n:12], month.name[1:n-1])
my_december 
month.name
colnames(m) <- my_december
rownames(m) <- my_december
m

m1 = m[ grepl( "^[^A-F]" , rownames(m)), grepl( "^[^A-F]" , colnames(m))]
m1
m1_det <- det(m1)
m1_det
zy<-eigen(m1)
eigen_values = y$val
eigen_vectors = y$vec
eigen_vectors
diag_values <- diag(m1)
diag_values
diag_squared <-diag_values^2
diag_squared
m1_squared <- m1^2
m1_squared
