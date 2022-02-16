#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/ml.hpp>

#define TRAIN_DATA_SIZE 25344
#define TEST_DATA_SIZE 4184
#define FEATURE_VECTOR_SIZE 16

using namespace std;
using namespace cv;
using namespace cv::ml;


int main()
{
	int labels[TRAIN_DATA_SIZE][10];
	float trainingData[TRAIN_DATA_SIZE][FEATURE_VECTOR_SIZE];
	int highest = 0;

	FILE* fp = fopen("train_labels.txt", "r");

	for (int a = 0; a < TRAIN_DATA_SIZE; a++)
	{
		int temp;
		fscanf(fp,"%d ",&temp);
		for (int b = 0; b < 10; b++)
			if(temp == b)
				labels[a][b] = 1;
			else
				labels[a][b] = 0;
	}
	fclose(fp);

	FILE* fp2 = fopen("cch.txt", "r");
	
	for (int a = 0; a < TRAIN_DATA_SIZE; a++)
		for (int b = 0; b < FEATURE_VECTOR_SIZE; b++)
			fscanf(fp2,"%f ",&trainingData[a][b]);

	fclose(fp2);

	for (int a = 0; a < TRAIN_DATA_SIZE; a++)
		for (int b = 0; b < FEATURE_VECTOR_SIZE; b++)
			trainingData[a][b] = 1.0/80.0 * trainingData[a][b];
				
	
	Mat trainingDataMat(TRAIN_DATA_SIZE, FEATURE_VECTOR_SIZE, CV_32F, trainingData);
	Mat labelsMat(TRAIN_DATA_SIZE, 10, CV_32F, labels);


	Mat_<int> layerSizes(1, 3);
        layerSizes(0, 0) = FEATURE_VECTOR_SIZE;
        layerSizes(0, 1) = 10*16;
	layerSizes(0, 2) = 10;
	
    
        Ptr<ANN_MLP> network = ANN_MLP::create();
        network->setLayerSizes(layerSizes);
        network->setActivationFunction(ANN_MLP::SIGMOID_SYM, 1, 1);
        network->setTrainMethod(ANN_MLP::BACKPROP,0.5,0.5);
	network->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER + TermCriteria::EPS, 1000, 0.00001f));

        Ptr<TrainData> trainData = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);
    
        network->train(trainData);


	int t_labels[TEST_DATA_SIZE];
	float testData[TEST_DATA_SIZE][FEATURE_VECTOR_SIZE];

	FILE* fp6 = fopen("test_labels.txt", "r");

	for (int a = 0; a < TEST_DATA_SIZE; a++)
		fscanf(fp6,"%d ",&t_labels[a]);

	fclose(fp6);

	FILE* fp7 = fopen("test_cch.txt", "r"); //9088
	
	for (int a = 0; a < TEST_DATA_SIZE; a++)
		for (int b = 0; b < FEATURE_VECTOR_SIZE; b++)
			fscanf(fp7,"%f ",&testData[a][b]);

	fclose(fp7);

	for (int a = 0; a < TEST_DATA_SIZE; a++)
		for (int b = 0; b < FEATURE_VECTOR_SIZE; b++)
			testData[a][b] = 1.0/80.0 * testData[a][b];

	float response_labels[TEST_DATA_SIZE][10];

	for (int i = 0; i < TEST_DATA_SIZE; ++i) 
    	{
		Mat res;
		float one_row[FEATURE_VECTOR_SIZE];
		for (int x = 0; x < FEATURE_VECTOR_SIZE; ++x) 
			one_row[x] = testData[i][x];
		Mat testDataMat(1, FEATURE_VECTOR_SIZE, CV_32F, one_row);
		network->predict(testDataMat, res);
		for (int y = 0; y < 10; ++y)
			response_labels[i][y] = res.at<float>(y);
	}


	int result_matrix[10][2];
	for (int y = 0; y < 10; ++y)
		for (int z = 0; z < 2; ++z)
			result_matrix[y][z] = 0;

	int correct = 0, incorrect = 0;
	for (int i = 0; i < TEST_DATA_SIZE; ++i)
	{
		int strongest_output =  0;
		float strongest_value = 0.0;
		for (int k = 0; k < 10; ++k)
			if(strongest_value < response_labels[i][k])
			{
				strongest_output = k;
				strongest_value = response_labels[i][k];
			}

		if(t_labels[i] == strongest_output)
		{
			correct++;
			result_matrix[t_labels[i]][0]++;
		}
		else
		{
			incorrect++;
			result_matrix[t_labels[i]][1]++;
		}

	}

	printf("correct: %d -- incorrect: %d\npercentage: %% %.2f\n\n",correct,incorrect,100 * ((float)correct/(correct+incorrect)));
	for (int y = 0; y < 10; ++y)
		printf("DIGIT %d :: true: %d -- false: %d percentage: %% %.2f\n",y,result_matrix[y][0],result_matrix[y][1]
			,100 * ((float)result_matrix[y][0]/(result_matrix[y][0]+result_matrix[y][1])));

}





