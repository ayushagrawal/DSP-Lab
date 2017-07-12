% Plotting a pole zero plot for a discrete filter transfer function
% H(z) = Numerator(z)/Denominator(z)
% Numerator(z) = 1 - 1.2728z(-1) + 0.81z(-2) = b
% Denominator(z) = 1 + 0.81z(-2) = a
b = [1,-1.2728,0.81];
a = [1,-1.6,0.64];
% x = poles and o = zeroes
% zplane(b,a);

% Plots the frequency response
% figure;
%freqz(b,a);
% Clearly a low pass filter
% Fpass = (0.04102/2)*Fs = 984.48
% Fstop = (0.6121/2)*Fs = 14690.4
% Length of the FIR filter = 11

freqz(Num);
hold on;
Num = Num*(Num(1)^(-1));
Num_single_digit = round(((Num*10))/10);
%figure;
freqz(Num_single_digit);
hold on;
lines = findall(gcf,'type','line');
