close all;
clear all;


path='c:\wy2.txt'; %path of file with parameters
outfile='d:\obraz.tif';
obraz(:,:,1)=imread('d:\TestS.tif'); %original image
obraz(:,:,2)=obraz(:,:,1);
obraz(:,:,3)=obraz(:,:,2);
rozmiar=size(obraz);
rawData1 = importdata(path); 
[unused,name] = fileparts(path);
newData1.(genvarname(name)) = rawData1;

% Create new variables in the base workspace from those fields.
vars = fieldnames(newData1);

for i = 1:length(vars)
    assignin('base', vars{i}, newData1.(vars{i}));
    
end
rozz=size(rawData1.data);
z=rozz(1);
zero=[0 0 0 1 1 0 0 0 0
      0 0 1 0 0 1 0 0 0
      0 1 0 0 0 0 1 0 0
      0 1 0 0 0 0 1 0 0
      0 1 0 0 0 0 1 0 0
      0 1 0 0 0 0 1 0 0
      0 1 0 0 0 0 1 0 0
      0 0 1 0 0 1 0 0 0
      0 0 0 1 1 0 0 0 0];
  
one=[ 0 0 0 0 1 0 0 0 0
      0 0 0 1 1 0 0 0 0
      0 0 1 0 1 0 0 0 0
      0 1 0 0 1 0 0 0 0
      0 0 0 0 1 0 0 0 0
      0 0 0 0 1 0 0 0 0
      0 0 0 0 1 0 0 0 0
      0 0 0 0 1 0 0 0 0
      0 0 0 0 1 0 0 0 0];
  
two=[ 0 0 1 1 1 1 0 0 0
      0 0 0 0 0 1 0 0 0
      0 0 0 0 0 1 0 0 0
      0 0 0 0 0 1 0 0 0
      0 0 1 1 1 1 0 0 0
      0 0 1 0 0 0 0 0 0
      0 0 1 0 0 0 0 0 0
      0 0 1 0 0 0 0 0 0
      0 0 1 1 1 1 0 0 0];
  
 three=[ 0 0 1 1 1 1 0 0 0
      0 0 0 0 0 1 0 0 0
      0 0 0 0 0 1 0 0 0
      0 0 0 0 0 1 0 0 0
      0 0 1 1 1 1 0 0 0
      0 0 0 0 0 1 0 0 0
      0 0 0 0 0 1 0 0 0
      0 0 0 0 0 1 0 0 0
      0 0 1 1 1 1 0 0 0];
  
four=[ 0 0 1 0 0 1 0 0 0
      0 0 1 0 0 1 0 0 0
      0 0 1 0 0 1 0 0 0
      0 0 1 0 0 1 0 0 0
      0 0 1 1 1 1 0 0 0
      0 0 0 0 0 1 0 0 0
      0 0 0 0 0 1 0 0 0
      0 0 0 0 0 1 0 0 0
      0 0 0 0 0 1 0 0 0];
  
five=[ 0 0 1 1 1 1 0 0 0
      0 0 1 0 0 0 0 0 0
      0 0 1 0 0 0 0 0 0
      0 0 1 0 0 0 0 0 0
      0 0 1 1 1 1 0 0 0
      0 0 0 0 0 1 0 0 0
      0 0 0 0 0 1 0 0 0
      0 0 0 0 0 1 0 0 0
      0 0 1 1 1 1 0 0 0];
  
six=[ 0 0 1 1 1 1 1 0 0
      0 0 1 0 0 0 0 0 0
      0 0 1 0 0 0 0 0 0
      0 0 1 0 0 0 0 0 0
      0 0 1 1 1 1 1 0 0
      0 0 1 0 0 0 1 0 0
      0 0 1 0 0 0 1 0 0
      0 0 1 0 0 0 1 0 0
      0 0 1 1 1 1 1 0 0];
  
seven=[0 0 1 1 1 1 1 0 0
      0 0 0 0 0 0 1 0 0
      0 0 0 0 0 1 0 0 0
      0 0 0 0 1 0 0 0 0
      0 0 0 0 1 0 0 0 0
      0 0 0 0 1 0 0 0 0
      0 0 0 0 1 0 0 0 0
      0 0 0 0 1 0 0 0 0
      0 0 0 0 1 0 0 0 0];

eight=[0 0 1 1 1 1 1 0 0
      0 0 1 0 0 0 1 0 0
      0 0 1 0 0 0 1 0 0
      0 0 1 0 0 0 1 0 0
      0 0 1 1 1 1 1 0 0
      0 0 1 0 0 0 1 0 0
      0 0 1 0 0 0 1 0 0
      0 0 1 0 0 0 1 0 0
      0 0 1 1 1 1 1 0 0];
      
