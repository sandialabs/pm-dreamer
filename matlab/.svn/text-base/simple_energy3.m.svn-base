function ans=simple_energy3(x_i,x_j,x_k,sigma)
  rsq=x_j-x_i;
  rsq2=x_k-x_i;
  a=sum(rsq.*rsq2);
  rsq=sum(rsq.*rsq);
  rsq2=sum(rsq2.*rsq2);
  a=a/sqrt(rsq*rsq2);
  r1=sqrt(rsq);
  r2=sqrt(rsq2);
  
  ans=sigma*(r1*r2+a);
  
  
