

       layers = [];
       for k = 1:net.numLayers
       layers = [ layers net.layers{k}.dimensions];
       end
       if net.numLayers >= 1
            W1 = net.IW{1};      W1_ = prepareVector(W1);    B1 = net.b{1,1}; 	B1_ = prepareVector(B1);   
            sizeW1 = size(W1);   sizeB1 = size(B1);
       end
       
       if net.numLayers >= 2
            W2 = net.LW{2,1};    W2_ = prepareVector(W2);    B2 = net.b{2,1}; 	B2_ = prepareVector(B2);
            sizeW2 = size(W2);   sizeB2 = size(B2);
       end
       
       if net.numLayers >= 3
            W3 = net.LW{3,2};    W3_ = prepareVector(W3);     B3 = net.b{3,1}; 	B3_ = prepareVector(B3);
            sizeW3 = size(W3);    sizeB3 = size(B3);
       end
       
       sizeX = [net.inputs{1}.size, 1];
              
       % save to file
       ANN_name = ['ffnet_ex4_NN_classifier_' num2str(layers(1)) ];
       if net.numLayers > 1
           for k=2:net.numLayers
                ANN_name = [ANN_name '_' num2str(layers(k)) ];
           end
       end
       ANN_name = [ANN_name '.ann']
       
       fid = fopen(ANN_name, 'wt');
       fprintf(fid, 'net\n');         fprintf(fid, '%d\n', layers);
       fprintf(fid, ['E\n' net.layers{1}.transferFcn '\n']);  
       for k=2:net.numLayers
           fprintf(fid, [ net.layers{k}.transferFcn '\n']);   
       end
       
       if net.numLayers >= 1
           fprintf(fid, 'E\nW1\n');        fprintf(fid, '%18.12f\n', W1_);
           fprintf(fid, 'E\nB1\n');        fprintf(fid, '%18.12f\n', B1_);
           fprintf(fid, 'E\nsizeW1\n');    fprintf(fid, '%d\n', sizeW1);
           fprintf(fid, 'E\nsizeB1\n');    fprintf(fid, '%d\n', sizeB1);
       end
       
       if net.numLayers >= 2
           fprintf(fid, 'E\nW2\n');        fprintf(fid, '%18.12f\n', W2_);
           fprintf(fid, 'E\nB2\n');        fprintf(fid, '%18.12f\n', B2_);
           fprintf(fid, 'E\nsizeW2\n');    fprintf(fid, '%d\n', sizeW2);
           fprintf(fid, 'E\nsizeB2\n');    fprintf(fid, '%d\n', sizeB2);
       end
       
       if net.numLayers >= 3
           fprintf(fid, 'E\nW3\n');        fprintf(fid, '%18.12f\n', W3_);
           fprintf(fid, 'E\nB3\n');        fprintf(fid, '%18.12f\n', B3_);
           fprintf(fid, 'E\nsizeW3\n');    fprintf(fid, '%d\n', sizeW3);
           fprintf(fid, 'E\nsizeB3\n');    fprintf(fid, '%d\n', sizeB3);
       end
       
       fprintf(fid, 'E\nsizeX\n');     fprintf(fid, '%d\n', sizeX);
       fprintf(fid, 'E\n');
       fclose(fid)
       
       save ffnet_ex4_NN_classifierWORKSPC 