#include <iostream>
#include <stack>   
#include <queue>       
#include <fstream>
#include <string>
#include <cmath> 
#include <algorithm>   
#include <vector>    
#include <limits>
#include <iomanip>  
#include <cmath>  
#include <algorithm>    // std::sort
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>
using namespace std;


int imageArray[28][28];
float weightArray[10][28][28];
int t=0;
vector <int> estimate;
vector <int> label;
int confusion_matrix[10][10];
int all_number[10];

void initial(){
	for(int i=0;i<28;i++){
		for(int j=0;j<28;j++){
			imageArray[i][j]=0;
		}
	}

	for(int t=0;t<10;t++){ 
		for(int i=0;i<28;i++){
			for(int j=0;j<28;j++){
				weightArray[t][i][j]=1.0;
			}
		}
	}

	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			confusion_matrix[i][j]=0;
		}
	}

	for(int j=0;j<10;j++){
		all_number[j]=0;
	}

}

int getMax(float compare[10]){
	int estimate_result=0;
	for(int j=0;j<10;j++){
			if(compare[estimate_result]<compare[j]){
				estimate_result=j;
			}

	}
	return estimate_result;

}


void one_letter_train(int label){
	float compare[10];
	for(int j=0;j<10;j++){
			compare[j]=0.0;
	}
	for(int t=0;t<10;t++){ 
		for(int i=0;i<28;i++){
			for(int j=0;j<28;j++){
				    compare[t] = compare[t] + imageArray[i][j] * weightArray[t][i][j];
			}
		}
	}
	int estimate_result=getMax(compare);
	if(estimate_result==label){
		return;
	}
	for(int t=0;t<10;t++){
		if(t==label){
			for(int i=0;i<28;i++){
				for(int j=0;j<28;j++){
						weightArray[label][i][j] = weightArray[label][i][j] + imageArray[i][j] * ( 1000.0/(1000.0 + t) );
				}
			}
		} 
		else{
			for(int i=0;i<28;i++){
				for(int j=0;j<28;j++){
						weightArray[t][i][j] = weightArray[t][i][j] - imageArray[i][j] * ( 1000.0/(1000.0 + t) );
				}
			}
		}
	}



}



void train(){

	string lineLabel;
	string lineImage;


	ifstream myImageFile("trainingimages");
	ifstream myLabelFile("traininglabels");
	while ( getline(myLabelFile,lineLabel) )
    {
    	int correct_label = atoi(lineLabel.c_str());
		t=t+1;
    	for (int i = 0; i < 28; i++)
    	{
			getline(myImageFile, lineImage);
    		for (int j = 0; j < 28; j++)
    		{
    			if (lineImage[j] == '#' || lineImage[j] == '+')
    			{
    				imageArray[i][j] = 1;
    			}
    			else{
    				imageArray[i][j] = 0;
    			}
    		}
    	}

    	one_letter_train(correct_label);

    }





}






void one_letter_test(){
	float compare[10];
	for(int j=0;j<10;j++){
			compare[j]=0.0;
	}
	for(int t=0;t<10;t++){ 
		for(int i=0;i<28;i++){
			for(int j=0;j<28;j++){
				    compare[t] = compare[t] + imageArray[i][j] * weightArray[t][i][j];
			}
		}
	}
	
	estimate.push_back( getMax(compare) );
	


}


void test(){


	string lineLabel;
	string lineImage;


	ifstream myImageFile("testimages");
	ifstream myLabelFile("testlabels");
	while ( getline(myLabelFile,lineLabel) )
    {
    	int correct_label = atoi(lineLabel.c_str());
    	label.push_back(correct_label);
    	all_number[correct_label]++;
    	for (int i = 0; i < 28; i++)
    	{
			getline(myImageFile, lineImage);
    		for (int j = 0; j < 28; j++)
    		{
    			if (lineImage[j] == '#' || lineImage[j] == '+')
    			{
    				imageArray[i][j] = 1;
    			}
    			else{
    				imageArray[i][j] = 0;
    			}
    		}
    	}

    	one_letter_test();

    }
}

void accuracy(){
	for(int j=0;j<label.size();j++){
		confusion_matrix[label[j]][estimate[j]]++;	    
	}


}


void print_accuracy(){
	/*cout<<"print confusion_matrix:"<<endl;
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			float temp=100.0*confusion_matrix[i][j]*1.0/all_number[i];
			cout<<std::setprecision(4) <<temp<< " ";
		}
		cout<<endl;
	}

	cout<<"print_accuracy:"<<endl;
	for(int j=0;j<10;j++){
			cout<<std::setprecision(4) <<confusion_matrix[j][j]*100.0/all_number[j]<< "%% ";
	}
	cout<<endl;*/


	int all_correct=0.0;
	for(int j=0;j<label.size();j++){
		if(label[j] == estimate[j]){
			all_correct=all_correct+1.0;
		}	    
	}
	cout<<all_correct*100.0/label.size()<<"%% "<<endl;
}

int main(){
	initial();
	train();

	int count = 0;

	while(count < 9){
		t = 0;
		train();
		count++;
	}

	
	test();
	accuracy();
	print_accuracy();
	
	

	return 0;
}