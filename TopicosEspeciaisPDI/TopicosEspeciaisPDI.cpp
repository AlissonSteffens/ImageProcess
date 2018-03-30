#include <iostream>
#include <math.h>
#include "opencv2\opencv.hpp"


using namespace cv;

Mat canal(Mat imagemColorida, int canal);
Mat negativo(Mat imagemBase);
Mat escalaCinza(Mat imagemColorida);
Mat limiarizacao(Mat imagemBase, int limiar);
Mat logaritmo(Mat imagemBase);
Mat potencia(Mat imagemBase, double gamma);
Mat janelamento(Mat imagemBase, int limiar_inf, int limiar_sup);
Mat suavizacao(Mat imagemBase);

Mat escalaCinzaOPenCV(Mat imagemColorida);

int mat[5][5]={ 1,1,1,1,1 ,
				1,1,1,1,1 ,
				1,1,1,1,1 ,
				1,1,1,1,1 ,
				1,1,1,1,1 };

int main()
{
	Mat imagemTeste;
	Mat imagemNegativo;
	Mat imagemGrayscale;
	imagemTeste = imread("Imagens/placa.tif");
	imagemNegativo = negativo(imagemTeste);
	imagemGrayscale = escalaCinza(imagemTeste);
	imshow("Teste", imagemTeste);
	/*imshow("Negativo", imagemNegativo);
	imshow("Cinza", imagemGrayscale);
	imshow("OPCV", escalaCinzaOPenCV(imagemTeste));
	imshow("BLUE", canal(imagemTeste,0));
	imshow("VERDE", canal(imagemTeste,1));
	imshow("VERMELHO", canal(imagemTeste,2));
	imshow("LIMIAR", limiarizacao(escalaCinza(imagemTeste), 125));
	imshow("LOGARITMO", logaritmo(imagemTeste));
	imshow("POTENCIA", potencia(imagemTeste, 2));*/
	//imshow("Janelamento", janelamento(imagemTeste, 90, 138));
	imshow("Suavizaçãop", suavizacao(imagemTeste));
	waitKey(0);
    
	return 0;
}

Mat canal(Mat imagemColorida, int canal) {
	Mat aux = imagemColorida.clone();

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			Vec3b pixel = aux.at<Vec3b>(x, y);

			pixel[0] = pixel[canal];
			pixel[1] = pixel[canal];
			pixel[2] = pixel[canal];

			aux.at<Vec3b>(x, y) = pixel;
		}
	}

	return aux;
}

Mat negativo(Mat imagemBase) {
	Mat aux = imagemBase.clone();

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			Vec3b pixel = aux.at<Vec3b>(x, y);				
			
			uchar Nred = 255 - pixel[2];
			uchar Ngreen = 255 - pixel[1];
			uchar Nblue = 255 - pixel[0];

			pixel[2] = Nred;
			pixel[1] = Ngreen;
			pixel[0] = Nblue;

			aux.at<Vec3b>(x, y) = pixel;

		}
	}

	return aux;
}

Mat escalaCinza(Mat imagemColorida) {
	Mat aux = imagemColorida.clone();

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			Vec3b pixel = aux.at<Vec3b>(x, y);
			
			uchar media = (pixel[0]*0.114 + pixel[1]*0.587 + pixel[2] * 0.299);

			uchar Nred = media;
			uchar Ngreen = media;
			uchar Nblue = media;

			pixel[2] = Nred;
			pixel[1] = Ngreen;
			pixel[0] = Nblue;

			aux.at<Vec3b>(x, y) = pixel;
		}
	}

	return aux;
}

Mat limiarizacao(Mat imagemBase, int limiar) {
	Mat aux = imagemBase.clone();
	aux = escalaCinzaOPenCV(aux);
	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			uchar pixel = aux.at<uchar>(x, y);

			if (pixel > limiar)
			{
				pixel = 255;
			}
			else
			{
				pixel = 0;
			}

			aux.at<uchar>(x, y) = pixel;
		}
	}

	return aux;
}

Mat janelamento(Mat imagemBase, int li, int ls) {
	Mat aux = imagemBase.clone();
	aux = escalaCinzaOPenCV(aux);
	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			uchar pixel = aux.at<uchar>(x, y);


			if (pixel >= ls)
			{
				pixel = 255;
			}
			else if (pixel <= li)
			{
				pixel = 0;
			}
			else
			{
				pixel = (pixel - li) * (255 / (ls - li));
			}

			aux.at<uchar>(x, y) = pixel;
		}
	}

	return aux;
}
Mat suavizacao(Mat imagemBase)
{
	Mat aux = imagemBase.clone();
	aux = escalaCinzaOPenCV(aux);
	for (int x = 2; x < aux.rows-2; x++) {
		for (int y = 2; y < aux.cols-2; y++) {
			uchar pixel = aux.at<uchar>(x, y);

			double soma = 0;

			for (int i = 0, xi = x-2; i < 5; i++, xi++)
			{
				for (int j = 0, yj = y-2; j < 5; j++, yj++)
				{
					soma += mat[i][j] * aux.at<uchar>(xi, yj);
				}
			}

			pixel = soma / 25;

			aux.at<uchar>(x, y) = pixel;
		}
	}

	return aux;
}
Mat logaritmo(Mat imagemBase) {
	Mat aux = escalaCinzaOPenCV(imagemBase.clone());
	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			uchar pixel = aux.at<uchar>(x, y);

			double c = 255 / log(256);

			pixel = c * log(1 + pixel);

			aux.at<uchar>(x, y) = pixel;
		}
	}

	return aux;
}

Mat potencia(Mat imagemBase, double gamma) {
	Mat aux = escalaCinzaOPenCV(imagemBase.clone());
	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			uchar pixel = aux.at<uchar>(x, y);

			double c = 255 / pow(256, gamma);

			pixel = c * pow(pixel,gamma);

			aux.at<uchar>(x, y) = pixel;
		}
	}

	return aux;
}

Mat escalaCinzaOPenCV(Mat imagemColorida) {
	Mat aux;

	cv::cvtColor(imagemColorida, aux, CV_BGR2GRAY);

	return aux;
}
