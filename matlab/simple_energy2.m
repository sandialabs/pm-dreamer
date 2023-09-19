function ans=simple_energy2(x_i,x_j,sigma)
  rsq=x_j-x_i;
  rsq=sum(rsq.*rsq);
  r1=sqrt(rsq);
  ans=sigma*r1;
  
