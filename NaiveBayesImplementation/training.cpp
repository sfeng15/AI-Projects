#include <iostream>
#include <stack>   
#include <queue>       
#include <fstream>
#include <string>
#include <cmath> 
#include <algorithm>   
#include <vector>    
#include <limits> 
#include <map> 
#include <sstream>
#include <cstring>
#include <iomanip> 
#include <math.h>

using namespace std;

double smooth = 0.0;

double myCount[10];

double confusionMatrix[10][10];

double highest[10];

double lowest[10];

double myArray[10][28][28];

char tempExample[28][28];

char examples[2][10][28][28];

double bigCount[10];

int highexample[10]; // show highest example and store the line number

int lowexample[10]; // show lowest example and store the line number

double allImageLabel = 0.0;

void initialize(){

	for(int i = 0; i < 10; i++){

		bigCount[i] = 0.0;
        myCount[i] = 0.0;

		for (int j = 0; j < 28; j++)
		{
			for (int k = 0; k < 28; k++)
			{
				myArray[i][j][k] = 0.0;
			}
		}
	}

    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            confusionMatrix[i][j] = 0.0;
        }
    }

    for (int j = 0; j < 10; j++)
    {
        highest[j] = 0.0;
        lowest[j] = 100000.0;
    }

}

void parseTrainingImage(){

	string lineLabel;
	string lineImage;


	ifstream myImageFile("trainingimages");
	ifstream myLabelFile("traininglabels");


	while ( getline(myLabelFile,lineLabel) )
    {
    	int size = atoi(lineLabel.c_str());



    	bigCount[size]++;

    	allImageLabel++;

    	for (int i = 0; i < 28; i++)
    	{
			getline(myImageFile, lineImage);

    		for (int j = 0; j < 28; j++)
    		{
               
    			if (lineImage[j] == '#' || lineImage[j] == '+')
    			{
    				myArray[size][i][j]++;
    			}
    		}
    	}
    }


    for (int k = 0; k < 10; k++)
    {
    	for(int i = 0; i < 28; i++)
    	{    
			for(int j = 0; j < 28; j++)
			{
				if(bigCount[k] != 0.0)
				{
					myArray[k][i][j] = myArray[k][i][j] / (bigCount[k] + smooth * bigCount[k]);	
				}

				if(myArray[k][i][j]==0.0)
				{
					myArray[k][i][j]=myArray[k][i][j] + smooth; // need to modify the smoothing
                    myArray[k][i][j] = myArray[k][i][j] / (bigCount[k] + smooth * bigCount[k]); 
				}
				
			}
		}
    }
}



void parseTestImage() {

	string lineLabel;
	string lineImage;

	ifstream myImageFile("testimages");
	ifstream myLabelFile("testlabels");

    int linecount = 0;

	while ( getline(myLabelFile,lineLabel) )
    {

    	int real = atoi(lineLabel.c_str()); // real class value



    	double predict[10];
    	for (int i = 0; i < 10; i++)
    	{
    		predict[i] = 0.0;
    	}
    	
    	for (int i = 0; i < 28; i++)  // image
    	{
			getline(myImageFile, lineImage);

    		for (int j = 0; j < 28; j++)
    		{
                 tempExample[i][j]=lineImage[j];
    			if (lineImage[j] == '#' || lineImage[j] == '+')
    			{
    				for(int k=0;k < 10;k++)
    				{
    					predict[k] = predict[k] + myArray[k][i][j];
    				}
    			}
    		}
    	}

        // do predicting
    	double max = 0.0;
    	int estimate = 0;

    	for(int k=0;k<10;k++)
    	{

    		predict[k]=predict[k]*(bigCount[k]/allImageLabel);

    	}
          

        for(int k=0;k<10;k++){

            if (max < predict[k])
            {

                max = predict[k];
                estimate = k;

            }
        }


        if (highest[estimate] <= predict[estimate])
        {
            highest[estimate] = predict[estimate];
            for (int i = 0; i < 28; i++)
            {
                for (int j = 0; j < 28; j++)
                {
                    examples[0][estimate][i][j]=tempExample[i][j];
                }
            }
            
            highexample[estimate] = linecount;
        }

        if (lowest[estimate] >= predict[estimate])
        {
            lowest[estimate] = predict[estimate];
             for (int i = 0; i < 28; i++)
            {
                for (int j = 0; j < 28; j++)
                {
                    examples[1][estimate][i][j]=tempExample[i][j];
                }
            }
        }
        
        confusionMatrix[real][estimate] = confusionMatrix[real][estimate] + 1.0;

        linecount++;

        myCount[real] = myCount[real] + 1.0;
        

    }

    double count = 0.0;

    for(int i=0;i<10;i++){    
        for(int j=0;j<10;j++){
            confusionMatrix[i][j] = confusionMatrix[i][j] / myCount[i];
        }
           
    }

}

void printHighIndex(int index){
 for (int i = 0; i < 28; i++)
    {
        for (int j = 0; j < 28; j++)
        {
            cout<<examples[0][index][i][j];
        }
        cout<<endl;
    }

}
void printLowIndex(int index){
 for (int i = 0; i < 28; i++)
    {
        for (int j = 0; j < 28; j++)
        {
            cout<<examples[1][index][i][j];
        }
        cout<<endl;
    }

}

void printLikelihood(int index){

    cout << "[";
 for (int i = 0; i < 28; i++)
    {
        for (int j = 0; j < 28; j++)
        {   
            if(int(myArray[index][i][j]*10)!=0){
                 cout<< int(myArray[index][i][j]*10) << " ";
            }
            else{
                 cout<< "0 ";
            }

           
        }
        cout << "; ";
        cout<<endl;
    }
    cout << "]";

}


//draw pic in matlab
/*
C = [0 0 5 0; 0 2 6 0; 0 1 5 0];
image(C)
colorbar;
*/



void odd(int c1,int c2){
    double oddMatrix[28][28];


  for (int i = 0; i < 28; i++)
    {
        for (int j = 0; j < 28; j++)
        {   
            oddMatrix[i][j]=0;
        }
    }


     for (int i = 0; i < 28; i++)
    {
        for (int j = 0; j < 28; j++)
        {   

            if(myArray[c2][i][j]!=0){
                oddMatrix[i][j] = log(myArray[c1][i][j]  /  myArray[c2][i][j]);
            }
            
           
        }
    }





cout<<"[";
     for (int i = 0; i < 28; i++)
    {
        for (int j = 0; j < 28; j++)
        {   
                 cout<< oddMatrix[i][j] << " ";
            
           
        }
        cout<<";";
        cout << endl;
    }

cout<<"]";


cout << endl;
cout << endl;
cout << endl;

}

int main()
{
	
	initialize();
	parseTrainingImage();
	parseTestImage();
    // for (int i = 0; i < 10; i++)
    // {
    //     printLowIndex(i);
    //     cout << endl;
    // }
    

    odd(6,0);
    odd(9,4);
    odd(8,3);
    odd(2,0);
// cout.setf(ios::fixed,ios::floatfield);
// cout.precision(2);
//     for (int i = 0; i < 10; i++)
//     {
//         for (int j = 0; j < 10; j++)
//         {
//             cout<<confusionMatrix[i][j]*100;
//             printf("%%  ");

//         }
//         cout<<endl;
//     }

	return 0;
}