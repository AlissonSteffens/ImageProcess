#include <iostream>
#include <math.h>
#include "PDI.h"
#include "opencv2\opencv.hpp"

using namespace cv;


int main()
{
	Mat imagem;
	Mat imagemResultado;
	imagem = imread("Imagens/QBOriginal.png");
	imshow("Original", imagem);
	imagem = PDI::escalaCinza(imagem);
	imagem = PDI::fechamento(imagem, PDI::esCirculo(40));

	//Local do processamento
	//imagemResultado = PDIUtils::<Funcão>(imagem, <Parâmetros>)

	imshow("Resultado", imagem);
	//imshow("Resultado", imagemResultado);
	waitKey(0);


	return 0;
}