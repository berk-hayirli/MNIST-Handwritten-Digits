Byte files must be on the same directory as the executables
OpenCV Must be installed
OS: Linux Ubuntu 16.04


Order of Compilation (Pipelining):


gcc train_images.c -o train_images.out && gcc gaus_filter222.c -o gaus_filter222.out && gcc sobel_nonmax.c -lm -o sobel_nonmax.out && gcc thresholding_hyst.c -o thresholding_hyst.out && gcc feature_extract.c -o feature_extract.out

gcc test_images.c -o test_images.out && gcc test_gaus_filter222.c -o test_gaus_filter222.out && gcc test_sobel_nonmax.c -lm -o test_sobel_nonmax.out && gcc test_thresholding_hyst.c -o test_thresholding_hyst.out && gcc test_feature_extract.c -o test_feature_extract.out


Execution order (Pipelining): 


./train_images.out && ./gaus_filter222.out && ./sobel_nonmax.out && ./thresholding_hyst.out && ./feature_extract.out

./test_images.out && ./test_gaus_filter222.out && ./test_sobel_nonmax.out && ./test_thresholding_hyst.out && ./test_feature_extract.out


g++ knn.cpp -std=c++11 `pkg-config --cflags --libs opencv`

./a.out

g++ svm.cpp -std=c++11 `pkg-config --cflags --libs opencv`

./a.out

g++ bayes.cpp -std=c++11 `pkg-config --cflags --libs opencv`

./a.out

g++ logistic_regression.cpp -std=c++11 `pkg-config --cflags --libs opencv`

./a.out
