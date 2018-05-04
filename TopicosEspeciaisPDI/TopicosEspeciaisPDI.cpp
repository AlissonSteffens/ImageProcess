#include <iostream>
#include <math.h>
#include "opencv2\opencv.hpp"


using namespace cv;
using Matriz = std::vector<std::vector<int>>;

Mat canal(Mat imagemColorida, int canal);
Mat negativo(Mat imagemBase);
Mat escalaCinza(Mat imagemColorida);
Mat limiarizacao(Mat imagemBase, int limiar);
Mat logaritmo(Mat imagemBase);
Mat potencia(Mat imagemBase, double gamma);
Mat janelamento(Mat imagemBase, int limiar_inf, int limiar_sup);
Mat suavizacao(Mat imagemBase, int size);
Mat agucamento(Mat imagemBase, bool tipo);
Mat agucamento_cinza(Mat imagemBase, int tipo);
Mat somar(Mat imagem1, Mat imagem2);
Mat somar128(Mat imagem1, Mat imagem2);
Mat subtrair(Mat imagem1, Mat imagem2);
Mat alargamento(cv::Mat imagemBase);
Mat somarHard(Mat imagem1, Mat imagem2);
Mat escalaCinzaOPenCV(Mat imagemColorida);
Mat erosao(Mat imagemBase, Matriz elemEstrut);
Mat dilatacao(Mat imagemBase, Matriz elemEstrut);
Mat abertura(Mat imageBase, Matriz elemEstrut);
Mat fechamento(Mat imageBase, Matriz elemEstrut);
Matriz esCustom(int tamX, int tamY, ...);
Matriz elemEstrut11x11circle();
Matriz elemEstrut33circle();
Matriz kernelPonderado5();
Matriz kernelCompleto(int tamanho);
float limiarizacaoGlobalSimples(Mat imagemBase);
void histograma(Mat imagemBase, float vec[256]);


//int mat[5][5]={ 1,1,1,1,1 ,1,1,1,1,1 ,1,1,1,1,1 ,1,1,1,1,1 ,1,1,1,1,1 };

int main()
{
	Mat imagemOriginal;
	Mat imagemTeste;
	Mat imagemNegativo;
	Mat imagemGrayscale;
	imagemTeste = imread("Imagens/erosao.tif");
	imagemNegativo = negativo(imagemTeste);
	imagemGrayscale = escalaCinzaOPenCV(imagemTeste);
	imagemOriginal = imagemTeste.clone();
	//int a = limiarizacaoGlobalSimples(imagemTeste);

	imshow("Original", imagemTeste);
	imshow("Erosao", erosao(imagemGrayscale, kernelCompleto(11)));
	//imshow("Dilatacao", dilatacao(imagemGrayscale, kernelCompleto(11)));
	/*imshow("Abertura", abertura(imagemGrayscale, elemEstrut33circle()));
	imshow("Fechamento", fechamento(imagemGrayscale, elemEstrut33circle()));*/
	/*imshow("Negativo", imagemNegativo);
	imshow("Cinza", imagemGrayscale);
	imshow("OPCV", escalaCinzaOPenCV(imagemTeste));
	imshow("BLUE", canal(imagemTeste,0));
	imshow("VERDE", canal(imagemTeste,1));
	imshow("VERMELHO", canal(imagemTeste,2));
	imshow("LIMIAR", limiarizacao(escalaCinza(imagemTeste), 125));*/
	
	//imshow("POTENCIA", potencia(imagemTeste, 2));
	
	//imshow("Suavização", suavizacao(imagemTeste));

	/*imagemTeste = agucamento(imagemTeste, true);
	imshow("Aguçamento normalizado", imagemTeste);
	imshow("Soma Aguçamento normal", somar128(imagemTeste, imagemOriginal));


	imagemTeste = agucamento(imagemOriginal.clone(), false);
	imshow("Aguçamento", imagemTeste);
	imshow("Soma Aguçamento", somarHard(imagemTeste, imagemOriginal));

	imagemTeste = agucamento(imagemOriginal.clone(), false);
	imshow("Aguçamentoasd", imagemTeste);
	imshow("Soma Aguçamento Hard", somarHard(imagemTeste, imagemOriginal));
	*/
	/*imagemTeste = imagemOriginal.clone();
	imshow("Suave", suavizacao(imagemTeste, 9));
	imshow("SubSuave", subtrair(imagemTeste, suavizacao(imagemTeste, 9)));
	imshow("Mascara de Nitidez", somarHard(imagemTeste, subtrair(imagemTeste, suavizacao(imagemTeste, 9))));*/



	//imshow("LOGARITMO", alargamento(imagemTeste));
	waitKey(0);
    
	return 0;
}

