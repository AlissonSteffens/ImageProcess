#include "PDI.h"
#include <algorithm>
#include <cmath>


cv::Mat PDI::escalaCinza(cv::Mat imagemColorida) {
	cv::Mat aux = imagemColorida.clone();

	/*for (int x = 0; x < aux.rows; x++) {
	for (int y = 0; y < aux.cols; y++) {
	Pixel pixel = aux.at<Pixel>(x, y);
	int cinza = pixel[2] * 0.21 + pixel[1] * 0.72 + pixel[0] * 0.07;

	pixel[2] = cinza;
	pixel[1] = cinza;
	pixel[0] = cinza;

	aux.at<Pixel>(x, y) = pixel;
	}
	}*/

	cv::cvtColor(imagemColorida, aux, CV_BGR2GRAY);

	return aux;
}

cv::Mat PDI::limiarizacao(cv::Mat imagemBase, int limiar) {
	cv::Mat aux = imagemBase.clone();

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			PixelEC pixel = imagemBase.at<PixelEC>(x, y);
			PixelEC cPixel;

			if (pixel <= limiar)
				cPixel = 0;
			else
				cPixel = 255;

			aux.at<PixelEC>(x, y) = cPixel;
		}
	}

	return aux;
}

cv::Mat PDI::negativo(cv::Mat imagemBase) {
	cv::Mat aux = imagemBase.clone();

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			PixelEC pixel = imagemBase.at<PixelEC>(x, y);
			PixelEC negativo = 255 - pixel;
			aux.at<PixelEC>(x, y) = negativo;
		}
	}

	return aux;
}

cv::Mat PDI::canal(cv::Mat imagemColorida, int canal) {
	cv::Mat aux = escalaCinza(imagemColorida);

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {

			Pixel pixel = imagemColorida.at<Pixel>(x, y);
			PixelEC pCanal = pixel[canal];


			aux.at<PixelEC>(x, y) = pCanal;
		}
	}

	return aux;
}

cv::Mat PDI::logaritmo(cv::Mat imagemBase) {
	cv::Mat aux = imagemBase.clone();

	float c = 255 / (log(intensidadeMaxima(imagemBase)));

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			PixelEC pixel = imagemBase.at<PixelEC>(x, y);

			PixelEC pLog = c * log(1 + pixel);

			aux.at<PixelEC>(x, y) = pLog;
		}
	}

	return aux;
}

cv::Mat PDI::potencia(cv::Mat imagemBase, float gama) {
	cv::Mat aux = imagemBase.clone();

	float c = 255 / pow(intensidadeMaxima(imagemBase), gama);

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {
			PixelEC pixel = imagemBase.at<PixelEC>(x, y);

			PixelEC pExp = c * pow(pixel, gama);

			aux.at<PixelEC>(x, y) = pExp;
		}
	}

	return aux;
}

cv::Mat PDI::alargamento(cv::Mat imagemBase) {
	cv::Mat aux = imagemBase.clone();

	int minI = intensidadeMinima(imagemBase);
	int maxI = intensidadeMaxima(imagemBase);
	float escala = maxI - minI;

	for (int x = 0; x < imagemBase.rows; x++) {
		for (int y = 0; y < imagemBase.cols; y++) {
			PixelEC pixel = imagemBase.at<PixelEC>(x, y);
			PixelEC novoPixel = (pixel - minI) * (255 / escala);

			aux.at<PixelEC>(x, y) = novoPixel;
		}
	}

	return aux;
}

cv::Mat PDI::janelamento(cv::Mat imagemBase, int li, int ls) {
	cv::Mat aux = imagemBase.clone();

	float escala = ls - li;

	for (int x = 0; x < imagemBase.rows; x++) {
		for (int y = 0; y < imagemBase.cols; y++) {
			PixelEC pixel = imagemBase.at<PixelEC>(x, y);
			PixelEC novoPixel = pixel;

			if (pixel <= li)
				novoPixel = 0;
			else if (pixel >= ls)
				novoPixel = 255;
			else
				novoPixel = (pixel - li) * (255 / escala);

			aux.at<PixelEC>(x, y) = novoPixel;
		}
	}

	return aux;
}

