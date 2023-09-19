function calc_simple_3body(count)
  % number of particles
  N=3;
  % const coefficients
  sigma2=[8 9 10];
  sigma3=[2 3 4 5 6 7];
  eps=1e-8;
  
  for ii=1:count
    x=rand(N,3);
    xp=x;
    types=floor(rand(N,1)*2);
    
    energy=0;
    force_x=0;
    force_y=0;
    force_z=0;
    
    for i=1:N
      for j=(i+1):N
        ui=ui_pair(types(i),types(j))+1;
        energy=energy+simple_energy2(x(i,:),x(j,:),sigma2(ui));
        xp(1,:)=x(1,:)+eps*[1 0 0];
        force_x=force_x+simple_energy2(xp(i,:),xp(j,:),sigma2(ui));
        xp(1,:)=x(1,:)+eps*[0 1 0];
        force_y=force_y+simple_energy2(xp(i,:),xp(j,:),sigma2(ui));
        xp(1,:)=x(1,:)+eps*[0 0 1];
        force_z=force_z+simple_energy2(xp(i,:),xp(j,:),sigma2(ui));
      end
    end
        
    for i=1:N
      for j=1:(N-1)
        if (i==j)
          continue;
        end
        for k=(j+1):3;
          if (i==k)
            continue;
          end

          ui=ui_three(types(i),types(j),types(k),2)+1;
          % Compute the energy with r1, r2, cos(a), and sigma
          energy=energy+simple_energy3(x(i,:),x(j,:),x(k,:),sigma3(ui));
          xp(1,:)=x(1,:)+eps*[1 0 0];
          force_x=force_x+simple_energy3(xp(i,:),xp(j,:),xp(k,:),sigma3(ui));
          xp(1,:)=x(1,:)+eps*[0 1 0];
          force_y=force_y+simple_energy3(xp(i,:),xp(j,:),xp(k,:),sigma3(ui));
          xp(1,:)=x(1,:)+eps*[0 0 1];
          force_z=force_z+simple_energy3(xp(i,:),xp(j,:),xp(k,:),sigma3(ui));
        end
      end
    end
    
    force_x=(energy-force_x)/eps;
    force_y=(energy-force_y)/eps;
    force_z=(energy-force_z)/eps;
    fprintf('%3.16f 1 %3.16f %3.16f %3.16f %d %3.16f %3.16f %3.16f %d %3.16f %3.16f %3.16f %d %3.16f %3.16f %3.16f\n',energy,force_x,force_y,force_z,types(1)+1,x(1,1),x(1,2),x(1,3),types(2)+1,x(2,1),x(2,2),x(2,3),types(3)+1,x(3,1),x(3,2),x(3,3));
  end
  