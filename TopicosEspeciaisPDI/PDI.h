#pragma once
#include <opencv2/opencv.hpp>
#include <cstdarg>

using Matriz = std::vector<std::vector<int>>;
using Pixel = cv::Vec3b;
using PixelEC = uchar;

class PDI
{
public:

	
	static cv::Mat escalaCinza(cv::Mat imagemColorida);
	static cv::Mat canal(cv::Mat imagemColorida, int canal);
	static cv::Mat negativo(cv::Mat imagemBase);
	static cv::Mat limiarizacao(cv::Mat imagemBase, int limiar);

	static cv::Mat labeling(cv::Mat imagemColorida);

	static cv::Mat logaritmo(cv::Mat imagemBase);
	static cv::Mat potencia(cv::Mat imagemBase, float gama);
	static cv::Mat alargamento(cv::Mat imagemBase);
	static cv::Mat janelamento(cv::Mat imagemBase, int li, int ls);

	static cv::Mat suavizacaoMedia(cv::Mat imagemBase, Matriz kernel);
	static cv::Mat suavizacaoMediana(cv::Mat imagemBase, int tamanhoKernel);

	static cv::Mat agucamentoLaplaciano(cv::Mat imagemBase, Matriz kernel);
	static cv::Mat agucamentoMascaraNitidez(cv::Mat imagemBase, Matriz kernel);

	static std::vector<float> histograma(cv::Mat imagem);
	static cv::Mat limiarizacaoGS(cv::Mat imagem, float deltaT);

	static cv::Mat dilatacao(cv::Mat imagemBase, Matriz elementoEstruturante);
	static cv::Mat dilatacao(cv::Mat imagemBase, Matriz elementoEstruturante, int cGravidadeX, int cGravidadeY);
	static cv::Mat erosao(cv::Mat imagemBase, Matriz elementoEstruturante);
	static cv::Mat erosao(cv::Mat imagemBase, Matriz elementoEstruturante, int cGravidadeX, int cGravidadeY);


	static cv::Mat erosao_sementes(cv::Mat imagemBase, Matriz elementoEstruturante);
	static cv::Mat dilatacao_sementes(cv::Mat imagemBase, cv::Mat imagemSementes, bool &tem_que_dilatar, Matriz elementoEstruturante);


	static cv::Mat abertura(cv::Mat imagemBase, Matriz elementoEstruturante);
	static cv::Mat abertura(cv::Mat imagemBase, Matriz elementoEstruturante, int cGravidadeX, int cGravidadeY);
	static cv::Mat fechamento(cv::Mat imagemBase, Matriz elementoEstruturante);
	static cv::Mat fechamento(cv::Mat imagemBase, Matriz elementoEstruturante, int cGravidadeX, int cGravidadeY);

	static Matriz laplaciano();
	static Matriz laplacianoDiagonal();

	static Matriz kernelCompleto(int tamanho);
	static Matriz kernelPonderado5();

	static Matriz esQuadrado(int tamanho);
	static Matriz esDiamante(int tamanho);
	static Matriz esCirculo(int tamanho);
	static Matriz esRetangulo(int tamanhoX, int tamanhoY);

	static Matriz esCustom(int tamX, int tamY, ...);


private:
	static int intensidadeMinima(cv::Mat imagemBase);
	static int intensidadeMaxima(cv::Mat imagemBase);
	static int somarKernel(Matriz kernel);

	static bool testeDilatar(cv::Mat imagemBase, Matriz elementoEstruturante, int deslEsq, int deslDir, int deslCima, int deslBaixo, int x, int y);
	static bool testeErodir(cv::Mat imagemBase, Matriz elementoEstruturante, int deslEsq, int deslDir, int deslCima, int deslBaixo, int x, int y);

	static bool testeDilatarSemente(cv::Mat imagemSemente, Matriz elementoEstruturante, int deslEsq, int deslDir, int deslCima, int deslBaixo, int x, int y);
	static bool testeErodirSemente(cv::Mat imagemBase, Matriz elementoEstruturante, int deslEsq, int deslDir, int deslCima, int deslBaixo, int x, int y);

	static int compare(const void * a, const void * b);
};