cv::Mat PDI::suavizacaoMedia(cv::Mat imagemBase, Matriz kernel) {
	cv::Mat aux = imagemBase.clone();
	int somaKernel = somarKernel(kernel);
	int desl = kernel.size() / 2;

	for (int x = desl; x < aux.rows - desl; x++) {
		for (int y = desl; y < aux.cols - desl; y++) {
			float soma = 0;
			for (int i = x - desl; i <= x + desl; i++) {
				for (int j = y - desl; j <= y + desl; j++) {
					PixelEC pixel = imagemBase.at<PixelEC>(i, j);
					soma += pixel * kernel[i - x + desl][j - y + desl];
				}
			}
			soma = soma / somaKernel;
			PixelEC pixel = imagemBase.at<PixelEC>(x, y);
			pixel = soma;
			aux.at<PixelEC>(x, y) = pixel;
		}
	}
	return aux;
}

cv::Mat PDI::suavizacaoMediana(cv::Mat imagemBase, int tamanhoKernel) {
	cv::Mat aux = imagemBase.clone();
	int desl = tamanhoKernel / 2;
	for (int x = desl; x < aux.rows - desl; x++) {
		for (int y = desl; y < aux.cols - desl; y++) {
			PixelEC *vet = new PixelEC[tamanhoKernel * tamanhoKernel];
			for (int i = x - desl; i <= x + desl; i++) {
				for (int j = y - desl; j <= y + desl; j++) {
					PixelEC pixel = imagemBase.at<PixelEC>(i, j);
					vet[(i - x + desl) * tamanhoKernel + (j - y + desl)] = pixel;
				}
			}
			qsort(vet, tamanhoKernel * tamanhoKernel, sizeof(PixelEC), compare);
			PixelEC mediana = vet[(tamanhoKernel * tamanhoKernel) / 2];
			delete[] vet;
			aux.at<PixelEC>(x, y) = mediana;
		}
	}
	return aux;
}

cv::Mat PDI::agucamentoLaplaciano(cv::Mat imagemBase, Matriz kernel) {
	cv::Mat aux = imagemBase.clone();
	int desl = kernel.size() / 2;

	for (int x = desl; x < aux.rows - desl; x++) {
		for (int y = desl; y < aux.cols - desl; y++) {
			float soma = 0;
			for (int i = x - desl; i <= x + desl; i++) {
				for (int j = y - desl; j <= y + desl; j++) {
					PixelEC pixel = imagemBase.at<PixelEC>(i, j);
					soma += pixel * kernel[i - x + desl][j - y + desl];
				}
			}
			PixelEC pixel = imagemBase.at<PixelEC>(x, y);
			pixel = (soma + 128 < 0) ? 0 : ((soma + 128 >255) ? 255 : (soma + 128));
			aux.at<PixelEC>(x, y) = pixel;
		}
	}
	return (imagemBase + aux - 128);
}

cv::Mat PDI::agucamentoMascaraNitidez(cv::Mat imagemBase, Matriz kernel) {
	cv::Mat aux = suavizacaoMedia(imagemBase, kernel);
	aux = (imagemBase - aux + 128);
	return (imagemBase + aux - 128);
}

std::vector<float> PDI::histograma(cv::Mat imagem) {
	std::vector<float> hist = std::vector<float>(256);
	for (int x = 0; x < imagem.rows; x++) {
		for (int y = 0; y < imagem.cols; y++) {
			uchar pixel = imagem.at<uchar>(x, y);
			hist[pixel] = hist[pixel] + 1;
		}
	}
	for (int i = 0; i < 256; i++) {
		hist[i] = hist[i] / (imagem.rows * imagem.cols);
	}

	return hist;
}

cv::Mat PDI::limiarizacaoGS(cv::Mat imagem, float deltaT) {
	std::vector<float> histo = histograma(imagem);
	float tAnt = 0, tProx = 0;
	for (int i = 0; i < 256; i++) {
		tAnt = tAnt + histo[i] * i;
	}

	while (true) {
		float sm1 = 0, sm2 = 0, pg1 = 0, pg2 = 0, m1, m2;

		for (int i = 0; i <= tAnt; i++) {
			sm1 += histo[i] * i;
			pg1 += histo[i];
		}
		m1 = sm1 / pg1;

		for (int i = tAnt + 1; i < 256; i++) {
			sm2 += histo[i] * i;
			pg2 += histo[i];
		}
		m2 = sm2 / pg2;

		tProx = (m1 + m2) / 2.0;

		float delta = tProx - tAnt;
		delta = (delta < 0) ? (delta * -1) : delta;

		tAnt = tProx;

		if (delta < deltaT)
			break;

	}//*/

	return limiarizacao(imagem, tAnt);
}


