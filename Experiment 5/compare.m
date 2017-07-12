function [abs_diff]= compare(filename1,filename2)
y_floating=load(filename1);
y=load(filename2);

plot(y);

abs_diff = y_floating - y;
end