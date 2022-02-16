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
	float labels[TRAIN_DATA_SIZE];
	float trainingData[TRAIN_DATA_SIZE][FEATURE_VECTOR_SIZE];

	FILE* fp = fopen("train_labels.txt", "r");

	for (int a = 0; a < TRAIN_DATA_SIZE; a++)
		fscanf(fp,"%f ",&labels[a]);

	fclose(fp);

	FILE* fp2 = fopen("cch.txt", "r");
	
	for (int a = 0; a < TRAIN_DATA_SIZE; a++)
		for (int b = 0; b < FEATURE_VECTOR_SIZE; b++)
			fscanf(fp2,"%f ",&trainingData[a][b]);

	fclose(fp2);

	
	Mat trainingDataMat(TRAIN_DATA_SIZE, FEATURE_VECTOR_SIZE, CV_32F, trainingData);
	Mat labelsMat(TRAIN_DATA_SIZE, 1, CV_32F, labels);


	Ptr<LogisticRegression> lr1 = LogisticRegression::create();
	lr1->setLearningRate(0.01);
	lr1->setIterations(10);
	lr1->setRegularization(LogisticRegression::REG_L2);
	lr1->setTrainMethod(LogisticRegression::MINI_BATCH);
	lr1->setMiniBatchSize(5);	
	Ptr<TrainData> td = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);
	lr1->train(td);


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

	int response_labels[TEST_DATA_SIZE];

	for (int i = 0; i < TEST_DATA_SIZE; ++i) 
    	{
		float one_row[FEATURE_VECTOR_SIZE];
		for (int x = 0; x < FEATURE_VECTOR_SIZE; ++x) 
			one_row[x] = testData[i][x];
		Mat testDataMat(1, FEATURE_VECTOR_SIZE, CV_32F, one_row);
		float pred = lr1->predict(testDataMat);
		response_labels[i] = (int)pred;
	}

	int result_matrix[10][2];
	for (int y = 0; y < 10; ++y)
		for (int z = 0; z < 2; ++z)
			result_matrix[y][z] = 0;

	int correct = 0, incorrect = 0;
	for (int i = 0; i < TEST_DATA_SIZE; ++i)
		if(t_labels[i] == response_labels[i])
		{
			correct++;
			result_matrix[t_labels[i]][0]++;
		}
		else
		{
			incorrect++;
			result_matrix[t_labels[i]][1]++;
		}

	printf("correct: %d -- incorrect: %d\npercentage: %% %.2f\n\n",correct,incorrect,100 * ((float)correct/(correct+incorrect)));
	for (int y = 0; y < 10; ++y)
		printf("DIGIT %d :: true: %d -- false: %d percentage: %% %.2f\n",y,result_matrix[y][0],result_matrix[y][1]
			,100 * ((float)result_matrix[y][0]/(result_matrix[y][0]+result_matrix[y][1])));
	

}