Matriz kernelPonderado5() {
	return esCustom(5, 5,
		1, 4, 6, 4, 1,
		4, 16, 24, 16, 4,
		6, 24, 36, 24, 6,
		4, 16, 24, 16, 4,
		1, 4, 6, 4, 1);
}

Matriz kernelCompleto(int tamanho) {
	Matriz retorno;

	for (int i = 0; i < tamanho; i++) {
		std::vector<int> aux;
		for (int j = 0; j < tamanho; j++) {
			aux.push_back(1);
		}
		retorno.push_back(aux);
	}

	return retorno;
}

Matriz elemEstrut33circle() {
	return esCustom(3, 3,
		0,1,0,
		1,1,1,
		0,1,0);
}

Matriz elemEstrut11x11circle() {
	return esCustom(11, 11,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0,
		0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
		0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0);
}

int intensidadeMaxima(cv::Mat imagemBase) {
	int maximo = 0;
	for (int x = 0; x < imagemBase.rows; x++) {
		for (int y = 0; y < imagemBase.cols; y++) {
			uchar pixel = imagemBase.at<uchar>(x, y);
			if (pixel > maximo) {
				maximo = pixel;
			}
		}
	}

	return maximo;
}

int intensidadeMinima(cv::Mat imagemBase) {
	int minimo = 255;
	for (int x = 0; x < imagemBase.rows; x++) {
		for (int y = 0; y < imagemBase.cols; y++) {
			uchar pixel = imagemBase.at<uchar>(x, y);
			if (pixel < minimo) {
				minimo = pixel;
			}
		}
	}

	return minimo;
}

Mat alargamento(cv::Mat imagemBase) {
	Mat aux = imagemBase.clone();

	int minI = intensidadeMinima(imagemBase);
	int maxI = intensidadeMaxima(imagemBase);
	float escala = maxI - minI;

	for (int x = 0; x < imagemBase.rows; x++) {
		for (int y = 0; y < imagemBase.cols; y++) {
			uchar pixel = imagemBase.at<uchar>(x, y);
			uchar novoPixel = (pixel - minI) * (255 / escala);

			aux.at<uchar>(x, y) = novoPixel;
		}
	}

	return aux;
}

int min(int a, int b) {
	return !(b<a) ? a : b;     // or: return !comp(b,a)?a:b; for version (2)
}

Mat somarHard(Mat imagem1, Mat imagem2)
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
				imagemResultado.at<Vec3b>(i, j)[0] = min((imagem1.at<Vec3b>(i, j)[0] + imagem2.at<Vec3b>(i, j)[0]),255);
				imagemResultado.at<Vec3b>(i, j)[1] = min((imagem1.at<Vec3b>(i, j)[1] + imagem2.at<Vec3b>(i, j)[1]), 255);
				imagemResultado.at<Vec3b>(i, j)[2] = min((imagem1.at<Vec3b>(i, j)[2] + imagem2.at<Vec3b>(i, j)[2]), 255);
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

int max(int a, int b) {
	return a < b ? b : a;
}

