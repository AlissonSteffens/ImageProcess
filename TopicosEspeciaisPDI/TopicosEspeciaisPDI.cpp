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
Mat agucamento(Mat imagemBase);
Mat somar(Mat imagem1, Mat imagem2);

Mat escalaCinzaOPenCV(Mat imagemColorida);

int mat[5][5]={ 1,1,1,1,1 ,
				1,1,1,1,1 ,
				1,1,1,1,1 ,
				1,1,1,1,1 ,
				1,1,1,1,1 };

int main()
{
	Mat imagemOriginal;
	Mat imagemTeste;
	Mat imagemNegativo;
	Mat imagemGrayscale;
	imagemTeste = imread("Imagens/moon.tif");
	imagemNegativo = negativo(imagemTeste);
	imagemGrayscale = escalaCinza(imagemTeste);
	imagemOriginal = imagemTeste.clone();
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
	//imshow("Suavização", suavizacao(imagemTeste));
	imshow("agucamento", agucamento(imagemTeste));
	imshow("soma", somar(imagemTeste, imagemOriginal));
	waitKey(0);
    
	return 0;
}

Mat somar(Mat imagem1, Mat imagem2)
{
	Mat imagemResultado;
	if (imagem1.size().area() > imagem2.size().area())
		imagem1.copyTo(imagemResultado);
	else
		imagem2.copyTo(imagemResultado);

	for (int i = 0; i < imagemResultado.rows - 1; i++)
		for (int j = 0; j < imagemResultado.cols - 1; j++)
		{
			if (imagem2.rows > i && imagem2.cols > j)
			{
				imagemResultado.at<Vec3b>(i, j)[0] = (imagem1.at<Vec3b>(i, j)[0] + imagem2.at<Vec3b>(i, j)[0]) / 2;
				imagemResultado.at<Vec3b>(i, j)[1] = (imagem1.at<Vec3b>(i, j)[1] + imagem2.at<Vec3b>(i, j)[1]) / 2;
				imagemResultado.at<Vec3b>(i, j)[2] = (imagem1.at<Vec3b>(i, j)[2] + imagem2.at<Vec3b>(i, j)[2]) / 2;
			}
			else
			{
				imagemResultado.at<Vec3b>(i, j)[0] = (imagem1.at<Vec3b>(i, j)[0]);
				imagemResultado.at<Vec3b>(i, j)[1] = (imagem1.at<Vec3b>(i, j)[1]);
				imagemResultado.at<Vec3b>(i, j)[2] = (imagem1.at<Vec3b>(i, j)[2]);
			}
		}

	return imagemResultado;
}

Mat agucamento(Mat imagemBase) {
	Mat aux = imagemBase;
	Vec3b pixel;

	//Filtro A do Slide
	/*int filtro[3][3] = { { 0,1,0 },
	{ 1,-4,1 },
	{ 0,1,0 } };*/

	//Filtro B do Slide
	/*int filtro[3][3] = { { 1,1,1 },
	{ 1,-8, 1 },
	{ 1,1,1 } };*/

	//Filtro C do Slide
	//int filtro[3][3] = { { 0,-1,0 },
	//{ -1,4,-1 },
	//{ 0,-1,0 } };

	//Filtro D do Slide
	int filtro[3][3] = { { -1,-1,-1 },
	{ -1,8,-1 },
	{ -1,-1,-1 } };

	int somaR;
	int somaG;
	int somaB;
	int somaK;


	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			for (int linha = 0; linha < 3; linha++) {
				for (int coluna = 0; coluna < 3; coluna++) {					
						
					pixel = aux.at<Vec3b>(x + linha, y + coluna);

					somaR += filtro[linha][coluna] * pixel[2]; //Multiplica Vermelho
					somaG += filtro[linha][coluna] * pixel[1]; //Multiplica Verde
					somaB += filtro[linha][coluna] * pixel[0]; //Multiplica azul
					somaK += filtro[linha][coluna];					
				}
			}

			if (somaK == 0) {
				if (somaR > 255)
					pixel[2] = 255;
				else if (somaR < 0)
					pixel[2] = 0;
				else
					pixel[2] = somaR;

				if (somaR > 255)
					pixel[1] = 255;
				else if (somaR < 0)
					pixel[1] = 0;
				else
				pixel[1] = somaG;

				if (somaR > 255)
					pixel[0] = 255;
				else if (somaR < 0)
					pixel[0] = 0;
				else
				pixel[0] = somaB;
			}
			else {
				if (somaR / somaK > 255)
					pixel[2] = 255;
				else if (somaR / somaK < 0)
					pixel[2] = 0;
				else
					pixel[2] = somaR / somaK;

				if (somaG / somaK > 255)
					pixel[1] = 255;
				else if (somaG / somaK < 0)
					pixel[1] = 0;
				else
					pixel[1] = somaG / somaK;
				if (somaB / somaK > 255)
					pixel[0] = 255;
				else if (somaB / somaK < 0)
					pixel[0] = 0;
				else
					pixel[0] = somaB / somaK;
			}

			somaR = 0;
			somaG = 0;
			somaB = 0;
			somaK = 0;
			aux.at<Vec3b>(x, y) = pixel;

		}
	}
	return aux;
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
