import mpmath as mp
import numpy as np
import matplotlib.pylab as pl
import sys

G=1.0;
rad=1.0

def rho_func(r,r0):
  #return (1-np.exp(-7.0*r))/np.sqrt(r)
  return (1.0)*np.exp(-r/r0)

def f_func(r,s):
  alpha=0.5*(r/s+s/r)
  t1 = 4*np.sqrt(alpha-1)/(alpha*alpha-1)*mp.ellipe(-2/(alpha-1))
  t2 = 2.0*(mp.ellipk(2./(1-alpha))/np.sqrt(alpha-1)+mp.ellipk(2/(1+alpha))/np.sqrt(alpha+1))
  return -float(s*s*G/np.power(2.0*r*s,1.5)*((r/s-alpha)*t1+t2))


def v_profile(r):
  return (1.0-np.exp(-r))/np.sqrt(r)



r2 = 10.0
ds=0.02
s=np.linspace(ds,(np.floor(r2/ds)+1)*ds,np.floor(r2/ds)+1)
n=len(s)
f=np.zeros((n,n))

for i in range(n):
  sys.stderr.write('i=%i\n' % i)
  for j in range(n):
    if i!=j:
      f[i,j]=f_func(s[i],s[j])
    else:
      f[i,j]=0.0
    pass
  pass

#g=-np.square(np.array([v_profile(i) for i in s]))/s/ds
#dens = np.linalg.solve(f.transpose(),g)

rho=np.array([rho_func(i,1.0) for i in s])
accel=np.dot(f,rho)*ds
#pl.plot(s,ans)
pl.plot(s,np.sqrt(-s*accel),s,rho)





F=open('points.txt','w')
#for i=range(50000):
close(F)