Mat somar128(Mat imagem1, Mat imagem2)
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
				imagemResultado.at<Vec3b>(i, j)[0] = max(min(((imagem1.at<Vec3b>(i, j)[0] + imagem2.at<Vec3b>(i, j)[0]) - 128), 255), 0);
				imagemResultado.at<Vec3b>(i, j)[1] = max(min(((imagem1.at<Vec3b>(i, j)[1] + imagem2.at<Vec3b>(i, j)[1]) - 128), 255), 0);
				imagemResultado.at<Vec3b>(i, j)[2] = max(min(((imagem1.at<Vec3b>(i, j)[2] + imagem2.at<Vec3b>(i, j)[2]) - 128), 255), 0);
			}
			else
			{
				imagemResultado.at<Vec3b>(i, j)[0] = (imagem1.at<Vec3b>(i, j)[0] - 128);
				imagemResultado.at<Vec3b>(i, j)[1] = (imagem1.at<Vec3b>(i, j)[1] - 128);
				imagemResultado.at<Vec3b>(i, j)[2] = (imagem1.at<Vec3b>(i, j)[2] - 128);
			}
		}

	return imagemResultado;
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
				imagemResultado.at<Vec3b>(i, j)[0] = max(min(((imagem1.at<Vec3b>(i, j)[0] + imagem2.at<Vec3b>(i, j)[0])) / 2, 255),0);
				imagemResultado.at<Vec3b>(i, j)[1] = max(min(((imagem1.at<Vec3b>(i, j)[1] + imagem2.at<Vec3b>(i, j)[1])) / 2, 255), 0);
				imagemResultado.at<Vec3b>(i, j)[2] = max(min(((imagem1.at<Vec3b>(i, j)[2] + imagem2.at<Vec3b>(i, j)[2])) / 2, 255), 0);
			}
			else
			{
				imagemResultado.at<Vec3b>(i, j)[0] = (imagem1.at<Vec3b>(i, j)[0] - 128);
				imagemResultado.at<Vec3b>(i, j)[1] = (imagem1.at<Vec3b>(i, j)[1] - 128);
				imagemResultado.at<Vec3b>(i, j)[2] = (imagem1.at<Vec3b>(i, j)[2] - 128);
			}
		}

	return imagemResultado;
}

Mat subtrair(Mat imagem1, Mat imagem2)
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
				imagemResultado.at<Vec3b>(i, j)[0] = max(min((imagem1.at<Vec3b>(i, j)[0] - imagem2.at<Vec3b>(i, j)[0]),255),0);
				imagemResultado.at<Vec3b>(i, j)[1] = max(min((imagem1.at<Vec3b>(i, j)[1] - imagem2.at<Vec3b>(i, j)[1]), 255), 0);
				imagemResultado.at<Vec3b>(i, j)[2] = max(min((imagem1.at<Vec3b>(i, j)[2] - imagem2.at<Vec3b>(i, j)[2]), 255), 0);

				int diferencaR = imagemResultado.at<Vec3b>(i, j)[0] - imagem1.at<Vec3b>(i, j)[0];
				int diferencaG = imagemResultado.at<Vec3b>(i, j)[1] - imagem1.at<Vec3b>(i, j)[1];
				int diferencaB = imagemResultado.at<Vec3b>(i, j)[2] - imagem1.at<Vec3b>(i, j)[2];

				
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

void histograma(Mat imagemBase, float vec[256]) {
	for (int i = 0; i < 256; i++){
		vec[i] = 0;
	}
	int a;
	for (int x = 0; x < imagemBase.rows; x++) {
		for (int y = 0; y < imagemBase.cols; y++) {
			uchar pixel = imagemBase.at<uchar>(x, y);
			a = pixel;
			vec[a]++;
		}
		
	}
	float size = imagemBase.rows * imagemBase.cols;
	
	for (int i = 0; i < 256; i++) {
		vec[i] = vec[i]/size;
	}
	
}

