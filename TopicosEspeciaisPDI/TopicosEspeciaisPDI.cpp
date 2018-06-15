#include <iostream>
#include <math.h>
#include "PDI.h"
#include "opencv2\opencv.hpp"

using namespace cv;


int main()
{
	Mat imagem;
	Mat imagem3;
	Mat imagemOriginal;
	Mat imagemTemp;
	Mat imagem2;
	imagem = imread("Imagens/chapa.tif");
	imagem = PDI::escalaCinza(imagem);
	imagemOriginal = imagem.clone();
	imshow("Original", imagem);

	imagem = PDI::limiarizacao(imagem, 254);
	
	imshow("Resultado first", imagem);
	for (int i = 0; i < 3; i++) {
		imagem = PDI::erosao_sementes(imagem, PDI::esCirculo(20));
		imshow("Sementes", imagem);
	}
	
	imagem2 = imagemOriginal.clone();
	imagem2 = PDI::limiarizacao(imagem2, 187);
	imshow("Limiarização 2", imagem2);
	bool tem_que_dilatar = true;
	while (tem_que_dilatar) {
		imagemTemp = imagem.clone();
		imagem = PDI::dilatacao_sementes(imagem2, imagem, tem_que_dilatar, PDI::esCirculo(3));
		imshow("Dilatando", imagem);
		waitKey(3);
	}
	imagem = PDI::suavizacaoMedia(imagem, PDI::kernelCompleto(3));
	imagem = PDI::negativo(imagem);
	
	imagem = imagemOriginal - imagem;

	imshow("Fim", imagem);
	//imagem = PDI::fechamento(imagem, PDI::esCirculo(40));

	//Local do processamento
	//imagemResultado = PDIUtils::<Funcão>(imagem, <Parâmetros>)

	
	//imshow("Resultado", imagemResultado);
	waitKey(0);


	return 0;
}