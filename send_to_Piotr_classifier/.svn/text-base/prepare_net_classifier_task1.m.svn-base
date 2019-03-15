

clear all; close all; clc

% P - input vector
P = [2 2 1 1  -1   -1  -2   -2   -2   -1   -1  -2  -2  4   1  1 -1;
     1 2 2 3  1.5   2  1.5  2.5   3   -1   -2  -1  -2  1  -3 -4 -4];
 
% T - output vector 
T = [1 1 1 1   0    0   0    0    0    0    0   0   0  0   0  0  0;
     0 0 0 0   1    1   1    1    1    0    0   0   0  0   0  0  0;
     0 0 0 0   0    0   0    0    0    1    1   1   1  0   0  0  0;
     0 0 0 0   0    0   0    0    0    0    0   0   0  1   1  1  1];


       net = newff(minmax(P),[ 4    ],{'logsig'},'trainlm');                     % I layer
       %net = newff(minmax(P),[10 4  ],{'tansig' 'logsig'},'trainlm');            % II layers
       %net = newff(minmax(P),[10 5 4],{'tansig' 'logsig' 'logsig'},'trainlm');   % III layers
       
       net.trainParam.epochs = 100;
       net.trainParam.show = 10;
       net.trainParam.goal = 0.00001;
       net.trainParam.mu_max =   1e17;
       
       net = train(net,P,T);
    
       ANN_classifier_Matlab_2_C