Mat abertura(Mat imageBase, Matriz elemEstrut) {
	Mat imagem = erosao(imageBase, elemEstrut);
	return dilatacao(imagem, elemEstrut);
}

Mat fechamento(Mat imageBase, Matriz elemEstrut) {
	Mat imagem = dilatacao(imageBase, elemEstrut);
	return erosao(imagem, elemEstrut);
}

Mat erosao(Mat imagemBase, Matriz elemEstrut) {
	Mat aux = imagemBase.clone();
	uchar pixel;


	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			
			for (int linha = 0; linha < 3; linha++) {
				for (int coluna = 0; coluna < 3; coluna++) {

					if (elemEstrut[linha][coluna] == 1)
					{
						pixel = imagemBase.at<uchar>(x + linha, y + coluna);

						if (pixel == 0) {
							aux.at<uchar>(x, y) = pixel;
						}
					}
				}
			}
		}
	}
	return aux;
}

Mat dilatacao(Mat imagemBase, Matriz elemEstrut) {
	Mat aux = imagemBase.clone();
	uchar pixel;
	
	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {

			for (int linha = 0; linha < 3; linha++) {
				for (int coluna = 0; coluna < 3; coluna++) {

					if (elemEstrut[linha][coluna] == 1)
					{
						pixel = imagemBase.at<uchar>(x + linha, y + coluna);

						if (pixel == 255) {
							aux.at<uchar>(x, y) = pixel;
						}
					}
				}
			}
		}
	}
	return aux;
}

Mat agucamento(Mat imagemBase, bool tipo) {
	Mat aux = imagemBase.clone();
	Vec3b pixel;
	
	//Filtro A do Slide
	
	/*int filtro[3][3] = { { 0,1,0 },
	{ 1,-4,1 },
	{ 0,1,0 } };*/

	//Filtro B do Slide
	//int filtro[3][3] = { { 1,1,1 },
	//{ 1,-8, 1 },
	//{ 1,1,1 } };

	//Filtro C do Slide
	int filtro[3][3] = { { 0,-1,0 },
	{ -1,4,-1 },
	{ 0,-1,0 } };

	//Filtro D do Slide
	//int filtro[3][3] = { { -1,-1,-1 },
	//{ -1,8,-1 },
	//{ -1,-1,-1 } };

	int somaR;
	int somaG;
	int somaB;
	int somaK;


	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			for (int linha = 0; linha < 3; linha++) {
				for (int coluna = 0; coluna < 3; coluna++) {					
						
					pixel = aux.at<Vec3b>(x + linha, y + coluna);

					somaR += filtro[linha][coluna] * pixel[2];//Multiplica Vermelho
					somaG += filtro[linha][coluna] * pixel[1]; //Multiplica Verde
					somaB += filtro[linha][coluna] * pixel[0]; //Multiplica azul			
				}
			}

			if (tipo)
			{
				somaR += 128;
				somaG += 128;
				somaB += 128;
			}

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

			somaR = 0;
			somaG = 0;
			somaB = 0;
			somaK = 0;
			aux.at<Vec3b>(x, y) = pixel;

		}
	}
	return aux;
}

