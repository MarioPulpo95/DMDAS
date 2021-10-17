clear 
clc

a = [0:2000]

timebase = 15

b = bitand(a,31);
c = bitand(a,2016);
c = bitshift(c,-5);

dur1 = (b+timebase)*timebase;
dur2 = (c+timebase)*timebase;


dur = dur1 + dur2


%% calcolo distribuzione di probabilità

tempi = dur'

padding = 500 %tra un onda quadra e l'altra c'è un padding di 100 ms (da verificare)
tempi = tempi+padding
pd = fitdist(tempi,'Normal')
x_pdf=[500:3000];
y= pdf(pd,x_pdf);

figure 
histogram(tempi, 'Normalization', 'pdf')
line(x_pdf, y)

%% calcolo durata salita e discesa usando un sistema di riferimento assoluto

secondi_abs = sum(tempi)*2/1000 % in origine sono millisecondi
minuti_abs = secondi_abs/60

%% calcolo durata salita e discesa usando un sistema di riferimento relativo (passo 1 decimo di millimetro)

secondi_per_step_rel = (tempi(2)/1000)
step_salita_discesa = 2000*2 % 2000 è il fondoscala

secondi_rel = secondi_per_step_rel * step_salita_discesa 

minuti_rel = secondi_rel/60



%% differenza tempi salita discesa assoluto vs relativo 

diff = minuti_abs - minuti_rel

