The RSL stands for the radiography scripting language.

Here are a basic rules of correct writing a script files:
	- only one instruction in the line is allowed
	- comments cant be in the same line as commands
	- one line cant be longer than 256 chars
	- do not use spaces in the files names
	- use double backslash in the file paths (see examples below)
	
Known Bugs:
	- tiff images with odd x or y resolution are read incoretly
	- some error messages are a little bit enigmatic


% implemented commands:

% IMAGE = imread('c:\\test.tif');
% handle = imshow('title',IMAGE,type);
%			type = 0	original image
%			type = 1	normalized image
%			title - an image title and its handle as well
% IMAGE = dilate(IMAGE,size_x,size_y);
% IMAGE = erode(IMAGE,size_x,size_y);
% IMAGE = bdilate(IMAGE,size_x,size_y);
% IMAGE = berode(IMAGE,size_x,size_y);
% IMAGE = outline(IMAGE);
% IMAGE = median(IMAGE,type,size_x,size_y);
%			type = 0 - Median rectangle
%			type = 1 - Median cross
%			type = 2 - Median X
% saveimagem('c:\\test.tif',IMAGE);
%			saveimagem nagrywa obrazek jako bin - do odczytu pod matlabem
% IMAGE = imadd(IMAGE, IMAGE);
% IMAGE = imsub(IMAGE, IMAGE);
% IMAGE = immulti(IMAGE, IMAGE);
% IMAGE = ath(IMAGE,mask,coef);
% IMAGE = gth(IMAGE,mask,coef,scale);
% IMAGE = oth(IMAGE,coef);
% IMAGE = eth(IMAGE,LargeWindowSize,SmallWindowSize,meanDiff,ThCoef,GrayScaleLevels);
% IMAGE = fftfilt(IMAGE, cutoff1, cutoff2);
%			cutoff - normalised cutoff frequency 0-1, cutoff1<cutoff2
% hist(IMAGE,scale);
%			scale - y-axis scale (magnification)
% particals(IMAGE,IMAGE,minsize,maxsize,filename1);
%			IMAGE1 - original image
%			IMAGE2 - binary image (eg after thresholding)
%			minsize, maxsize - the objects of size between minsize and maxsize will be marked as cracks
%			filename1 - output text file with parameters of flaws
% imclose(handle);


MEDIAN

Synatx:
	output = median(input, type, sizex, sizey);

Parameters:
	input - input image;
	output - output image
	type - type of a median; 0 - rectangle mask, 1 - cross mask, 2- X mask
	sizex - x size of a mask
	sizey - y size of a mask

Description:
	2-D median filtering. The median command applies an two-dimensional median filter of size x x y to image input. Output output has the same size as input.
	
Example code:

	after = MEDIAN(before, 0, 9, 9);


INDEX

Syntax:
	INDEX( image1, image2, min, max, path);
	
Parameters:
	image1 - original image
	image2 - binary image
	min - minimal size of a defect
	max - maximum size of a defect;
	path - path to the file where results will be recorded

Description:
	Part of this algorythm is taken from book R. Tadusiewicz and P. Korohoda. The binary image 

Example code:
	index(image,bin,30,60000,"D:\\test.txt");