cv::Mat PDI::dilatacao(cv::Mat imagemBase, Matriz elementoEstruturante) {
	return dilatacao(imagemBase, elementoEstruturante, elementoEstruturante.size() / 2, elementoEstruturante[0].size() / 2);
}

cv::Mat PDI::dilatacao(cv::Mat imagemBase, Matriz elementoEstruturante, int cGravidadeX, int cGravidadeY) {
	cv::Mat aux = imagemBase.clone();

	int deslCima = cGravidadeX;
	int deslBaixo = elementoEstruturante.size() - cGravidadeX - 1;
	int deslEsq = cGravidadeY;
	int deslDir = elementoEstruturante[0].size() - cGravidadeY - 1;

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {

			if (testeDilatar(imagemBase, elementoEstruturante, deslEsq, deslDir, deslCima, deslBaixo, x, y)) {
				aux.at<PixelEC>(x, y) = 255;
			}
		}
	}

	return aux;
}

cv::Mat PDI::erosao(cv::Mat imagemBase, Matriz elementoEstruturante) {
	return erosao(imagemBase, elementoEstruturante, elementoEstruturante.size() / 2, elementoEstruturante[0].size() / 2);
}

cv::Mat PDI::erosao(cv::Mat imagemBase, Matriz elementoEstruturante, int cGravidadeX, int cGravidadeY) {
	cv::Mat aux = imagemBase.clone();

	int deslCima = cGravidadeX;
	int deslBaixo = elementoEstruturante.size() - cGravidadeX - 1;
	int deslEsq = cGravidadeY;
	int deslDir = elementoEstruturante[0].size() - cGravidadeY - 1;

	for (int x = 0; x < aux.rows; x++) {
		for (int y = 0; y < aux.cols; y++) {

			if (testeErodir(imagemBase, elementoEstruturante, deslEsq, deslDir, deslCima, deslBaixo, x, y)) {
				aux.at<PixelEC>(x, y) = 0;
			}
		}
	}

	return aux;
}

cv::Mat PDI::abertura(cv::Mat imagemBase, Matriz elementoEstruturante) {
	return abertura(imagemBase, elementoEstruturante, elementoEstruturante.size() / 2, elementoEstruturante[0].size() / 2);
}

cv::Mat PDI::abertura(cv::Mat imagemBase, Matriz elementoEstruturante, int cGravidadeX, int cGravidadeY) {
	cv::Mat aux = erosao(imagemBase, elementoEstruturante, cGravidadeX, cGravidadeY);
	aux = dilatacao(aux, elementoEstruturante, cGravidadeX, cGravidadeY);

	return aux;
}

cv::Mat PDI::fechamento(cv::Mat imagemBase, Matriz elementoEstruturante) {
	return fechamento(imagemBase, elementoEstruturante, elementoEstruturante.size() / 2, elementoEstruturante[0].size() / 2);
}

cv::Mat PDI::fechamento(cv::Mat imagemBase, Matriz elementoEstruturante, int cGravidadeX, int cGravidadeY) {
	cv::Mat aux = dilatacao(imagemBase, elementoEstruturante, cGravidadeX, cGravidadeY);
	aux = erosao(aux, elementoEstruturante, cGravidadeX, cGravidadeY);

	return aux;
}

//////////////////////
//FUNÇÕES AUXILIARES//
//////////////////////


int PDI::compare(const void * a, const void * b)
{
	return (*(uchar*)a - *(uchar*)b);
}