Mat agucamento_cinza(Mat imagemBase, int tipo) {
	Mat aux = imagemBase;
	aux = escalaCinzaOPenCV(aux);
	uchar pixel;

	//Filtro A do Slide

	/*int filtro[3][3] = { { 0,1,0 },
	{ 1,-4,1 },
	{ 0,1,0 } };*/

	//Filtro B do Slide
	//int filtro[3][3] = { { 1,1,1 },
	//{ 1,-8, 1 },
	//{ 1,1,1 } };

	//Filtro C do Slide
	int filtro[3][3] = { { 0,-1,0 },
	{ -1,4,-1 },
	{ 0,-1,0 } };

	//Filtro D do Slide
	//int filtro[3][3] = { { -1,-1,-1 },
	//{ -1,8,-1 },
	//{ -1,-1,-1 } };

	int soma;
	int somaK;


	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			for (int linha = 0; linha < 3; linha++) {
				for (int coluna = 0; coluna < 3; coluna++) {

					pixel = aux.at<uchar>(x + linha, y + coluna);

					soma += filtro[linha][coluna] * pixel;// +128;
					somaK += filtro[linha][coluna];
				}
			}

			if (somaK == 0) {
				if (soma > 255)
					pixel = 255;
				else if (soma < 0)
					pixel = 0;
				else
					pixel = soma;
			}
			else {
				if (soma / somaK > 255)
					pixel = 255;
				else if (soma / somaK < 0)
					pixel = 0;
				else
					pixel = soma / somaK;

			}

			soma = 0;
			somaK = 0;
			aux.at<uchar>(x, y) = pixel;
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

float mediaImagem(float vec[256]) {
	float media = 0;
	for (int i = 0; i < 256; i++)
	{
		media += vec[i] * i;
	}
	return media;
}

float mediaParcialImagem(float vec[256], int tn, bool menor) {
	int inicio = 0;
	int fim = 256;
	float  media = 0;
	float pesos = 0;
	if (menor) {
		fim = tn;
	}
	else {
		inicio = tn;
	}
	for (int i = inicio; i < fim; i++)
	{
		media += vec[i] * i;
		pesos += vec[i];
	}
	media = media / pesos;

	return media;
}

float limiarizacaoGlobalSimples(Mat imagemBase) {
	Mat aux = imagemBase.clone();
	aux = escalaCinzaOPenCV(aux);
	float vec[256];
	histograma(aux, vec);
	float tn = mediaImagem(vec);
	float dt = 0.5;
	
	float tz = tn;
	float m1 = tn;
	float m2 = tn;
	do {
		
		tz = tn;
		m1 = mediaParcialImagem(vec, tn, true);
		m2 = mediaParcialImagem(vec, tn, false);
		tn = 0.5*(m1 + m2);
	} while (tn - tz > dt);

	std::cout << tn;
	return tn;
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

Mat suavizacao(Mat imagemBase,int size)
{
	int** mat = new int*[size];
	for (int i = 0; i < size; ++i)
		mat[i] = new int[size];

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			mat[i][j] = 1;
		}
	}
	Mat aux = imagemBase.clone();
	for (int x = size/2; x < aux.rows - size / 2; x++) {
		for (int y = size / 2; y < aux.cols - size / 2; y++) {
			Vec3b pixel = aux.at<Vec3b>(x, y);

			double somaR = 0;
			double somaG = 0;
			double somaB = 0;

			for (int i = 0, xi = x - size / 2; i < size; i++, xi++)
			{
				for (int j = 0, yj = y - size / 2; j < size; j++, yj++)
				{
					somaR += mat[i][j] * aux.at<Vec3b>(xi, yj)[2];
					somaG += mat[i][j] * aux.at<Vec3b>(xi, yj)[1];
					somaB += mat[i][j] * aux.at<Vec3b>(xi, yj)[0];
				}
			}

			int base = size * size;
			somaR = somaR / base;
			somaG = somaG / base;
			somaB = somaB / base;

			aux.at<Vec3b>(x, y) = Vec3b (somaR, somaG, somaB);
		}
	}

	return aux;
}

Mat suavizacao_cinza(Mat imagemBase,int size)
{
	Mat aux = imagemBase.clone();
	aux = escalaCinzaOPenCV(aux);

	int** mat = new int*[size];
	for (int i = 0; i < size; ++i)
		mat[i] = new int[size];

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

Matriz esCustom(int tamX, int tamY, ...) {
	Matriz retorno;

	va_list args;
	va_start(args, tamY);

	for (int i = 0; i < tamX; i++) {
		std::vector<int> aux;
		for (int j = 0; j < tamY; j++) {
			aux.push_back(va_arg(args, int));
		}
		retorno.push_back(aux);
	}
	va_end(args);

	return retorno;
}

