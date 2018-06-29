#include <iostream>
#include <math.h>
#include "PDI.h"
#include "opencv2\opencv.hpp"

using namespace cv;


int main()
{
	Mat imagem;
	imagem = imread("Imagens/label.png");
	imagem = PDI::escalaCinza(imagem);
	imagem = PDI::limiarizacao(imagem,128);
	imagem = PDI::labeling(imagem);
	for (int i = 1; i < 7; i = i + 2)
	{
		blur(imagem, imagem, Size(i, i), Point(-1, -1));
		

	}
	
	imshow("Fim", imagem);
	waitKey(0);


	return 0;
}