Matriz PDI::esCustom(int tamX, int tamY, ...) {
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

Matriz PDI::laplaciano() {
	return esCustom(3, 3,
		0, -1, 0,
		-1, 4, -1,
		0, -1, 0);
}

Matriz PDI::laplacianoDiagonal() {
	return esCustom(3, 3,
		-1, -1, -1,
		-1, 8, -1,
		-1, -1, -1);
}

Matriz PDI::kernelPonderado5() {
	return esCustom(5, 5,
		1, 4, 6, 4, 1,
		4, 16, 24, 16, 4,
		6, 24, 36, 24, 6,
		4, 16, 24, 16, 4,
		1, 4, 6, 4, 1);
}

Matriz PDI::esQuadrado(int tamanho) {
	return esRetangulo(tamanho, tamanho);
}

Matriz PDI::esDiamante(int tamanho) {
	Matriz retorno;

	for (int i = 0; i < tamanho; i++) {
		std::vector<int> aux;
		for (int j = 0; j < tamanho; j++) {
			if (i + j < tamanho / 2 || i + j >= tamanho + tamanho / 2 ||
				i - j < (tamanho / 2) * -1 || i - j > tamanho / 2)
				aux.push_back(0);
			else
				aux.push_back(1);
		}
		retorno.push_back(aux);
	}

	return retorno;
}

Matriz PDI::esCirculo(int tamanho) {
	if (tamanho < 7)
		return esDiamante(tamanho);

	Matriz retorno;

	for (int i = 0; i < tamanho; i++) {
		std::vector<int> aux;
		for (int j = 0; j < tamanho; j++) {
			if (sqrt((i - tamanho / 2) * (i - tamanho / 2) + (j - tamanho / 2) * (j - tamanho / 2)) > tamanho / 2 + 0.5f)
				aux.push_back(0);
			else
				aux.push_back(1);
		}
		retorno.push_back(aux);
	}

	return retorno;
}

Matriz PDI::esRetangulo(int tamanhoX, int tamanhoY) {
	Matriz retorno;

	for (int i = 0; i < tamanhoX; i++) {
		std::vector<int> aux;
		for (int j = 0; j < tamanhoY; j++) {
			aux.push_back(1);
		}
		retorno.push_back(aux);
	}

	return retorno;
}


Matriz PDI::kernelCompleto(int tamanho) {
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

int PDI::intensidadeMaxima(cv::Mat imagemBase) {
	int maximo = 0;
	for (int x = 0; x < imagemBase.rows; x++) {
		for (int y = 0; y < imagemBase.cols; y++) {
			PixelEC pixel = imagemBase.at<PixelEC>(x, y);
			if (pixel > maximo) {
				maximo = pixel;
			}
		}
	}

	return maximo;
}

int PDI::intensidadeMinima(cv::Mat imagemBase) {
	int minimo = 255;
	for (int x = 0; x < imagemBase.rows; x++) {
		for (int y = 0; y < imagemBase.cols; y++) {
			PixelEC pixel = imagemBase.at<PixelEC>(x, y);
			if (pixel < minimo) {
				minimo = pixel;
			}
		}
	}

	return minimo;
}

int PDI::somarKernel(Matriz kernel) {
	int soma = 0;
	for (std::vector<int> linha : kernel) {
		for (int elemento : linha) {
			soma += elemento;
		}
	}
	return soma;
}

bool PDI::testeDilatar(cv::Mat imagemBase, Matriz elementoEstruturante, int deslEsq, int deslDir, int deslCima, int deslBaixo, int x, int y) {
	for (int i = x - deslCima, esX = 0; i <= x + deslBaixo; i++, esX++) {
		for (int j = y - deslEsq, esY = 0; j <= y + deslDir; j++, esY++) {
			if (i < 0 || j < 0 || i >= imagemBase.rows || j >= imagemBase.cols)
				continue;
			PixelEC pixel = imagemBase.at<PixelEC>(i, j);
			if (pixel == 255 && elementoEstruturante[esX][esY] == 1)
				return true;
		}
	}

	return false;
}

bool PDI::testeErodir(cv::Mat imagemBase, Matriz elementoEstruturante, int deslEsq, int deslDir, int deslCima, int deslBaixo, int x, int y) {
	for (int i = x - deslCima, esX = 0; i <= x + deslBaixo; i++, esX++) {
		for (int j = y - deslEsq, esY = 0; j <= y + deslDir; j++, esY++) {
			if (i < 0 || j < 0 || i >= imagemBase.rows || j >= imagemBase.cols)
				continue;
			PixelEC pixel = imagemBase.at<PixelEC>(i, j);
			if (pixel == 0 && elementoEstruturante[esX][esY] == 1)
				return true;
		}
	}

	return false;
}