nine=[0 0 1 1 1 1 1 0 0
      0 0 1 0 0 0 1 0 0
      0 0 1 0 0 0 1 0 0
      0 0 1 0 0 0 1 0 0
      0 0 1 1 1 1 1 0 0
      0 0 0 0 0 0 1 0 0
      0 0 0 0 0 0 1 0 0
      0 0 0 0 0 0 1 0 0
      0 0 1 1 1 1 1 0 0];
  


for i=1:z
    kat=-rawData1.data(i,5);
    obraz(round(rawData1.data(i,1)),round(rawData1.data(i,2)),2)=65500;
    a=round(rawData1.data(i,3));
    b=round(rawData1.data(i,4));
    x=round(rawData1.data(i,1));
    
    y=round(rawData1.data(i,2));
              
    pom=mat2str(i);
    roz=size(pom);
    pom1=x;
    pom2=y;
    for j=1:roz(2)
        if(pom(j)=='0')
            for jj=1:9 
                for jjj=1:9 
                    if zero(jj,jjj)>0
                        obraz(pom1+jj,pom2+jjj,1)=zero(jj,jjj)*2000;
                        obraz(pom1+jj,pom2+jjj,2)=zero(jj,jjj)*65000;
                        obraz(pom1+jj,pom2+jjj,3)=zero(jj,jjj)*65000;
                    end;
                end;
            end;
        end;
        if(pom(j)=='1')
            for jj=1:9 
                for jjj=1:9 
                   
                    if one(jj,jjj)>0
                        obraz(pom1+jj,pom2+jjj,1)=one(jj,jjj)*2000;
                        obraz(pom1+jj,pom2+jjj,2)=one(jj,jjj)*65000;
                        obraz(pom1+jj,pom2+jjj,3)=one(jj,jjj)*65000;
                    end;
                end;
            end;
        end;
        if(pom(j)=='2')
            for jj=1:9 
                for jjj=1:9 
                    if two(jj,jjj)>0
                        obraz(pom1+jj,pom2+jjj,1)=two(jj,jjj)*2000;
                        obraz(pom1+jj,pom2+jjj,2)=two(jj,jjj)*65000;
                        obraz(pom1+jj,pom2+jjj,3)=two(jj,jjj)*65000;
                    end;
                end;
            end;
        end;
        if(pom(j)=='3')
            for jj=1:9 
                for jjj=1:9 
                    if three(jj,jjj)>0
                        obraz(pom1+jj,pom2+jjj,1)=three(jj,jjj)*2000;
                        obraz(pom1+jj,pom2+jjj,2)=three(jj,jjj)*65000;
                        obraz(pom1+jj,pom2+jjj,3)=three(jj,jjj)*65000;
                    end;
                end;
            end;
        end;
        if(pom(j)=='4')
            for jj=1:9 
                for jjj=1:9 
                    if four(jj,jjj)>0
                        obraz(pom1+jj,pom2+jjj,1)=four(jj,jjj)*2000;
                        obraz(pom1+jj,pom2+jjj,2)=four(jj,jjj)*65000;
                        obraz(pom1+jj,pom2+jjj,3)=four(jj,jjj)*65000;
                    end;
                end;
            end;
        end;
        if(pom(j)=='5')
            for jj=1:9 
                for jjj=1:9 
                    if five(jj,jjj)>0
                        obraz(pom1+jj,pom2+jjj,1)=five(jj,jjj)*2000;
                        obraz(pom1+jj,pom2+jjj,2)=five(jj,jjj)*65000;
                        obraz(pom1+jj,pom2+jjj,3)=five(jj,jjj)*65000;
                    end;
                end;
            end;
        end;
        if(pom(j)=='6')
            for jj=1:9 
                for jjj=1:9 
                    if six(jj,jjj)>0
                        obraz(pom1+jj,pom2+jjj,1)=six(jj,jjj)*2000;
                        obraz(pom1+jj,pom2+jjj,2)=six(jj,jjj)*65000;
                        obraz(pom1+jj,pom2+jjj,3)=six(jj,jjj)*65000;
                    end;
                end;
            end;
        end;
        if(pom(j)=='7')
            for jj=1:9 
                for jjj=1:9 
                    if seven(jj,jjj)>0
                        obraz(pom1+jj,pom2+jjj,1)=seven(jj,jjj)*2000;
                        obraz(pom1+jj,pom2+jjj,2)=seven(jj,jjj)*65000;
                        obraz(pom1+jj,pom2+jjj,3)=seven(jj,jjj)*65000;
                    end;
                end;
            end;
        end;
        if(pom(j)=='8')
            for jj=1:9 
                for jjj=1:9 
                    
                    if eight(jj,jjj)>0
                        obraz(pom1+jj,pom2+jjj,1)=eight(jj,jjj)*2000;
                        obraz(pom1+jj,pom2+jjj,2)=eight(jj,jjj)*65000;
                        obraz(pom1+jj,pom2+jjj,3)=eight(jj,jjj)*65000;
                    end;
                end;
            end;
        end;
        if(pom(j)=='9')
            for jj=1:9 
                for jjj=1:9 
                    if nine(jj,jjj)>0
                        obraz(pom1+jj,pom2+jjj,1)=nine(jj,jjj)*2000;
                        obraz(pom1+jj,pom2+jjj,2)=nine(jj,jjj)*65000;
                        obraz(pom1+jj,pom2+jjj,3)=nine(jj,jjj)*65000;
                    end;
                end;
            end;
        end;
        pom2=pom2+9;
    end;
                    
    xc=0;
    yc=b;
    aa=a*a;
    aa2=aa+aa;
    bb=b*b;
    bb2=bb+bb;
    d=bb-aa*b+(aa/4);
    dx=0;
    dy=aa2*b;
    obrz=[cos(kat) -sin(kat) 0 
        sin(kat) cos(kat) 0 
        0 0 1 ];
    przes1=[1 0 x
        0 1 y
        0 0 1];
   
        
    while(dx<dy)
        
        tab=[(-xc)
        (-yc)
        1];
        
        new=obrz*tab;
        new=przes1*new;
        if((round(new(1,1))>0)&&(round(new(2,1))>0))
            obraz(round(new(1,1)),round(new(2,1)),1)=65500;
            obraz(round(new(1,1)),round(new(2,1)),2)=0;
            obraz(round(new(1,1)),round(new(2,1)),3)=0;
        end;
        tab=[(xc)
        (-yc)
        1];
        
        new=obrz*tab;
        new=przes1*new;
       
        if((round(new(1,1))>0)&&(round(new(2,1))>0))
            obraz(round(new(1,1)),round(new(2,1)),1)=65500;
            obraz(round(new(1,1)),round(new(2,1)),2)=0;
            obraz(round(new(1,1)),round(new(2,1)),3)=0;
        end;
         tab=[(-xc)
        (yc)
        1];
        
        new=obrz*tab;
        new=przes1*new;
       
        if((round(new(1,1))>0)&&(round(new(2,1))>0))
            obraz(round(new(1,1)),round(new(2,1)),1)=65500;
            obraz(round(new(1,1)),round(new(2,1)),2)=0;
            obraz(round(new(1,1)),round(new(2,1)),3)=0;
        end;
         tab=[(xc)
        (yc)
        1];
        
        new=obrz*tab;
        new=przes1*new;
        
        if((round(new(1,1))>0)&&(round(new(2,1))>0))
            obraz(round(new(1,1)),round(new(2,1)),1)=65500;
            obraz(round(new(1,1)),round(new(2,1)),2)=0;
            obraz(round(new(1,1)),round(new(2,1)),3)=0;
        end;
         
   
         
         if(d>0)
             yc=yc-1;
             dy=dy-aa2;
             d=d-dy;
         end;
         xc=xc+1;
        
         dx=dx+bb2;
         d=d+bb+dx;
    end;
    d=d+(3*((aa-bb)/2)-(dx+dy)/2);


    while(yc>=0)
        tab=[(-xc)
        (-yc)
        1];
        
        new=obrz*tab;
        new=przes1*new;
        if((round(new(1,1))>0)&&(round(new(2,1))>0))
            obraz(round(new(1,1)),round(new(2,1)),1)=65500;
            obraz(round(new(1,1)),round(new(2,1)),2)=0;
            obraz(round(new(1,1)),round(new(2,1)),3)=0;
        end;
        tab=[(xc)
        (-yc)
        1];
        
        new=obrz*tab;
        new=przes1*new;
       
        if((round(new(1,1))>0)&&(round(new(2,1))>0))
            obraz(round(new(1,1)),round(new(2,1)),1)=65500;
            obraz(round(new(1,1)),round(new(2,1)),2)=0;
            obraz(round(new(1,1)),round(new(2,1)),3)=0;
        end;
         tab=[(-xc)
        (yc)
        1];
        
        new=obrz*tab;
        new=przes1*new;
       
        if((round(new(1,1))>0)&&(round(new(2,1))>0))
            obraz(round(new(1,1)),round(new(2,1)),1)=65500;
            obraz(round(new(1,1)),round(new(2,1)),2)=0;
            obraz(round(new(1,1)),round(new(2,1)),3)=0;
        end;
         tab=[(xc)
        (yc)
        1];
        
        new=obrz*tab;
        new=przes1*new;
        
        if((round(new(1,1))>0)&&(round(new(2,1))>0))
            obraz(round(new(1,1)),round(new(2,1)),1)=65500;
            obraz(round(new(1,1)),round(new(2,1)),2)=0;
            obraz(round(new(1,1)),round(new(2,1)),3)=0;
        end;
        if(d<0)
            xc=xc+1;
            if(xc>=x)
                yc=-1;
            end;
            dx=dx+bb2;
            d=d+(bb+dx);
        end;
        yc=yc-1;
        dy=dy-aa2;
        d=d+aa-dy;
    end;
end;
imagesc(obraz);
imwrite(obraz,outfile); 
