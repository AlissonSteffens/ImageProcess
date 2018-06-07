#include <iostream>
#include <math.h>
#include "PDI.h"
#include "opencv2\opencv.hpp"

using namespace cv;


int main()
{
	Mat imagem;
	Mat imagemResultado;
	imagem = imread("Imagens/QAMoedas.png");
	imshow("Original", imagem);
	imagem = PDI::escalaCinza(imagem);
	int size = 15;
	imagem = PDI::fechamento(imagem, PDI::esCirculo(10));
	imagem = PDI::abertura(imagem, PDI::esCirculo(20));

	//Local do processamento
	//imagemResultado = PDIUtils::<Funcão>(imagem, <Parâmetros>)

	imshow("Cinza", imagem);
	//imshow("Resultado", imagemResultado);
	waitKey(0);


	return 0;
}