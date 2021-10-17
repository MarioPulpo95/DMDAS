

a = 1

timebase = 15

b = bitand(a,31)
c = bitand(a,2016);
c = bitshift(c,-5)

dur1 = (b+timebase)*timebase
dur2 = (c+timebase)*timebase


dur = dur1 + dur2

