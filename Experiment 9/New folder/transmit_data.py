# -*- coding: utf-8 -*-
"""
Created on Tue Apr 11 11:15:43 2017

@author: dell
"""
n = 100
while(1):
    i = 0;
    input_str = input("")
    file_data = [c for c in input_str]
    listofzeros = [0] * n
    file_data = [1] + file_data + listofzeros
    f = open("Transmit_data.txt","w")
    
    while i<99:
        f.write(str(file_data[i]))
        i = i + 1   
    f.write(str(file_data[99]))
    f.close()  